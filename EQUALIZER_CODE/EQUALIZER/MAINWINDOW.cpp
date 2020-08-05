MAINWINDOW


// msgboxf( "%X", (WS_OVERLAPPEDWINDOW ) - ( WS_MAXIMIZEBOX | WS_SIZEBOX | WS_MINIMIZEBOX ) );

SetUnhandledExceptionFilter(OurCrashHandler);
IF FindWindowA( szClassName, szClassName ) THEN
   PostMessage( FindWindowA( szClassName, szClassName ), WM_COMMAND, 10, 0);
   return 0;
ENDIF;
AllocConsole();
HWndConsole = GetConsoleWindow();
ShowWindow( HWndConsole, 0 );
EnableWindow( HWndConsole, FALSE );
GetModuleInfo( INSTANCIA_GLOBAL );
sprintf( mpg123, "%s\\mpg123.exe ", GLOBAL_ACTUAL_DIRECTORY  );


if( Indexs==NULL ){
    Indexs = (int*)malloc( 2048 * sizeof(int) );
}

regclass( hThisInstance, WindowProcedure, szClassName, "A", NULL, 0, 0, 0 );
beforeW = 400;
beforeH = 400;
int CXSCREEN = GetSystemMetrics( SM_CXSCREEN );
int CYSCREEN = GetSystemMetrics( SM_CYSCREEN );
beforeX = (beforeW+CXSCREEN)/2 - beforeW;
beforeY = (beforeH+CYSCREEN)/2 - beforeH;
INSTANCIA_GLOBAL = hThisInstance;
VENTANA_PADRE = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED,
                szClassName, szClassName,
                WS_POPUP,
                CXSCREEN - beforeW, CYSCREEN - beforeH, beforeW, beforeH, HWND_DESKTOP, NULL,
                hThisInstance, NULL );
HWList = NULL;
HBitmap = loadImage( (char*)"FONDO.jpg" );
HDCMAIN = CreateCompatibleDC( GetDC( 0 ) );
SelectObject( HDCMAIN , HBitmap );
DeleteObject( HBitmap );
#define FSTATIC 128
pincel = CreateSolidBrush(RGB( FSTATIC, FSTATIC, FSTATIC));
/*

DWORD cbSize;
HWND hWnd;
UINT uID;
UINT uFlags;
UINT uCallbackMessage;
HICON hIcon;
*/

nid.hWnd = VENTANA_PADRE;
nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
nid.hIcon = LoadIcon( hThisInstance , "A" );
memcpy( nid.szTip , szClassName , 64 );



nid.uCallbackMessage =  WM_MOUSEMOVE;
nid.uID = 12345;
nid.cbSize = sizeof(nid);
nid.cbSize = WM_USER+20;
Shell_NotifyIconA( NIM_ADD , &nid);


ORIGEN_BUFF = (short*)&BUFFSONGRES[0];
BUFFSHORT = (short*)&BUFFSONG[0];
BUFFINT = (int*)&BUFFSONG[0];





CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "Abrir audio",
       WS_CHILD | WS_VISIBLE | WS_BORDER
       | BS_MULTILINE
       ,
       1, 201, 80, 50,
       VENTANA_PADRE, (HMENU)99, hThisInstance, NULL );



CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "Abrir lista",
       WS_CHILD | WS_VISIBLE | WS_BORDER
       | BS_MULTILINE
       ,
       140, 201, 80, 50,
       VENTANA_PADRE, (HMENU)89, hThisInstance, NULL );


CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "Guardar lista",
       WS_CHILD | WS_VISIBLE | WS_BORDER
       | BS_MULTILINE
       ,
       290, 251, 80, 50,
       VENTANA_PADRE, (HMENU)90, hThisInstance, NULL );





CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "Lista",
       WS_CHILD | WS_VISIBLE | WS_BORDER
       ,
       200, 251, 80, 50,
       VENTANA_PADRE, (HMENU)69, hThisInstance, NULL );



/// ///////////////////////////////////////////////////////////////////////////////////////    MUTE
CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "L(On)",
       WS_CHILD | WS_VISIBLE | WS_BORDER
       | BS_MULTILINE
       ,
       1, 261, 80, 30,
       VENTANA_PADRE, (HMENU)70, hThisInstance, NULL );


CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "R(On)",
       WS_CHILD | WS_VISIBLE | WS_BORDER
       | BS_MULTILINE
       ,
       91, 261, 80, 30,
       VENTANA_PADRE, (HMENU)71, hThisInstance, NULL );
/// ///////////////////////////////////////////////////////////////////////////////////////    MUTE




HWPlay = CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "",
            WS_CHILD | WS_VISIBLE |//WS_BORDER |
            BS_ICON,
            1, beforeH-100 + 2 + 25, 32, 32,
            VENTANA_PADRE, (HMENU)100, hThisInstance, NULL );
SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(hThisInstance, "IPLAY") );


HWStop = CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "A",
            WS_CHILD | WS_VISIBLE |//WS_BORDER |
            BS_ICON,
            34, beforeH-100 + 2 + 25, 32, 32,
            VENTANA_PADRE, (HMENU)101, hThisInstance, NULL );
