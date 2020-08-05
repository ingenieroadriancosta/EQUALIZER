BOOL MAKENORMALIZE( void );
double GetEnergy( short* );
char                mpg123[1024];
char                HeaderWAV[1024];
DWORD               ThrId = 0;
PROCESS_INFORMATION Process_Information;
SECURITY_ATTRIBUTES security_attributes1;
SECURITY_ATTRIBUTES security_attributes2;
STARTUPINFOA        StartupInfoa;

short Clamp16Bits( int x ){
    if (x > 32767) return 32767;
    if (x < -32767) return -32767;
    return (short)x;
}

char* SetBuff( int NMBs ){
    static char* TBuff;
    TBuff = (char*)malloc( NMBs );
    if( TBuff==NULL ){ return NULL; }
    memset( &TBuff[0], 0, NMBs );
    return TBuff;
}
//
char* TFree( char* TBuffIn, int NMBs ){
    if( TBuffIn!=NULL ){ free( TBuffIn ); }
    return SetBuff( NMBs );
}
//


bool           Loaded = false;
bool           InKare = false;
char           WAV_TEMP[256];
int            NBytes = 100 * MBs;
char*          BUFFSONG = SetBuff( NBytes );
char*          BUFFSONGRES = SetBuff( NBytes );
short*         BUFFSHORT   = NULL;
int*           BUFFINT     = NULL;
short*         ORIGEN_BUFF = NULL;
const int      POT_BEEP = 1;
const int      MAX_BEEP = POT_BEEP + 1;
int            INDEX_AAUDIO = 1;
HWAVEOUT       HWaveOut;
HWAVEOUT       OLD_HWaveOut;
WAVEFORMATEX   WaveFormat;
WAVEHDR        WHeader;
int            FS = 44100;
BOOL           REPEAT = FALSE;
BOOL           SOUND_OK = TRUE;
int            FileSizeA = 0;
DWORD           DPos = 0;
int CorrScrl = 0;
double  CONSTTIME = 0.0;
bool           IN_START = false;
void StartSong( int );
VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );
void MAKEREVERV( void );
void Norm( void );
///
char Text2[1024];
BOOL START_SOUND( void ){
IN_START = true;

system( "cls" );
printf( "Indexs[IndexSong], %u\t%u\n\n", IndexSong, Indexs[IndexSong] );

IF SongFile[ 1023&Indexs[IndexSong] ][0]==0 THEN
   IN_START = false;
   return FALSE;
ENDIF;

waveOutReset( HWaveOut );
waveOutReset( HWaveOut );
waveOutClose( HWaveOut );

while( InPlt ){
    Sleep( 50 );
}
Sleep( 50 );


ZeroMemory( &Text2[0], 1023 );
GetFileTitleA( SongFile[ 1023&Indexs[IndexSong] ], &Text2[0], 1023 );

sprintf( TextoGlobal, "Cargando,\n%s", &Text2[0] );
SetWindowTextA( EDIT_INFO, TextoGlobal );




DWORD Readed = 0;
HANDLE HFile = CreateFile(
               SongFile[ 1023&Indexs[IndexSong] ],
               GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
FileSizeA = GetLastError();
IF FileSizeA!=0 THEN
   sprintf( TextoGlobal, "Error de lectura.\nFileSizeI, %i\nNombre de archivo:\n%s", FileSizeA, SongFile[ 1023&Indexs[IndexSong] ] );
   msgbox( TextoGlobal, "Error", 16 );
   IN_START = false;
   return FALSE;
ENDIF;

FileSizeA = GetFileSize( HFile, NULL);

BUFFSONG = TFree( BUFFSONG, (int)( FileSizeA + 10 * MBs ) );
if( BUFFSONG==NULL ){
    msgboxE( "BUFFSONG==NULL 1", " Error " );
    exit( 0 );
}

ReadFile( HFile, &BUFFSONG[0], FileSizeA, &Readed, NULL);
CloseHandle( HFile );
IF FileSizeA!=(int)Readed || FileSizeA==0 THEN
   sprintf( TextoGlobal, "Error de lectura.\nFileSizeI, %i", FileSizeA );
   msgbox( TextoGlobal, "Error", 16 );
   IN_START = false;
   return FALSE;
ENDIF;
printf( "CreateFile .wav\n" );






IF BUFFSONG[0]!='R' || BUFFSONG[1]!='I' || BUFFSONG[2]!='F' || BUFFSONG[3]!='F' THEN
   fmpg123( GLOBAL_ACTUAL_DIRECTORY );
   sprintf( WAV_TEMP, "%s\\Temp_wav.wav", GLOBAL_ACTUAL_DIRECTORY );
   printf( "CD:\n%s\n", GLOBAL_ACTUAL_DIRECTORY );
   system( "CLS" );
   sprintf( TextoGlobal,
            "call \"%s\\mpg123.exe\" -q -w \"%s\" \"%s\"",
            GLOBAL_ACTUAL_DIRECTORY,
            WAV_TEMP,
            (char*)&SongFile[ 1023&Indexs[IndexSong] ][0] );
   system( TextoGlobal );
   HFile = CreateFile(
           WAV_TEMP,
           GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    FileSizeA = GetFileSize( HFile, NULL);
    BUFFSONG = TFree( BUFFSONG, (int)( FileSizeA + 10 * MBs ) );
    if( BUFFSONG==NULL ){
        msgboxE( "BUFFSONG==NULL 2", " Error " );
        exit( 0 );
    }
    ReadFile( HFile, &BUFFSONG[0], FileSizeA, &Readed, NULL);
    CloseHandle( HFile );
    sprintf( TextoGlobal, "del \"%s\"", WAV_TEMP );
    system( TextoGlobal );
    printf( "CreateFile .mp3\n" );
ENDIF;






FOR int i=0; i<44; i++ LOOP
    HeaderWAV[i] = BUFFSONG[i];
ENDLOOP;

int* DatFs = (int*)&BUFFSONG[24];

FS = DatFs[0];
// FS = 24000;msgboxf( "%i", FS );
FOR int i=0; i<FileSizeA; i++ LOOP
    BUFFSONG[i] = BUFFSONG[i+44];
ENDLOOP;
FileSizeA = FileSizeA - 44;



short*         TBSHORT   = (short*)&BUFFSONG[0];
short maxVAbs = -1;
FOR int i=0; i<FileSizeA/2; i = i + 2 LOOP
    maxVAbs = max( abs(TBSHORT[i]), maxVAbs );
    maxVAbs = max( abs(TBSHORT[i+1]), maxVAbs );
ENDLOOP;
const double V2Mul = 32767.0/( (double)maxVAbs );
FOR int i=0; i<FileSizeA/2; i = i + 2 LOOP
    TBSHORT[i]   = Clamp16Bits( (int)round( V2Mul * TBSHORT[i] ) );
    TBSHORT[i+1] = Clamp16Bits( (int)round( V2Mul * TBSHORT[i+1] ) );
ENDLOOP;






BOOL SaveWAV( void );
BUFFSONGRES = TFree( BUFFSONGRES, (int)( FileSizeA + 10 * MBs ) );
if( BUFFSONGRES==NULL ){
    msgboxE( "BUFFSONGRES==NULL", " Error " );
    WHeader.dwBufferLength = 0;
    WaveFormat.nSamplesPerSec = 0;
    SaveWAV();
    exit( 0 );
}
FOR int i=0; i<FileSizeA; i++ LOOP
    BUFFSONGRES[i] = BUFFSONG[i];
ENDLOOP;

ORIGEN_BUFF = (short*)&BUFFSONGRES[0];
BUFFINT = (int*)&BUFFSONG[0];




DPos = 0;
CorrScrl = 0;
//MAKEREVERV();
StartSong( 0 );
printf( "StartSong( 0 );\n" );
sprintf( TextoGlobal, "Cargando,\n%s", &Text2[0] );
SetWindowTextA( EDIT_INFO, TextoGlobal );





CONSTTIME = ( (double)FileSizeA )/(double)FS;
CONSTTIME = CONSTTIME/4.0;


Loaded = true;

/*
const double EnergyC = GetEnergy();
sprintf( TextoGlobal, "Canción actual,\n%s\nEnergia = %5.4f\n\n\n%i Canciones.", Text2, EnergyC, IndexList );
SetWindowTextA( EDIT_INFO, TextoGlobal );
//*/
IN_START = false;
DeleteFileA( WAV_TEMP );
MAKENORMALIZE();
return TRUE;
}


DWORD VOLUMEN = 0;
////////////////////////////////
void StartSong( int SecondInit )
{
IF FileSizeA==0 THEN
   return;
ENDIF;
//sprintf( TextoGlobal, "%i", (FileSizeA - 44)/(4*44100) );
//msgbox( TextoGlobal );

KillTimer( VENTANA_PADRE, 1000 );



OLD_HWaveOut = HWaveOut;
//
//msgbox();
// Sleep( 1000 );
//DWORD IdTh = 0;
// CreateThread( NULL , 0 , CloseMsgbox, NULL , 0 , &IdTh );msgbox( "Cargando...", " EQUALIZER Mensaje " );



SetWindowTextA( EDIT_INFO, TextoGlobal );

memset(&WHeader , 0, sizeof( WHeader ));
WHeader.lpData = (LPSTR)&BUFFSONG[0];
WHeader.dwBufferLength = FileSizeA;
WHeader.dwBytesRecorded=0;
WHeader.dwUser = 0L;
WHeader.dwFlags = 0L;
WHeader.dwLoops = 0L;

WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
WaveFormat.nChannels = 2;
WaveFormat.nSamplesPerSec = FS;
WaveFormat.wBitsPerSample = 16;
WaveFormat.nBlockAlign = WaveFormat.nChannels * (WaveFormat.wBitsPerSample/8);
WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
WaveFormat.cbSize = 0;

waveOutOpen( &HWaveOut , WAVE_MAPPER , &WaveFormat , (DWORD)NULL , 0 , CALLBACK_NULL );
waveOutPrepareHeader( HWaveOut , &WHeader , sizeof(WAVEHDR) );


waveOutWrite( HWaveOut, &WHeader , sizeof(WAVEHDR) );



IN_PAUSE = FALSE;
SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon( INSTANCIA_GLOBAL, "IPAUSE") );


KillTimer( VENTANA_PADRE, 1000 );
HscrollPitch.Position = 0;
SetTimer( VENTANA_PADRE, 1000, 1000, (TIMERPROC)TimerProc );
}










MMTIME  mmtime = {0};
double  tim = 0;

char    TextAux[256];

int     PPitch = 0;
int     NAdv = 0;
VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime ){
IF( IN_PAUSE || IN_START==true ) THEN
    return;
ENDIF;
////
IF ( waveOutUnprepareHeader( HWaveOut , &WHeader , sizeof(WAVEHDR) ) == WAVERR_STILLPLAYING ) THEN
    //*
    waveOutGetPosition( HWaveOut , &mmtime , sizeof(MMTIME) );
    tim = ( ((double)mmtime.u.sample) ) / ( (double)WHeader.dwBufferLength ) ;
    tim = (double)CorrScrl + (double)( HscrollPitch.maxrange - CorrScrl ) * tim;
    tim = round( tim );
    // printf( "%5.3f  \t\r", tim * CONSTTIME/(double)HscrollPitch.maxrange );
    IF !IN_SCROLL && IN_START==false THEN
       SetScrollPos( HscrollPitch.Tag, SB_CTL, (int)(tim), TRUE );
       sprintf( TextAux, " %2i : %2i" , int(tim)/60, int(tim - 60*(int(tim)/60) ) );
       SetWindowText( TIME_INFO, TextAux );
    ENDIF;



    /*
    PPitch = int(tim);
    IF VAdvance[NAdv]<PPitch THEN
       SetWindowTextA( EDIT_INFO, SongTextEdit );
       SendMessage( EDIT_INFO , EM_LINESCROLL, 0, NVAdvance[NAdv] );
       NAdv++;
    ENDIF;


    IF IN_START==false THEN
       sprintf( TextAux, " %i : %i" , int(tim)/60, int(tim - 60*(int(tim)/60) ) );
       SetWindowText( TIME_INFO, TextAux );
    ENDIF;
    //*/
ELSE
    KillTimer( VENTANA_PADRE, 1000 );
    SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon( INSTANCIA_GLOBAL, "IPLAY") );
    IF IN_START==false THEN
       SetWindowTextA( EDIT_INFO, SongTextEdit );
    ENDIF;
    //*/
    //KillTimer( VENTANA_PADRE, 1000 );
    /*
    SetScrollPos( HscrollPitch.Tag, SB_CTL, int(0), TRUE );
    IndexSong = 255&(IndexSong + 1);
    START_SOUND();
    //*/
    HscrollPitch.Position = 0;
    PostMessageA( VENTANA_PADRE, WM_COMMAND, 1029, 0 );
