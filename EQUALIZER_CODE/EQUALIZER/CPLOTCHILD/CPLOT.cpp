class CPLOTCHILD{
    private:
        int                         CPtr;
        HWND                        HWFath;
        HWND                        HWPlot;
        HBITMAP                     HBitmap;
        RGBQUAD                     ColorBack;
        RGBQUAD                     ColorLine;
        RGBQUAD                     ColorLineG;
        RGBQUAD*                    RgbBuf;
        int                         WF;
        int                         HF;
        int                         XFR;
        int                         WFR;
        int                         YFR;
        int                         HFR;
        BOOL                        ESMB;
        BOOL                        ISMAXIM;
        BOOL                        MaxedCC;
        BOOL                        InPlotV;
        BOOL                        InPlot;
        BOOL                        InMaxim;
        BOOL                        SetRePlot;
        BOOL                        SetMaxim;
        BOOL                        BMaxim;
        BOOL                        InSL;
        BOOL                        DoubleBuffP;
        int                         GNPos;
        float                       MaxVal;
        float                       MaxValabs;
        float                       MinVal;
        float                       MinValabs;
        float*                      UCBuff;
        //
        BOOL FreeBuff(void){
            if( UCBuff ){ free(UCBuff); }
            UCBuff = NULL;
            GNPos = 0;
            MaxVal = 0;
            MinVal = 0;
            return TRUE;
        }
        //
        BOOL FillGrid(void);
        BOOL FillGrid( BOOL );
        BOOL plotPUC(void);
        // static LRESULT CALLBACK CPLOTWProcedure( HWND, UINT, WPARAM, LPARAM );
        static HBITMAP	NewBitmap( int W_NEWHB, int H_NEWHB ){ return CreateCompatibleBitmap( GetDC( 0 ), W_NEWHB, H_NEWHB );}
    public:
        BOOL plot( void );
        BOOL plot( unsigned char*, int );
        BOOL plot( char*, int );
        BOOL plot( unsigned short*, int );
        BOOL plot( short*, int );
        BOOL plotV( int );
        //
        BOOL Create( HWND );
        BOOL RegClass(void);
        //
        BOOL WVisible(void){ return IsWindowVisible(HWFath); };
        //
        ~CPLOTCHILD( void ){
            free( RgbBuf );
            free( UCBuff );
        }
        //
        CPLOTCHILD( void ){
            CPtr = 0;
            CPtr = (int)&CPtr;
            HWFath  = NULL;
            HBitmap = NULL;
            WF = 0;
            HF = 0;
            XFR = 1;
            YFR = 1;
            ColorBack.rgbGreen = 255;
            ColorBack.rgbBlue = 255;
            ColorBack.rgbRed = 255;
            ColorLine.rgbGreen = 0;
            ColorLine.rgbBlue = 0;
            ColorLine.rgbRed = 255;
            FreeBuff();
            RegClass();
            ISMAXIM = TRUE;
            MaxedCC = FALSE;
            InPlotV = FALSE;
            InPlot  = FALSE;
            SetRePlot  = FALSE;
            SetMaxim  = FALSE;
            InMaxim  = FALSE;
            DoubleBuffP  = FALSE;
            RgbBuf = (RGBQUAD*)malloc( 8 * GetSystemMetrics( SM_CXSCREEN ) * GetSystemMetrics( SM_CYSCREEN ) + 1024 );
        }
        //
        BOOL FSetMaxim( BOOL OptM ){
            if( InMaxim || ISMAXIM==FALSE ){
                return FALSE;
            }
            BMaxim = OptM;
            return BMaxim;
        }
        //
        //
        BOOL SetSize( int WSize, int HSize ){
            InSL = TRUE;
            if( WSize==WFR && HSize==HFR ){
                InSL = FALSE;
                return FALSE;
            }
            if( WSize<10 ){
                WSize = 10;
            }
            if( HSize<10 ){
                HSize = 10;
            }
            WFR = WSize;
            HFR = HSize;
            MoveWindow( HWPlot, XFR, YFR, WFR, HFR, TRUE );
            DeleteObject( HBitmap );
            HBitmap = NewBitmap( WFR, HFR );
            if( InPlot ){
                InSL = FALSE;
                return FALSE;
            }
            plot();
            InSL = FALSE;
            return TRUE;
        }
        //
        BOOL SetPos( int XPos, int YPos ){
            XFR = XPos;
            YFR = YPos;
            MoveWindow( HWPlot, XFR, YFR, WFR, HFR, TRUE );
            return TRUE;
        }
        //
        BOOL SetCont( HWND Cont ){
            HWFath = Cont;
            if( IsWindow( HWFath ) ){
                SetParent( HWPlot, HWFath );
                ShowWindow( HWPlot, SW_RESTORE );
            }
            return FALSE;
        }
        //
        //
        BOOL SetDoubleBuff( BOOL DblBf ){
            DoubleBuffP = DblBf;
            return DoubleBuffP;
        }
        //
        //
        BOOL PlaySoundM( DWORD FS=44100 ){
             short* BufSound = ( short*)malloc( GNPos * sizeof(short) + 1024 );
            for( int i=0; i<GNPos; i++ ){
                BufSound[i] = (short)( 32767 * (UCBuff[i] - 0.5)/0.5 );
            }
            WAVEFORMATEX   WaveFormat;
            WAVEHDR        WHeader;
            HWAVEOUT       HWaveOut;
            memset(&WHeader , 0, sizeof( WHeader ));
            WHeader.lpData = (LPSTR)&BufSound[0];
            WHeader.dwBufferLength = GNPos * sizeof(short);
            WHeader.dwBytesRecorded=0;
            WHeader.dwUser = 0L;
            WHeader.dwFlags = 0L;
            WHeader.dwLoops = 0L;
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 1;
            WaveFormat.nSamplesPerSec = FS;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nBlockAlign = WaveFormat.nChannels * (WaveFormat.wBitsPerSample/8);
            WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
            WaveFormat.cbSize = 0;
            //*
            waveOutOpen( &HWaveOut, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL );
            waveOutPrepareHeader( HWaveOut, &WHeader, sizeof(WAVEHDR) );
            waveOutWrite( HWaveOut, &WHeader, sizeof(WAVEHDR) );
            while( waveOutUnprepareHeader( HWaveOut, &WHeader, sizeof(WAVEHDR) ) == WAVERR_STILLPLAYING ){
                    Sleep( 100 );
            }
            waveOutClose( HWaveOut );
            //*/
            return TRUE;
        }
        //
};
CPLOTCHILD   CplotC;
