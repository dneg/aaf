/*** 
*variant.h
*
*  Copyright (C) 1992-1997, Microsoft Corporation.  All Rights Reserved.
*
*Purpose:
*  This file declares VARIANT, and related data types.
*
*Implementation Notes:
*  This file requires ole2.h
*
*****************************************************************************/

#ifndef _VARIANT_H_
#define _VARIANT_H_


#ifndef HUGEP
# ifdef __MACAPI__
#  define HUGEP FAR
# else
#  if WIN32
#   define HUGEP
#  else
#   define HUGEP _huge
#  endif
# endif
#endif

#ifndef FAR
# ifdef _MAC
#  define FAR
# else
#  ifdef WIN32
#   define FAR
#  else
#   define FAR _far
#  endif
# endif
#endif


/* Forward Declarations */

#ifdef __cplusplus
interface IDispatch;
#else
typedef interface IDispatch IDispatch;
#endif


#ifndef OLESTR
#if defined(WIN32) && !defined(_MAC)
typedef WCHAR OLECHAR;
typedef LPWSTR LPOLESTR;
typedef LPCWSTR LPCOLESTR;
#define OLESTR(str) L##str
#else
typedef char OLECHAR;
typedef OLECHAR FAR* LPOLESTR;
typedef const OLECHAR FAR* LPCOLESTR;
#define OLESTR(str) str
#endif
#endif

typedef OLECHAR FAR* BSTR;
typedef BSTR FAR* LPBSTR;


typedef struct FARSTRUCT tagSAFEARRAYBOUND {
    unsigned long cElements;
    long lLbound;
} SAFEARRAYBOUND, FAR* LPSAFEARRAYBOUND;

typedef struct FARSTRUCT tagSAFEARRAY {
    unsigned short cDims;
    unsigned short fFeatures;
#if defined(WIN32) && !defined(_MAC)
    unsigned long cbElements;
    unsigned long cLocks;
#else
    unsigned short cbElements;
    unsigned short cLocks;
#ifdef _MAC
    Handle handle;
#else
    unsigned long handle;
#endif
#endif
    void HUGEP* pvData;
    SAFEARRAYBOUND rgsabound[1];
} SAFEARRAY, FAR* LPSAFEARRAY;

#define FADF_AUTO       0x0001  /* array is allocated on the stack         */
#define FADF_STATIC     0x0002  /* array is staticly allocated             */
#define FADF_EMBEDDED   0x0004  /* array is embedded in a structure        */
#define FADF_FIXEDSIZE  0x0010  /* array may not be resized or reallocated */
#define FADF_RECORD     0x0020  /* an array of records                     */
#define FADF_HAVEIID    0x0040  /* with FADF_DISPATCH or FADF_UNKNOWN,     */
                                /* array has an IID identifying interface  */
#define FADF_HAVEVARTYPE 0x0080 /* array has a VT type                     */
#define FADF_BSTR       0x0100  /* an array of BSTRs                       */
#define FADF_UNKNOWN    0x0200  /* an array of IUnknown*                   */
#define FADF_DISPATCH   0x0400  /* an array of IDispatch*                  */
#define FADF_VARIANT    0x0800  /* an array of VARIANTs                    */
#define FADF_RESERVED   0xF008  /* bits reserved for future use            */


/* 0 == FALSE, -1 == TRUE */
typedef short VARIANT_BOOL;
#ifndef VARIANT_TRUE
#define VARIANT_TRUE ((VARIANT_BOOL) 0xffff)
#endif
#ifndef VARIANT_FALSE
#define VARIANT_FALSE ((VARIANT_BOOL) 0)
#endif


typedef double DATE;

#if defined(_MSC_VER) && (_INTEGRAL_MAX_BITS >= 64)
typedef __int64 LONGLONG;
typedef unsigned __int64 DWORDLONG;
#else //_MSCVER
typedef double LONGLONG;
typedef double DWORDLONG;
#endif //_MSCVER


/* This is a helper struct for use in handling currency. */
typedef union FARSTRUCT tagCY {
    struct {         
#ifdef _MAC          
        long          Hi;
        unsigned long Lo;
#else                
        unsigned long Lo;
        long          Hi;
#endif               
    };               
    LONGLONG int64;  
} CY;                

typedef CY CURRENCY;
typedef CY *LPCY;