ENDIF;

}






void MAKEAMP( void )
{
return;
AmplitudDELAY = SendMessage( HWndAmpliDELAY, CB_GETCURSEL, 0, (LPARAM)0 );
BUFFSHORT = (short*)&BUFFSONG[0];
FOR int i=0; i<25*MBs; i++ LOOP
    BUFFSHORT[i] = (short)(AmplitudDELAY*BUFFSHORT[i])/10;
ENDLOOP;

}





void Norm( void )
{
ORIGEN_BUFF = (short*)&BUFFSONGRES[0];
BUFFINT = (int*)&BUFFSONG[0];

Maxim = 0;
Minim = 0;

FOR int i=0; i<25*MBs; i++ LOOP
    IF BUFFSHORT[i]>Maxim THEN
       Maxim = BUFFSHORT[i];
    ENDIF;
    IF BUFFSHORT[i]<Minim THEN
       Minim = BUFFSHORT[i];
    ENDIF;
ENDLOOP;
Coef = float(32767);
IF (32767&Minim)>Maxim THEN
   Coef = Coef/double(32767&Minim);
ELSE
   Coef = Coef/double(Maxim);
ENDIF;


int COEFI = int(10000.0*Coef);
FOR int i=0; i<25*MBs; i++ LOOP
    BUFFSHORT[i] = (short)((COEFI*BUFFSHORT[i])/10000);
ENDLOOP;




/*
Maxim = 0;
Minim = 0;
FOR int i=0; i<25*MBs; i++ LOOP
    IF BUFFSHORT[i]>Maxim THEN
       Maxim = BUFFSHORT[i];
    ENDIF;
    IF BUFFSHORT[i]<Minim THEN
       Minim = BUFFSHORT[i];
    ENDIF;
ENDLOOP;
sprintf( TextoGlobal, "[%i, %i]\nCoef, %f", Maxim, Minim, Coef );
msgbox( TextoGlobal );
//*/
}