SendMessage( HWStop, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(hThisInstance, "ISTOP") );





HWRw = CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "A",
            WS_CHILD | WS_VISIBLE |//WS_BORDER |
            BS_ICON,
            84, beforeH-100 + 2 + 25, 32, 32,
            VENTANA_PADRE, (HMENU)1026, hThisInstance, NULL );
SendMessage( HWRw, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(hThisInstance, "IRW") );

HWFw = CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "A",
            WS_CHILD | WS_VISIBLE |//WS_BORDER |
            BS_ICON,
            120, beforeH-100 + 2 + 25, 32, 32,
            VENTANA_PADRE, (HMENU)1025, hThisInstance, NULL );
SendMessage( HWFw, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(hThisInstance, "IFW") );



CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "Repetir\n(Off)",
            WS_CHILD | WS_VISIBLE  | BS_MULTILINE,
            160, beforeH-100 + 2 + 25, 70, 38,
            VENTANA_PADRE, (HMENU)1028, hThisInstance, NULL );


CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "Random\n(Off)",
            WS_CHILD | WS_VISIBLE  | BS_MULTILINE,
            234, beforeH-100 + 2 + 25, 70, 38,
            VENTANA_PADRE, (HMENU)1030, hThisInstance, NULL );



HWEXIT = CreateWindowEx( WS_EX_TOPMOST , "BUTTON", "A",
            WS_CHILD | WS_VISIBLE |//WS_BORDER |
            BS_ICON,
            308, beforeH-100 + 2 + 25, 32, 32,
            VENTANA_PADRE, (HMENU)1, hThisInstance, NULL );
SendMessage( HWEXIT, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(hThisInstance, "IEXIT") );



TIME_INFO = CreateWindowEx( WS_EX_TOPMOST , "STATIC", " ",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            1, beforeH-100 + 2 + 25 + 48, beforeW-2, 24,
            VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );




HscrollVol.maxrange = 100;
HscrollVol.minrange = 0;
HscrollVol.create( true, beforeW-27, 1, 25, beforeH - 101 );
waveOutOpen( &HWaveOut , WAVE_MAPPER , &WaveFormat , (DWORD)NULL , 0 , CALLBACK_NULL );
waveOutGetVolume( HWaveOut, &VOLUMEN );
HscrollVol.Position = 100  - (100*LOWORD( VOLUMEN )/65535);
SetScrollPos( HscrollVol.Tag , SB_CTL , HscrollVol.Position, TRUE );
waveOutRestart( HWaveOut );
waveOutReset( HWaveOut );
waveOutClose( HWaveOut );





HscrollPitch.maxrange = 260;
HscrollPitch.minrange = 0;
HscrollPitch.Father = VENTANA_PADRE;
HscrollPitch.create( false, 1, beforeH-100 + 1, beforeW-1, 25 );



EDIT_INFO = CreateWindowEx( WS_EX_TOPMOST , "STATIC", SongTextEdit,
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       1, 1, beforeW-30, 200,
       VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );
/*
CplotC.SetDoubleBuff( FALSE );
CplotC.SetSize( beforeW-30, 100 );
CplotC.SetPos( 1, 101 );
CplotC.Create( VENTANA_PADRE );
// CplotC.plot( (unsigned short*)&US_EFRC_F0_P2[0], 65536 );
//*/


HWndVALPHA =
CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                beforeW-50, beforeH-100 + 2 + 25, 50, beforeH - 100,
                VENTANA_PADRE, (HMENU)512, INSTANCIA_GLOBAL, NULL );
FOR int i=0; i<256; i++ LOOP
    sprintf( TextoGlobal, "%i", i );
    SendMessage( HWndVALPHA, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndVALPHA, CB_SETCURSEL, VALPHA, (LPARAM)0 );
SetLayeredWindowAttributes( VENTANA_PADRE, 0, VALPHA, LWA_ALPHA);




#define WWW_PRB 68
HWndNormalize =
CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                2*WWW_PRB+150 + 38, 203, 48, beforeH - 100,
                VENTANA_PADRE, (HMENU)519, INSTANCIA_GLOBAL, NULL );
FOR int i=0; i<101; i++ LOOP
    sprintf( TextoGlobal, "%i", i );
    SendMessage( HWndNormalize, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndNormalize, CB_SETCURSEL, DELAY, (LPARAM)0 );



