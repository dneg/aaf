/*****************************************************************************\
*                                                                             *
* wintypes.h -  Windows compatible types and definitions                      *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
// >>> © 1996-1997 Microsoft Corporation.  All rights reserved. <<<
*                                                                             *
\*****************************************************************************/

#ifndef _WINTYPES_H_
#define _WINTYPES_H_

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

/*
 * BASETYPES is defined in ntdef.h if these types are already defined
 */

#ifndef BASETYPES
#define BASETYPES
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#endif  /* !BASETYPES */

/*
 * _WINDEF_ is defined in windef.h if these types are already defined
 */

#ifndef _WINDEF_

typedef	unsigned short		WCHAR;
typedef	WCHAR				*LPWSTR;
typedef	char*				LPCSTR;
typedef	char*				LPSTR;
typedef short				SHORT;
typedef long                LONG;
typedef unsigned long       DWORD;
#if defined(_WIN32) // trr 19980824 - Need to use the "same" declaration for BOOL as OLEBOOL
#include <limits.h>
#if UINT_MAX != ULONG_MAX
#pragma message("### warning: sizeof(int) != sizeof(long)!")
#endif
typedef int                 BOOL;
#else
typedef unsigned long		BOOL;
#endif
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef BOOL                *LPBOOL;
typedef BYTE                *LPBYTE;
typedef int                 *LPINT;
typedef WORD                *LPWORD;
typedef long                *LPLONG;
typedef DWORD               *LPDWORD;
typedef void                *LPVOID;
typedef const void          *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef unsigned long CLIPFORMAT;
typedef unsigned long TEXTMETRIC;
typedef unsigned long SIZE;
typedef unsigned long COLORREF;
typedef struct _RECT
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
}
RECT; 
typedef RECT *LPCRECT;

typedef UINT WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;
typedef LPVOID    LPMSG;
#define DECLSPEC_IMPORT

#ifdef STRICT
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#else
typedef LPVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif


#endif /* _WINDEF */

/* Opaque handle types when interoperating with WLM-based components */
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HOLEMENU);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HACCEL);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HMETAFILEPICT);
DECLARE_HANDLE(HENHMETAFILE);
DECLARE_HANDLE(HGLOBAL);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HMETAFILE);
DECLARE_HANDLE(HCURSOR);
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HINSTANCE);




#endif /* _WINTYPES_H_ */
