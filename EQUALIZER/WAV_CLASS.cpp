#include "mpg123.cpp"
typedef struct   TagWAV_CLASS{
private:
    int                 PTRCLASS;
    DWORD               Fs;
    int                 Size;
    int                 Size_2;
    HWAVEOUT            HWaveOut;
    HWAVEOUT            OLD_HWaveOut;
    WAVEFORMATEX        WaveFormat;
    WAVEHDR             WHeader;
    double*             x;
    double*             y;
    short*              BufferD;
public:
    //
    int getptr(void){return PTRCLASS;}
    int GetSize(void){return (Size/4);}
    BOOL SetPitch(DWORD Pitch){
        waveOutSetPitch(HWaveOut, Pitch);
        return TRUE;
    }
    //
    ~TagWAV_CLASS(){
        if(x!=NULL){
            free(x);
        }
        if(y!=NULL){
            free(y);
        }
        if(BufferD!=NULL){
            free(BufferD);
        }
    }
    TagWAV_CLASS(){
        PTRCLASS = (int)&PTRCLASS;
        HWaveOut = NULL;
        OLD_HWaveOut = NULL;
        if(x!=NULL){
            free(x);
        }
        if(y!=NULL){
            free(y);
        }
        if(BufferD!=NULL){
            free(BufferD);
        }
        x = NULL;
        y = NULL;
        BufferD = NULL;
        Fs = 44100;
        Size = 0;
        Size_2 = 0;
        waveOutReset( HWaveOut );
        waveOutReset( HWaveOut );
        waveOutClose( HWaveOut );
    }
    //
    BOOL Close(void){
        waveOutPause( HWaveOut );
        waveOutRestart( HWaveOut );
        waveOutReset( HWaveOut );
        return TRUE;
    }
    //
    BOOL SetSin(int Frq, double chirp=0){
        if(BufferD!=NULL){
            free(BufferD);
        }
        if(x!=NULL){
            free(x);
        }
        if(y!=NULL){
            free(y);
        }
        Size = 3 * Fs * sizeof(short);
        Size_2 = Size/2;
        x = (double*)malloc( 3 * Fs * sizeof(double) );
        y = (double*)malloc( 3 * Fs * sizeof(double) );
        BufferD = (short*)malloc( Size );
        double FU = 0;
        if(chirp==0){
            for(int i=0;i<Size_2;i++){
                BufferD[i] = (short)(32767.0 * sin( (2.0 * pi * Frq * i)/((double)Fs)  ));
            }
        }else{
            for(int i=0;i<Size_2;i++){
                FU = ((double)i)/(chirp*Fs);
                BufferD[i] = (short)(32767.0 * sin( (2.0 * pi * Frq * i * FU)/((double)Fs)  ));
            }
        }
        return Play();
    }
    //
    BOOL PlaySong(char* FileT){
        DWORD Readed = 0;
        HANDLE HFile = CreateFile( FileT, GENERIC_READ,
                                   0, NULL, OPEN_ALWAYS,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
        DWORD FileSizeA = GetFileSize( HFile, NULL);
        char*  TBuffer = (char*)malloc( FileSizeA + 1024 );
        ReadFile( HFile, &TBuffer[0], FileSizeA, &Readed, NULL);
        CloseHandle( HFile );
        if( FileSizeA!=Readed || FileSizeA==0 ){
            free(TBuffer);
            msgboxf((char*)"Error de lectura.\nFileSizeI, %i", FileSizeA );
            return FALSE;
        }
        // MP3.
        if(TBuffer[0]!='R' || TBuffer[1]!='I' || TBuffer[2]!='F' || TBuffer[3]!='F'){
            fmpg123( (char*)"C:\\Users\\AdrianJCosta" );
            char WAV_TEMP[256];
            sprintf( WAV_TEMP, "%s\\Temp_wav.wav", "C:\\Users\\AdrianJCosta" );
            sprintf( TextoGlobal,
                     "call \"%s\\mpg123.exe\" -q -w \"%s\" \"%s\"",
                     "C:\\Users\\AdrianJCosta",
                     WAV_TEMP,
                     (char*)&FileT[0] );
            msgbox(TextoGlobal);
            system( TextoGlobal );
            HFile = CreateFile(WAV_TEMP,GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            FileSizeA = GetFileSize( HFile, NULL);
            free(TBuffer);
            TBuffer = (char*)malloc( FileSizeA + 1024 );
            ReadFile( HFile, &TBuffer[0], FileSizeA, &Readed, NULL);
            CloseHandle( HFile );
            sprintf( TextoGlobal, "del \"%s\"", WAV_TEMP );
            system( TextoGlobal );
        }
        if(BufferD!=NULL){
            free(BufferD);
        }
        Size = FileSizeA-44;
        Size_2 = Size/2;
        BufferD = (short*)malloc( Size );
        memcpy( &BufferD[0], &TBuffer[44], Size );
        free(TBuffer);
        FillDBuf();
        FillSBufX();
        return Play();
    }
    //
    BOOL FillDBuf(void){
        if(x!=NULL){free(x);}
        if(y!=NULL){free(y);}
        x = (double*)malloc( Size_2 * sizeof(double) );
        y = (double*)malloc( Size_2 * sizeof(double) );
        for(int i=0;i<Size_2/2;i++){
            x[2*i] = BufferD[2*i];
            x[2*i+1] = BufferD[2*i+1];
        }
        NormBufX();
        memcpy( &y[0], &x[0], Size_2 * sizeof(double) );
        return TRUE;
    }
    //
    //
    BOOL NormBufX(void){
        double MV = 0;
        for(int i=0;i<Size_2/2;i++){
            MV = max(x[2*i],MV);
            MV = max(x[2*i+1],MV);
        }
        for(int i=0;i<Size_2/2;i++){
            x[2*i] = x[2*i]/MV;
            x[2*i+1] = x[2*i+1]/MV;
        }
        return TRUE;
    }
    //
    BOOL NormBufY(void){
        double MV = 0;
        for(int i=0;i<Size_2/2;i++){
            MV = max(y[2*i],MV);
            MV = max(y[2*i+1],MV);
        }
        for(int i=0;i<Size_2/2;i++){
            y[2*i] = y[2*i]/MV;
            y[2*i+1] = y[2*i+1]/MV;
        }
        return TRUE;
    }
    //
    //
    BOOL FillSBufX(void){
        for(int i=0;i<Size_2/2;i++){
            BufferD[2*i] = (short)(32767 * x[2*i]);
            BufferD[2*i+1] = (short)(32767 * x[2*i+1]);
        }
        return TRUE;
    }
    //
    BOOL FillSBufY(void){
        for(int i=0;i<Size_2/2;i++){
            BufferD[2*i] = (short)(32767 * y[2*i]);
            BufferD[2*i+1] = (short)(32767 * y[2*i+1]);
        }
        //memcpy( &y[0], &x[0], Size_2 * sizeof(double) );
        return TRUE;
    }
    //
    //
    BOOL Play(){
        if(x==NULL || y==NULL || BufferD==NULL){
            return FALSE;
        }
        Close();
        memset(&WHeader , 0, sizeof( WHeader ));
        WHeader.lpData = (LPSTR)&BufferD[0];
        WHeader.dwBufferLength = Size;
        WHeader.dwBytesRecorded=0;
        WHeader.dwUser = 0L;
        WHeader.dwFlags = 0L;
        WHeader.dwLoops = 0L;
        WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
        WaveFormat.nChannels = 2;
        WaveFormat.nSamplesPerSec = Fs;
        WaveFormat.wBitsPerSample = 16;
        WaveFormat.nBlockAlign = WaveFormat.nChannels * (WaveFormat.wBitsPerSample/8);
        WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
        WaveFormat.cbSize = 0;
        waveOutOpen( &HWaveOut , WAVE_MAPPER , &WaveFormat , (DWORD)NULL , 0 , CALLBACK_NULL );
        waveOutPrepareHeader( HWaveOut , &WHeader , sizeof(WAVEHDR) );
        waveOutWrite( HWaveOut, &WHeader , sizeof(WAVEHDR) );
        return TRUE;
    }
    //
    //
    //
    //
    double* GetX(void){return x;}
    double* GetY(void){return y;}
    short*  GetD(void){return BufferD;}
    //
    BOOL ReSetVal(void){
        //memcpy( &BufferD[0], &Buffer[0], Size );
        return TRUE;
    }
    //
    //double GetValL(int DPos){return Buffer[2*DPos];}
    //
    //short GetValR(int DPos){return Buffer[2*DPos+1];}
    //

    //
    short GetValLD(int DPos){return BufferD[2*DPos];}
    //
    short GetValRD(int DPos){return BufferD[2*DPos+1];}
    //
    //
    BOOL SetValL( int DPos, short Value ){
        BufferD[2*DPos] = Value;
        return TRUE;
    }
    //
    BOOL SetValR( int DPos, short Value ){
        BufferD[2*DPos+1] = Value;
        return TRUE;
    }
    //
    //
}WAV_CLASS;
WAV_CLASS   WCLS;
