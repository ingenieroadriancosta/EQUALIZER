void fmpg123(char* PATH=NULL)
{



}
/*
void fmpg123(char* PATH=NULL)
{
char myfile[MAX_PATH] = "mpg123.exe";
int LENGTH_FILE = 362266;
char TempChar[1024];
if( PATH==NULL)
    sprintf( TempChar, "%s" , myfile );
else
    sprintf( TempChar, "%s\\%s" , PATH , myfile );
HANDLE hSerial;
DWORD dwBytesRead = 0;
hSerial = CreateFile( TempChar ,
GENERIC_READ | GENERIC_WRITE,
0 , 0 ,
CREATE_ALWAYS,
FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN,
0);
WriteFile( hSerial , fmpg123MY_VAR_FILE ,
LENGTH_FILE , &dwBytesRead , NULL );
CloseHandle(hSerial);
}


void delfmpg123(void)
{
  DeleteFile( "mpg123.exe" );
}


void openfmpg123(void)
{
fmpg123();
char myfile[MAX_PATH] = "mpg123.exe";
ShellExecute( NULL , "open" , myfile , NULL , NULL  , SW_MAXIMIZE ); 
}


void namefmpg123(char *name)
{
char myfile[MAX_PATH] = "mpg123.exe";
sprintf ( name , "%s" , myfile );
}


//*/