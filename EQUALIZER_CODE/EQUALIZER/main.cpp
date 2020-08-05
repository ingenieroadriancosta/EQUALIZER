#include <DEF.h>
#include <DEF_FUNC.h>

// #include "CPLOTCHILD\CPLOT.h"


#include "LILI.h"
#include "mpg123.cpp"
DWORD WINAPI CloseMsgbox( LPVOID );
DWORD WINAPI PlayNBCommands( LPVOID );
#include "GVARS.cpp"





#include "OurCrashHandler.cpp"


void functry(){
ShowWindow( HWndConsole, 3);
system("CLS");
sprintf(TextoGlobal,
        "ECHO "
        "IndexSong: %i\n"
        "IndexList: %i\n"
        "Song: %s", IndexSong, IndexList, &SongFile[IndexSong][0] );
system(TextoGlobal);
Sleep( 3000 );
}


#include "OPEN_AND_SAVE_EQUALIZER.cpp"



#include "SCROLLBAR.cpp"
#include "SONG.cpp"
#include "MAKEREVERV.cpp"
#include "MAKENORMALIZE.cpp"


#include "MAKELOWPASS.cpp"
#include "MAKEHIPASS.cpp"

#include "DOPPLER.cpp"
#include "MUTE_R_L_ALL.cpp"



#include "SaveWAV.cpp"
#include "SaveList.cpp"

#include "LISTMNG.cpp"


// #include "PLOT_M.cpp"
#include "Copy2Folder.cpp"



#include "MAINWINDOW.cpp"



