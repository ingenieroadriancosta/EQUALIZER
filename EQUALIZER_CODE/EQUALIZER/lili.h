#ifndef LILI_INCLUDED
#define LILI_INCLUDED 1

/*
+---------------------------------------------------------------+
|    LILI (Liteweight Image Library)
|    -------------------------------
|    Version    : 1.1
|    Intro      : linkable lighweight image loading library for win32
|    Formats    : JPEG, GIF (transparent also), BMP
|    Copyrights : www.apitalk.com
|
|
|    Remember to link with libraries:
|	 lili, ole32, olepro32, and uuid
|
|    mingW linker parameter : -llili -lole32 -lolepro32	-luuid
|
|
+---------------------------------------------------------------+
*/
#include <windows.h>
#include <olectl.h>

/* Load image from a char string provided, which must be gif, jpg, bmp file data */
HBITMAP loadImageString(char *imagedata, int size);

/* Load image from an external gif, jpg, bmp file */
HBITMAP loadImage(char *filename);

/* Load image from resources, resource type must not be BITMAP, but any other identifier */
HBITMAP loadResImage(int resId, char *resType);


#endif

