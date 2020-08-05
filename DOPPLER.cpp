void DOPPLER( void )
{
sprintf( TextoGlobal, "MAKEDOPPLER" );
SetWindowTextA( EDIT_INFO, TextoGlobal );
DOITDOPPLER:
BUFFSHORT = (short*)&BUFFSONG[0];
CurDoppler = SendMessage( HWndDOPPLER, CB_GETCURSEL, 0, (LPARAM)0 );
IDOPPLER = 1;//(CurDoppler-5)/100.0 + 1;
DOP_SR   = DOP_SR_MAX;
int FileSizeAMED = FileSizeA/4;
if( FileSizeAMED==0 )
    FileSizeAMED = 2;
tic
FOR int i=0; i<25*MBs; i++ LOOP
    VOL_DOP_SR = (float)abs(FileSizeAMED - i);
    VOL_DOP_SR = VOL_DOP_SR/(float)FileSizeAMED;
    VOL_DOP_SR = 1 - VOL_DOP_SR;
    BUFFSHORT[i] = (short)(VOL_DOP_SR*ORIGEN_BUFF[i]);
ENDLOOP;



//sprintf( TextoGlobal, "%f", (float)log(10) );
//msgbox( TextoGlobal );

/*
FOR int i=0; i<25*MBs; i++ LOOP
    IDPINT = i;//(int)(i*IDOPPLER);
    IF IDPINT>=25*MBs || IDPINT<0 THEN
       IDPINT = 25*MBs;
    ENDIF;
    
    IF i%2==0 && IDPINT%2!=0 THEN
       IF IDPINT%2!=0 THEN
          IDPINT++;
       ENDIF;
    ELSIF i%2!=0 THEN
       IF IDPINT%2==0 THEN
          IDPINT++;
       ENDIF;
    ENDIF;
    
    IF DOP_SR<0 THEN
       VOL_DOP_SR = 1 + (100*(DOP_SR)) - 0.1;
    ELSE
       VOL_DOP_SR = 1 - 100*(DOP_SR) - 0.1 ;
    ENDIF;
    BUFFSHORT[i] = (short)(VOL_DOP_SR*ORIGEN_BUFF[IDPINT]);
    
    
    
    //IDOPPLER = 0.95;
    IDOPPLER = 1.00000000 + DOP_SR;
    IF DOP_SR>DOP_SR_MAX || DOP_SR<DOP_SR_MIN THEN
       SUMDOPPLER = !SUMDOPPLER;
       sprintf( TextoGlobal, "SUMDOPPLER = !SUMDOPPLER;\n\n%i", (i)/(2*FS) );
       msgbox( TextoGlobal );
       IF DOP_SR>DOP_SR_MAX THEN
          DOP_SR = DOP_SR_MAX;
       ELSIF DOP_SR<DOP_SR_MIN THEN
          DOP_SR = DOP_SR_MIN;
       ENDIF;
    ENDIF;
    IF i%(2*FS)==0 THEN
       IF SUMDOPPLER THEN
          DOP_SR = DOP_SR + 0.0001;//0.001
       ELSE
          DOP_SR = DOP_SR - 0.0001;
       ENDIF;
    ENDIF;
    //*/
    
    
    
//ENDLOOP;



EndToc = toc;
IF CurDoppler!=SendMessage( HWndDOPPLER, CB_GETCURSEL, 0, (LPARAM)0 ) THEN
   goto DOITDOPPLER;
ENDIF;
sprintf( TextoGlobal,
         "MAKEDOPPLER\n\n"
         "EndToc, %f Sec\n\n",
         EndToc );
SetWindowTextA( EDIT_INFO, TextoGlobal );
}