WPROC
MESSAGES
case WM_COMMAND:

    if( LOWORD(wParam)==1 ){
        waveOutSetVolume( HWaveOut, MAKELONG( 65535, 65535 ) );
        waveOutRestart( HWaveOut );
        waveOutReset( HWaveOut );
        waveOutClose( HWaveOut );
        EndDialog( HWList, 0 );
        PostMessage(VENTANA_PADRE, WM_CLOSE, 0, 0);
        return 0;
    }
     IF LOWORD(wParam)==69 THEN
        if( HWList==NULL ){
                DialogBoxParam( INSTANCIA_GLOBAL, "LISTMNG", NULL, LISTMNG_PROC, 0 );
        }else{
            SetWindowPos( HWList, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            if( !AlwOnTop ){
                SetWindowPos( HWList, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            }
            ShowWindow( HWList, 1 );
        }
        return 0;
     ENDIF;


     IF LOWORD(wParam)==519 && HIWORD(wParam)==CBN_SELENDOK  THEN
        MAKENORMALIZE();
        break;
     ENDIF;





    IF LOWORD(wParam)==50 THEN
        ShowWindow( HWndConsole, !IsWindowVisible(HWndConsole) );
        return 0;
     ENDIF;

     IF LOWORD(wParam)==10 THEN
        SHOWWINDOW = TRUE;
        ShowWindow( VENTANA_PADRE, SHOWWINDOW );
        return 0;
     ENDIF;

     IF LOWORD(wParam)==512 THEN
        VALPHA = 255&SendMessage( HWndVALPHA, CB_GETCURSEL, 0, (LPARAM)0 );
        IF VALPHA<=20 THEN
           VALPHA = 20;
           SendMessage( HWndVALPHA, CB_SETCURSEL, VALPHA, (LPARAM)0 );
        ENDIF;
        SetLayeredWindowAttributes( VENTANA_PADRE, 0, VALPHA, LWA_ALPHA);
        return 0;
     ENDIF;


     IF LOWORD(wParam)>=512 && LOWORD(wParam)<612 && HIWORD(wParam)!=CBN_SELCHANGE THEN
        return 0;
     ENDIF;

     IF INPROCESS==true THEN
        return 0;
     ELSE
        wParamC = wParam;
        lParamC = lParam;
        HWThread = hwnd;
        DWORD IdTh = 0;
        CreateThread( NULL , 0 , ThreadCommands , NULL , 0 , &IdTh );
        return 0;
     ENDIF;
     break;


case WM_VSCROLL:
case WM_HSCROLL:
    if( IN_START ){
        break;
    }
     IN_SCROLL = true;
     IF HscrollPitch.ProcesarScrollH( wParam , lParam )!=0 THEN
        CorrScrl = HscrollPitch.Position;
        DPos = ( FileSizeA * ((float)HscrollPitch.Position)/( (float)HscrollPitch.maxrange ) );
        while( DPos%4!=0 ){
            DPos--;
        }
        double ISeg = CONSTTIME * CorrScrl/( (double)HscrollPitch.maxrange );
        sprintf( TextAux, " %2i : %2i" , (int)(ISeg/60), (int)((ISeg) - 60 * floor(ISeg/60)) );
        SetWindowText( TIME_INFO, TextAux );
        WHeader.lpData = (LPSTR)&BUFFSONG[DPos];
        WHeader.dwBufferLength = FileSizeA - DPos;
        // waveOutWrite( HWaveOut, &WHeader , sizeof(WAVEHDR) );
        waveOutReset(HWaveOut);
        waveOutPrepareHeader( HWaveOut , &WHeader , sizeof(WAVEHDR) );
        waveOutWrite( HWaveOut, &WHeader , sizeof(WAVEHDR) );
        if( IN_PAUSE ){
            IN_PAUSE = FALSE;
            SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon( INSTANCIA_GLOBAL, "IPAUSE") );
        }
        Sleep( 50 );
        IN_SCROLL = false;
        // printf( "[%i, %i]\n", (int)FileSizeA, (int)DPos );
        // msgboxf( "%i", DPos );
        break;
        StartSong( HscrollPitch.Position );


        waveOutGetPosition( HWaveOut , &mmtime , sizeof(MMTIME) );
        CONSTTIME = (WaveFormat.nChannels * WaveFormat.wBitsPerSample * WaveFormat.nSamplesPerSec)/8.0;
        tim = ( (mmtime.u.sample)/CONSTTIME ) + HscrollPitch.Position;
        PPitch = int(tim);
        NAdv = 0;
        while( !(VAdvance[NAdv]<PPitch && VAdvance[NAdv+1]>PPitch) )
            NAdv++;



        TimerProc( hwnd, 0, 0, 0 );


     ENDIF;


     IF HscrollVol.ProcesarScrollH( wParam , lParam ) THEN
        waveOutSetVolume( HWaveOut , MAKELONG( (65535*(100-HscrollVol.Position))/100, (65535*(100-HscrollVol.Position))/100) );
     ENDIF;
     IN_SCROLL = false;
     break;

case WM_LBUTTONDBLCLK:
     wParam = 12345;
     lParam = WM_LBUTTONDOWN;
case WM_MOUSEMOVE:
     IF wParam==12345  THEN
        IF lParam==WM_LBUTTONDOWN THEN
           SHOWWINDOW = !SHOWWINDOW;
           ShowWindow( VENTANA_PADRE, SHOWWINDOW );
           break;
        ENDIF;
        /*
        IF lParam==WM_RBUTTONUP THEN
           int pos_x = 0;
           int pos_y = 0;
           CursorPos( pos_x , pos_y );
           HMENU popMenu = GetSubMenu( menu1 , 0 );
           TrackPopupMenu( popMenu, 0, pos_x, pos_y, 0, hwnd, NULL );
           break;
        ENDIF;
        //*/
     ENDIF;


    if( lParam==WM_RBUTTONDOWN ){
        int CursorPos_x = 0;
        int CursorPos_y = 0;
        CursorPos( &CursorPos_x, &CursorPos_y );
        HMENU popMenu = GetSubMenu( MENUGLOBAL , 0 );
        TrackPopupMenu( popMenu , 0 ,CursorPos_x , CursorPos_y , 0 , hwnd , NULL );
        return 0;
    }
     break;

case WM_RBUTTONDOWN:{
    int CursorPos_x = 0;
    int CursorPos_y = 0;
    CursorPos( &CursorPos_x, &CursorPos_y );
    HMENU popMenu = GetSubMenu( MENUGLOBAL , 0 );
    TrackPopupMenu( popMenu , 0 ,CursorPos_x , CursorPos_y , 0 , hwnd , NULL );
    }
    break;
case WM_CTLCOLORSCROLLBAR:
     pincel = CreateSolidBrush(RGB( 0*FSTATIC, 0*FSTATIC, FSTATIC));
     SetTextColor( (HDC)wParam, RGB( 0, 0, 0));
     SetBkColor( (HDC)wParam, RGB( 0*FSTATIC, 0*FSTATIC, FSTATIC));
     return (LRESULT)pincel;
     break;
case WM_CTLCOLORSTATIC:
     pincel = CreateSolidBrush(RGB( FSTATIC, FSTATIC, FSTATIC));
     SetTextColor( (HDC)wParam, RGB( 0, 0, 0));
     SetBkColor( (HDC)wParam, RGB( FSTATIC, FSTATIC, FSTATIC));
     return (LRESULT)pincel;
     break;
// case WM_PAINT:
     IF hwnd!=NULL THEN
        HDCPROC = BeginPaint( hwnd, &ps);
        BitBlt( HDCPROC, 0, 0, beforeW, beforeH, HDCMAIN, 0, 0, SRCCOPY);
        EndPaint( hwnd, &ps);
     ELSE
        return DefWindowProc (hwnd, message, wParam, lParam);
     ENDIF;
     break;
case WM_CLOSE:
    EndDialog( HWList, 0 );
     FreeConsole();
     Shell_NotifyIconA( NIM_DELETE , &nid);
     VALPHA = 250;
     WHILE( VALPHA>0 )
         VALPHA = VALPHA - 1;
         SetLayeredWindowAttributes( VENTANA_PADRE, 0, VALPHA, LWA_ALPHA);
         Sleep( 5 );
     ENDWHILE
     Sleep( 10 );
case WM_DESTROY:
///////////////////////////////////////////////////////////////////////////////////////////////
     // IndexSong = IndexList;
     FILE * pFile;
     //pFile = fopen ( "C:\\Users\\AdrianJCosta\\myfile.lst" , "wb" );
     pFile = fopen ( "C:\\myfile.lst" , "wb" );
     fprintf( pFile, "%i\n", IndexSong );
     FOR int i=0; i<IndexList; i++ LOOP
        fprintf( pFile, "%s\n", &SongFile[i][0] );
     ENDLOOP;
     fclose( pFile );
///////////////////////////////////////////////////////////////////////////////////////////////
     DeleteDC( HDCMAIN );
     DeleteDC( HDCPROC );
     GLOBALQUIT_GLOBAL = true;
     PostQuitMessage (0);
     Sleep( 100 );
     break;
default:
     return DefWindowProc (hwnd, message, wParam, lParam);
ENDMESSAGES
ENDWPROC





DWORD WINAPI ThreadCommands( LPVOID pv=NULL )
{
// HWND hwnd;WPARAM wParam;WPARAM lParam;
IF GLOBALQUIT_GLOBAL==true THEN
   return 0;
ENDIF;
INPROCESS = true;
/*
wParam = wParamC;
lParam = lParamC;
hwnd = HWThread;
//*/
CASE LOWORD( wParamC ) IS
WHEN(1)
    waveOutSetVolume( HWaveOut, MAKELONG( 65535, 65535 ) );
    waveOutRestart( HWaveOut );
    waveOutReset( HWaveOut );
    waveOutClose( HWaveOut );
    EndDialog( HWList, 0 );
    PostMessage(VENTANA_PADRE, WM_CLOSE, 0, 0);




WHEN( 68 )
    if( Loaded ){
        if( !InKare ){
            InKare = TRUE;
            short* TLR0 = (short*)&BUFFSONG[0];
            for( int i=0; i<FileSizeA/2; i = i + 2 ){
                TLR0[i] = (short)( 0.5*TLR0[i+1] - ( 0.5*TLR0[i] ) );
                TLR0[i+1] = TLR0[i];
            }
        }else{
            InKare = FALSE;
            memcpy( &BUFFSONG[0], &BUFFSONGRES[0], FileSizeA );
        }
    }
WHEN( 69 )
    if( HWList==NULL ){
        DialogBoxParam( INSTANCIA_GLOBAL, "LISTMNG", NULL, LISTMNG_PROC, 0 );
    }else{
        ResetAndLoad();
    }


WHEN( 70 )
    BMuteL = !BMuteL;
    MUTE_R_L_ALL( BMuteR, BMuteL );
WHEN( 71 )
    BMuteR = !BMuteR;
    MUTE_R_L_ALL( BMuteR, BMuteL );

WHEN( 88 )
    HWND HWFat = HWList;
    if( SHOWWINDOW ){
        HWFat = VENTANA_PADRE;
    }
    if( MessageBoxA( HWFat, "¿Desea borrar la lista?", " ", MB_YESNO | MB_ICONQUESTION)==IDYES ){
        FOR int i=0; i<256; i++ LOOP
            FOR int i2=0; i2<256; i2++ LOOP
                SongFile[i][i2] = 0;
            ENDLOOP;
        ENDLOOP;
        IndexList = 0;
        IndexSong = 0;
        SetWindowTextA( EDIT_INFO, "Lista borrada." );
        ResetAndLoad();
    }
WHEN( 90 )
    SaveList();
WHEN( 89 )
    if( OpenList() ){
        ResetAndLoad();
    }
WHEN( 98 )
    SaveWAV();
WHEN(99)
    static BOOL InOpen;
    if( InOpen ){ break; }
    InOpen = TRUE;
    // msgbox( SongFile[IndexSong] );
    static char* ActFile;
    if( ActFile==NULL ){
        ActFile = (char*)malloc( MBs );
    }
    ZeroMemory( ActFile, MBs-1 );
    strcpy( ActFile, SongFile[IndexSong] );
    HWND HWFat = HWList;
    if( SHOWWINDOW ){
        HWFat = VENTANA_PADRE;
    }
    if( OpenFileAs(ActFile, HWFat) ){
        int StrL = strlen( ActFile );
        if( (ActFile[StrL]==0 && ActFile[StrL+1]==0) ){
            strcpy( SongFile[IndexList], ActFile );
            IndexSong = IndexList;
            if( START_SOUND() ){
                IndexList++;
            }else{
                InOpen = FALSE;
                break;
            }
        }else{
            char* TPTRT = &ActFile[0] + StrL + 1;
            char* TDir = (char*)malloc( StrL + 1024 );
            strcpy( TDir, ActFile );
            IndexSong = IndexList;
            while( TPTRT[0]!=0 ){
                sprintf( SongFile[IndexList], "%s\\%s", TDir, TPTRT );
                // msgbox( SongFile[IndexList] );
                IndexList++;
                TPTRT = TPTRT + strlen(TPTRT) + 1;
                StrL = strlen( TPTRT );
            }
            while( !START_SOUND() ){
                IndexSong++;
                if( IndexSong>=IndexList ){
                    IndexSong = 0;
                }
            }
            free( TDir );
        }
        FILE * pFile = fopen ( "C:\\myfile.lst" , "wb" );
        //pFile = fopen ( "C:\\Users\\AdrianJCosta\\myfile.lst" , "wb" );
        fprintf( pFile, "%i\n", IndexSong );
        for( int i=0; i<IndexList; i++ ){
            fprintf( pFile, "%s\n", &SongFile[i][0] );
        }
        fclose( pFile );
        ResetAndLoad();
        InOpen = FALSE;
    }
    InOpen = FALSE;


WHEN(100)
    IF IN_PAUSE THEN
       // StartSong( HscrollPitch.Position );
       waveOutRestart( HWaveOut );
       IN_PAUSE = FALSE;
       SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon( INSTANCIA_GLOBAL, "IPAUSE") );
    ELSIF waveOutUnprepareHeader( HWaveOut , &WHeader , sizeof(WAVEHDR) )==WAVERR_STILLPLAYING THEN
       waveOutGetPosition( HWaveOut , &mmtime , sizeof(MMTIME) );
       CONSTTIME = (WaveFormat.nChannels * WaveFormat.wBitsPerSample * WaveFormat.nSamplesPerSec)/8.0;
       tim = ( (mmtime.u.sample)/CONSTTIME ) + HscrollPitch.Position;
       HscrollPitch.Position = int(tim);
       waveOutPause( HWaveOut );
       IN_PAUSE = TRUE;
       SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon( INSTANCIA_GLOBAL, "IPLAY") );
    ELSE
       HscrollPitch.Position = 0;
       IF Loaded THEN
          StartSong( 0 );
       ELSE
          START_SOUND();
       ENDIF;
    ENDIF;

