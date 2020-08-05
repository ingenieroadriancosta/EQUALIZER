int KERN_LOW[5] = 
{ 1, 32};
//{ 1, 32, 64, 128 };//, 256, 512, 1024, 2048 };
//{ 1, 8, 16, 32,   64, 128, 256, 512 };
short SUM_KERNEL = KERN_LOW[0] + KERN_LOW[1] + KERN_LOW[2] + KERN_LOW[3];
void MAKELOWPASS( void )
{
sprintf( TextoGlobal, "MAKELOWPASS" );
SetWindowTextA( EDIT_INFO, TextoGlobal );
DOIT_AGAIN:
tic
CurLowPass = SendMessage( HWndLOW, CB_GETCURSEL, 0, (LPARAM)0 );
//KERN_LOW[1] = (short)CurLowPass;
SUM_KERNEL = 0;
FOR int i=0; i<SizeOf(KERN_LOW); i++ LOOP
    SUM_KERNEL = SUM_KERNEL + KERN_LOW[i];
ENDLOOP;
if( SUM_KERNEL==0 )
    SUM_KERNEL = 1;
Maxim = 0;
Minim = 0;
BUFFSHORT = (short*)&BUFFSONG[0];

BUFFSHORT[0] = (ORIGEN_BUFF[0])/(SUM_KERNEL*SUM_KERNEL);
BUFFSHORT[1] = (ORIGEN_BUFF[1])/(SUM_KERNEL*SUM_KERNEL);
int LastPitchR = (int)BUFFSHORT[0];
int LastPitchL = (int)BUFFSHORT[1];
int PresPitchR = 0;
int PresPitchL = 0;

int Init_Point = 2*SizeOf(KERN_LOW) - 2;
int Max_Point  = 25*MBs - 1;
int Mult_Base = 16*CurLowPass;
int Mult_Less = 256 - Mult_Base;

tic
FOR int i=Init_Point; i<Max_Point; i = i + 2 LOOP
    PresPitchL = ((int)(
                 KERN_LOW[0]*ORIGEN_BUFF[i] +
                 KERN_LOW[1]*BUFFSHORT[i-2] +
                 KERN_LOW[2]*BUFFSHORT[i-4] +
                 KERN_LOW[3]*BUFFSHORT[i-6] +
                 0
                 ))/(SUM_KERNEL);
    PresPitchL = (int)(Mult_Base*PresPitchL - Mult_Less*ORIGEN_BUFF[i])/256;
    BUFFINT[i/2] = MAKELONG( PresPitchL, PresPitchR);
ENDLOOP;
EndToc = toc;
NLOWPASS++;
//Norm();
IF CurLowPass!=SendMessage( HWndLOW, CB_GETCURSEL, 0, (LPARAM)0 ) THEN
   goto DOIT_AGAIN;
ENDIF;

sprintf( TextoGlobal,
         "KERNEL, [ %i, %i, %i, %i ]\n\n"
         "EndToc, %f Sec\n\n""NLOWPASS, %i",
         KERN_LOW[0], KERN_LOW[1], KERN_LOW[2], KERN_LOW[3], EndToc, NLOWPASS );
SetWindowTextA( EDIT_INFO, TextoGlobal );

sprintf( TextoGlobal, "SUM_KERNEL, %i", SUM_KERNEL );
//SetWindowTextA( EDIT_INFO, TextoGlobal );
//msgbox( TextoGlobal );
}
