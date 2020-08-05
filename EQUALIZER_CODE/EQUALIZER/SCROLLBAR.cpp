
/////////////////////////////// S C R O L L B A R //////////////////////////////
struct SCROLLBAR
{
int        x;
int        y;
int        width;
int        height;
int        maxrange;
int        minrange;
int        Position;
bool       ShowMode;
HWND       Tag;
HWND       Father;
HINSTANCE  Instance;
TIMERPROC  Function;

void create( bool ShowM=true , int xx=0 , int yy=0 , int ww=10 , int hh=10 )
{
SCROLLINFO si;
DWORD Mode;

ShowMode = ShowM;
x = xx;
y = yy;
width = ww;
height = hh;

if( Father==NULL )
    Father = VENTANA_PADRE;
if( Instance==NULL )
    Instance = INSTANCIA_GLOBAL;

DestroyWindow( Tag );

if( ShowMode )
    Mode = SBS_VERT;
else
    Mode = SBS_HORZ;
Tag = CreateWindowEx( 0 , "SCROLLBAR" , "SCROLL",
                     WS_CHILD | WS_VISIBLE | Mode ,
                     x , y ,  width , height ,
                     Father , (HMENU)NULL, Instance , NULL);
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin   = minrange;
    si.nMax   = maxrange;
    si.nPage  = 0;
    si.nPos   = 10;
    SetScrollInfo( Tag , SB_CTL , &si , TRUE );
    SetScrollPos( Tag , SB_CTL , Position , TRUE );
}

int ProcesarScrollH( WPARAM wParam, LPARAM lParam )
{
   if( (HWND)lParam!=Tag ){
       return false;
   }
    int I2Ret = 1;
   Position = GetScrollPos( Tag , SB_CTL );
   int Posicion = (int)HIWORD(wParam);
   int Codigo = (int)LOWORD(wParam);
   switch( Codigo ){
      case SB_BOTTOM:
         Position = minrange;
         break;
      case SB_TOP:
         Position = maxrange;
         break;
      case SB_LINEDOWN:
         Position++;
         break;
      case SB_LINEUP:
         Position--;
         break;
      case SB_PAGEDOWN:
         Position += 5;
         break;
      case SB_PAGEUP:
         Position -= 5;
         break;
      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         Position = Posicion;
      case SB_ENDSCROLL:
          I2Ret = -1;
         break;
   }
   if( Position<=minrange) Position = minrange;
   if( Position>=maxrange ) Position = maxrange;
   SetScrollPos( Tag , SB_CTL , Position , TRUE );

   return I2Ret;
}
}HscrollPitch, HscrollVol;