WHEN(101)
    waveOutRestart( HWaveOut );
    waveOutReset( HWaveOut );
    waveOutClose( HWaveOut );
    HscrollPitch.Position = 0;
    SetScrollPos( HscrollPitch.Tag , SB_CTL , 0 , TRUE );
    KillTimer( VENTANA_PADRE, 1000 );
    SendMessage( HWPlay, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon( INSTANCIA_GLOBAL, "IPLAY") );
    SetWindowTextA( EDIT_INFO, SongTextEdit );

    IN_PAUSE = FALSE;

    PPitch = 0;
    NAdv = 0;


WHEN(102)
    MAKELOWPASS();
WHEN(103)
    MAKEHIPASS();


WHEN(255)
    Norm();

WHEN(514)
    MAKEREVERV();
WHEN(515)
    // MAKEREVERV();
    //MAKEAMP();



WHEN(516)
    MAKELOWPASS();
WHEN(517)
    MAKEHIPASS();





WHEN(518)
    DOPPLER();




WHEN(1024)
    FOR int i=0; i<25*MBs; i++ LOOP
        BUFFSHORT[i] = ORIGEN_BUFF[i];
    ENDLOOP;
    NLOWPASS = 0;


WHEN(1025)
    if( IN_START ){ break; }
    if( IndexList==0 && IndexSong==0 ){ SendMessage( HWCBoxSongs, CB_RESETCONTENT, IndexSong, 0 ); break; }
    ReplyFw:
    // msgboxf( "IndexList = %i\nIndexSong = %i", IndexList, IndexSong );
    IndexSong = 1023&(IndexSong + 1);
    IF IndexSong>=IndexList THEN
       IndexSong = 0;
    ENDIF;
    IF START_SOUND()==FALSE THEN
       goto ReplyFw;
    ENDIF;
    SendMessage( HWCBoxSongs, CB_SETCURSEL, 1023&Indexs[IndexSong], 0 );