// Decimal data type
//
typedef struct tagDEC {
    unsigned short wReserved;
    union {
      struct {
        char scale;
        char sign;
      };
      unsigned short signscale;
    };
    unsigned long Hi32;
    union {
      struct {
#ifdef _MAC
        unsigned long Mid32;
        unsigned long Lo32;
#else
        unsigned long Lo32;
        unsigned long Mid32;
#endif
      };
      DWORDLONG Lo64;
    };
} DECIMAL;

typedef DECIMAL *LPDECIMAL;

#define DECIMAL_NEG ((char)0x80)
#define DECIMAL_SETZERO(dec) {(dec).Lo64 = 0; (dec).Hi32 = 0; (dec).signscale = 0;}


/* 0 == FALSE, -1 == TRUE */
/* size is 2 */
typedef short VARIANT_BOOL;

#ifndef VARIANT_TRUE
#define VARIANT_TRUE ((VARIANT_BOOL)0xffff)
#endif
#ifndef VARIANT_FALSE
#define VARIANT_FALSE ((VARIANT_BOOL)0)
#endif
/*
 * VARENUM usage key,
 *
 *   [V] - may appear in a VARIANT
 *   [T] - may appear in a TYPEDESC
 *   [P] - may appear in an OLE property set
 *   [S] - may appear in a Safe Array
 *
 */
enum VARENUM
{
    VT_EMPTY           = 0,   /* [V]   [P]     nothing                     */
    VT_NULL            = 1,   /* [V]           SQL style Null              */
    VT_I2              = 2,   /* [V][T][P][S]  2 byte signed int           */
    VT_I4              = 3,   /* [V][T][P][S]  4 byte signed int           */
    VT_R4              = 4,   /* [V][T][P][S]  4 byte real                 */
    VT_R8              = 5,   /* [V][T][P][S]  8 byte real                 */
    VT_CY              = 6,   /* [V][T][P][S]  currency                    */
    VT_DATE            = 7,   /* [V][T][P][S]  date                        */
    VT_BSTR            = 8,   /* [V][T][P][S]  OLE Automation string       */
    VT_DISPATCH        = 9,   /* [V][T]   [S]  IDispatch FAR*              */
    VT_ERROR           = 10,  /* [V][T]   [S]  SCODE                       */
    VT_BOOL            = 11,  /* [V][T][P][S]  True=-1, False=0            */
    VT_VARIANT         = 12,  /* [V][T][P][S]  VARIANT FAR*                */
    VT_UNKNOWN         = 13,  /* [V][T]   [S]  IUnknown FAR*               */
    VT_DECIMAL         = 14,  /* [V][T]   [S]  16 byte fixed point         */

    VT_I1              = 16,  /* [v][T]   [s]  char                        */
    VT_UI1             = 17,  /* [V][T]   [S]  unsigned char               */
    VT_UI2             = 18,  /* [V][T]   [S]  unsigned short              */
    VT_UI4             = 19,  /* [V][T]   [S]  unsigned short              */
    VT_I8              = 20,  /*    [T][P]     signed 64-bit int           */
    VT_UI8             = 21,  /*    [T]        unsigned 64-bit int         */
    VT_INT             = 22,  /* [V][T]   [S]  signed machine int          */
    VT_UINT            = 23,  /* [V][T]   [S]  unsigned machine int        */
    VT_VOID            = 24,  /*    [T]        C style void                */
    VT_HRESULT         = 25,  /*    [T]                                    */
    VT_PTR             = 26,  /*    [T]        pointer type                */
    VT_SAFEARRAY       = 27,  /*    [T]        (use VT_ARRAY in VARIANT)   */
    VT_CARRAY          = 28,  /*    [T]        C style array               */
    VT_USERDEFINED     = 29,  /*    [T]        user defined type           */
    VT_LPSTR           = 30,  /*    [T][P]     null terminated string      */
    VT_LPWSTR          = 31,  /*    [T][P]     wide null terminated string */

    VT_FILETIME        = 64,  /*       [P]     FILETIME                    */
    VT_BLOB            = 65,  /*       [P]     Length prefixed bytes       */
    VT_STREAM          = 66,  /*       [P]     Name of the stream follows  */
    VT_STORAGE         = 67,  /*       [P]     Name of the storage follows */
    VT_STREAMED_OBJECT = 68,  /*       [P]     Stream contains an object   */
    VT_STORED_OBJECT   = 69,  /*       [P]     Storage contains an object  */
    VT_BLOB_OBJECT     = 70,  /*       [P]     Blob contains an object     */
    VT_CF              = 71,  /*       [P]     Clipboard format            */
    VT_CLSID           = 72   /*       [P]     A Class ID                  */
};

