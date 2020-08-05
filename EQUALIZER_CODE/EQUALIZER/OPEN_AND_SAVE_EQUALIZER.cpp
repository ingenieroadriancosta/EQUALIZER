char* FileNameAux = NULL;
inline bool OpenFileAs( char *FileName, HWND HWFath ){
    if( FileNameAux==NULL ){
        FileNameAux = (char*)malloc(MBs+1);
    }
    strcpy( FileNameAux, FileName );
    int StrL = strlen( FileNameAux );
    ZeroMemory( &FileNameAux[StrL], MBs - StrL - 2 );
    for( int i=StrL; i>0; i-- ){
        if( FileName[i]=='\\' ){ FileName[i] = 0;break; }
        FileName[i] = 0;
    }


OPENFILENAME  openFileDialogfunction = {0};
ZeroMemory(&openFileDialogfunction, sizeof(openFileDialogfunction));
openFileDialogfunction.lStructSize= sizeof(openFileDialogfunction);
openFileDialogfunction.hwndOwner = HWFath;//VENTANA_PADRE;
openFileDialogfunction.nFilterIndex=3;
openFileDialogfunction.lpstrTitle="Abrir";
openFileDialogfunction.hInstance = INSTANCIA_GLOBAL;
openFileDialogfunction.lpstrInitialDir = &FileName[0];
// openFileDialogfunction.lpstrInitialDir="X:\\SegAll\\MUSICA\\";
openFileDialogfunction.lpstrFilter = "MP3 Files (*.mp3*)\0*.mp3\0WAV Files (*.wav*)\0*.wav\0All Files(*.*)\0*.*\0\0";
openFileDialogfunction.lpstrFile = FileNameAux;
openFileDialogfunction.nMaxFile = MBs-1;
openFileDialogfunction.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
openFileDialogfunction.lpstrDefExt = "";
if(!GetOpenFileName(&openFileDialogfunction)){
   return false;
}
int i=0;
while( FileNameAux[i]!=0 || FileNameAux[i+1]!=0 ){
    FileName[i] = FileNameAux[i];
    i++;
}
// lstrcat(FileName,FileNameAux);
// memcpy( FileName, FileNameAux, MBs );
// sprintf( FileName, "%s", FileNameAux);
return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI SaveFileAs( char *FileName, BYTE BOpt, HWND HWFat )
{
    if( FileNameAux==NULL ){
        FileNameAux = (char*)malloc(MBs);
    }
    strcpy( FileNameAux, FileName );
OPENFILENAME ofn;
ZeroMemory(&ofn, sizeof(ofn));
ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = HWFat;
ofn.nFilterIndex = 2;
ofn.lpstrFileTitle = NULL;
// ofn.lpstrInitialDir= "C:";
switch( BOpt ){
    case 0:
        ofn.lpstrFilter = "WAV Files (*.wav*)\0*.wav\0\0";
        break;
    case 1:
        ofn.lpstrInitialDir= "X:\\SegAll\\MUSICA\\_LIST";
        ofn.lpstrFilter = "LST Files (*.lst*)\0*.lst\0\0";
        break;
    default:
        ofn.lpstrFilter = "WAV Files (*.wav*)\0*.wav\0\0";
        break;
}
ofn.lpstrFile = FileNameAux;
ofn.nMaxFile = MAX_PATH;
ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
 OFN_OVERWRITEPROMPT ;
ofn.lpstrDefExt = ".wav";
if( !GetSaveFileNameA(&ofn) )
    return 0;
sprintf( FileName, "%s", FileNameAux );

int LenStr = strlen( FileName )-1;

if( FileName[LenStr]=='g' || FileName[LenStr]=='G' )
    return 2;
return 1;
}





