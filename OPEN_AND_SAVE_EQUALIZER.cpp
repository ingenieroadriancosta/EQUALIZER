char FileNameAux[40*KBs];
inline bool OpenFileAs( char *FileName, HWND hwnd=VENTANA_PADRE, int DirOption=0 )
{
OPENFILENAME  openFileDialogfunction = {0};
ZeroMemory(&openFileDialogfunction, sizeof(openFileDialogfunction));
openFileDialogfunction.lStructSize= sizeof(openFileDialogfunction);
openFileDialogfunction.hwndOwner = hwnd;
openFileDialogfunction.nFilterIndex=3;
openFileDialogfunction.lpstrTitle="Abrir";
openFileDialogfunction.hInstance = INSTANCIA_GLOBAL;
IF DirOption==0 THEN
   openFileDialogfunction.lpstrInitialDir="E:\\MUSICA";
ELSIF DirOption==1 THEN
   openFileDialogfunction.lpstrInitialDir="E:\\IMAGENES\\IMAGENES DE PRUEBAS\\VGA_IMAGENES";
ELSIF DirOption==2 THEN
   openFileDialogfunction.lpstrInitialDir="E:\\IMAGENES\\PANTALLAS DE FONDO\\MALANDRAS";
ELSIF DirOption==3 THEN
   openFileDialogfunction.lpstrInitialDir="E:\\IMAGENES\\PERSONAL\\FAMILIA\\__MI_PAPA\\101_PANA\\09_12_2011";
ELSIF DirOption==4 THEN
   openFileDialogfunction.lpstrInitialDir="E:\\IMAGENES\\IMAGENES DE PRUEBAS\\FOTOS\\";
ELSE
   
ENDIF;
//  openFileDialogfunction.lpstrInitialDir = BeforeDir;
openFileDialogfunction.lpstrFilter = "MP3 Files (*.mp3*)\0*.mp3\0WAV Files (*.wav*)\0*.wav\0All Files(*.*)\0*.*\0\0";
openFileDialogfunction.lpstrFile = FileNameAux;
openFileDialogfunction.nMaxFile = 32*MAX_PATH;
openFileDialogfunction.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
openFileDialogfunction.lpstrDefExt = "";
if(!GetOpenFileName(&openFileDialogfunction))
   return false;
sprintf( FileName, "%s", FileNameAux);
return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI SaveFileAs( char *FileName )
{
OPENFILENAME ofn;
ZeroMemory(&ofn, sizeof(ofn));
ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = VENTANA_PADRE;
ofn.nFilterIndex = 2;
ofn.lpstrFileTitle = NULL;
ofn.lpstrInitialDir= "C:";
ofn.lpstrFilter = "JPG Files (*.jpg*)\0*.jpg\0BMP Files (*.bmp*)\0*.bmp\0\0";
//  ofn.lpstrFilter = "JPG Files (*.jpg*)\0*.jpg\0\0";
ofn.lpstrFile = FileNameAux;
ofn.nMaxFile = MAX_PATH;
ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
 OFN_OVERWRITEPROMPT ;
ofn.lpstrDefExt = ".bmp";
if( !GetSaveFileNameA(&ofn) )
    return 0;
sprintf( FileName, "%s", FileNameAux );

int LenStr = strlen( FileName )-1;

if( FileName[LenStr]=='g' || FileName[LenStr]=='G' )
    return 2;
return 1;
}





