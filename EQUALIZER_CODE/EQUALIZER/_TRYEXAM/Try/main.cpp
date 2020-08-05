/*
 * This example demonstrates how to enumerate and select from the available audio inputs
 * (using the Mixer API), and then records digital audio (to a disk file) using the low
 * level API.
 *
 * You must link with winmm.lib. If using Visual C++, go to Build->Settings. Flip to the
 * Link page, and add winmm.lib to the library/object modules.
 */

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <mmsystem.h>





/* Handle to the WAVE In Device */
HWAVEIN				WaveInHandle;

/* Handle to Mixer for WAVE In Device */
HMIXER				MixerHandle;

/* Handle to the disk file where we permanently store the recorded audio data */
HANDLE				WaveFileHandle = INVALID_HANDLE_VALUE;

/* We use two WAVEHDR's for recording (ie, double-buffering) in this example */
WAVEHDR				WaveHeader[2];

/* Variable used to indicate whether we are in record */
BOOL				InRecord = FALSE;

/* Variable used by recording thread to indicate whether we are in record */
unsigned char		DoneAll;





/*********************** PrintWaveErrorMsg() **************************
 * Retrieves and displays an error message for the passed Wave In error
 * number. It does this using mciGetErrorString().
 *************************************************************************/

void PrintWaveErrorMsg(DWORD err, TCHAR * str)
{
#define BUFFERSIZE 128
	char	buffer[BUFFERSIZE];

	printf("ERROR 0x%08X: %s\r\n", err, str);
	if (mciGetErrorString(err, &buffer[0], sizeof(buffer)))
	{
		printf("%s\r\n", &buffer[0]);
	}
	else
	{
		printf("0x%08X returned!\r\n", err);
	}
}





/**************************** waveInProc() *******************************
 * This is my thread to which the Windows Low Level WAVE API passes messages
 * regarding digital audio recording (such as MM_WIM_DATA, MM_WIM_OPEN, and
 * MM_WIM_CLOSE).
 *
 * Before opening the WAVE In Device, I use CreateThread() (from my main
 * thread) to start up waveInProc() as a background thread. waveInProc()
 * simply loops around a call to GetMessage() to process those messages
 * that the audio driver sends me during recording.
 *
 * When I subsequently open the WAVE In device (using waveInOpen() in my
 * main thread), I pass CALLBACK_THREAD and the ID of this thread.
 *
 * Why don't I use CALLBACK_FUNCTION instead, and use a real callback (instead
 * of this background thread)? That would actually be more efficient since a
 * callback function gets called directly by the driver at interrupt time,
 * right when the driver needs to inform us of something, and the driver
 * directly passes args on the stack (instead of needing to queue a MSG to
 * some thread queue).
 *
 * By contrast, this thread is passed a message usually after the driver's
 * interrupt routine is done, which can be some time later than it would be
 * with a callback. And args are retrieved via filling in a MSG with
 * GetMessage().
 *
 * I don't use CALLBACK_FUNCTION because it is restricted to calling only
 * a few particular Windows functions, namely some of the time functions,
 * and a few of the Low Level MIDI API. If you violate this rule, your app can
 * hang inside of the callback). One of the Windows API that a callback can't
 * call is waveInAddBuffer() which is what I need to use whenever I receive a
 * MM_WIM_DATA. My callback would need to defer that job to another thread
 * anyway, so I just use CALLBACK_THREAD here instead.
 *
 * NOTE: You have to link with the multi-threaded version of the C library
 * if you intend to call any C library functions in this secondary thread.
 * (I don't). And in that case, you should also use beginthread() to create
 * the thread instead of CreateThread().
 *************************************************************************/