WHEN(1026)
    if( IN_START ){ break; }
    if( IndexList==0 && IndexSong==0 ){ SendMessage( HWCBoxSongs, CB_RESETCONTENT, IndexSong, 0 ); break; }
    ReplyRw:
    IndexSong = 1023&(IndexSong - 1);
    IF IndexSong>=IndexList THEN
       IndexSong = 0;
    ENDIF;
    IF START_SOUND()==FALSE THEN
       goto ReplyRw;
    ENDIF;
    SendMessage( HWCBoxSongs, CB_SETCURSEL, 1023&Indexs[IndexSong], 0 );

WHEN(1027)
    EnableWindow( HWThread, FALSE );
    START_SOUND();
    EnableWindow( HWThread, TRUE );
    SetFocus( HWThread );

WHEN(1028)
    BRepeat = !BRepeat;
    if( BRepeat ){
        SetWindowTextA( GetDlgItem( VENTANA_PADRE, 1028 ), "Repetir\n(On)" );
    }else{
        SetWindowTextA( GetDlgItem( VENTANA_PADRE, 1028 ), "Repetir\n(Off)" );
    }

WHEN(1029)
    if( IN_START ){ break; }
    if( IndexList==0 && IndexSong==0 ){ SendMessage( HWCBoxSongs, CB_RESETCONTENT, IndexSong, 0 ); break; }
    Reply29:
    // msgboxf( "IndexList = %i\nIndexSong = %i", IndexList, IndexSong );
    if( !BRepeat ){
        IndexSong = 1023&(IndexSong + 1);
    }
    IF IndexSong>=IndexList THEN
       IndexSong = 0;
    ENDIF;
    IF START_SOUND()==FALSE THEN
       goto Reply29;
    ENDIF;
    SendMessage( HWCBoxSongs, CB_SETCURSEL, 1023&Indexs[IndexSong], 0 );

