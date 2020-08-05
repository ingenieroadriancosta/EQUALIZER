char            szClassName[] = " EQUALIZER ";
HWND            HWndConsole;
HDC             HDCMAIN, HDCPROC;
HBRUSH          pincel;
PAINTSTRUCT     ps;
HBITMAP         HBitmap;


BOOL SHOWWINDOW = true;
NOTIFYICONDATAA nid;
LPCMENUITEMINFOA Item;

HWND HWPlay, HWStop, HWEXIT;
HWND EDIT_INFO;
HWND TIME_INFO;
HWND HWndVALPHA;


HWND HWndDELAY;
int  DELAY = 0;
int  REALDELAY = 0;


HWND HWndAmpliDELAY;
int  AmplitudDELAY = 255;
int  ValorDelay = 255;

//HWND HWndPRB;
HWND HWndLOW;
HWND HWndHI;

HBITMAP HBitmapPlay;


int  CurLine = 0;

BYTE VALPHA = 255;
bool IN_SCROLL = false;
bool IN_PAUSE = false;



const int  VAdvance[16] = { 
        0,   1,  44,  62,  66,  93, 101, 112,
      135, 181, 181, 204, 226, 250,
      265};

const int  NVAdvance[16] = { 
        1,   2,  13,  21,  28,  28,  43,  50,
       59,  77,  77, 86, 97, 117, 
      200 };








char       SongTextEdit[100*1024];



int  IndexSong = 0;
int  IndexList = 4;
char SongFile[256][256] ={
"X:\\SegAll\\MUSICA\\VALLENATO\\SILVESTRE\\_FAV\\La difunta.mp3",
"X:\\SegAll\\MUSICA\\PREDETERMINADO\\NO MORIRA.mp3",
"X:\\SegAll\\MUSICA\\VALLENATO\\KVRASS\\AQUI VA VE VAINA\\el tira que jala.mp3",
"X:\\SegAll\\MUSICA\\VALLENATO\\KVRASS\\ME QUEDO ENCUERO\\me quedo encuero.mp3"
}
//"E:\\ENTERPRISE\\C++\\EJERCICIOS\\HE_STILL_LOVES_ME\\HE STILL LOVES ME.wav",
;

////////////////////////////////////////////////////////////////////////////////
int       Valor = 0;
int       ValorHI = 0;
int       ValorLW = 0;
short     Maxim = 0;
short     Minim = 0;
float     Coef = 0;

int       CurLowPass = 0;
int       CurLHiPass = 0;
int       HiPassC    = 0;

int       NLOWPASS = 0;




HWND    HWndDOPPLER;
float   IDOPPLER = 1;
bool    SUMDOPPLER = false;

int     IDPINT = 0;
int     CurDoppler = 0;


const float  DOP_SR_MAX = 0.00900000;
const float  DOP_SR_MIN = -DOP_SR_MAX;
float        DOP_SR = DOP_SR_MAX;
float        VOL_DOP_SR = 1.0;