DWORD WINAPI waveInProc(LPVOID arg)
{
	MSG		msg;

	/* Wait for a message sent to me by the audio driver */
	while (GetMessage(&msg, 0, 0, 0) == 1)
	{
		/* Figure out which message was sent */
		switch (msg.message)
		{
			/* A buffer has been filled by the driver */
			case MM_WIM_DATA:
			{
				/* msg.lParam contains a pointer to the WAVEHDR structure for the filled buffer.
				 * NOTE: The WAVEHDR's dwBytesRecorded field specifies how many bytes of
				 * audio data are in the buffer. It may not be full, as would be the case
				 * if recording was stopped *while* this buffer was being filled. (ie, A
				 * final, partial block of audio data may be returned)
				 */
				if (((WAVEHDR *)msg.lParam)->dwBytesRecorded)
				{
					/* Let's write out this next block of data to our already open file. NOTE: The main
					 * thread opened this file prior to starting recording, and stored its handle in
					 * 'WaveFileHandle'
					 */
					if (!WriteFile(WaveFileHandle, ((WAVEHDR *)msg.lParam)->lpData, ((WAVEHDR *)msg.lParam)->dwBytesRecorded, &msg.time, 0) ||

						/* Check for an error */
						msg.time != ((WAVEHDR *)msg.lParam)->dwBytesRecorded)
					{
						/* Normally you'd do something here like signal the main thread to have it stop recording and
						 * display an error message about there being a problem writing the audio data to disk
						 */
					}
				}

				/* Are we still recording? */
				if (InRecord)
				{
					/* Yes. Now we need to requeue this buffer so the driver can use it for another block of audio
					 * data. NOTE: We shouldn't need to waveInPrepareHeader() a WAVEHDR that has already been prepared once
					 */
					waveInAddBuffer(WaveInHandle, (WAVEHDR *)msg.lParam, sizeof(WAVEHDR));
				}

				/* We aren't recording, so another WAVEHDR has been returned to us after recording has stopped.
				 * When we get all of them back, DoneAll will be equal to how many WAVEHDRs we queued
				 */
				else
				{
					++DoneAll;
				}

				/* Keep waiting for more messages */
                continue;
			}

			/* Our main thread is opening the WAVE device */
			case MM_WIM_OPEN:
			{
				/* Re-initialize 'DoneAll' */
				DoneAll = 0;

				/* Keep waiting for more messages */
                continue;
			}

			/* Our main thread is closing the WAVE device */
			case MM_WIM_CLOSE:
			{
				/* Terminate this thread (by return'ing) */
				break;
			}
		}
	}

	return(0);
}








/****************************** set_mute() *********************************
 * Uses the Mixer API to check if the specified line has a mute control, and
 * if so, sets it to the specified value.
 *
 * mixerLine =		Pointer to a MIXERLINE filled in with information about
 *					the desired line (ie, its name and ID and number of
 *					channels).
 *
 * value =			The value to set mute control to (ie, 0 = mute, 1 = unmute).
 *
 * errmsg =			ERRMSG_PRINT if we display an error message whenever we
 *					find that the line has no mute control. ERRMSG_NONE if we
 *					wish to ignore this.
 *
 * NOTE: The Mixer must be open and its handle stored in the global
 * 'MixerHandle'.
 *************************************************************************/

#define ERRMSG_PRINT	0
#define ERRMSG_NONE		1

