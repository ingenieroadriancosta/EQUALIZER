WPROC
MESSAGES
WHEN( WM_COMMAND )
     IF INPROCESS==TRUE || LOWORD(wParam)==0 THEN
        return 0;
     ELSE
        WCLS.Close();
        wParamC = wParam;
        lParamC = lParam;
        HWThread = hwnd;
        DWORD IdTh = 0;
        CreateThread( NULL, 0, ThreadCommands, NULL, 0, &IdTh );
        return 0;
     ENDIF;
     break;
WHEN( WM_CREATE )
     CREATE( hwnd );
WHEN( WM_GETMINMAXINFO )
     MINMAXINFO*      mmiStruct = (MINMAXINFO*)lParam;
     POINT            ptPoint;
     ptPoint.x = 1;
     ptPoint.y = 1;
     mmiStruct->ptMinTrackSize = ptPoint;
     ptPoint.x = GetSystemMetrics(SM_CXSCREEN);
     ptPoint.y = GetSystemMetrics(SM_CYSCREEN);
     mmiStruct->ptMaxTrackSize = ptPoint;
     return 0;

WHEN( WM_MOUSEMOVE )
    IF wParam==12345  THEN
        IF lParam==WM_LBUTTONDOWN THEN
            IF IsWindowVisible(VENTANA_PADRE) THEN
                ShowWindow( VENTANA_PADRE, 0 );
            ELSE
                ShowWindow( VENTANA_PADRE, 1 );
            ENDIF;
        ENDIF;
     ENDIF;
     break;
WHEN( WM_DESTROY )
    WCLS.Close();
    GLOBALQUIT_GLOBAL = true;
    Shell_NotifyIconA( NIM_DELETE , &nid);
    PostQuitMessage (0);
    break;
DEFAULT
     return DefWindowProc (hwnd, message, wParam, lParam);
ENDMESSAGES
ENDWPROC
