DWORD WINAPI Thread_PLOT_M( LPVOID pv=NULL ){
    if( InPlt ){ return 0; }
    InPlt = TRUE;
    MMTIME  mmtime = {0};
    const int TopPlot = FS/20;
    const int TopP = 2 * NBytes;
    unsigned short* B2PlotT = (unsigned short*)(&BUFFSONGRES[0]);
    unsigned short* B2Plot = (unsigned short*)malloc( ( FileSizeA * sizeof(short) )/WaveFormat.nChannels );
    for( int i=0; i<(FileSizeA/(4*WaveFormat.nChannels)); i++ ){
        B2Plot[i] = B2PlotT[2*i];
    }
    while( waveOutUnprepareHeader( HWaveOut, &WHeader, sizeof(WAVEHDR) ) == WAVERR_STILLPLAYING ){
        waveOutGetPosition( HWaveOut , &mmtime , sizeof(MMTIME) );
        if( IsWindowVisible(VENTANA_PADRE) ){
            if( (int)mmtime.u.sample<TopP && ((int)mmtime.u.sample/4 - TopPlot)>0 ){
                CplotC.plot( (short*)&B2Plot[mmtime.u.sample/4 - TopPlot], TopPlot );
            }
        }else{
            while( !IsWindowVisible(VENTANA_PADRE) ){
                if( IN_START ){
                    free( B2Plot );
                    InPlt = FALSE;
                    return 0;
                }
                Sleep( 100 );
            }
        }
        if( IN_START ){
            free( B2Plot );
            InPlt = FALSE;
            return 0;
        }
        Sleep( 10 );
    }
    free( B2Plot );
    InPlt = FALSE;
    return 0;
}
