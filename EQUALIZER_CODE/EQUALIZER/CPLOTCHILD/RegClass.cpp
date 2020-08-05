BOOL CPLOTCHILD::RegClass( void ){
    /*
    WNDCLASSEXA wincl = {0};
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = (LPCSTR)(szClassName);
    wincl.lpfnWndProc = CPLOTWProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon( hThisInstance , "A" );
    wincl.hIconSm = LoadIcon( hThisInstance , "A" );
    wincl.hCursor = LoadCursor( NULL, IDC_ARROW );
    wincl.hbrBackground = (HBRUSH)CreateSolidBrush( RGB( 223, 223, 223 ) );
    if( !SetCWindow ){
        SetCWindow = TRUE;
        INITCOMMONCONTROLSEX icex;
        // Nos aseguramos que se carguen las clases de la DLL.
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_BAR_CLASSES;
        // InitCommonControlsEx(&icex);
        if ( !RegisterClassEx(&wincl) ){
            return FALSE;
        }
    }
    //*/
    // CreateThread( NULL, 0, RegClassProcs, (PVOID)&CPtr, 0, NULL );
    Create( NULL );
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPLOTCHILD::Create( HWND HFT ){
    HWFath = HFT;
    HINSTANCE HInstance = GetModuleHandle( NULL );
    DestroyWindow( HWPlot );
    HWPlot = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | SS_BITMAP | WS_VISIBLE,
                             XFR, YFR, WFR, HFR, HWFath, NULL,
                             HInstance, NULL );
    DeleteObject( HBitmap );
    HBitmap = NewBitmap( WFR, HFR );
    if( UCBuff==NULL ){
        FillGrid();
    }
    SetBitmapBits( HBitmap, 4 * WFR * HFR, &RgbBuf[0] );
    SendMessage( HWPlot, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmap );
    return TRUE;
}