#define VT_VECTOR      0x1000 /*       [P]     simple counted array        */
#define VT_ARRAY       0x2000 /* [V]           SAFEARRAY*                  */
#define VT_BYREF       0x4000 /* [V]                                       */
#define VT_RESERVED    0x8000


typedef unsigned short VARTYPE;

typedef struct FARSTRUCT tagVARIANT VARIANT;
typedef struct FARSTRUCT tagVARIANT FAR* LPVARIANT;
typedef struct FARSTRUCT tagVARIANT VARIANTARG;
typedef struct FARSTRUCT tagVARIANT FAR* LPVARIANTARG;

#if defined(NONAMELESSUNION) || (defined(_MAC) && !defined(__cplusplus) && !defined(_MSC_VER))
#define __VARIANT_NAME_1 n1
#define __VARIANT_NAME_2 n2
#define __VARIANT_NAME_3 n3
#else
#define __VARIANT_NAME_1
#define __VARIANT_NAME_2
#define __VARIANT_NAME_3
#endif

struct FARSTRUCT tagVARIANT {
  union {
    struct {
      VARTYPE vt;
      unsigned short wReserved1;
      unsigned short wReserved2;
      unsigned short wReserved3;
      union {
        unsigned char         bVal;           /* VT_UI1               */
        short                 iVal;           /* VT_I2                */
        long                  lVal;           /* VT_I4                */
        float                 fltVal;         /* VT_R4                */
        double                dblVal;         /* VT_R8                */
        VARIANT_BOOL          boolVal;        /* VT_BOOL              */
        SCODE                 scode;          /* VT_ERROR             */
        CY                    cyVal;          /* VT_CY                */
        DATE                  date;           /* VT_DATE              */
        BSTR                  bstrVal;        /* VT_BSTR              */
        IUnknown FAR*         punkVal;        /* VT_UNKNOWN           */
        IDispatch FAR*        pdispVal;       /* VT_DISPATCH          */
        SAFEARRAY FAR*        parray;         /* VT_ARRAY|*           */
  
        unsigned char FAR *   pbVal;          /* VT_BYREF|VT_UI1      */
        short FAR *           piVal;          /* VT_BYREF|VT_I2       */
        long FAR *            plVal;          /* VT_BYREF|VT_I4       */
        float FAR *           pfltVal;        /* VT_BYREF|VT_R4       */
        double FAR *          pdblVal;        /* VT_BYREF|VT_R8       */
        VARIANT_BOOL FAR *    pboolVal;       /* VT_BYREF|VT_BOOL     */
        SCODE FAR *           pscode;         /* VT_BYREF|VT_ERROR    */
        CY FAR *              pcyVal;         /* VT_BYREF|VT_CY       */
        DATE FAR *            pdate;          /* VT_BYREF|VT_DATE     */
        BSTR FAR *            pbstrVal;       /* VT_BYREF|VT_BSTR     */
        IUnknown  FAR * FAR * ppunkVal;       /* VT_BYREF|VT_UNKNOWN  */
        IDispatch FAR * FAR * ppdispVal;      /* VT_BYREF|VT_DISPATCH */
        SAFEARRAY FAR * FAR * pparray;        /* VT_BYREF|VT_ARRAY|*  */
        VARIANT FAR *         pvarVal;        /* VT_BYREF|VT_VARIANT  */
  
        void FAR *            byref;          /* Generic ByRef        */
  
        char                  cVal;           /* VT_I1                */
        unsigned short        uiVal;          /* VT_UI2               */
        unsigned long         ulVal;          /* VT_UI4               */
        int                   intVal;         /* VT_INT               */
        unsigned int          uintVal;        /* VT_UINT              */
  
