BOOL Copy2Folder( void ){
    static BOOL InCPY;
    if( InCPY ){ return TRUE; }
    InCPY = TRUE;
    //*
    BROWSEINFO bi;
    char Buffer[MAX_PATH];
    ZeroMemory(Buffer, MAX_PATH);
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = VENTANA_PADRE;
    bi.pszDisplayName = Buffer;
    bi.lpszTitle = " Elegir carpeta ";
    bi.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE;
    LPCITEMIDLIST pFolder = SHBrowseForFolder(&bi);
    if( pFolder == NULL ){
        InCPY = FALSE;
        return FALSE;
    }
    if ( !SHGetPathFromIDList(pFolder, Buffer) ){
        InCPY = FALSE;
        return FALSE;
    }
    char T2T[1024];
    GetWindowTextA( EDIT_INFO, T2T, 1023 );
    // SetWindowTextA( EDIT_INFO, TextoGlobal );
    char TFTitle[256];
    char TFDir[256];
    ZeroMemory( &TFTitle[0], 255 );
    ZeroMemory( &TFDir[0], 255 );
    int NFiles = 0;
    for( int i=0; i<IndexList; i++ ){
        strcpy( TFDir, Buffer );
        ZeroMemory( &TFTitle[0], 255 );
        GetFileTitleA( &SongFile[ 1023&Indexs[i] ][0], &TFTitle[0], 255 );
        sprintf( TFDir, "%s\\%08d%s", Buffer, i, &TFTitle[0] );
        // msgbox( TFDir );exit( 0 );
        NFiles = NFiles + (int)CopyFileA( &SongFile[ 1023&Indexs[i] ][0], &TFDir[0], TRUE );
        SetWindowTextF( EDIT_INFO, FALSE, "%i%%\n%i de %i Archivos",
                       (100*i)/(IndexList-1), NFiles, IndexList );
        // break;
    }
    msgbox( "La copia ha terminado." );
    SetWindowTextA( EDIT_INFO, T2T );
    //*/
    InCPY = FALSE;
    return TRUE;
}
