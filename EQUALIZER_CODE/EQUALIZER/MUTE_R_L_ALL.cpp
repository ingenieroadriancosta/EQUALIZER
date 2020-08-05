// #include "Mixer.cpp"
BOOL MUTE_R_L_ALL( BOOL BRMute, BOOL BLMute ){
	//out audio
	// set_mute( 1, FALSE);return FALSE;
    //
	//WAVEOUTCAPS   woc = {0};waveOutGetDevCaps( 0, &woc, sizeof(WAVEOUTCAPS) );
    // Get the number of Digital Audio Out devices in this computer
    //iNumDevs = waveOutGetNumDevs();
    //printf( "iNumDevs = %i\n" , iNumDevs );
   // Go through all of those devices, displaying their names
    // ShowWindow( HWndConsole, 3 );
    WORD BytR = 65535 * BRMute;
    WORD BytL = 65535 * BLMute;
    DWORD DWVol = MAKELONG( BytL, BytR );
    waveOutSetVolume( HWaveOut, MAKELONG( 65535, 65535 ) );
    waveOutSetVolume( HWaveOut, DWVol );
    CheckMenuItem( MENUGLOBAL, 71, 0 );
    CheckMenuItem( MENUGLOBAL, 70, 0 );
    if( BRMute ){
        SendDlgItemMessage( VENTANA_PADRE, 71, WM_SETTEXT, 0, (LPARAM)"R(On)" );
    }else{
        SendDlgItemMessage( VENTANA_PADRE, 71, WM_SETTEXT, 0, (LPARAM)"R(Off)" );
        CheckMenuItem( MENUGLOBAL, 71, 8 );
    }
    if( BLMute ){
        SendDlgItemMessage( VENTANA_PADRE, 70, WM_SETTEXT, 0, (LPARAM)"L(On)" );
    }else{
        SendDlgItemMessage( VENTANA_PADRE, 70, WM_SETTEXT, 0, (LPARAM)"L(Off)" );
        CheckMenuItem( MENUGLOBAL, 70, 8 );
    }
    return TRUE;
    //*
    // waveOutSetVolume( HWaveOut, MAKEWORD( 0, 255 ) );
    for( int i=0; i<65536; i = i + 16 ){
        BytR = i;
        BytL = 0;
        DWVol = MAKELONG( BytL, BytR );
        waveOutSetVolume( NULL, DWVol );
        printf( "%i   \r", i  );
        Sleep( 1 );
    }
    waveOutSetVolume( HWaveOut, MAKELONG( 65535, 65535 ) );
    waveOutSetVolume( NULL, MAKELONG( 65535, 65535 ) );
    // printf( "%i", waveOutSetVolume( HWaveOut, DWVol ) );
    //*/
    ShowWindow( HWndConsole, 0 );
    return TRUE;
}
