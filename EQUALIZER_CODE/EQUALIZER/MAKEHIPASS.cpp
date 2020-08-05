void MAKEHIPASS( void ){
BUFFSHORT = (short*)&BUFFSONG[0];
sprintf( TextoGlobal, "MAKEHIPASS" );
SetWindowTextA( EDIT_INFO, TextoGlobal );
DOIT_AGAINH:
tic
CurLHiPass = SendMessage( HWndHI, CB_GETCURSEL, 0, (LPARAM)0 );
HiPassC = CurLHiPass;
FOR int i=256; i<25*MBs; i++ LOOP
    //Valor = (int)BUFFSHORT[i-2] - (int)ORIGEN_BUFF[i-8]/4 + (int)ORIGEN_BUFF[i-2]/4;
    ValorHI = (int)ORIGEN_BUFF[i] - (int)HiPassC*BUFFSHORT[i-10];// + (int)BUFFSHORT[i-4] - (int)BUFFSHORT[i-6];
    ValorHI = ValorHI/(HiPassC+1);
    IF ValorHI>32767 THEN
       BUFFSHORT[i] = 32767;
    ELSIF ValorHI<-32767 THEN
       BUFFSHORT[i] = -32767;
    ELSE
       BUFFSHORT[i] = (short)ValorHI;
    ENDIF;
    //*/
ENDLOOP;
EndToc = toc;
Norm();
IF CurLHiPass!=SendMessage( HWndHI, CB_GETCURSEL, 0, (LPARAM)0 ) THEN
   goto DOIT_AGAINH;
ENDIF;


sprintf( TextoGlobal,
         "HIPASS\n\n"
         "EndToc, %f Sec\n\n",
         EndToc );
SetWindowTextA( EDIT_INFO, TextoGlobal );






/*
MAKELOWPASS();
BUFFSHORT = (short*)&BUFFSONG[0];
FOR int i=256; i<25*MBs; i++ LOOP
    Valor = (int)BUFFSHORT[i] - (int)BUFFSHORT[i-2];
    //Valor = Valor/2;
    IF Valor>32767 THEN
       BUFFSHORT[i] = 32767;
    ELSIF Valor<-32767 THEN
       BUFFSHORT[i] = -32767;
    ELSE
       BUFFSHORT[i] = (short)Valor;
    ENDIF;
ENDLOOP;
Norm();
//*/
}

