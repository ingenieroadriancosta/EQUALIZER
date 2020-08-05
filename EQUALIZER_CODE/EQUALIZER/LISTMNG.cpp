BOOL ResetAndLoad( void );
BOOL CALLBACK LISTMNG_PROC(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HFONT HFont;
    switch (msg){
        case WM_INITDIALOG:{
            HWList = hDlg;
            AlwOnTop = TRUE;
            HFont = CreateFont( 24, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_ROMAN, "Times New Roman" );
            const int WSCREEN = GetSystemMetrics( SM_CXSCREEN );
            const int HSCREEN = GetSystemMetrics( SM_CYSCREEN );
            int WDIALOG = 455;
            int HDIALOG = 610;
            int XDIALOG = (WSCREEN+WDIALOG)/2 - WDIALOG;
            int YDIALOG = (HSCREEN+HDIALOG)/2 - HDIALOG;
            MoveWindow( hDlg, XDIALOG, YDIALOG, WDIALOG, HDIALOG, TRUE );
            HWCBoxSongs = GetDlgItem( hDlg, 256 );
            MoveWindow( HWCBoxSongs, 1, 36, WDIALOG-8, HDIALOG-70, TRUE );
            SendMessage( HWCBoxSongs, WM_SETFONT, (WPARAM)HFont, MAKELPARAM(TRUE, 0));
            ResetAndLoad();
            }
            return TRUE;
        case WM_COMMAND:
            switch( LOWORD(wParam) ){
                case IDCANCEL:
                    HWList = NULL;
                    HWCBoxSongs = NULL;
                    DeleteObject( HFont );
                    EndDialog(hDlg, 0 );
                    return TRUE;
                case 256:{
                    switch( HIWORD(wParam) ){
                        case CBN_DBLCLK:{
                            int ICur = SendMessage( HWCBoxSongs, CB_GETCURSEL, 0, 0 );
                            if( ICur>-1 && ICur<IndexList ){
                                for( int i=0; i<IndexList; i++ ){
                                    // msgboxf( "%i  --  %i", ICur, Indexs[i] );
                                    if( Indexs[i]==ICur ){
                                        ICur = i;
                                        break;
                                    }
                                }
                                IndexSong = ICur;
                                wParamC = 1027;
                                lParamC = 0;
                                HWThread = HWCBoxSongs;
                                CreateThread( NULL , 0 , ThreadCommands, NULL , 0 , NULL );
                            }
                            }
                            break;
                    }
                    break;
                    }
                case 100:{
                    int ICur = SendMessage( HWCBoxSongs, CB_GETCURSEL, 0, 0 );
                    if( ICur>-1 && ICur<IndexList ){
                        char* Tex2TQ = (char*)malloc( 1024 );
                        GetFileNameA( SongFile[ICur], TGBL );
                        sprintf( Tex2TQ, "Desea borrar la canción:\n%s", TGBL );
                        if( MessageBoxA( HWList, Tex2TQ, " ", MB_YESNO | MB_ICONQUESTION)==IDYES ){
                            int I2Del = ICur;
                            int ICount = 0;
                            for( int i=0; i<IndexList; i++ ){
                                if( i!=I2Del ){
                                    strcpy( SongFile[ICount], SongFile[i] );
                                    ICount++;
                                }
                            }
                            IndexList--;
                            if( IndexSong>=IndexList ){
                                IndexSong = IndexList - 1;
                            }
                            ResetAndLoad();
                        }
                        free( Tex2TQ );
                    }
                    break;
                    }
                case 101:{
                    int ICur = SendMessage( HWCBoxSongs, CB_GETCURSEL, 0, 0 );
                    if( ICur>-1 && ICur<IndexList ){
                        //char* Tex2TQ = (char*)malloc( 1024 );
                        //GetFileNameA( SongFile[ICur], TGBL );
                        //sprintf( Tex2TQ, "Desea cambiar la canción:\n%s", TGBL );
                        //if( MessageBoxA( HWList, Tex2TQ, " ", MB_YESNO | MB_ICONQUESTION)==IDYES ){
                            static char ActFile[256];
                            strcpy( ActFile, SongFile[ICur] );
                            if( OpenFileAs( &ActFile[0], HWList )==0 ){
                                break;
                            }
                            strcpy( SongFile[ICur], ActFile );
                            ResetAndLoad();
                        //}
                        //free( Tex2TQ );
                    }
                    }
                    break;
                case 102:{
                    AlwOnTop = !AlwOnTop;
                    if( AlwOnTop ){
                        SetWindowPos( HWList, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                    }else{
                        SetWindowPos( HWList, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                    }
                }
            }
            //
            break;


        case WM_LBUTTONDBLCLK:
             SHOWWINDOW = !SHOWWINDOW;
             ShowWindow( VENTANA_PADRE, SHOWWINDOW );
             break;
        case WM_RBUTTONDOWN:{
            int CursorPos_x = 0;
            int CursorPos_y = 0;
            CursorPos( &CursorPos_x, &CursorPos_y );
            HMENU popMenu = GetSubMenu( MENUGLOBAL , 0 );
            TrackPopupMenu( popMenu, 0, CursorPos_x, CursorPos_y, 0, VENTANA_PADRE, NULL );
            }
            break;
            //
            return TRUE;
    }
    return FALSE;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ResetAndLoad( void ){
    SendMessage( HWCBoxSongs , CB_RESETCONTENT, 0, 0 );
    char* Tex2T = (char*)malloc( 1024 );
    for( int i=0; i<IndexList; i++ ){
        GetFileNameA( SongFile[i], Tex2T );
        SendMessage( HWCBoxSongs, CB_ADDSTRING, 0, (LPARAM)&Tex2T[0] );
    }
    free( Tex2T );
    SendMessage( HWCBoxSongs, CB_SETCURSEL, IndexSong, 0 );
    return TRUE;
}
