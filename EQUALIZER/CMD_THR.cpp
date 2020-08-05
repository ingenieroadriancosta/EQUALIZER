DWORD WINAPI ThreadCommands( LPVOID pv=NULL ){
WPARAM wParam;

INPROCESS = true;
wParam = wParamC;
CASE LOWORD( wParam ) IS
WHEN( ID_PRUEBAS )
    PRUEBAS();

WHEN( ID_CHIRP )
    WCLS.SetSin( 1000, 2.0 );
DEFAULT
    break;
ENDCASE
INPROCESS = false;
return 0;
}