void set_mute(MIXERLINE *mixerLine, DWORD val, BOOL errmsg)
{
	MIXERCONTROL					mixerControlArray;
	MIXERLINECONTROLS				mixerLineControls;
	MIXERCONTROLDETAILS_UNSIGNED	value[2];
	MIXERCONTROLDETAILS				mixerControlDetails;
	MMRESULT						err;

	/* First, does this source even have a mute switch? We'll check that by trying to
	 * fetch information about any mute control (if there is one). This will also
	 * fetch us the control's ID which we'll need later in order to set its value
	 */

	mixerLineControls.cbStruct = sizeof(MIXERLINECONTROLS);

	/* Tell mixerGetLineControls() for which line we're retrieving info.
	 * We do this by putting the desired line's ID number in dwLineID
	 */
	mixerLineControls.dwLineID = mixerLine->dwLineID;

	/* We want to fetch info on only 1 control */
	mixerLineControls.cControls = 1;

	/* Tell mixerGetLineControls() for which type of control we're
	 * retrieving info. We do this by putting the desired control type
	 * in dwControlType. Here, we're looking for a mute switch
	 */
	mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;

	/* Give mixerGetLineControls() the address of the MIXERCONTROL struct to return info */
	mixerLineControls.pamxctrl = &mixerControlArray;

	/* Tell mixerGetLineControls() how big the MIXERCONTROL is. This
	 * saves having to initialize the cbStruct of the MIXERCONTROL itself
	 */
	mixerLineControls.cbmxctrl = sizeof(MIXERCONTROL);

	/* Retrieve info only upon any mute control for this line */
	if ((err = mixerGetLineControls((HMIXEROBJ)MixerHandle, &mixerLineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE)))
	{
		/* An error */
		if (errmsg == ERRMSG_PRINT) printf("%s has no mute control!\n", mixerLine->szName);
	}
	else
	{
		/* Let's mute this line. You may want to first fetch its current mute value(s) and
		 * save them somewhere, then restore them before your program exits. That way, you
		 * won't permanently alter those value(s). But we don't do that here
		 */

		mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);

		/* Tell mixerSetControlDetails() which control whose value(s) we
		 * want to set. We do this by putting the desired control's
		 * ID number in dwControlID. Since we just retrieved info above
		 * on the control, we have its ID in the mixerControlArray
		 */
		mixerControlDetails.dwControlID = mixerControlArray.dwControlID;

		/* Tell how many channels we're muting. NOTE: We're assuming that there aren't more
		 * than 2 channels. Normally, we'd allocate a value[] array as big as
		 * mixerLine->cChannels * mixerControlArray.cMultipleItems here, and store a value
		 * for each channel's parameters. Here we'll just limit the mute to the
		 * first two channels since our wave recording here is limited to stereo anyway
		 */
		mixerControlDetails.cChannels = mixerLine->cChannels;
		if (mixerControlDetails.cChannels > 2) mixerControlDetails.cChannels = 2;
		if (mixerControlArray.fdwControl & MIXERCONTROL_CONTROLF_UNIFORM) mixerControlDetails.cChannels = 1;

		/* This is always 0 except for a MIXERCONTROL_CONTROLF_MULTIPLE control. NOTE: We're
		 * assuming that the mute switch doesn't have multiple parameters. That's a pretty
		 * good assumption since it would be weird otherwise. But ideally, you should check
		 * if mixerControlArray.fdwControl has the MIXERCONTROL_CONTROLF_MULTIPLE flag set,
		 * and if so store mixerControlArray.cMultipleItems here and make sure we have
		 * a value[] array with enough (mixerLine->cChannels * mixerControlArray.cMultipleItems)
		 * initialized values in it.
		 */
		mixerControlDetails.cMultipleItems = 0;

		/* Give mixerSetControlDetails() the address of the
		 * MIXERCONTROLDETAILS_UNSIGNED struct(s) into which we place the value(s)
		 */
		mixerControlDetails.paDetails = &value[0];

		/* Tell mixerSetControlDetails() how big the MIXERCONTROLDETAILS_UNSIGNED is */
		mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);

		/* Store the value. Initialize for 2 channels, just in case */
		value[0].dwValue = value[1].dwValue = val;

		/* Set the value of the mute control for this line */
		if ((err = mixerSetControlDetails((HMIXEROBJ)MixerHandle, &mixerControlDetails, MIXER_SETCONTROLDETAILSF_VALUE)))
		{
			/* An error */
			printf("Error #%d setting mute for %s!\n", err, mixerLine->szName);
		}
	}
}





