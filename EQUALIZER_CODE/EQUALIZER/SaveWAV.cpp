BOOL SaveWAV( void ){
//
char FLNAME[256];
FLNAME[0] = 0;
HWND HWFat = HWList;
if( SHOWWINDOW ){
    HWFat = VENTANA_PADRE;
}
IF SaveFileAs(&FLNAME[0], 0, HWFat)==0 THEN
    return FALSE;
ENDIF;
DWORD B2Write = 0;
HANDLE HFile = CreateFile( FLNAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
DWORD LastError = GetLastError();
IF LastError==ERROR_SHARING_VIOLATION THEN
   msgbox( "El archivo está siendo utilizado por otra aplicación.", " Error ", 16 );
   return FALSE;
ENDIF;
/////////////////////////////////
BYTE DataSave[256];
DWORD VARDWORD = 0;
int subchunk2size = ( WHeader.dwBufferLength );
if( subchunk2size<1 ){
    subchunk2size = FileSizeA;
}
////////////////////////////////////////////////////////////////////////////////
//DWORD VARDWORD = 0;
DataSave[0]++;
DataSave[0] = 'R';
DataSave[1] = 'I';
DataSave[2] = 'F';
DataSave[3] = 'F';

VARDWORD = 36 + subchunk2size;
DataSave[4] = GETBYTE( VARDWORD );
DataSave[5] = GETBYTE( VARDWORD , 1 );
DataSave[6] = GETBYTE( VARDWORD , 2 );
DataSave[7] = GETBYTE( VARDWORD , 3 );

DataSave[8] = 'W';
DataSave[9] = 'A';
DataSave[10]= 'V';
DataSave[11]= 'E';

DataSave[12] = 'f';
DataSave[13] = 'm';
DataSave[14] = 't';
DataSave[15] = 32;


DataSave[16] = 16;
DataSave[17] =  0;
DataSave[18] =  0;
DataSave[19] =  0;



//AudioFormat  2bytes     1;//for PCM.
DataSave[20] = 1;
DataSave[21] = 0;


//NumChannels  2bytes
VARDWORD = 2;
DataSave[22] = GETBYTE( VARDWORD );
DataSave[23] = GETBYTE( VARDWORD , 1 );

VARDWORD = WaveFormat.nSamplesPerSec;
if( VARDWORD<1 ){
    VARDWORD = FS;
    WaveFormat.nSamplesPerSec = FS;
}
DataSave[24] = GETBYTE( VARDWORD );
DataSave[25] = GETBYTE( VARDWORD , 1 );
DataSave[26] = GETBYTE( VARDWORD , 2 );
DataSave[27] = GETBYTE( VARDWORD , 3 );

VARDWORD = (4 * WaveFormat.nSamplesPerSec );
DataSave[28] = GETBYTE( VARDWORD );
DataSave[29] = GETBYTE( VARDWORD , 1 );
DataSave[30] = GETBYTE( VARDWORD , 2 );
DataSave[31] = GETBYTE( VARDWORD , 3 );

//NumChannels * BitsPerSample/8
VARDWORD = (4);
DataSave[32] = GETBYTE( VARDWORD );
DataSave[33] = GETBYTE( VARDWORD , 1 );

// bits per sample
VARDWORD = 16;
DataSave[34] = GETBYTE( VARDWORD );
DataSave[35] = GETBYTE( VARDWORD , 1 );

// subchunk2ID
DataSave[36] = 'd';
DataSave[37] = 'a';
DataSave[38] = 't';
DataSave[39] = 'a';

// subchunk2size (NumSamples * NumChannels * BitsPerSample/8)
subchunk2size = ( subchunk2size  );
VARDWORD = subchunk2size;
DataSave[40] = GETBYTE( VARDWORD );
DataSave[41] = GETBYTE( VARDWORD , 1 );
DataSave[42] = GETBYTE( VARDWORD , 2 );
DataSave[43] = GETBYTE( VARDWORD , 3 );

DWORD WriteBytes = 0;
// WriteFile( HFile , DataSave , 44 , &WriteBytes , NULL );
WriteFile( HFile , HeaderWAV , 44 , &WriteBytes , NULL );
B2Write++;
B2Write = subchunk2size;
/*
short* Data2W = (short*)malloc( 2 * B2Write );
IF Data2W==NULL THEN
   msgbox( " Error, NO HAY ESPACIO EN MEMORIA. ", " Error. ", 16 );
   CloseHandle( HFile );
   return FALSE;
ENDIF;
//*/

WriteBytes = 0;
WriteFile( HFile , &BUFFSONG[0], FileSizeA, &WriteBytes , NULL );
CloseHandle( HFile );
/////////////
char T2T[1024];
sprintf( T2T, "Desea abrir el archivo?\n%s", FLNAME );
IF msgbox( T2T, " Abrir archivo con programa predeterminado ", MB_YESNO | MB_ICONQUESTION  )==IDYES THEN
   ShellExecuteA( NULL, "open", FLNAME, NULL, NULL, 1 );
ENDIF;
/////////////
return TRUE;
}
