/*+---------------------------------------------------------------------------
**
** File:        WChar.h
**
** Contents:    Defines wide character equivalents for standard functions
**              usually in strings.h and ctypes.h
**
** Note:        These routines uses WCHAR which is unsigned short (2 bytes)
**              They are not compatible with some systems that uses 4 bytes
**              wide characters
**--------------------------------------------------------------------------*/

#ifndef __WCHAR__H__
#define __WCHAR__H__

#define _WSTRING_DEFINED // prevent incompatibility with <string.h>
#include <stdlib.h>

#if !defined(FLAT) || defined(OLE32)
#include <stdarg.h>

#if defined(__cplusplus) && !defined(__sgi) && !defined(__linux__) && !defined(__FreeBSD__)
extern "C" {
#endif

#ifndef _MSC_VER
#define _CRTAPI1
#endif

typedef unsigned short WCHAR, *LPWSTR;
typedef const WCHAR* LPCWSTR;

/* use an alias */
#define _wcsnicmp wcsnicmp

/* Unicode Byte Order Mark (BOM) for Unicode text files */
#define BOM 0xFEFF

/* Padding constant and macro for localized buffer allocation*/
#define INTL_PADDING_VALUE 3
#define INTL_PADDING(cb) (INTL_PADDING_VALUE * (cb))

long     _CRTAPI1 wcsatol(const WCHAR *wsz);
int	 _CRTAPI1 wcsatoi(const WCHAR *wsz);
WCHAR *  _CRTAPI1 wcscat(WCHAR *wsz1, const WCHAR *wsz2);
WCHAR *  _CRTAPI1 wcschr ( const WCHAR * string, WCHAR ch );
int      _CRTAPI1 wcscmp(const WCHAR *wsz1, const WCHAR *wsz2);
int      _CRTAPI1 wcsicmp(const WCHAR *wsz1, const WCHAR *wsz2);
int      _CRTAPI1 wcscoll(const WCHAR * wsz1, const WCHAR * wsz2);
WCHAR *  _CRTAPI1 wcscpy(WCHAR *wsz1, WCHAR const *wsz2);
WCHAR *  _CRTAPI1 wcsitoa(int ival, WCHAR *wsz, int radix);
size_t   _CRTAPI1 wcslen(WCHAR const *wsz);
WCHAR *  _CRTAPI1 wcsltoa(long lval, WCHAR *wsz, int radix);
WCHAR *  _CRTAPI1 wcslwr(WCHAR *wsz);
int      _CRTAPI1 wcsncmp(const WCHAR *wsz1, const WCHAR *wsz2, size_t count);
int      _CRTAPI1 wcsnicmp(const WCHAR *wsz1, const WCHAR *wsz2, size_t count);
WCHAR *  _CRTAPI1 wcsncpy ( WCHAR * dest, const WCHAR * source, size_t count );
WCHAR *  _CRTAPI1 wcsrchr(const WCHAR * wcs, WCHAR wc);
WCHAR *  _CRTAPI1 wcsupr(WCHAR *wsz);
WCHAR *  _CRTAPI1 wcswcs(const WCHAR *wsz1, const WCHAR *wsz2);
size_t   _CRTAPI1 wcstosbs( char * s, const WCHAR * pwcs, size_t n);
size_t   _CRTAPI1 sbstowcs(WCHAR *wcstr, const char *mbstr, size_t count);

#ifndef STDCALL
#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif
#endif

extern int STDCALL MultiByteToWideChar(
    unsigned int CodePage,              /* code page */
    unsigned long dwFlags,              /* character-type options  */
    const char * lpMultiByteStr,	/* address of string to map  */
    int cchMultiByte,           /* number of characters in string  */
    WCHAR* lpWideCharStr,	/* address of wide-character buffer  */
    int cchWideChar             /* size of buffer  */
   );	
 
extern int STDCALL WideCharToMultiByte(
    unsigned int CodePage,              /* code page */
    unsigned long dwFlags,              /* performance and mapping flags */
    const WCHAR* lpWideCharStr,	/* address of wide-character string */
    int cchWideChar,            /* number of characters in string */
    char* lpMultiByteStr,	/* address of buffer for new string */
    int cchMultiByte,           /* size of buffer  */
    const char* lpDefaultChar,	/* addr of default for unmappable chars */
    int* lpUsedDefaultChar 	/* addr of flag set when default char. used */
   );

#if defined(__cplusplus) && !defined(__sgi) && !defined(__linux__) && !defined(__FreeBSD__)
}
#endif

#endif /* !defined(FLAT) || defined(OLE32) */

#endif  /* __WCHAR__H__ */
