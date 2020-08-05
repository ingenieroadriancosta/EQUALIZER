void MAKEREVERV( void )
{
REDO:
BUFFSHORT = (short*)&BUFFSONG[0];
DELAY = SendMessage( HWndDELAY, CB_GETCURSEL, 0, (LPARAM)0 );
AmplitudDELAY = SendMessage( HWndAmpliDELAY, CB_GETCURSEL, 0, (LPARAM)0 );
REALDELAY = (int)(FS*DELAY)/1000;
int InitMem = REALDELAY;
IF REALDELAY!=0 && AmplitudDELAY!=0 THEN
   FOR int i=InitMem; i<24*MBs; i++ LOOP
       ValorDelay = (int)( ( 255*ORIGEN_BUFF[i] + AmplitudDELAY*ORIGEN_BUFF[i-2*REALDELAY] )/(AmplitudDELAY+255) );
       BUFFSHORT[i] = (short)ValorDelay;
   ENDLOOP;
ELSE
   FOR int i=InitMem; i<24*MBs; i++ LOOP
       BUFFSHORT[i] = ORIGEN_BUFF[i];
   ENDLOOP;
ENDIF;


IF DELAY!=SendMessage( HWndDELAY, CB_GETCURSEL, 0, (LPARAM)0 ) THEN
   goto REDO;
ENDIF;
IF AmplitudDELAY!=SendMessage( HWndAmpliDELAY, CB_GETCURSEL, 0, (LPARAM)0 ) THEN
   goto REDO;
ENDIF;
}


 