WHEN(1030)
    if( IN_START ){ break; }
    BRandom = !BRandom;
    ZeroMemory( &Indexs[0], 1024*sizeof(short) );
    if( BRandom ){
        unsigned short ValTi;
        unsigned short V_1;
        Indexs[0] = IndexList/2 + ( 1023&( (unsigned short)clock() ) );
        while( Indexs[0]>=IndexList ){
            Indexs[0] = IndexList/2 + ( 1023&( (unsigned short)clock() ) );
        }
        for( int i=1; i<IndexList; i++ ){
            V_1 = Indexs[i-1];
            //
            ValTi = (i+V_1)*(i+3) + ( 1023&( (unsigned short)clock() ) );
            while( !ParserInd( i, 1023&ValTi, IndexList ) ){
                ValTi = ValTi + 3 + V_1 + ( 1023&( (unsigned short)clock() ) );
                V_1--;
            }
            Indexs[i] = 1023&ValTi;
        }
        SetWindowTextA( GetDlgItem( VENTANA_PADRE, 1030 ), "Random\n(On)" );
    }else{
        SetWindowTextA( GetDlgItem( VENTANA_PADRE, 1030 ), "Random\n(Off)" );
        for( int i=0; i<IndexList; i++ ){
            Indexs[i] = i;
        }
    }
    system( "cls" );
    printf( "IndexList, %i\n\n", IndexList );
    for( int i=0; i<IndexList; i++ ){
        printf( "%i\n", Indexs[i] );
        if( Indexs[i]>=IndexList ){
            msgbox( "if( Indexs[i]>=IndexList ){" );
        }
    }
    //
