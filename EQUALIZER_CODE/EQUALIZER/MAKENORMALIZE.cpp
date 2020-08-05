BOOL InNormalize = FALSE;
int INOrm = 0;
BOOL MAKENORMALIZE( void ){

// EnableWindow( HWndNormalize, FALSE );

INOrm = SendMessage( HWndNormalize, CB_GETCURSEL, 0, (LPARAM)0 );


BUFFSHORT = (short*)&BUFFSONG[0];
short*         TBSHORT   = (short*)&BUFFSONG[0];
short maxVAbs = -1;
FOR int i=0; i<FileSizeA/2; i = i + 2 LOOP
    maxVAbs = max( abs(ORIGEN_BUFF[i]), maxVAbs );
    maxVAbs = max( abs(ORIGEN_BUFF[i+1]), maxVAbs );
ENDLOOP;
const double V2Mul = 32767.0/( (double)maxVAbs );
FOR int i=0; i<FileSizeA/2; i = i + 2 LOOP
    TBSHORT[i]   = Clamp16Bits( (int)round( (1+INOrm/10) * V2Mul * ORIGEN_BUFF[i] ) );
    TBSHORT[i+1] = Clamp16Bits( (int)round( (1+INOrm/10) * V2Mul * ORIGEN_BUFF[i+1] ) );
ENDLOOP;

const double EnergyC = GetEnergy( TBSHORT );
sprintf( TextoGlobal, "Canción actual,\n%s\nEnergia = %5.4f\n\n\n%i Canciones.", Text2, EnergyC, IndexList );
SetWindowTextA( EDIT_INFO, TextoGlobal );

EnableWindow( HWndNormalize, TRUE );
return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
double GetEnergy( short* SInArray ){
    double Energy = 0;
    double E2Temp = 0;
    for( int i=0; i<FileSizeA/2; i = i + 2 ){
        E2Temp = ( ( SInArray[i+1]/32768.0 + SInArray[i]/32768.0 )/2.0 );
        E2Temp = E2Temp * E2Temp;
        Energy = Energy + E2Temp;
    }
return Energy/(FileSizeA/2);
}


