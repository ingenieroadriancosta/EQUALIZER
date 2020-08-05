BOOL SaveList( void ){
    static char FLNAME[256];
    FLNAME[0] = 0;
    HWND HWFat = HWList;
    if( SHOWWINDOW ){
        HWFat = VENTANA_PADRE;
    }
    if( SaveFileAs( &FLNAME[0], 1, HWFat )==0 ){
        return FALSE;
    }
    FILE * pFile;
    pFile = fopen ( &FLNAME[0], "wb" );
    if( pFile==NULL ){
        sprintf( TextoGlobal, "Error guardando:,\n%s", &FLNAME[0] );
        SetWindowTextA( EDIT_INFO, TextoGlobal );
        return FALSE;
    }
    fprintf( pFile, "%i\n", IndexSong );
    for( int i=0; i<IndexList; i++ ){
        fprintf( pFile, "%s\n", &SongFile[i][0] );
    }
    fclose( pFile );
    sprintf( TextoGlobal, "Lista guradada:,\n%s", &FLNAME[0] );
    SetWindowTextA( EDIT_INFO, TextoGlobal );
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL OpenList( void ){
    static char FLNAME[256];
    HWND HWFat = HWList;
    if( SHOWWINDOW ){
        HWFat = VENTANA_PADRE;
    }
    if( OpenFileAs( &FLNAME[0], HWFat )==0 ){
        return FALSE;
    }
    FILE * pFile = NULL;
    pFile = fopen( &FLNAME[0], "rb" );
    if( pFile ){
        int result = 0;
        char buffer[2] = {};
        int Ni = 0;
        int IndLi = 0;
        IndexList = 0;
        BOOL IndSeted = FALSE;
        while( 1 ){
            result = fread( &buffer[0], 1, 1, pFile );
            if( result!=1 ){
                SongFile[IndexList][0] = 0;
                break;
            }
            //
            if( buffer[0]=='\n' ){
                SongFile[IndexList][Ni] = 0;
                IndexList++;
                if( IndSeted==FALSE ){
                    IndSeted = TRUE;
                    IndLi = (int)atoi( &SongFile[0][0] );
                    IndexList = 0;
                }
                Ni = 0;
            }else{
                SongFile[IndexList][Ni] = buffer[0];
                Ni++;
            }
       }
       fclose( pFile );
       IndexSong = IndLi;
       START_SOUND();
    }
    return TRUE;
}



