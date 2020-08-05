int CREATE( HWND HWnd ){

HW_Pruebas = CreateWindowEx( 0, "BUTTON", " Pruebas ", WS_CHILD | WS_VISIBLE,
                             400, 1, 60, 40, HWnd, (HMENU)ID_PRUEBAS,
                             INSTANCIA_GLOBAL, NULL );


CreateWindowEx( 0, "BUTTON", "Chirp", WS_CHILD | WS_VISIBLE,
                             1, 1, 40, 40, HWnd, (HMENU)ID_CHIRP,
                             INSTANCIA_GLOBAL, NULL );




HW_Info = CreateWindowEx( 0, "STATIC", "Info", WS_CHILD | WS_VISIBLE,
                             1, 50, 300, 200, HWnd, (HMENU)ID_CHIRP,
                             INSTANCIA_GLOBAL, NULL );





return 0;
}