/*
CreateWindowEx( WS_EX_TOPMOST , "STATIC", "LOWPASS",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       1, 201, WWW_PRB, 20,
       VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );
HWndLOW =
CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                1, 221, WWW_PRB, beforeH - 100,
                VENTANA_PADRE, (HMENU)516, INSTANCIA_GLOBAL, NULL );
FOR int i=0; i<17; i=i+1 LOOP
    sprintf( TextoGlobal, "%i", i );
    SendMessage( HWndLOW, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndLOW, CB_SETCURSEL, 0, (LPARAM)0 );







CreateWindowEx( WS_EX_TOPMOST , "STATIC", "HIPASS",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       WWW_PRB+2, 201, WWW_PRB, 20,
       VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );
HWndHI =
CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                WWW_PRB+2, 221, WWW_PRB, beforeH - 100,
                VENTANA_PADRE, (HMENU)517, INSTANCIA_GLOBAL, NULL );
FOR int i=0; i<256; i=i+1 LOOP
    sprintf( TextoGlobal, "%i", i );
    SendMessage( HWndHI, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndHI, CB_SETCURSEL, KERN_LOW[1], (LPARAM)0 );









  REVERV.


CreateWindowEx( WS_EX_TOPMOST , "STATIC", "DELAY(ms)",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       2*WWW_PRB+50, 201, 86, 20,
       VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );
HWndDELAY =
CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                2*WWW_PRB+50, 221, 86, beforeH - 100,
                VENTANA_PADRE, (HMENU)514, INSTANCIA_GLOBAL, NULL );
FOR int i=0; i<101; i++ LOOP
    sprintf( TextoGlobal, "%i", i );
    SendMessage( HWndDELAY, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndDELAY, CB_SETCURSEL, DELAY, (LPARAM)0 );




CreateWindowEx( WS_EX_TOPMOST , "STATIC", "DELAY(Vol)",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       2*WWW_PRB+50+88, 201, 86, 20,
       VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );
HWndAmpliDELAY =
CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                2*WWW_PRB+50+88, 221, 86, beforeH - 100,
                VENTANA_PADRE, (HMENU)515, INSTANCIA_GLOBAL, NULL );
FOR int i=0; i<256; i++ LOOP
    sprintf( TextoGlobal, "%u", i );
    SendMessage( HWndAmpliDELAY, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndAmpliDELAY, CB_SETCURSEL, AmplitudDELAY, (LPARAM)0 );






CreateWindowEx( WS_EX_TOPMOST , "STATIC", "DOPPLER",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       1, 250, 86, 20,
       VENTANA_PADRE, (HMENU)NULL, hThisInstance, NULL );
HWndDOPPLER = CreateWindowEx( 0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                1, 271, 86, beforeH - 100,
                VENTANA_PADRE, (HMENU)518, INSTANCIA_GLOBAL, NULL );
FOR int i=-5; i<=5; i++ LOOP
    sprintf( TextoGlobal, "%i", i );
    SendMessage( HWndDOPPLER, CB_ADDSTRING, 0, (LPARAM)TextoGlobal );
ENDLOOP;
SendMessage( HWndDOPPLER, CB_SETCURSEL, 5, (LPARAM)0 );
//*/






SetLayeredWindowAttributes( VENTANA_PADRE, 0, VALPHA, LWA_ALPHA);
ShowWindow( VENTANA_PADRE, 1 );
ShowWindow( VENTANA_PADRE, 1 );

DWORD IdTh = 0;
CreateThread( NULL , 0 , ThreadCommands , NULL , 0 , &IdTh );
///


IdTh = 0;
CreateThread( NULL , 0 , PlayNBCommands, NULL , 0 , &IdTh );
///



// ShowWindow( HscrollPitch.Tag, 0 );

FILE * pFile = NULL;
pFile = fopen( "c:\\myfile.lst" , "rb" );
IF pFile THEN
   int result = 0;
   char buffer[2] = {};
   int Ni = 0;
   int IndLi = 0;
   IndexList = 0;
   BOOL IndSeted = FALSE;
   WHILE( 1 )
       //
       result = fread( &buffer[0], 1, 1, pFile );
       IF result!=1 THEN
          SongFile[IndexList][0] = 0;
          break;
       ENDIF;
       //
       IF buffer[0]=='\n' THEN
          SongFile[IndexList][Ni] = 0;
          IndexList++;
          if( IndSeted==FALSE ){
            IndSeted = TRUE;
            IndLi = (int)atoi( &SongFile[0][0] );
            IndexList = 0;
          }
          Ni = 0;
       ELSE
          SongFile[IndexList][Ni] = buffer[0];
          Ni++;
       ENDIF;
   ENDLOOP;
   fclose( pFile );
   IndexSong = IndLi;
    for( int i=0; i<IndexList; i++ ){
        Indexs[i] = i;
    }
   // msgboxf( 0, "", "IndexList, %u\nIndexSong %u \n%s", IndexList, IndexSong, &SongFile[0][0] );
ENDIF;
FOR int i=0; i<IndexList; i=i+1 LOOP
    //msgbox( SongFile[i] );
ENDLOOP;
for( int i=0; i<1024; i++ ){
    Indexs[i] = i;
}


// ShowWindow( HWndConsole, 3);

MENUGLOBAL = LoadMenu( INSTANCIA_GLOBAL , "Menu" );
MSG messages;
HACCEL hAcelerador;
hAcelerador = LoadAccelerators( hThisInstance , "aceleradores" );
WHILE( GLOBALQUIT_GLOBAL==false )
    GetMessage (&messages, NULL, 0, 0);
    IF !TranslateAccelerator( VENTANA_PADRE, hAcelerador, &messages ) THEN
       TranslateMessage(&messages);
    ENDIF;
    DispatchMessage(&messages);
ENDWHILE
ENDMAINWINDOW