int main(int argc , char ** argv)
{
	MMRESULT						err;
	WAVEFORMATEX					waveFormat;
	MIXERLINE						mixerLine;
	HANDLE							waveInThread;
	unsigned long					n, numSrc;

	/* Create our thread that we use to store our incoming "blocks" of digital audio data (sent to us
	 * from the driver. The main procedure of this thread is waveInProc(). We need to get the threadID
	 * and pass that to waveInOpen(). CreateThread() will give it to us
	 */
	waveInThread = (HANDLE)CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&waveInProc, 0, 0, (DWORD*)&err);
	if (!waveInThread)
	{
		printf("Can't create WAVE recording thread! -- %08X\n", GetLastError());
		return(-1);
	}
	CloseHandle(waveInThread);

	/* Clear out both of our WAVEHDRs. At the very least, waveInPrepareHeader() expects the dwFlags field to
	 * be cleared
	 */
	ZeroMemory(&WaveHeader[0], sizeof(WAVEHDR) * 2);

	/* Initialize the WAVEFORMATEX for 16-bit, 44KHz, stereo. That's what I want to record */
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample/8);
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	/* Open the default WAVE In Device, specifying my callback. Note that if this device doesn't
	 * support 16-bit, 44KHz, stereo recording, then Windows will attempt to open another device
	 * that does. So don't make any assumptions about the name of the device that opens. After
	 * waveInOpen returns the handle, use waveInGetID to fetch its ID, and then waveInGetDevCaps
	 * to retrieve the actual name
	 */
	err = waveInOpen(&WaveInHandle, WAVE_MAPPER, &waveFormat, (DWORD)err, 0, CALLBACK_THREAD);
	if (err)
	{
		PrintWaveErrorMsg(err, "Can't open WAVE In Device!");
		return(-2);
	}
	else
	{
	    /* Open the mixer associated with the WAVE In device opened above. Note that
		 * I pass the handle obtained via waveInOpen() above and specify MIXER_OBJECTF_HWAVEIN
		 */
		err = mixerOpen(&MixerHandle, (DWORD)WaveInHandle, 0, 0, MIXER_OBJECTF_HWAVEIN);
		if (err)
		{
			/* This card's driver may not support the Mixer API, so this error check is
			 * very important. NOTE: Hopefully, the card ships with some sort of proprietary
			 * software to set the recording level, or the card has a hardware knob that the
			 * user can adjust. In either case, setting the recording level is not doable here.
			 * You may want to inform the user of that.
			 */
			printf("Device does not have mixer support! -- %08X\n", err);
		}

		else
		{
			/* This device should have a WAVEIN destination line. Let's get its ID so
			 * that we can determine what source lines are available to record from
			 */
			mixerLine.cbStruct = sizeof(MIXERLINE);
			mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
			err = mixerGetLineInfo((HMIXEROBJ)MixerHandle, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
			if (err)
			{
				printf("Device does not have a WAVE recording control! -- %08X\n", err);
				goto record;
			}

			/* Get how many source lines are available from which to record. For example, there could
			 * be a Mic In (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE), Line In (MIXERLINE_COMPONENTTYPE_SRC_LINE),
			 * and/or SPDIF In (MIXERLINE_COMPONENTTYPE_SRC_DIGITAL)
			 */
			numSrc = mixerLine.cConnections;

			/* If the WAVEIN destination line has a mute switch, then make sure that it is not muted. So too, you should
			 * make sure that the volume is turned up. This would be the master volume and master mute for all input
			 * source lines. But you may wish to save the current settings (by fetching them with mixerGetControlDetails)
			 * and subsequently restore them (when your program exits) so that you don't permanently alter his settings.
			 * Here we only unmute the line, but don't set its volume. Take a look at set_mute() to see how we adjust the
			 * value of any mute control. You can modify this easily to create a set_volume() function that sets the value
			 * of any MIXERCONTROL_CONTROLTYPE_VOLUME type of control for the line. The only thing that you may need to do
			 * is make sure the value is within the MIXERCONTROLDETAILS' dwMinimum and dwMaximum range. (And you need to
			 * fetch the current value to get a filled in MIXERCONTROLDETAILS)
			 */
			mixerLine.dwSource = mixerLine.dwDestination;
			set_mute(&mixerLine, 1, ERRMSG_PRINT);

			/* Make sure that there is at least one WAVEIN source line available. If not, then just go with the
			 * WAVEIN master volume and mute above
			 */
			if (!numSrc)
			{
				printf("ERROR: There are no WAVE inputs to adjust!\n");
				goto record;
			}

			/* If there is more than one source line, let's choose which ones to record from (ie, which to leave
			 * unmuted). NOTE: The audio card may allow all the inputs to simultaneously record (and the card will
			 * digitally mix all wave data into our buffer that we waveInAddBuffer()). The user may actually want
			 * this. So what you may wish to do here is present a list of the available source lines, and allow him
			 * to mute inputs that he doesn't want to record from. That will perhaps reduce any noise. If a particular
			 * source line has no mute switch, then if it has a volume, you can set its volume to 0. But you may wish
			 * to save the current settings and subsequently restore them (when your program exits) so that you don't
			 * permanently alter his settings.
			 */

			/* Now let's enumerate the names of source lines that are available for WAVE recording. For example, there
			 * could be a Mic In (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE), Line In (MIXERLINE_COMPONENTTYPE_SRC_LINE),
			 * and/or SPDIF In (MIXERLINE_COMPONENTTYPE_SRC_DIGITAL). NOTE: mixerLine.dwDestination already has the
			 * index of our WAVEIN destination line.
			 *
			 * NOTE: Some sound cards will list source lines such as MIDI input, CD audio (analog) input, and other
			 * types of inputs that aren't really connected to audio recording (ie, aren't capable of being digitized
			 * by the ADC into a WAVE file), under the MIXERLINE_COMPONENTTYPE_DST_WAVEIN destination line. In other
			 * words, some manufacturers list every input source line under MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
			 * regardless of whether it can be recorded (into a WAVE or MIDI file) by software. Other card manufacturers
			 * may take a more sensible software approach and list only those sources that can be recorded (but still
			 * you may have a MIDI input listed under MIXERLINE_COMPONENTTYPE_DST_WAVEIN, which is a bit of misnomer
			 * since MIDI is not digital audio, and is not recorded via the waveXXX API). Ideally, any sources that
			 * aren't recordable by a card's ADC shouldn't be listed under MIXERLINE_COMPONENTTYPE_DST_WAVEIN. For
			 * example, a CD audio input jack (that can't be digitized) should be a MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC
			 * attached to MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, and a MIDI IN should be a
			 * MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER attached to MIXERLINE_COMPONENTTYPE_DST_UNDEFINED (since
			 * unfortunately Microsoft never came up with some sort of MIXERLINE_COMPONENTTYPE_DST_MIDIIN). But we live
			 * in an imperfect world. At the least, you should filter out any MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER
			 * from your listing.
			 *
			 * If you're writing a windowed app, what you should probably do is create a dialog that contains all of
			 * the controls for each source line below, plus the WAVEIN destination line (ie, a small mixer that contains
			 * the individual volume sliders, mute switches, etc, for each source line, plus the master volume for the
			 * WAVEIN destination line). That way, he can more easily mute desired inputs, as well as adjust the recording
			 * levels. In this simple example, we haven't provided a way to adjust the input levels.
			 */
remute:
			printf("The device has the following WAVE inputs. Choose which one to mute,\nor press ENTER to start recording.\n\n");

			/* Print the names of the source lines for the MIXERLINE_COMPONENTTYPE_DST_WAVEIN destination. But,
			 * omit any MIDI In
			 */
			for (n = 0; n < numSrc; n++)
			{
				mixerLine.cbStruct = sizeof(MIXERLINE);
				mixerLine.dwSource = n;

				if (!(err = mixerGetLineInfo((HMIXEROBJ)MixerHandle, &mixerLine, MIXER_GETLINEINFOF_SOURCE)))
				{
					if (mixerLine.dwComponentType != MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER)
						printf("\t#%lu: %s\n", n, mixerLine.szName);
				}
			}

			/* Get his choice of source line to mute, or ENTER to continue with recording */
			{
			TCHAR	buffer[3];

			fgets(&buffer[0], 3, stdin);

			/* Did he choose a source line to mute (ie, he typed a number)? */
			if (buffer[0] >= '1' && buffer[0] <= '9')
			{
				/* Let's use the mixer API to mute this line. This is an example of how you can adjust the
				 * setting of some line
				 */
				buffer[0] -= '1';
				if ((unsigned long)buffer[0] < numSrc)
				{
					/* set_mute() wants us to fill in a MIXERLINE struct with information about
					 * the desired source line
					 */
					mixerLine.cbStruct = sizeof(MIXERLINE);
					mixerLine.dwSource = buffer[0];
					mixerGetLineInfo((HMIXEROBJ)MixerHandle, &mixerLine, MIXER_GETLINEINFOF_SOURCE);

					/* Check if there is a mute control for this line, and if so, set it's value to
					 * 0 (ie, mute it)
					 */
					set_mute(&mixerLine, 0, ERRMSG_PRINT);
				}

				/* Go back to check for more muted sources */
				goto remute;
			}
			}

			/* Close the mixer */
record:		mixerClose(MixerHandle);
		}

		/* Now that the inputs are all set up, we can record an audio file using waveInPrepareHeader(),
		 * waveInAddBuffer(), and waveInStart().
		 */

		/* First, let's create a file on disk where we'll store the audio data we record. We'll name it
		 * test.snd in the current directory. We'll save the data in raw format, with no headers on it,
		 * just for simplicity. Normally, you would instead save it in WAVE file format so any program
		 * can read it and know the sample rate and number of channels, etc.
		 */
		WaveFileHandle = CreateFile("test.snd", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (WaveFileHandle == INVALID_HANDLE_VALUE)
		{
			printf("Can't create test.snd disk file!\n");
		}
		else
		{
			/* Allocate, prepare, and queue two buffers that the driver can use to record blocks of audio data.
			 * (ie, We're using double-buffering. You can use more buffers if you'd like, and you should do that
			 * if you suspect that you may lag the driver when you're writing a buffer to disk and are too slow
			 * to requeue it with waveInAddBuffer. With more buffers, you can take your time requeueing
			 * each).
			 *
			 * I'll allocate 2 buffers large enough to hold 2 seconds worth of waveform data at 44Khz. NOTE:
			 * Just to make it easy, I'll use 1 call to VirtualAlloc to allocate both buffers, but you can
			 * use 2 separate calls since the buffers do NOT need to be contiguous. You should do the latter if
			 * using many, large buffers
			 */
			WaveHeader[1].dwBufferLength = WaveHeader[0].dwBufferLength = waveFormat.nAvgBytesPerSec << 1;
			if (!(WaveHeader[0].lpData = (char *)VirtualAlloc(0, WaveHeader[0].dwBufferLength * 2, MEM_COMMIT, PAGE_READWRITE)))
			{
				printf("ERROR: Can't allocate memory for WAVE buffer!\n");
			}
			else
			{
				/* Fill in WAVEHDR fields for buffer starting address. We've already filled in the size fields above */
				WaveHeader[1].lpData = WaveHeader[0].lpData + WaveHeader[0].dwBufferLength;

				/* Leave other WAVEHDR fields at 0 */

				/* Prepare the 2 WAVEHDR's */
				if ((err = waveInPrepareHeader(WaveInHandle, &WaveHeader[0], sizeof(WAVEHDR))))
				{
					printf("Error preparing WAVEHDR 1! -- %08X\n", err);
				}
				else
				{
					if ((err = waveInPrepareHeader(WaveInHandle, &WaveHeader[1], sizeof(WAVEHDR))))
					{
						printf("Error preparing WAVEHDR 2! -- %08X\n", err);
					}
					else
					{
						/* Queue first WAVEHDR (recording hasn't started yet) */
						if ((err = waveInAddBuffer(WaveInHandle, &WaveHeader[0], sizeof(WAVEHDR))))
						{
							printf("Error queueing WAVEHDR 1! -- %08X\n", err);
						}
						else
						{
							/* Queue second WAVEHDR */
							if ((err = waveInAddBuffer(WaveInHandle, &WaveHeader[1], sizeof(WAVEHDR))))
							{
								printf("Error queueing WAVEHDR 2! -- %08X\n", err);
								DoneAll = 1;
								goto abort;
							}
							else
							{
								/* Start recording. Our secondary thread will now be receiving
								 * and storing audio data to disk
								 */
								InRecord = TRUE;
								if ((err = waveInStart(WaveInHandle)))
								{
									printf("Error starting record! -- %08X\n", err);
								}
								else
								{
									/* Wait for user to stop recording */
									printf("Recording has started. Press ENTER key to stop recording...\n");
									getchar();
								}

								/* Tell our recording thread not to queue any WAVEHDRs it gets back via MM_WIM_DONE */
abort:							InRecord = FALSE;

								/* Stop recording and tell the driver to unqueue/return all of our WAVEHDRs.
								 * The driver will return any partially filled buffer that was currently
								 * recording. Because we use waveInReset() instead of waveInStop(),
								 * all of the other WAVEHDRs will also be returned via MM_WIM_DONE too
								 */
								waveInReset(WaveInHandle);

								/* Wait for the recording thread to receive the MM_WIM_DONE for each
								 * of our queued WAVEHDRs. It will count them off, and after processing
								 * them all, 'DoneAll' will be equal to how many WAVEHDRs we used. We
								 * can then unprepare the WAVEHDRs, close our WAVE device and our disk
								 * file, and free our buffers
								 */
								while (DoneAll < 2) Sleep(100);
							}
						}

						/* Unprepare second WAVEHDR. NOTE: It should be ok to unprepare a WAVEHDR
						 * that was never successfully prepared, as long as you initially cleared
						 * out the dwFlags field (unless the audio driver is badly written)
						 */
						if ((err = waveInPrepareHeader(WaveInHandle, &WaveHeader[1], sizeof(WAVEHDR))))
						{
							printf("Error unpreparing WAVEHDR 2! -- %08X\n", err);
						}
					}

					/* Unprepare first WAVEHDR */
					if ((err = waveInPrepareHeader(WaveInHandle, &WaveHeader[0], sizeof(WAVEHDR))))
					{
						printf("Error unpreparing WAVEHDR 1! -- %08X\n", err);
					}
				}
			}
		}
	}

	/* Close the WAVE In device */
	do
	{
		err = waveInClose(WaveInHandle);
		if (err) PrintWaveErrorMsg(err, "Can't close WAVE In Device!");
	} while (err);

	/* Close the disk file if it opened */
	if (WaveFileHandle != INVALID_HANDLE_VALUE) CloseHandle(WaveFileHandle);

	/* Free any memory allocated for our buffers */
	if (WaveHeader[0].lpData) VirtualFree(WaveHeader[0].lpData, 0, MEM_RELEASE);

	return(0);
}