WHEN(1031)
    Copy2Folder();



DEFAULT
    break;
ENDCASE
INPROCESS = false;
return 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI CloseMsgbox( LPVOID pv=NULL ){

HWND EMSG = NULL;
WHILE( IsWindow(EMSG)==FALSE )
    EMSG = FindWindow( NULL, " EQUALIZER Mensaje " );
    IF IsWindow(EMSG)==TRUE THEN
        SendMessageA( EMSG, WM_CLOSE, 0, 0 );
    ENDIF;
    Sleep( 10 );
ENDWHILE;
return 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BOOL IsDownK( int ValK ){
return (BOOL)( GetAsyncKeyState(  ValK  )==-32767 );
}
////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI PlayNBCommands( LPVOID pv=NULL ){
    //return 0;
WHILE( GLOBALQUIT_GLOBAL==false )
    //SetWindowTextF( EDIT_INFO, (char*)" %i", GetAsyncKeyState(17) );
    IF GetAsyncKeyState(  VK_F9  )!=0 THEN
        // msgbox( (char*)"100" );
        SendMessage( VENTANA_PADRE, WM_COMMAND, 100, 0 );
        Sleep( 1000 );
    ELSIF GetAsyncKeyState(  VK_F10  )!=0 THEN
       SendMessage( VENTANA_PADRE, WM_COMMAND, 1025, 0 );
       Sleep( 1000 );
    ELSIF GetAsyncKeyState(  VK_F8  )!=0 THEN
       SendMessage( VENTANA_PADRE, WM_COMMAND, 1026, 0 );
       Sleep( 1000 );
    ENDIF;
    Sleep( 300 );
ENDWHILE
//msgbox( (char*)"100 OUT" );
return 0;
}
