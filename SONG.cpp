char                mpg123[1024];
DWORD               ThrId = 0;
PROCESS_INFORMATION Process_Information;
SECURITY_ATTRIBUTES security_attributes1;
SECURITY_ATTRIBUTES security_attributes2;
STARTUPINFOA        StartupInfoa;

bool           Loaded = false;
char           WAV_TEMP[256];
char           BUFFSONG[50*MBs];
char           BUFFSONGRES[50*MBs];
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
bool           IN_START = false;
void StartSong( int );
VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );
void MAKEREVERV( void );
void Norm( void );
///
BOOL START_SOUND( void ){
IN_START = true;
IF SongFile[IndexSong][0]==0 THEN
   IN_START = false;
   return FALSE;
ENDIF;
waveOutReset( HWaveOut );
waveOutReset( HWaveOut );
waveOutClose( HWaveOut );

sprintf( TextoGlobal, "Cargando,\n%s", SongFile[IndexSong] );
SetWindowTextA( EDIT_INFO, TextoGlobal );

memset( &BUFFSONG[0], 0, 50*MBs );
DWORD Readed = 0;
HANDLE HFile = CreateFile(
               SongFile[IndexSong],
               GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
FileSizeA = GetFileSize( HFile, NULL);
ReadFile( HFile, &BUFFSONG[0], FileSizeA, &Readed, NULL);
CloseHandle( HFile );
IF FileSizeA!=Readed || FileSizeA==0 THEN
   sprintf( TextoGlobal, "Error de lectura.\nFileSizeI, %i", FileSizeA );
   msgbox( TextoGlobal, "Error", 16 );
   IN_START = false;
   return FALSE;
ENDIF;







IF BUFFSONG[0]!='R' || BUFFSONG[1]!='I' || BUFFSONG[2]!='F' || BUFFSONG[3]!='F' THEN
   fmpg123( GLOBAL_ACTUAL_DIRECTORY );
   sprintf( WAV_TEMP, "%s\\Temp_wav.wav", GLOBAL_ACTUAL_DIRECTORY );
   sprintf( TextoGlobal,
            "call \"%s\\mpg123.exe\" -q -w \"%s\" \"%s\"",
            GLOBAL_ACTUAL_DIRECTORY,
            WAV_TEMP,
            (char*)&SongFile[IndexSong][0] );
   system( TextoGlobal );
   HFile = CreateFile(
           WAV_TEMP,
           GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   FileSizeA = GetFileSize( HFile, NULL);
   ReadFile( HFile, &BUFFSONG[0], FileSizeA, &Readed, NULL);
   CloseHandle( HFile );
   sprintf( TextoGlobal, "del \"%s\"", WAV_TEMP );
   system( TextoGlobal );
ENDIF;













FOR int i=0; i<FileSizeA; i++ LOOP
    BUFFSONG[i] = BUFFSONG[i+44];
ENDLOOP;
FileSizeA = FileSizeA - 44;

FOR int i=0; i<50*MBs; i++ LOOP
    BUFFSONGRES[i] = BUFFSONG[i];
ENDLOOP;

ORIGEN_BUFF = (short*)&BUFFSONGRES[0];
BUFFINT = (int*)&BUFFSONG[0];







//MAKEREVERV();
StartSong( 0 );
sprintf( TextoGlobal, "Cargando,\n%s", SongFile[IndexSong] );
SetWindowTextA( EDIT_INFO, TextoGlobal );






Loaded = true;

sprintf( TextoGlobal, "Canción actual,\n%s", SongFile[IndexSong] );
SetWindowTextA( EDIT_INFO, TextoGlobal );
IN_START = false;
DeleteFileA( WAV_TEMP );
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


OLD_HWaveOut = HWaveOut;
waveOutRestart( OLD_HWaveOut );
waveOutReset( OLD_HWaveOut );
waveOutClose( OLD_HWaveOut );

memset(&WHeader , 0, sizeof( WHeader ));
WHeader.lpData = (LPSTR)&BUFFSONG[SecondInit*(44100*4)];
WHeader.dwBufferLength = FileSizeA - SecondInit*(44100*4);
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
SetTimer( VENTANA_PADRE, 1000, 800, (TIMERPROC)TimerProc );
}










MMTIME  mmtime;
double  tim = 0;
double  CONSTTIME = 0.0;
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
    CONSTTIME = (WaveFormat.nChannels * WaveFormat.wBitsPerSample * WaveFormat.nSamplesPerSec)/8.0;
    tim = ( (mmtime.u.sample)/CONSTTIME ) + HscrollPitch.Position;
    
    IF !IN_SCROLL THEN
       SetScrollPos( HscrollPitch.Tag, SB_CTL, (int)(tim), TRUE );
    ENDIF;
    
    
    
    /*
    PPitch = int(tim);
    IF VAdvance[NAdv]<PPitch THEN
       SetWindowTextA( EDIT_INFO, SongTextEdit );
       SendMessage( EDIT_INFO , EM_LINESCROLL, 0, NVAdvance[NAdv] );
       NAdv++;
    ENDIF;
    //*/
    
    IF IN_START==false THEN
       sprintf( TextAux, " %i : %i\tNAdv, %i\tDELAY, %i" , int(tim)/60, int(tim - 60*(int(tim)/60) ), NAdv, DELAY );
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
    PostMessageA( VENTANA_PADRE, WM_COMMAND, 1025, 0 );
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
