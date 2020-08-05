#include <DEF.h>
#include <DEF_FUNC.h>
#include "LILI.h"
#include "mpg123.cpp"
DWORD WINAPI PlayNBCommands( LPVOID );
#include "GVARS.cpp"
#include "OPEN_AND_SAVE_EQUALIZER.cpp"


#include "SCROLLBAR.cpp"
#include "SONG.cpp"
#include "MAKEREVERV.cpp"


#include "MAKELOWPASS.cpp"
#include "MAKEHIPASS.cpp"

#include "DOPPLER.cpp"

 


#include "MAINWINDOW.cpp"
WPROC
MESSAGES
/*
case 0x0319:
     msgbox();
     break;
//*/
     
     
/*
case WM_CREATE:
     RegisterHotKey( hwnd, 100, 0, VK_MEDIA_PLAY_PAUSE );
     // UnregisterHotKey(hwnd, ID_VERDE);
     
     break;
//*/
case WM_COMMAND:
     IF LOWORD(wParam)==10 THEN
        SHOWWINDOW = TRUE;
        ShowWindow( VENTANA_PADRE, SHOWWINDOW );
        return 0;
     ENDIF;
     
     IF LOWORD(wParam)==512 THEN
        VALPHA = 255&SendMessage( HWndVALPHA, CB_GETCURSEL, 0, (LPARAM)0 );
        IF VALPHA<=90 THEN
           VALPHA = 90;
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
        SetEvent( EventControls );
        return 0;
     ENDIF;
     break;


case WM_VSCROLL:
case WM_HSCROLL:
     IN_SCROLL = true;
     IF HscrollPitch.ProcesarScrollH( wParam , lParam ) THEN
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
case WM_PAINT:
     IF hwnd!=NULL THEN
        HDCPROC = BeginPaint( hwnd, &ps);
        BitBlt( HDCPROC, 0, 0, beforeW, beforeH, HDCMAIN, 0, 0, SRCCOPY);
        EndPaint( hwnd, &ps);
     ELSE
        return DefWindowProc (hwnd, message, wParam, lParam);
     ENDIF;
     break;
case WM_CLOSE:
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
HWND hwnd;WPARAM wParam;WPARAM lParam;
DeregisterEventSource( EventControls );
EventControls = CreateEvent(0, FALSE, FALSE, 0);
WHILE( GLOBALQUIT_GLOBAL==false )
WaitForSingleObject( EventControls, INFINITE);
IF GLOBALQUIT_GLOBAL==true THEN
   return 0;
ENDIF;
INPROCESS = true;wParam = wParamC;lParam = lParamC;hwnd = HWThread;
CASE LOWORD( wParamC ) IS
WHEN(1)
    waveOutRestart( HWaveOut );
    waveOutReset( HWaveOut );
    waveOutClose( HWaveOut );
    PostMessage(VENTANA_PADRE, WM_CLOSE, 0, 0);



WHEN(99)
    IF OpenFileAs(SongFile[IndexList]) THEN
       IndexSong = IndexList;
       START_SOUND();
       IndexList++;
       
       FILE * pFile;
       char buffer[] = { 'x' , 'y' , 'z' };
       //pFile = fopen ( "C:\\Users\\AdrianJCosta\\myfile.lst" , "wb" );
       pFile = fopen ( "C:\\myfile.lst" , "wb" );
       FOR int i=0; i<IndexList; i++ LOOP
           fprintf( pFile, "%s\n", &SongFile[i][0] );
       ENDLOOP;
       fclose( pFile );
       
       
       
    ENDIF;
    
    
WHEN(100)
    IF IN_PAUSE THEN
       StartSong( HscrollPitch.Position );
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
    MAKEREVERV();
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
    IndexSong = 255&(IndexSong + 1);
    IF IndexSong>IndexList THEN
       IndexSong = 0;
    ENDIF;
    IF START_SOUND()==FALSE THEN
       IndexSong = 0;
       START_SOUND();
    ENDIF;
    
WHEN(1026)
    IndexSong = 255&(IndexSong - 1);
    
    IF IndexSong>IndexList THEN
       IndexSong = IndexList;
    ENDIF;
    START_SOUND();
    
    
DEFAULT
    break;
ENDCASE
INPROCESS = false;
ENDWHILE
CloseHandle( EventControls );
return 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BOOL IsDownK( int ValK ){
return (BOOL)( GetAsyncKeyState(  ValK  )==-32767 );
}
////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI PlayNBCommands( LPVOID pv=NULL ){

return 0;

// keybd_event( VK_MEDIA_PLAY_PAUSE, 0xB3, KEYEVENTF_EXTENDEDKEY, 0 );
// keybd_event( VK_MEDIA_PLAY_PAUSE, 0xB3, KEYEVENTF_KEYUP, 0 );
//msgbox();

unsigned short Tec = 1;
WHILE( GLOBALQUIT_GLOBAL==false )
    //*
    IF IsDownK(  179  ) THEN
       SendMessage( VENTANA_PADRE, WM_COMMAND, 100, 0 );
       Sleep( 1000 );
    ELSIF IsDownK(  176  ) THEN
       SendMessage( VENTANA_PADRE, WM_COMMAND, 1025, 0 );
       Sleep( 1000 );
    ELSIF IsDownK(  177  ) THEN
       SendMessage( VENTANA_PADRE, WM_COMMAND, 1026, 0 );
       Sleep( 1000 );
    ENDIF;//                                                    MapVirtualKey
    //*/
    Sleep( 50 );
    // break;
ENDWHILE




// WAIT_OBJECT_0
return 0;
}
