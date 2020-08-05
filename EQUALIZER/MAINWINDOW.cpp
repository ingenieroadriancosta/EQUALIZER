MAINWINDOW
SetUnhandledExceptionFilter(OurCrashHandler);
regclass( hThisInstance, WindowProcedure, szClassName, "A" );
beforeW = 500;
beforeH = 300;
int CXSCREEN = GetSystemMetrics( SM_CXSCREEN );
int CYSCREEN = GetSystemMetrics( SM_CYSCREEN );
beforeX = (beforeW+CXSCREEN)/2 - beforeW;
beforeY = (beforeH+CYSCREEN)/2 - beforeH;
VENTANA_PADRE = CreateWindowEx( WS_EX_TOPMOST, //WS_EX_TOOLWINDOW,// | WS_EX_TOPMOST,
                                szClassName, szClassName,
                                (WS_OVERLAPPEDWINDOW - (WS_SIZEBOX | WS_MAXIMIZEBOX)),
                beforeX, beforeY, beforeW, beforeH, HWND_DESKTOP, NULL,
                hThisInstance, NULL );

nid.hWnd = VENTANA_PADRE;
nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
nid.hIcon = LoadIcon( hThisInstance , "A" );
memcpy( nid.szTip , szClassName , 64 );


nid.uCallbackMessage =  WM_MOUSEMOVE;
nid.uID = 12345;
nid.cbSize = sizeof(nid);
nid.cbSize = WM_USER+20;
Shell_NotifyIconA( NIM_ADD , &nid);


ShowWindow( VENTANA_PADRE, 1 );
MSG messages;
WHILE( GLOBALQUIT_GLOBAL==false )
    GetMessage (&messages, NULL, 0, 0);
    TranslateMessage(&messages);
    DispatchMessage(&messages);
ENDWHILE
ENDMAINWINDOW