        DECIMAL FAR *         pdecVal;        /* VT_BYREF|VT_DECIMAL  */
        char FAR *            pcVal;          /* VT_BYREF|VT_I1       */
        unsigned short FAR *  puiVal;         /* VT_BYREF|VT_UI2      */
        unsigned long FAR *   pulVal;         /* VT_BYREF|VT_UI4      */
        int FAR *             pintVal;        /* VT_BYREF|VT_INT      */
        unsigned int FAR *    puintVal;       /* VT_BYREF|VT_UINT     */

#if !__STDC__ && defined(_MSC_VER) && (_MSC_VER <= 1000)
        VARIANT_BOOL          bool;           /* (obsolete)           */
        VARIANT_BOOL FAR *    pbool;          /* (obsolete)           */
#endif

      } __VARIANT_NAME_3;
    } __VARIANT_NAME_2;

    DECIMAL  decVal;
  } __VARIANT_NAME_1;
};

#if defined(NONAMELESSUNION) || (defined(_MAC) && !defined(__cplusplus) && !defined(_MSC_VER))
#define V_UNION(X, Y)    ((X)->n1.n2.n3.Y)
#define V_VT(X)          ((X)->n1.n2.vt)
#else
#define V_UNION(X, Y)    ((X)->Y)
#define V_VT(X)          ((X)->vt)
#endif

//
// UNDONE: Remove when we get a new winerror.h
// MessageId: DISP_E_DIVBYZERO
//
// MessageText:
//
//  Division by zero.
//
#define DISP_E_DIVBYZERO                 0x80020012L

/* Variant access macros
 */
#define V_ISBYREF(X)     (V_VT(X)&VT_BYREF)
#define V_ISARRAY(X)     (V_VT(X)&VT_ARRAY)
#define V_ISVECTOR(X)    (V_VT(X)&VT_VECTOR)
#define V_NONE(X)        V_I2(X)

#define V_UI1(X)         V_UNION(X, bVal)
#define V_UI1REF(X)      V_UNION(X, pbVal)
#define V_I2(X)          V_UNION(X, iVal)
#define V_I2REF(X)       V_UNION(X, piVal)
#define V_I4(X)          V_UNION(X, lVal)
#define V_I4REF(X)       V_UNION(X, plVal)
#define V_R4(X)          V_UNION(X, fltVal)
#define V_R4REF(X)       V_UNION(X, pfltVal)
#define V_R8(X)          V_UNION(X, dblVal)
#define V_R8REF(X)       V_UNION(X, pdblVal)
#define V_I1(X)          V_UNION(X, cVal)
#define V_I1REF(X)       V_UNION(X, pcVal)
#define V_UI2(X)         V_UNION(X, uiVal)
#define V_UI2REF(X)      V_UNION(X, puiVal)
#define V_UI4(X)         V_UNION(X, ulVal)
#define V_UI4REF(X)      V_UNION(X, pulVal)
#define V_INT(X)         V_UNION(X, intVal)
#define V_INTREF(X)      V_UNION(X, pintVal)
#define V_UINT(X)        V_UNION(X, uintVal)
#define V_UINTREF(X)     V_UNION(X, puintVal)
#define V_CY(X)          V_UNION(X, cyVal)
#define V_CYREF(X)       V_UNION(X, pcyVal)
#define V_DATE(X)        V_UNION(X, date)
#define V_DATEREF(X)     V_UNION(X, pdate)
#define V_BSTR(X)        V_UNION(X, bstrVal)
#define V_BSTRREF(X)     V_UNION(X, pbstrVal)
#define V_DISPATCH(X)    V_UNION(X, pdispVal)
#define V_DISPATCHREF(X) V_UNION(X, ppdispVal)
#define V_ERROR(X)       V_UNION(X, scode)
#define V_ERRORREF(X)    V_UNION(X, pscode)
#define V_BOOL(X)        V_UNION(X, boolVal)
#define V_BOOLREF(X)     V_UNION(X, pboolVal)
#define V_UNKNOWN(X)     V_UNION(X, punkVal)
#define V_UNKNOWNREF(X)  V_UNION(X, ppunkVal)
#define V_VARIANTREF(X)  V_UNION(X, pvarVal)
#define V_ARRAY(X)       V_UNION(X, parray)
#define V_ARRAYREF(X)    V_UNION(X, pparray)
#define V_BYREF(X)       V_UNION(X, byref)

#define V_DECIMAL(X)     V_UNION(X, decVal)
#define V_DECIMALREF(X)  V_UNION(X, pdecVal)

#endif /* _VARIANT_H_ */
