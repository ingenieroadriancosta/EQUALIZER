class Mixer{

private:
    /*************************************************************************/
    UINT _nNumMixers;
    HMIXER _hMixer;
    MIXERCAPS _mxcaps;
    DWORD _controlID;
public:
    /*************************************************************************/
    bool init()
    {
        _nNumMixers = mixerGetNumDevs();
        _hMixer = NULL;
        ZeroMemory(&_mxcaps, sizeof(MIXERCAPS));

        if(_nNumMixers != 0)
        {
            if (mixerOpen(&_hMixer, 0, 0, NULL, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
            {
                return FALSE;
            }

            if (mixerGetDevCaps((UINT)_hMixer, &_mxcaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)
            {
                return FALSE;
            }
        }

        return _hMixer != NULL;
    }

    /*************************************************************************/
    bool close()
    {
        return mixerClose(_hMixer) == MMSYSERR_NOERROR;
    }

    /*************************************************************************/
    bool GetMuteControl()
    {
        MIXERLINE mixerLine;
        mixerLine.cbStruct = sizeof(MIXERLINE);
        mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

        if (mixerGetLineInfo((HMIXEROBJ)_hMixer,
                             &mixerLine,
                             MIXER_OBJECTF_HMIXER |
                             MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
        {
            return FALSE;
        }

        MIXERCONTROL mixerControl;
        MIXERLINECONTROLS mixerLineControl;

        mixerLineControl.cbStruct         = sizeof(MIXERLINECONTROLS);
        mixerLineControl.dwLineID         = mixerLine.dwLineID;
        mixerLineControl.dwControlType    = MIXERCONTROL_CONTROLTYPE_MUTE;
        mixerLineControl.cControls        = 1;
        mixerLineControl.cbmxctrl         = sizeof(MIXERCONTROL);
        mixerLineControl.pamxctrl         = &mixerControl;

        if (mixerGetLineControls((HMIXEROBJ)_hMixer,
                                 &mixerLineControl,
                                 MIXER_OBJECTF_HMIXER |
                                 MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
        {
            return FALSE;
        }

        _controlID = mixerControl.dwControlID;

        return TRUE;
    }

    /*************************************************************************/
    bool GetMuteValue(LONG* value)
    {

        MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
        MIXERCONTROLDETAILS mixerControlDetails;

        mixerControlDetails.cbStruct          = sizeof(MIXERCONTROLDETAILS);
        mixerControlDetails.dwControlID       = _controlID;
        mixerControlDetails.cChannels         = 1;
        mixerControlDetails.cMultipleItems    = 0;
        mixerControlDetails.cbDetails         = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mixerControlDetails.paDetails         = &mxcdMute;

        if (mixerGetControlDetails((HMIXEROBJ)_hMixer,
                                     &mixerControlDetails,
                                     MIXER_OBJECTF_HMIXER
                                     | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
        {
            return FALSE;
        }

        *value = mxcdMute.fValue;
        return TRUE;
    }

    /*************************************************************************/
    bool SetMute( bool state)
    {

        MIXERCONTROLDETAILS_BOOLEAN mxcdMute = { (LONG) state };
        MIXERCONTROLDETAILS mixerControlDetails;

        mixerControlDetails.cbStruct        = sizeof(MIXERCONTROLDETAILS);
        mixerControlDetails.dwControlID     = _controlID;
        mixerControlDetails.cChannels       = 1;
        mixerControlDetails.cMultipleItems  = 0;
        mixerControlDetails.cbDetails       = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mixerControlDetails.paDetails       = &mxcdMute;

        if (mixerSetControlDetails((HMIXEROBJ)_hMixer,
                                     &mixerControlDetails,
                                     MIXER_OBJECTF_HMIXER
                                     | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
        {
            return FALSE;
        }
        return TRUE;
    }
};
/*
// Ejemplo:
Mixer mixer;
    if( mixer.init() && mixer.GetMuteControl() )
    {
        LONG currValue;
        mixer.GetMuteValue(&currValue);
        //std::cout << "Mute state:" <<  currValue << std::endl;

        mixer.SetMute(true);
        mixer.GetMuteValue(&currValue);
        //std::cout << "Mute state:" <<  currValue << std::endl;

        //mixer.SetMute(false);
        //mixer.GetMuteValue(&currValue);
        //std::cout << "Mute state:" <<  currValue << std::endl;

        mixer.close();
        return TRUE;
    }


//*/
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////

void set_mute( DWORD val, BOOL errmsg){
	MIXERCONTROL					mixerControlArray;
	MIXERLINECONTROLS				mixerLineControls;
	MIXERCONTROLDETAILS_UNSIGNED	value[2];
	MIXERCONTROLDETAILS				mixerControlDetails;
	MMRESULT						err;
    HMIXER				MixerHandle;

    MIXERLINE						mixerLine;
    mixerLine.cbStruct = sizeof(MIXERLINE);
    mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
    err = mixerOpen(&MixerHandle, (DWORD)HWaveOut, 0, 0, MIXER_OBJECTF_HWAVEOUT );
    err = mixerGetLineInfo((HMIXEROBJ)MixerHandle, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	/* First, does this source even have a mute switch? We'll check that by trying to
	 * fetch information about any mute control (if there is one). This will also
	 * fetch us the control's ID which we'll need later in order to set its value
	 */

	mixerLineControls.cbStruct = sizeof(MIXERLINECONTROLS);

	/* Tell mixerGetLineControls() for which line we're retrieving info.
	 * We do this by putting the desired line's ID number in dwLineID
	 */
	mixerLineControls.dwLineID = mixerLine.dwLineID;

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
	err = mixerGetLineControls((HMIXEROBJ)MixerHandle, &mixerLineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );

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
		mixerControlDetails.cChannels = mixerLine.cChannels;
		mixerControlDetails.dwControlID = MIXERCONTROL_CONTROLF_MULTIPLE;
		;
		if (mixerControlDetails.cChannels > 2) mixerControlDetails.cChannels = 2;
		if (mixerControlArray.fdwControl & MIXERCONTROL_CONTROLF_UNIFORM){
                //mixerControlDetails.cChannels = 1;
                //msgbox( "\n mixerControlDetails.cChannels = 1; \n" );
		}

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
        err = mixerSetControlDetails((HMIXEROBJ)MixerHandle, &mixerControlDetails, MIXER_SETCONTROLDETAILSF_VALUE);
		/* Set the value of the mute control for this line */
		mixerClose(MixerHandle);

}

















