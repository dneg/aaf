/*** 
*dispatch.h - OLE Automation definitions.
*
*  Copyright (C) 1992-1997, Microsoft Corporation.  All Rights Reserved.
*
*Purpose:
*  This file defines the Ole Automation interfaces and APIs.
*
*Implementation Notes:
*  This file requires ole2.h
*
*****************************************************************************/

#ifndef _DISPATCH_H_
#define _DISPATCH_H_

#include "variant.h"

#ifndef BEGIN_INTERFACE
# define BEGIN_INTERFACE
#endif

#if defined(NONAMELESSUNION) || (defined(_MAC) && !defined(__cplusplus) && !defined(_MSC_VER))
# define UNION_NAME(X) X
#else
# define UNION_NAME(X)
#endif


DEFINE_OLEGUID(IID_IDispatch,		0x00020400L, 0, 0);
DEFINE_OLEGUID(IID_IEnumVARIANT,	0x00020404L, 0, 0);
DEFINE_OLEGUID(IID_ITypeInfo,		0x00020401L, 0, 0);
DEFINE_OLEGUID(IID_ITypeInfo2,      0x00020412L, 0, 0);
DEFINE_OLEGUID(IID_ITypeLib,		0x00020402L, 0, 0);
DEFINE_OLEGUID(IID_ITypeLib2,       0x00020411L, 0, 0);
DEFINE_OLEGUID(IID_ITypeComp,		0x00020403L, 0, 0);
DEFINE_OLEGUID(IID_ICreateTypeInfo,	0x00020405L, 0, 0);
DEFINE_OLEGUID(IID_ICreateTypeInfo2,0x0002040EL, 0, 0);
DEFINE_OLEGUID(IID_ICreateTypeLib,	0x00020406L, 0, 0);
DEFINE_OLEGUID(IID_ICreateTypeLib2, 0x0002040FL, 0, 0);
DEFINE_OLEGUID(IID_ITypeChangeEvents, 0x00020410L, 0, 0);

DEFINE_OLEGUID(IID_StdOle,  0x00020430L, 0, 0);
#define STDOLE_MAJORVERNUM      0x1
#define STDOLE_MINORVERNUM      0x0
#define STDOLE_LCID             0x0000

// Version # of stdole2.tlb
#define STDOLE2_MAJORVERNUM     0x2
#define STDOLE2_MINORVERNUM     0x0
#define STDOLE2_LCID            0x0000

// {1CF2B120-547D-101B-8E65-08002B2BD119}
DEFINE_GUID(IID_IErrorInfo,
  0x1CF2B120L, 0x547D, 0x101B, 0x8E, 0x65, 0x08, 0x00, 0x2B, 0x2B, 0xD1, 0x19);

// {22F03340-547D-101B-8E65-08002B2BD119}
DEFINE_GUID(IID_ICreateErrorInfo,
  0x22F03340L, 0x547D, 0x101B, 0x8E, 0x65, 0x08, 0x00, 0x2B, 0x2B, 0xD1, 0x19);

// {DF0B3D60-548F-101B-8E65-08002B2BD119}
DEFINE_GUID(IID_ISupportErrorInfo,
  0xDF0B3D60L, 0x548F, 0x101B, 0x8E, 0x65, 0x08, 0x00, 0x2B, 0x2B, 0xD1, 0x19);

/* forward declarations */
#ifdef __cplusplus

  interface IDispatch;
  interface IEnumVARIANT;
  interface ITypeInfo;
  interface ITypeInfo2;
  interface ITypeLib;
  interface ITypeLib2;
  interface ITypeComp;
  interface ICreateTypeInfo;
  interface ICreateTypeInfo2;
  interface ICreateTypeLib;
  interface ICreateTypeLib2;
  interface IErrorInfo;
  interface ICreateErrorInfo;
  interface ISupportErrorInfo;
  interface ITypeChangeEvents;
  
#else

  typedef interface IDispatch IDispatch;
  typedef interface IEnumVARIANT IEnumVARIANT;
  typedef interface ITypeInfo ITypeInfo;
  typedef interface ITypeInfo2 ITypeInfo2;
  typedef interface ITypeLib ITypeLib;
  typedef interface ITypeLib2 ITypeLib2;
  typedef interface ITypeComp ITypeComp;
  typedef interface ICreateTypeInfo ICreateTypeInfo;
  typedef interface ICreateTypeInfo2 ICreateTypeInfo2;
  typedef interface ICreateTypeLib ICreateTypeLib;
  typedef interface ICreateTypeLib2 ICreateTypeLib2;
  typedef interface IErrorInfo IErrorInfo;
  typedef interface ICreateErrorInfo ICreateErrorInfo;
  typedef interface ISupportErrorInfo ISupportErrorInfo;
  typedef interface ITypeChangeEvents ITypeChangeEvents;
  
#endif


/* OLE Automation error codes */

#ifndef _WINERROR_

#define DISP_ERROR(X) MAKE_SCODE(SEVERITY_ERROR, FACILITY_DISPATCH, X)

#define DISP_E_UNKNOWNINTERFACE		DISP_ERROR(1)
#define DISP_E_MEMBERNOTFOUND		DISP_ERROR(3)
#define DISP_E_PARAMNOTFOUND		DISP_ERROR(4)
#define DISP_E_TYPEMISMATCH		DISP_ERROR(5)
#define DISP_E_UNKNOWNNAME		DISP_ERROR(6)
#define DISP_E_NONAMEDARGS		DISP_ERROR(7)
#define DISP_E_BADVARTYPE		DISP_ERROR(8)
#define DISP_E_EXCEPTION		DISP_ERROR(9)
#define DISP_E_OVERFLOW			DISP_ERROR(10)
#define DISP_E_BADINDEX			DISP_ERROR(11)
#define DISP_E_UNKNOWNLCID		DISP_ERROR(12)
#define DISP_E_ARRAYISLOCKED		DISP_ERROR(13)
#define DISP_E_BADPARAMCOUNT		DISP_ERROR(14)
#define DISP_E_PARAMNOTOPTIONAL		DISP_ERROR(15)
#define DISP_E_BADCALLEE		DISP_ERROR(16)
#define DISP_E_NOTACOLLECTION		DISP_ERROR(17)


#define TYPE_ERROR(X) MAKE_SCODE(SEVERITY_ERROR, FACILITY_DISPATCH, X)

#define TYPE_E_BUFFERTOOSMALL		TYPE_ERROR(32790)
#define TYPE_E_INVDATAREAD		TYPE_ERROR(32792)
#define TYPE_E_UNSUPFORMAT		TYPE_ERROR(32793)
#define TYPE_E_REGISTRYACCESS		TYPE_ERROR(32796)
#define TYPE_E_LIBNOTREGISTERED 	TYPE_ERROR(32797)
#define TYPE_E_UNDEFINEDTYPE		TYPE_ERROR(32807)
#define TYPE_E_QUALIFIEDNAMEDISALLOWED	TYPE_ERROR(32808)
#define TYPE_E_INVALIDSTATE		TYPE_ERROR(32809)
#define TYPE_E_WRONGTYPEKIND		TYPE_ERROR(32810)
#define TYPE_E_ELEMENTNOTFOUND		TYPE_ERROR(32811)
#define TYPE_E_AMBIGUOUSNAME		TYPE_ERROR(32812)
#define TYPE_E_NAMECONFLICT		TYPE_ERROR(32813)
#define TYPE_E_UNKNOWNLCID		TYPE_ERROR(32814)
#define TYPE_E_DLLFUNCTIONNOTFOUND	TYPE_ERROR(32815)
#define TYPE_E_BADMODULEKIND		TYPE_ERROR(35005)
#define TYPE_E_SIZETOOBIG		TYPE_ERROR(35013)
#define TYPE_E_DUPLICATEID		TYPE_ERROR(35014)
#define TYPE_E_INVALIDID		TYPE_ERROR(35023)
#define TYPE_E_TYPEMISMATCH		TYPE_ERROR(36000)
#define TYPE_E_OUTOFBOUNDS		TYPE_ERROR(36001)
#define TYPE_E_IOERROR			TYPE_ERROR(36002)
#define TYPE_E_CANTCREATETMPFILE	TYPE_ERROR(36003)
#define TYPE_E_CANTLOADLIBRARY		TYPE_ERROR(40010)
#define TYPE_E_INCONSISTENTPROPFUNCS	TYPE_ERROR(40067)
#define TYPE_E_CIRCULARTYPE		TYPE_ERROR(40068)

#endif //_WINERROR_

/* if not already picked up from olenls.h */
#ifndef _LCID_DEFINED
typedef unsigned long LCID;
# define _LCID_DEFINED
#endif



/*---------------------------------------------------------------------*/
/*                            BSTR API                                 */
/*---------------------------------------------------------------------*/

STDAPI_(BSTR) SysAllocString(const OLECHAR FAR*);
STDAPI_(int)  SysReAllocString(BSTR FAR*, const OLECHAR FAR*);
STDAPI_(BSTR) SysAllocStringLen(const OLECHAR FAR*, unsigned int);
STDAPI_(int)  SysReAllocStringLen(BSTR FAR*, const OLECHAR FAR*, unsigned int);
STDAPI_(void) SysFreeString(BSTR);  
STDAPI_(unsigned int) SysStringLen(BSTR);

#ifdef WIN32
STDAPI_(unsigned int) SysStringByteLen(BSTR bstr);
STDAPI_(BSTR) SysAllocStringByteLen(const char FAR* psz, unsigned int len);
#endif


/*---------------------------------------------------------------------*/
/*                            Time API                                 */
/*---------------------------------------------------------------------*/

STDAPI_(int)
DosDateTimeToVariantTime(
    unsigned short wDosDate,
    unsigned short wDosTime,
    double FAR* pvtime);

STDAPI_(int)
VariantTimeToDosDateTime(
    double vtime,
    unsigned short FAR* pwDosDate,
    unsigned short FAR* pwDosTime);

#if defined(WIN32)
STDAPI_(int)
SystemTimeToVariantTime(
    LPSYSTEMTIME    lpSystemTime,
    double *        pvtime);

STDAPI_(int)
VariantTimeToSystemTime(
    double          vtime,
    LPSYSTEMTIME    lpSystemTime);
#endif  // WIN32

/*---------------------------------------------------------------------*/
/*                          SafeArray API                              */
/*---------------------------------------------------------------------*/

STDAPI
SafeArrayAllocDescriptor(unsigned int cDims, SAFEARRAY FAR* FAR* ppsaOut);

STDAPI SafeArrayAllocData(SAFEARRAY FAR* psa);

STDAPI_(SAFEARRAY FAR*)
SafeArrayCreate(
    VARTYPE vt,
    unsigned int cDims,
    SAFEARRAYBOUND FAR* rgsabound);

STDAPI SafeArrayDestroyDescriptor(SAFEARRAY FAR* psa);

STDAPI SafeArrayDestroyData(SAFEARRAY FAR* psa);

STDAPI SafeArrayDestroy(SAFEARRAY FAR* psa);

STDAPI SafeArrayRedim(SAFEARRAY FAR* psa, SAFEARRAYBOUND FAR* psaboundNew);

STDAPI_(unsigned int) SafeArrayGetDim(SAFEARRAY FAR* psa);

STDAPI_(unsigned int) SafeArrayGetElemsize(SAFEARRAY FAR* psa);

STDAPI
SafeArrayGetUBound(SAFEARRAY FAR* psa, unsigned int nDim, long FAR* plUbound);

STDAPI
SafeArrayGetLBound(SAFEARRAY FAR* psa, unsigned int nDim, long FAR* plLbound);

STDAPI SafeArrayLock(SAFEARRAY FAR* psa);

STDAPI SafeArrayUnlock(SAFEARRAY FAR* psa);

STDAPI SafeArrayAccessData(SAFEARRAY FAR* psa, void HUGEP* FAR* ppvData);

STDAPI SafeArrayUnaccessData(SAFEARRAY FAR* psa);

STDAPI
SafeArrayGetElement(
    SAFEARRAY FAR* psa,
    long FAR* rgIndices,
    void FAR* pv);

STDAPI
SafeArrayPutElement(
    SAFEARRAY FAR* psa,
    long FAR* rgIndices,
    void FAR* pv);

STDAPI
SafeArrayCopy(
    SAFEARRAY FAR* psa,
    SAFEARRAY FAR* FAR* ppsaOut);

STDAPI
SafeArrayPtrOfIndex(
    SAFEARRAY FAR* psa,
    long FAR* rgIndices,
    void HUGEP* FAR* ppvData);

STDAPI_(SAFEARRAY *)
SafeArrayCreateVector(
    VARTYPE vt,
    long lLbound,
    unsigned long cElements);

STDAPI
SafeArrayCopyData(
    SAFEARRAY *psaSource,
    SAFEARRAY *psaTarget);

STDAPI VectorFromBstr (BSTR bstr, SAFEARRAY ** ppsa);

STDAPI BstrFromVector (SAFEARRAY *psa, BSTR *pbstr);


/*---------------------------------------------------------------------*/
/*                           VARIANT API                               */
/*---------------------------------------------------------------------*/

STDAPI_(void)
VariantInit(VARIANTARG FAR* pvarg);

STDAPI
VariantClear(VARIANTARG FAR* pvarg);

STDAPI
VariantCopy(
    VARIANTARG FAR* pvargDest,
    VARIANTARG FAR* pvargSrc);

STDAPI
VariantCopyInd(
    VARIANT FAR* pvarDest,
    VARIANTARG FAR* pvargSrc);

STDAPI
VariantChangeType(
    VARIANTARG FAR* pvargDest,
    VARIANTARG FAR* pvarSrc,
    unsigned short wFlags,
    VARTYPE vt);

STDAPI
VariantChangeTypeEx(
    VARIANTARG FAR* pvargDest,
    VARIANTARG FAR* pvarSrc,
    LCID lcid,	    
    unsigned short wFlags,
    VARTYPE vt);

// Flags for VariantChangeType/VariantChangeTypeEx
#define VARIANT_NOVALUEPROP 1
#define VARIANT_ALPHABOOL 0x2       // For VT_BOOL to VT_BSTR conversions,
                                    // convert to "True"/"False" instead of
                                    // "-1"/"0"
#define VARIANT_NOUSEROVERRIDE   0x4	// For conversions to/from VT_BSTR,
					// passes LOCALE_NOUSEROVERRIDE
					// to core coercion routines


/*---------------------------------------------------------------------*/
/*                     VARTYPE Coercion API                            */
/*---------------------------------------------------------------------*/

/* Note: The routines that convert *from* a string are defined
 * to take a OLECHAR* rather than a BSTR because no allocation is
 * required, and this makes the routines a bit more generic.
 * They may of course still be passed a BSTR as the strIn param.
 */


/* Any of the coersion functions that converts either from or to a string
 * takes an additional lcid and dwFlags arguments. The lcid argument allows
 * locale specific parsing to occur.  The dwFlags allow additional function
 * specific condition to occur.  All function that accept the dwFlags argument
 * can include either 0 or LOCALE_NOUSEROVERRIDE flag. In addition, the
 * VarDateFromStr functions also accepts the VAR_TIMEVALUEONLY and 
 * VAR_DATEVALUEONLY flags
 */	 

#define VAR_TIMEVALUEONLY   ((DWORD)0x00000001)    /* return time value */
#define VAR_DATEVALUEONLY   ((DWORD)0x00000002)    /* return date value */

   
STDAPI VarUI1FromI2(short sIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromI4(long lIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromR4(float fltIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromR8(double dblIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromCy(CY cyIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromDate(DATE dateIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, unsigned char FAR* pbOut);
STDAPI VarUI1FromDisp(IDispatch FAR* pdispIn, LCID lcid, unsigned char FAR* pbOut);
STDAPI VarUI1FromBool(VARIANT_BOOL boolIn, unsigned char FAR* pbOut);
STDAPI VarUI1FromI1(char cIn, unsigned char *pbOut);
STDAPI VarUI1FromUI2(unsigned short uiIn, unsigned char *pbOut);
STDAPI VarUI1FromUI4(unsigned long ulIn, unsigned char *pbOut);
STDAPI VarUI1FromDec(DECIMAL *pdecIn, unsigned char *pbOut);

STDAPI VarI2FromUI1(unsigned char bIn, short FAR* psOut);
STDAPI VarI2FromI4(long lIn, short FAR* psOut);
STDAPI VarI2FromR4(float fltIn, short FAR* psOut);
STDAPI VarI2FromR8(double dblIn, short FAR* psOut);
STDAPI VarI2FromCy(CY cyIn, short FAR* psOut);
STDAPI VarI2FromDate(DATE dateIn, short FAR* psOut);
STDAPI VarI2FromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, short FAR* psOut);
STDAPI VarI2FromDisp(IDispatch FAR* pdispIn, LCID lcid, short FAR* psOut);
STDAPI VarI2FromBool(VARIANT_BOOL boolIn, short FAR* psOut);
STDAPI VarI2FromI1(char cIn, short *psOut);
STDAPI VarI2FromUI2(unsigned short uiIn, short *psOut);
STDAPI VarI2FromUI4(unsigned long ulIn, short *psOut);
STDAPI VarI2FromDec(DECIMAL *pdecIn, short *psOut);

STDAPI VarI4FromUI1(unsigned char bIn, long FAR* plOut);
STDAPI VarI4FromI2(short sIn, long FAR* plOut);
STDAPI VarI4FromR4(float fltIn, long FAR* plOut);
STDAPI VarI4FromR8(double dblIn, long FAR* plOut);
STDAPI VarI4FromCy(CY cyIn, long FAR* plOut);
STDAPI VarI4FromDate(DATE dateIn, long FAR* plOut);
STDAPI VarI4FromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, long FAR* plOut);
STDAPI VarI4FromDisp(IDispatch FAR* pdispIn, LCID lcid, long FAR* plOut);
STDAPI VarI4FromBool(VARIANT_BOOL boolIn, long FAR* plOut);
STDAPI VarI4FromI1(char cIn, long *plOut);
STDAPI VarI4FromUI2(unsigned short uiIn, long *plOut);
STDAPI VarI4FromUI4(unsigned long ulIn, long *plOut);
STDAPI VarI4FromDec(DECIMAL *pdecIn, long *plOut);
STDAPI VarI4FromInt(int intIn, long *plOut);

STDAPI VarR4FromUI1(unsigned char bIn, float FAR* pfltOut);
STDAPI VarR4FromI2(short sIn, float FAR* pfltOut);
STDAPI VarR4FromI4(long lIn, float FAR* pfltOut);
STDAPI VarR4FromR8(double dblIn, float FAR* pfltOut);
STDAPI VarR4FromCy(CY cyIn, float FAR* pfltOut);
STDAPI VarR4FromDate(DATE dateIn, float FAR* pfltOut);
STDAPI VarR4FromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, float FAR* pfltOut);
STDAPI VarR4FromDisp(IDispatch FAR* pdispIn, LCID lcid, float FAR* pfltOut);
STDAPI VarR4FromBool(VARIANT_BOOL boolIn, float FAR* pfltOut);
STDAPI VarR4FromI1(char cIn, float *pfltOut);
STDAPI VarR4FromUI2(unsigned short uiIn, float *pfltOut);
STDAPI VarR4FromUI4(unsigned long ulIn, float *pfltOut);
STDAPI VarR4FromDec(DECIMAL *pdecIn, float *pfltOut);

STDAPI VarR8FromUI1(unsigned char bIn, double FAR* pdblOut);
STDAPI VarR8FromI2(short sIn, double FAR* pdblOut);
STDAPI VarR8FromI4(long lIn, double FAR* pdblOut);
STDAPI VarR8FromR4(float fltIn, double FAR* pdblOut);
STDAPI VarR8FromCy(CY cyIn, double FAR* pdblOut);
STDAPI VarR8FromDate(DATE dateIn, double FAR* pdblOut);
STDAPI VarR8FromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, double FAR* pdblOut);
STDAPI VarR8FromDisp(IDispatch FAR* pdispIn, LCID lcid, double FAR* pdblOut);
STDAPI VarR8FromBool(VARIANT_BOOL boolIn, double FAR* pdblOut);
STDAPI VarR8FromI1(char cIn, double *pdblOut);
STDAPI VarR8FromUI2(unsigned short uiIn, double *pdblOut);
STDAPI VarR8FromUI4(unsigned long ulIn, double *pdblOut);
STDAPI VarR8FromDec(DECIMAL *pdecIn, double *pdblOut);

STDAPI VarDateFromUI1(unsigned char bIn, DATE FAR* pdateOut);
STDAPI VarDateFromI2(short sIn, DATE FAR* pdateOut);
STDAPI VarDateFromI4(long lIn, DATE FAR* pdateOut);
STDAPI VarDateFromR4(float fltIn, DATE FAR* pdateOut);
STDAPI VarDateFromR8(double dblIn, DATE FAR* pdateOut);
STDAPI VarDateFromCy(CY cyIn, DATE FAR* pdateOut);
STDAPI VarDateFromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, DATE FAR* pdateOut);
STDAPI VarDateFromDisp(IDispatch FAR* pdispIn, LCID lcid, DATE FAR* pdateOut);
STDAPI VarDateFromBool(VARIANT_BOOL boolIn, DATE FAR* pdateOut);
STDAPI VarDateFromI1(char cIn, DATE *pdateOut);
STDAPI VarDateFromUI2(unsigned short uiIn, DATE *pdateOut);
STDAPI VarDateFromUI4(unsigned long ulIn, DATE *pdateOut);
STDAPI VarDateFromDec(DECIMAL *pdecIn, DATE *pdateOut);

STDAPI VarCyFromUI1(unsigned char bIn, CY FAR* pcyOut);
STDAPI VarCyFromI2(short sIn, CY FAR* pcyOut);
STDAPI VarCyFromI4(long lIn, CY FAR* pcyOut);
STDAPI VarCyFromR4(float fltIn, CY FAR* pcyOut);
STDAPI VarCyFromR8(double dblIn, CY FAR* pcyOut);
STDAPI VarCyFromDate(DATE dateIn, CY FAR* pcyOut);
STDAPI VarCyFromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, CY FAR* pcyOut);
STDAPI VarCyFromDisp(IDispatch FAR* pdispIn, LCID lcid, CY FAR* pcyOut);
STDAPI VarCyFromBool(VARIANT_BOOL boolIn, CY FAR* pcyOut);
STDAPI VarCyFromI1(char cIn, CY *pcyOut);
STDAPI VarCyFromUI2(unsigned short uiIn, CY *pcyOut);
STDAPI VarCyFromUI4(unsigned long ulIn, CY *pcyOut);
STDAPI VarCyFromDec(DECIMAL *pdecIn, CY *pcyOut);

STDAPI VarBstrFromUI1(unsigned char bVal, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromI2(short iVal, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromI4(long lIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromR4(float fltIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromR8(double dblIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromCy(CY cyIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromDate(DATE dateIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromDisp(IDispatch FAR* pdispIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromBool(VARIANT_BOOL boolIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut);
STDAPI VarBstrFromI1(char cIn, LCID lcid, unsigned long dwFlags, BSTR *pbstrOut);
STDAPI VarBstrFromUI2(unsigned short uiIn, LCID lcid, unsigned long dwFlags, BSTR *pbstrOut);
STDAPI VarBstrFromUI4(unsigned long ulIn, LCID lcid, unsigned long dwFlags, BSTR *pbstrOut);
STDAPI VarBstrFromDec(DECIMAL *pdecIn, LCID lcid, unsigned long dwFlags, BSTR *pbstrOut);

STDAPI VarBoolFromUI1(unsigned char bIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromI2(short sIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromI4(long lIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromR4(float fltIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromR8(double dblIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromDate(DATE dateIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromCy(CY cyIn, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromDisp(IDispatch FAR* pdispIn, LCID lcid, VARIANT_BOOL FAR* pboolOut);
STDAPI VarBoolFromI1(char cIn, VARIANT_BOOL *pboolOut);
STDAPI VarBoolFromUI2(unsigned short uiIn, VARIANT_BOOL *pboolOut);
STDAPI VarBoolFromUI4(unsigned long ulIn, VARIANT_BOOL *pboolOut);
STDAPI VarBoolFromDec(DECIMAL *pdecIn, VARIANT_BOOL *pboolOut);

STDAPI VarI1FromUI1(unsigned char bIn, char *pcOut);
STDAPI VarI1FromI2(short uiIn, char *pcOut);
STDAPI VarI1FromI4(long lIn, char *pcOut);
STDAPI VarI1FromR4(float fltIn, char *pcOut);
STDAPI VarI1FromR8(double dblIn, char *pcOut);
STDAPI VarI1FromDate(DATE dateIn, char *pcOut);
STDAPI VarI1FromCy(CY cyIn, char *pcOut);
STDAPI VarI1FromStr(OLECHAR *strIn, LCID lcid, unsigned long dwFlags, char *pcOut);
STDAPI VarI1FromDisp(IDispatch *pdispIn, LCID lcid, char *pcOut);
STDAPI VarI1FromBool(VARIANT_BOOL boolIn, char *pcOut);
STDAPI VarI1FromUI2(unsigned short uiIn, char *pcOut);
STDAPI VarI1FromUI4(unsigned long ulIn, char *pcOut);
STDAPI VarI1FromDec(DECIMAL *pdecIn, char *pcOut);

STDAPI VarUI2FromUI1(unsigned char bIn, unsigned short *puiOut);
STDAPI VarUI2FromI2(short uiIn, unsigned short *puiOut);
STDAPI VarUI2FromI4(long lIn, unsigned short *puiOut);
STDAPI VarUI2FromR4(float fltIn, unsigned short *puiOut);
STDAPI VarUI2FromR8(double dblIn, unsigned short *puiOut);
STDAPI VarUI2FromDate(DATE dateIn, unsigned short *puiOut);
STDAPI VarUI2FromCy(CY cyIn, unsigned short *puiOut);
STDAPI VarUI2FromStr(OLECHAR *strIn, LCID lcid, unsigned long dwFlags, unsigned short *puiOut);
STDAPI VarUI2FromDisp(IDispatch *pdispIn, LCID lcid, unsigned short *puiOut);
STDAPI VarUI2FromBool(VARIANT_BOOL boolIn, unsigned short *puiOut);
STDAPI VarUI2FromI1(char cIn, unsigned short *puiOut);
STDAPI VarUI2FromUI4(unsigned long ulIn, unsigned short *puiOut);
STDAPI VarUI2FromDec(DECIMAL *pdecIn, unsigned short *puiOut);

STDAPI VarUI4FromUI1(unsigned char bIn, unsigned long *pulOut);
STDAPI VarUI4FromI2(short uiIn, unsigned long *pulOut);
STDAPI VarUI4FromI4(long lIn, unsigned long *pulOut);
STDAPI VarUI4FromR4(float fltIn, unsigned long *pulOut);
STDAPI VarUI4FromR8(double dblIn, unsigned long *pulOut);
STDAPI VarUI4FromDate(DATE dateIn, unsigned long *pulOut);
STDAPI VarUI4FromCy(CY cyIn, unsigned long *pulOut);
STDAPI VarUI4FromStr(OLECHAR *strIn, LCID lcid, unsigned long dwFlags, unsigned long *pulOut);
STDAPI VarUI4FromDisp(IDispatch *pdispIn, LCID lcid, unsigned long *pulOut);
STDAPI VarUI4FromBool(VARIANT_BOOL boolIn, unsigned long *pulOut);
STDAPI VarUI4FromI1(char cIn, unsigned long *pulOut);
STDAPI VarUI4FromUI2(unsigned short uiIn, unsigned long *pulOut);
STDAPI VarUI4FromDec(DECIMAL *pdecIn, unsigned long *pulOut);

STDAPI VarDecFromUI1(unsigned char bIn, DECIMAL *pdecOut);
STDAPI VarDecFromI2(short uiIn, DECIMAL *pdecOut);
STDAPI VarDecFromI4(long lIn, DECIMAL *pdecOut);
STDAPI VarDecFromR4(float fltIn, DECIMAL *pdecOut);
STDAPI VarDecFromR8(double dblIn, DECIMAL *pdecOut);
STDAPI VarDecFromDate(DATE dateIn, DECIMAL *pdecOut);
STDAPI VarDecFromCy(CY cyIn, DECIMAL *pdecOut);
STDAPI VarDecFromStr(OLECHAR *strIn, LCID lcid, unsigned long dwFlags, DECIMAL *pdecOut);
STDAPI VarDecFromDisp(IDispatch *pdispIn, LCID lcid, DECIMAL *pdecOut);
STDAPI VarDecFromBool(VARIANT_BOOL boolIn, DECIMAL *pdecOut);
STDAPI VarDecFromI1(char cIn, DECIMAL *pdecOut);
STDAPI VarDecFromUI2(unsigned short uiIn, DECIMAL *pdecOut);
STDAPI VarDecFromUI4(unsigned long ulIn, DECIMAL *pdecOut);

#define VarUI4FromUI4(in, pOut) (*(pOut) = (in))
#define VarI4FromI4(in, pOut)   (*(pOut) = (in))

#define VarUI1FromInt       VarUI1FromI4
#define VarUI1FromUint      VarUI1FromUI4
#define VarI2FromInt        VarI2FromI4
#define VarI2FromUint       VarI2FromUI4
#define VarI4FromInt        VarI4FromI4
#define VarI4FromUint       VarI4FromUI4
#define VarR4FromInt        VarR4FromI4
#define VarR4FromUint       VarR4FromUI4
#define VarR8FromInt        VarR8FromI4
#define VarR8FromUint       VarR8FromUI4
#define VarDateFromInt      VarDateFromI4
#define VarDateFromUint     VarDateFromUI4
#define VarCyFromInt        VarCyFromI4
#define VarCyFromUint       VarCyFromUI4
#define VarBstrFromInt      VarBstrFromI4
#define VarBstrFromUint     VarBstrFromUI4
#define VarBoolFromInt      VarBoolFromI4
#define VarBoolFromUint     VarBoolFromUI4
#define VarI1FromInt        VarI1FromI4
#define VarI1FromUint       VarI1FromUI4
#define VarUI2FromInt       VarUI2FromI4
#define VarUI2FromUint      VarUI2FromUI4
#define VarUI4FromInt       VarUI4FromI4
#define VarUI4FromUint      VarUI4FromUI4
#define VarDecFromInt       VarDecFromI4
#define VarDecFromUint      VarDecFromUI4
#define VarIntFromUI1       VarI4FromUI1
#define VarIntFromI2        VarI4FromI2
#define VarIntFromI4        VarI4FromI4
#define VarIntFromR4        VarI4FromR4
#define VarIntFromR8        VarI4FromR8
#define VarIntFromDate      VarI4FromDate
#define VarIntFromCy        VarI4FromCy
#define VarIntFromStr       VarI4FromStr
#define VarIntFromDisp      VarI4FromDisp
#define VarIntFromBool      VarI4FromBool
#define VarIntFromI1        VarI4FromI1
#define VarIntFromUI2       VarI4FromUI2
#define VarIntFromUI4       VarI4FromUI4
#define VarIntFromDec       VarI4FromDec
#define VarIntFromUint      VarI4FromUI4
#define VarUintFromUI1      VarUI4FromUI1
#define VarUintFromI2       VarUI4FromI2
#define VarUintFromI4       VarUI4FromI4
#define VarUintFromR4       VarUI4FromR4
#define VarUintFromR8       VarUI4FromR8
#define VarUintFromDate     VarUI4FromDate
#define VarUintFromCy       VarUI4FromCy
#define VarUintFromStr      VarUI4FromStr
#define VarUintFromDisp     VarUI4FromDisp
#define VarUintFromBool     VarUI4FromBool
#define VarUintFromI1       VarUI4FromI1
#define VarUintFromUI2      VarUI4FromUI2
#define VarUintFromUI4      VarUI4FromUI4
#define VarUintFromDec      VarUI4FromDec
#define VarUintFromInt      VarUI4FromI4

/* Mac Note: On the Mac, the coersion functions support the 
 * Symantec C++ calling convention for float/double. To support
 * float/double arguments compiled with the MPW C compiler, 
 * use the following APIs to move MPW float/double values into
 * a VARIANT.
 */

#ifdef _MAC
STDAPI MPWVarFromR4(float FAR* pfltIn, VARIANT FAR* pvarOut);
STDAPI MPWVarFromR8(double FAR* pdblIn, VARIANT FAR* pvarOut);
STDAPI MPWR4FromVar(VARIANT FAR* pvarIn, float FAR* pfltOut);
STDAPI MPWR8FromVar(VARIANT FAR* pvarIn, double FAR* pdblOut);
#endif

/*---------------------------------------------------------------------*/
/*            New VARIANT <-> string parsing functions
/*---------------------------------------------------------------------*/

typedef struct {
    int            cDig;
    unsigned long  dwInFlags;
    unsigned long  dwOutFlags;
    int            cchUsed;
    int            nBaseShift;
    int            nPwr10;
} NUMPARSE;

// flags used by both dwInFlags and dwOutFlags:
//
#define NUMPRS_LEADING_WHITE    0x0001
#define NUMPRS_TRAILING_WHITE   0x0002
#define NUMPRS_LEADING_PLUS     0x0004
#define NUMPRS_TRAILING_PLUS    0x0008
#define NUMPRS_LEADING_MINUS    0x0010
#define NUMPRS_TRAILING_MINUS   0x0020
#define NUMPRS_HEX_OCT          0x0040
#define NUMPRS_PARENS           0x0080
#define NUMPRS_DECIMAL          0x0100
#define NUMPRS_THOUSANDS        0x0200
#define NUMPRS_CURRENCY         0x0400
#define NUMPRS_EXPONENT         0x0800
#define NUMPRS_USE_ALL          0x1000
#define NUMPRS_STD              0x1FFF

// flags used by dwOutFlags only:
//
#define NUMPRS_NEG              0x10000
#define NUMPRS_INEXACT          0x20000

// flags used by VarNumFromParseNum to indicate acceptable result types:
//
#define VTBIT_I1		(1 << VT_I1)
#define VTBIT_UI1		(1 << VT_UI1)
#define VTBIT_I2		(1 << VT_I2)
#define VTBIT_UI2		(1 << VT_UI2)
#define VTBIT_I4		(1 << VT_I4)
#define VTBIT_UI4		(1 << VT_UI4)
#define VTBIT_R4		(1 << VT_R4)
#define VTBIT_R8		(1 << VT_R8)
#define VTBIT_CY		(1 << VT_CY)
#define VTBIT_DECIMAL		(1 << VT_DECIMAL)


STDAPI VarParseNumFromStr(OLECHAR *        strIn,
                          LCID             lcid,
                          unsigned long    dwFlags,
                          NUMPARSE *       pnumprs,
                          unsigned char *  rgbDig);

STDAPI VarNumFromParseNum(NUMPARSE *       pnumprs,
                          unsigned char *  rgbDig,
                          unsigned long    dwVtBits,
                          VARIANT *        pvar);


/*---------------------------------------------------------------------*/
/*                             ITypeLib                                */
/*---------------------------------------------------------------------*/

typedef long DISPID;
typedef DISPID MEMBERID;

#define MEMBERID_NIL DISPID_UNKNOWN
#define ID_DEFAULTINST  -2

typedef enum tagSYSKIND {
      SYS_WIN16
    , SYS_WIN32
    , SYS_MAC
#ifdef _MAC
    , SYS_FORCELONG = 2147483647
#endif
} SYSKIND;

typedef enum tagLIBFLAGS {
      LIBFLAG_FRESTRICTED = 0x01
      , LIBFLAG_FCONTROL = 0x02
      , LIBFLAG_FHIDDEN = 0x04
      , LIBFLAG_FHASDISKIMAGE = 0x08
#ifdef _MAC
    , LIBFLAG_FORCELONG  = 2147483647
#endif
} LIBFLAGS;

typedef struct FARSTRUCT tagTLIBATTR {
    GUID guid;			/* globally unique library id */
    LCID lcid;			/* locale of the TypeLibrary */
    SYSKIND syskind;
    unsigned short wMajorVerNum;/* major version number	*/
    unsigned short wMinorVerNum;/* minor version number	*/
    unsigned short wLibFlags;	/* library flags */
} TLIBATTR, FAR* LPTLIBATTR;

typedef enum tagTYPEKIND {
      TKIND_ENUM = 0
    , TKIND_RECORD
    , TKIND_MODULE
    , TKIND_INTERFACE
    , TKIND_DISPATCH
    , TKIND_COCLASS
    , TKIND_ALIAS
    , TKIND_UNION
    , TKIND_MAX			/* end of enum marker */
#ifdef _MAC
    , TKIND_FORCELONG = 2147483647
#endif
} TYPEKIND;

typedef struct tagCUSTDATAITEM {
    GUID guid;           /* guid identifying this custom data item */
    VARIANTARG varValue; /* value of this custom data item */
} CUSTDATAITEM, * LPCUSTDATAITEM;

typedef struct tagCUSTDATA {
    unsigned long cCustData;   	/* number of custom data items in rgCustData */
    LPCUSTDATAITEM prgCustData;
				/* array of custom data items */
} CUSTDATA, * LPCUSTDATA;

#undef  INTERFACE
#define INTERFACE ITypeLib

DECLARE_INTERFACE_(ITypeLib, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ITypeLib methods */
    STDMETHOD_(unsigned int,GetTypeInfoCount)(THIS) PURE;

    STDMETHOD(GetTypeInfo)(THIS_
      unsigned int index, ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetTypeInfoType)(THIS_
      unsigned int index, TYPEKIND FAR* ptypekind) PURE;

    STDMETHOD(GetTypeInfoOfGuid)(THIS_
      REFGUID guid, ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetLibAttr)(THIS_
      TLIBATTR FAR* FAR* pptlibattr) PURE;

    STDMETHOD(GetTypeComp)(THIS_
      ITypeComp FAR* FAR* pptcomp) PURE;

    STDMETHOD(GetDocumentation)(THIS_
      int index,
      BSTR FAR* pbstrName,
      BSTR FAR* pbstrDocString,
      unsigned long FAR* pdwHelpContext,
      BSTR FAR* pbstrHelpFile) PURE;

    STDMETHOD(IsName)(THIS_ 
      OLECHAR FAR* szNameBuf,
      unsigned long lHashVal,
      int FAR* lpfName) PURE;

    STDMETHOD(FindName)(THIS_
      OLECHAR FAR* szNameBuf,
      unsigned long lHashVal,
      ITypeInfo FAR* FAR* rgptinfo,
      MEMBERID FAR* rgmemid,
      unsigned short FAR* pcFound) PURE;

    STDMETHOD_(void, ReleaseTLibAttr)(THIS_ TLIBATTR FAR* ptlibattr) PURE;
};

typedef ITypeLib FAR* LPTYPELIB;


/*---------------------------------------------------------------------*/
/*                   ITypeLib2 interface                               */
/*---------------------------------------------------------------------*/

#undef  INTERFACE
#define INTERFACE ITypeLib2

DECLARE_INTERFACE_(ITypeLib2, ITypeLib)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ITypeLib methods */
    STDMETHOD_(unsigned int, GetTypeInfoCount)(THIS) PURE;

    STDMETHOD(GetTypeInfo)(THIS_
                           unsigned int index, 
                           ITypeInfo ** pptinfo) PURE;

    STDMETHOD(GetTypeInfoType)(THIS_
                               unsigned int index, 
                               TYPEKIND *   ptypekind) PURE;

    STDMETHOD(GetTypeInfoOfGuid)(THIS_
                                 REFGUID        guid, 
                                 ITypeInfo **   pptinfo) PURE;

    STDMETHOD(GetLibAttr)(THIS_
                          TLIBATTR **pptlibattr) PURE;

    STDMETHOD(GetTypeComp)(THIS_
                           ITypeComp ** pptcomp) PURE;

    STDMETHOD(GetDocumentation)(THIS_
                                int             index,
                                BSTR *          pbstrName,
                                BSTR *          pbstrDocString,
                                unsigned long * pdwHelpContext,
                                BSTR *          pbstrHelpFile) PURE;

    STDMETHOD(IsName)(THIS_ 
                      OLECHAR *     szNameBuf,
                      unsigned long lHashVal,
                      int *         lpfName) PURE;

    STDMETHOD(FindName)(THIS_
                        OLECHAR *           szNameBuf,
                        unsigned long       lHashVal,
                        ITypeInfo **        rgptinfo,
                        MEMBERID *          rgmemid,
                        unsigned short *    pcFound) PURE;

    STDMETHOD_(void, ReleaseTLibAttr)(THIS_ TLIBATTR *ptlibattr) PURE;

    /* ITypeLib2 methods */
    STDMETHOD(GetCustData)(THIS_
			   REFGUID guid,
			   VARIANT *pVarVal) PURE;

    STDMETHOD(GetLibStatistics)(THIS_ 
                                unsigned long * pcUniqueNames, 
                                unsigned long * pcchUniqueNames) PURE;

    STDMETHOD(GetDocumentation2)(THIS_
                                 int   index,
				 LCID  lcid,
				 BSTR  *pbstrHelpString,
				 unsigned long *pdwHelpStringContext,
				 BSTR  *pbstrHelpStringDll) PURE;

    STDMETHOD(GetAllCustData)(THIS_
			   LPCUSTDATA pCustData) PURE;

};

typedef ITypeLib2 *LPTYPELIB2;


/*---------------------------------------------------------------------*/
/*                            ITypeInfo                                */
/*---------------------------------------------------------------------*/

typedef unsigned long HREFTYPE;

#if defined(_MAC)
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2) // These structures require this packing, which was the
			// compiler default in OLE 2.02, but not anymore.	
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif
#endif

typedef struct FARSTRUCT tagTYPEDESC {
    union {
      /* VT_PTR - the pointed-at type */
      struct FARSTRUCT tagTYPEDESC FAR* lptdesc;

      /* VT_CARRAY */
      struct FARSTRUCT tagARRAYDESC FAR* lpadesc;

      /* VT_USERDEFINED - this is used to get a TypeInfo for the UDT */
      HREFTYPE hreftype;

    }UNION_NAME(u);
    VARTYPE vt;
} TYPEDESC;

typedef struct FARSTRUCT tagARRAYDESC {
    TYPEDESC tdescElem;		/* element type */
    unsigned short cDims;	/* dimension count */
    SAFEARRAYBOUND rgbounds[1];	/* variable length array of bounds */
} ARRAYDESC;

// parameter description
typedef struct FARSTRUCT tagPARAMDESCEX {
    unsigned long cBytes;		// size of this structure
    VARIANTARG varDefaultValue;		// default value of this parameter
} PARAMDESCEX, FAR* LPPARAMDESCEX;

typedef struct FARSTRUCT tagPARAMDESC {
    LPPARAMDESCEX  pparamdescex;	// valid if PARAMFLAG_FHASDEFAULT
					// bit is set
    unsigned short wParamFlags;		// IN, OUT, etc
} PARAMDESC, FAR* LPPARAMDESC;

#define PARAMFLAG_NONE		0
#define PARAMFLAG_FIN		0x1
#define PARAMFLAG_FOUT		0x2
#define PARAMFLAG_FLCID		0x4
#define PARAMFLAG_FRETVAL	0x8
#define PARAMFLAG_FOPT		0x10
#define PARAMFLAG_FHASDEFAULT	0x20
#define PARAMFLAG_FHASCUSTDATA	0x40

// retain old IDLDESC for compatibility
typedef struct FARSTRUCT tagIDLDESC {
#if defined(WIN32) || defined(_MAC)
    unsigned long dwReserved;
#else
    BSTR bstrIDLInfo;		/* reserved, but original name retained for
				   compatibilty */
#endif
    unsigned short wIDLFlags;	/* IN, OUT, etc */
} IDLDESC, FAR* LPIDLDESC;

#define IDLFLAG_NONE	PARAMFLAG_NONE
#define IDLFLAG_FIN	PARAMFLAG_FIN
#define IDLFLAG_FOUT	PARAMFLAG_FOUT
#define IDLFLAG_FLCID	PARAMFLAG_FLCID
#define IDLFLAG_FRETVAL	PARAMFLAG_FRETVAL


typedef struct FARSTRUCT tagELEMDESC {
    TYPEDESC tdesc;		/* the type of the element */
    union {
      IDLDESC idldesc;		/* info for remoting the element */ 
      PARAMDESC paramdesc;	/* info about the parameter */ 
    }UNION_NAME(u);
} ELEMDESC, FAR* LPELEMDESC;

#if defined(_MAC)
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)		// restore default packing
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif
#endif


typedef struct FARSTRUCT tagTYPEATTR {
    GUID guid;			/* the GUID of the TypeInfo */
    LCID lcid;			/* locale of member names and doc strings */
    unsigned long dwReserved;
    MEMBERID memidConstructor;	/* ID of constructor, MEMBERID_NIL if none */
    MEMBERID memidDestructor;	/* ID of destructor, MEMBERID_NIL if none */
    OLECHAR FAR* lpstrSchema;	/* reserved for future use */
    unsigned long cbSizeInstance;/* the size of an instance of this type */
    TYPEKIND typekind;		/* the kind of type this typeinfo describes */
    unsigned short cFuncs;	/* number of functions */
    unsigned short cVars;	/* number of variables / data members */
    unsigned short cImplTypes;	/* number of implemented interfaces */
    unsigned short cbSizeVft;	/* the size of this types virtual func table */
    unsigned short cbAlignment;	/* the alignment for an instance of this type */
    unsigned short wTypeFlags;
    unsigned short wMajorVerNum;/* major version number */
    unsigned short wMinorVerNum;/* minor version number */
    TYPEDESC tdescAlias;	/* if typekind == TKIND_ALIAS this specifies
				   the type for which this type is an alias */
    IDLDESC idldescType;        /* IDL attributes of the described type */
} TYPEATTR, FAR* LPTYPEATTR;

typedef struct FARSTRUCT tagDISPPARAMS{
    VARIANTARG FAR* rgvarg;
    DISPID FAR* rgdispidNamedArgs;
    unsigned int cArgs;
    unsigned int cNamedArgs;
} DISPPARAMS;

typedef struct FARSTRUCT tagEXCEPINFO {
    unsigned short wCode;             /* An error code describing the error. */
				      /* Either (but not both) the wCode or */
				      /* scode fields must be set */
    unsigned short wReserved;

    BSTR bstrSource;	    /* A textual, human readable name of the
			       source of the exception. It is up to the
			       IDispatch implementor to fill this in.
			       Typically this will be an application name. */

    BSTR bstrDescription;   /* A textual, human readable description of the
			       error. If no description is available, NULL
			       should be used. */

    BSTR bstrHelpFile;      /* Fully qualified drive, path, and file name
			       of a help file with more information about
			       the error.  If no help is available, NULL
			       should be used. */

    unsigned long dwHelpContext;
			    /* help context of topic within the help file. */

    void FAR* pvReserved;

    /* Use of this field allows an application to defer filling in
       the bstrDescription, bstrHelpFile, and dwHelpContext fields
       until they are needed.  This field might be used, for example,
       if loading the string for the error is a time-consuming
       operation. If deferred fill-in is not desired, this field should
       be set to NULL. */
#ifdef __MACAPI__
# ifdef _MSC_VER
    HRESULT (STDAPICALLTYPE FAR* pfnDeferredFillIn)(struct tagEXCEPINFO FAR*);
# else
    STDAPICALLTYPE HRESULT (FAR* pfnDeferredFillIn)(struct tagEXCEPINFO FAR*);
# endif
#else
    HRESULT (STDAPICALLTYPE FAR* pfnDeferredFillIn)(struct tagEXCEPINFO FAR*);
#endif

    SCODE scode;		/* An SCODE describing the error. */

} EXCEPINFO, FAR* LPEXCEPINFO;

typedef enum tagCALLCONV {
    CC_FASTCALL = 0
    , CC_CDECL = 1
    , CC_MSCPASCAL
    , CC_PASCAL = CC_MSCPASCAL
    , CC_MACPASCAL
    , CC_STDCALL
    , CC_FPFASTCALL
    , CC_SYSCALL
    , CC_MPWCDECL
    , CC_MPWPASCAL
    , CC_MAX			/* end of enum marker */
#ifdef _MAC
    , CC_FORCELONG = 2147483647
#endif
} CALLCONV;

typedef enum tagFUNCKIND {
      FUNC_VIRTUAL
    , FUNC_PUREVIRTUAL
    , FUNC_NONVIRTUAL
    , FUNC_STATIC
    , FUNC_DISPATCH
#ifdef _MAC
    , FUNC_FORCELONG = 2147483647
#endif
} FUNCKIND;

/* Flags for IDispatch::Invoke */
#define DISPATCH_METHOD		0x1
#define DISPATCH_PROPERTYGET	0x2
#define DISPATCH_PROPERTYPUT	0x4
#define DISPATCH_PROPERTYPUTREF	0x8

typedef enum tagINVOKEKIND {
      INVOKE_FUNC = DISPATCH_METHOD
    , INVOKE_PROPERTYGET = DISPATCH_PROPERTYGET
    , INVOKE_PROPERTYPUT = DISPATCH_PROPERTYPUT
    , INVOKE_PROPERTYPUTREF = DISPATCH_PROPERTYPUTREF
#ifdef _MAC
    , INVOKE_FORCELONG = 2147483647
#endif
} INVOKEKIND;

typedef struct FARSTRUCT tagFUNCDESC {
    MEMBERID memid;
    SCODE FAR* lprgscode;
    ELEMDESC FAR* lprgelemdescParam;  /* array of parameter types */
    FUNCKIND funckind;
    INVOKEKIND invkind;
    CALLCONV callconv;
    short cParams;
    short cParamsOpt;
    short oVft;
    short cScodes;
    ELEMDESC elemdescFunc;
    unsigned short wFuncFlags;
} FUNCDESC, FAR* LPFUNCDESC;

typedef enum tagVARKIND {
      VAR_PERINSTANCE
    , VAR_STATIC
    , VAR_CONST
    , VAR_DISPATCH
#ifdef _MAC
    , VAR_FORCELONG = 2147483647
#endif
} VARKIND;

#if defined(_MAC)
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2) // These structures require this packing, which was the
			// compiler default in OLE 2.02, but not anymore.	
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif
#endif
typedef struct FARSTRUCT tagVARDESC {
    MEMBERID memid;
    OLECHAR FAR* lpstrSchema;		/* reserved for future use */
    union {
      /* VAR_PERINSTANCE - the offset of this variable within the instance */
      unsigned long oInst;

      /* VAR_CONST - the value of the constant */
      VARIANT FAR* lpvarValue;

    }UNION_NAME(u);
    ELEMDESC elemdescVar;
    unsigned short wVarFlags;
    VARKIND varkind;
} VARDESC, FAR* LPVARDESC;

#if defined(_MAC)
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)		// restore default packing
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif
#endif

typedef enum tagTYPEFLAGS {
      TYPEFLAG_FAPPOBJECT = 0x01
    , TYPEFLAG_FCANCREATE = 0x02
    , TYPEFLAG_FLICENSED = 0x04
    , TYPEFLAG_FPREDECLID = 0x08
    , TYPEFLAG_FHIDDEN = 0x10
    , TYPEFLAG_FCONTROL = 0x20
    , TYPEFLAG_FDUAL = 0x40
    , TYPEFLAG_FNONEXTENSIBLE = 0x80
    , TYPEFLAG_FOLEAUTOMATION = 0x100
    , TYPEFLAG_FRESTRICTED = 0x200
    , TYPEFLAG_FAGGREGATABLE = 0x400
    , TYPEFLAG_FREPLACEABLE = 0x800
    , TYPEFLAG_FDISPATCHABLE = 0x1000
    , TYPEFLAG_FREVERSEBIND = 0x2000
    , TYPEFLAG_FPROXY = 0x4000
#ifdef _MAC
    , TYPEFLAG_FORCELONG  = 2147483647
#endif
} TYPEFLAGS;

typedef enum tagFUNCFLAGS {
      FUNCFLAG_FRESTRICTED= 1
      , FUNCFLAG_FSOURCE= 0x2
      , FUNCFLAG_FBINDABLE= 0x4
      , FUNCFLAG_FREQUESTEDIT= 0x8
      , FUNCFLAG_FDISPLAYBIND= 0x10
      , FUNCFLAG_FDEFAULTBIND= 0x20
      , FUNCFLAG_FHIDDEN= 0x40
      , FUNCFLAG_FUSESGETLASTERROR= 0x80
      , FUNCFLAG_FDEFAULTCOLLELEM = 0x100
      , FUNCFLAG_FUIDEFAULT = 0x200
      , FUNCFLAG_FNONBROWSABLE = 0x400
      , FUNCFLAG_FREPLACEABLE = 0x800
      , FUNCFLAG_FIMMEDIATEBIND = 0x1000
#ifdef _MAC
    , FUNCFLAG_FORCELONG  = 2147483647
#endif
} FUNCFLAGS;

typedef enum tagVARFLAGS {
      VARFLAG_FREADONLY = 1
      , VARFLAG_FSOURCE= 0x2
      , VARFLAG_FBINDABLE= 0x4
      , VARFLAG_FREQUESTEDIT= 0x8
      , VARFLAG_FDISPLAYBIND= 0x10
      , VARFLAG_FDEFAULTBIND= 0x20
      , VARFLAG_FHIDDEN	= 0x40
      , VARFLAG_FRESTRICTED = 0x80
      , VARFLAG_FDEFAULTCOLLELEM = 0x100
      , VARFLAG_FUIDEFAULT = 0x200
      , VARFLAG_FNONBROWSABLE = 0x400
      , VARFLAG_FREPLACEABLE = 0x800
      , VARFLAG_FIMMEDIATEBIND = 0x1000
#ifdef _MAC
    , VARFLAG_FORCELONG   = 2147483647
#endif
} VARFLAGS;

/* IMPLTYPE Flags */
#define IMPLTYPEFLAG_FDEFAULT		0x1
#define IMPLTYPEFLAG_FSOURCE		0x2
#define IMPLTYPEFLAG_FRESTRICTED	0x4
#define IMPLTYPEFLAG_FDEFAULTVTABLE 0x8

#undef  INTERFACE
#define INTERFACE ITypeInfo

DECLARE_INTERFACE_(ITypeInfo, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ITypeInfo methods */
    STDMETHOD(GetTypeAttr)(THIS_ TYPEATTR FAR* FAR* pptypeattr) PURE;

    STDMETHOD(GetTypeComp)(THIS_ ITypeComp FAR* FAR* pptcomp) PURE;

    STDMETHOD(GetFuncDesc)(THIS_
      unsigned int index, FUNCDESC FAR* FAR* ppfuncdesc) PURE;

    STDMETHOD(GetVarDesc)(THIS_
      unsigned int index, VARDESC FAR* FAR* ppvardesc) PURE;

    STDMETHOD(GetNames)(THIS_
      MEMBERID memid,
      BSTR FAR* rgbstrNames,
      unsigned int cMaxNames,
      unsigned int FAR* pcNames) PURE;

    STDMETHOD(GetRefTypeOfImplType)(THIS_
      unsigned int index, HREFTYPE FAR* phreftype) PURE;

    STDMETHOD(GetImplTypeFlags)(THIS_
      unsigned int index, int FAR* pimpltypeflags) PURE;

    STDMETHOD(GetIDsOfNames)(THIS_
      OLECHAR FAR* FAR* rgszNames,
      unsigned int cNames,
      MEMBERID FAR* rgmemid) PURE;

    STDMETHOD(Invoke)(THIS_
      void FAR* pvInstance,
      MEMBERID memid,
      unsigned short wFlags,
      DISPPARAMS FAR *pdispparams,
      VARIANT FAR *pvarResult,
      EXCEPINFO FAR *pexcepinfo,
      unsigned int FAR *puArgErr) PURE;

    STDMETHOD(GetDocumentation)(THIS_
      MEMBERID memid,
      BSTR FAR* pbstrName,
      BSTR FAR* pbstrDocString,
      unsigned long FAR* pdwHelpContext,
      BSTR FAR* pbstrHelpFile) PURE;

    STDMETHOD(GetDllEntry)(THIS_
      MEMBERID memid,
      INVOKEKIND invkind, 
      BSTR FAR* pbstrDllName,
      BSTR FAR* pbstrName,
      unsigned short FAR* pwOrdinal) PURE;

    STDMETHOD(GetRefTypeInfo)(THIS_
      HREFTYPE hreftype, ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(AddressOfMember)(THIS_
      MEMBERID memid, INVOKEKIND invkind, void FAR* FAR* ppv) PURE;

    STDMETHOD(CreateInstance)(THIS_
      IUnknown FAR* punkOuter,
      REFIID riid,
      void FAR* FAR* ppvObj) PURE;

    STDMETHOD(GetMops)(THIS_ MEMBERID memid, BSTR FAR* pbstrMops) PURE;

    STDMETHOD(GetContainingTypeLib)(THIS_
      ITypeLib FAR* FAR* pptlib, unsigned int FAR* pindex) PURE;

    STDMETHOD_(void, ReleaseTypeAttr)(THIS_ TYPEATTR FAR* ptypeattr) PURE;
    STDMETHOD_(void, ReleaseFuncDesc)(THIS_ FUNCDESC FAR* pfuncdesc) PURE;
    STDMETHOD_(void, ReleaseVarDesc)(THIS_ VARDESC FAR* pvardesc) PURE;
};

typedef ITypeInfo FAR* LPTYPEINFO;


/*---------------------------------------------------------------------*/
/*                   ITypeInfo2 interface                              */
/*---------------------------------------------------------------------*/

#undef  INTERFACE
#define INTERFACE ITypeInfo2

DECLARE_INTERFACE_(ITypeInfo2, ITypeInfo)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ITypeInfo methods */
    STDMETHOD(GetTypeAttr)(THIS_ TYPEATTR **pptypeattr) PURE;

    STDMETHOD(GetTypeComp)(THIS_ ITypeComp **pptcomp) PURE;

    STDMETHOD(GetFuncDesc)(THIS_
                           unsigned int index, 
                           FUNCDESC **  ppfuncdesc) PURE;

    STDMETHOD(GetVarDesc)(THIS_
                          unsigned int  index, 
                          VARDESC **    ppvardesc) PURE;

    STDMETHOD(GetNames)(THIS_
                        MEMBERID        memid,
                        BSTR *          rgbstrNames,
                        unsigned int    cMaxNames,
                        unsigned int *  pcNames) PURE;

    STDMETHOD(GetRefTypeOfImplType)(THIS_
                                    unsigned int    index, 
                                    HREFTYPE *      phreftype) PURE;

    STDMETHOD(GetImplTypeFlags)(THIS_
                                unsigned int    index, 
                                int *           pimpltypeflags) PURE;

    STDMETHOD(GetIDsOfNames)(THIS_
                             OLECHAR **     rgszNames,
                             unsigned int   cNames,
                             MEMBERID *     rgmemid) PURE;

    STDMETHOD(Invoke)(THIS_
                      void *            pvInstance,
                      MEMBERID memid,
                      unsigned short    wFlags,
                      DISPPARAMS *      pdispparams,
                      VARIANT *         pvarResult,
                      EXCEPINFO *       pexcepinfo,
                      unsigned int *    puArgErr) PURE;

    STDMETHOD(GetDocumentation)(THIS_
                                MEMBERID        memid,
                                BSTR *          pbstrName,
                                BSTR *          pbstrDocString,
                                unsigned long * pdwHelpContext,
                                BSTR *          pbstrHelpFile) PURE;

    STDMETHOD(GetDllEntry)(THIS_
                           MEMBERID         memid,
                           INVOKEKIND       invkind, 
                           BSTR *           pbstrDllName,
                           BSTR *           pbstrName,
                           unsigned short * pwOrdinal) PURE;

    STDMETHOD(GetRefTypeInfo)(THIS_
                              HREFTYPE      hreftype, 
                              ITypeInfo **  pptinfo) PURE;

    STDMETHOD(AddressOfMember)(THIS_
                               MEMBERID     memid, 
                               INVOKEKIND   invkind, 
                               void **      ppv) PURE;

    STDMETHOD(CreateInstance)(THIS_
                              IUnknown *punkOuter,
                              REFIID    riid,
                              void **   ppvObj) PURE;

    STDMETHOD(GetMops)(THIS_ MEMBERID memid, BSTR *pbstrMops) PURE;

    STDMETHOD(GetContainingTypeLib)(THIS_
                                    ITypeLib **     pptlib, 
                                    unsigned int *  pindex) PURE;

    STDMETHOD_(void, ReleaseTypeAttr)(THIS_ TYPEATTR *ptypeattr) PURE;
    STDMETHOD_(void, ReleaseFuncDesc)(THIS_ FUNCDESC *pfuncdesc) PURE;
    STDMETHOD_(void, ReleaseVarDesc)(THIS_ VARDESC *pvardesc) PURE;

    /* ITypeInfo2 methods */
    STDMETHOD(GetTypeKind)(THIS_ TYPEKIND *pTypeKind) PURE;

    STDMETHOD(GetTypeFlags)(THIS_ unsigned long *pdwTypeFlags) PURE;

    STDMETHOD(GetFuncIndexOfMemId)(THIS_ 
                                   MEMBERID         memid, 
                                   INVOKEKIND       invkind, 
                                   unsigned int *   pFuncIndex) PURE;

    STDMETHOD(GetVarIndexOfMemId)(THIS_ 
                                  MEMBERID          memid, 
                                  unsigned int *    pVarIndex) PURE;

    STDMETHOD(GetCustData)(THIS_
			   REFGUID    guid,
			   VARIANT    *pVarVal) PURE;
    
    STDMETHOD(GetFuncCustData)(THIS_ 
                               unsigned int index, 
                               REFGUID      guid, 
                               VARIANT *    pVarVal) PURE;
    
    STDMETHOD(GetParamCustData)(THIS_ 
                                unsigned int    indexFunc, 
                                unsigned int    indexParam, 
                                REFGUID         guid, 
                                VARIANT *       pVarVal) PURE;

    STDMETHOD(GetVarCustData)(THIS_ 
                              unsigned int  index, 
                              REFGUID       guid, 
                              VARIANT *     pVarVal) PURE;

    STDMETHOD(GetImplTypeCustData)(THIS_ 
                                   unsigned int  index, 
                                   REFGUID       guid, 
                                   VARIANT *     pVarVal) PURE;

    STDMETHOD(GetDocumentation2)(THIS_
                                 MEMBERID        memid,
				 LCID  lcid,
				 BSTR  *pbstrHelpString,
				 unsigned long *pdwHelpStringContext,
				 BSTR  *pbstrHelpStringDll) PURE;

    STDMETHOD(GetAllCustData)(THIS_
			   LPCUSTDATA pCustData) PURE;
    
    STDMETHOD(GetAllFuncCustData)(THIS_ 
                               unsigned int index, 
			       LPCUSTDATA   pCustData) PURE;
    
    STDMETHOD(GetAllParamCustData)(THIS_ 
                                unsigned int    indexFunc, 
                                unsigned int    indexParam, 
			        LPCUSTDATA      pCustData) PURE;

    STDMETHOD(GetAllVarCustData)(THIS_ 
                              unsigned int  index, 
			      LPCUSTDATA    pCustData) PURE;

    STDMETHOD(GetAllImplTypeCustData)(THIS_ 
                                   unsigned int  index, 
			           LPCUSTDATA    pCustData) PURE;

};

typedef ITypeInfo2 *LPTYPEINFO2;


/*---------------------------------------------------------------------*/
/*                            ITypeComp                                */
/*---------------------------------------------------------------------*/

typedef enum tagDESCKIND {
      DESCKIND_NONE = 0
    , DESCKIND_FUNCDESC
    , DESCKIND_VARDESC
    , DESCKIND_TYPECOMP
    , DESCKIND_IMPLICITAPPOBJ
    , DESCKIND_MAX		/* end of enum marker */
#ifdef _MAC
    , DESCKIND_FORCELONG = 2147483647
#endif
} DESCKIND;

typedef union tagBINDPTR {
    FUNCDESC FAR* lpfuncdesc;
    VARDESC FAR* lpvardesc;
    ITypeComp FAR* lptcomp;
} BINDPTR, FAR* LPBINDPTR;


#undef  INTERFACE
#define INTERFACE ITypeComp

DECLARE_INTERFACE_(ITypeComp, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ITypeComp methods */
    STDMETHOD(Bind)(THIS_
      OLECHAR FAR* szName,
      unsigned long lHashVal,
      unsigned short wflags,
      ITypeInfo FAR* FAR* pptinfo,
      DESCKIND FAR* pdesckind,
      BINDPTR FAR* pbindptr) PURE;

    STDMETHOD(BindType)(THIS_
      OLECHAR FAR* szName,
      unsigned long lHashVal,
      ITypeInfo FAR* FAR* pptinfo,
      ITypeComp FAR* FAR* pptcomp) PURE;
};

typedef ITypeComp FAR* LPTYPECOMP;



/*---------------------------------------------------------------------*/
/*                         ICreateTypeLib                              */
/*---------------------------------------------------------------------*/


#undef  INTERFACE
#define INTERFACE ICreateTypeLib

DECLARE_INTERFACE_(ICreateTypeLib, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ICreateTypeLib methods */
    STDMETHOD(CreateTypeInfo)(THIS_
      OLECHAR FAR* szName,
      TYPEKIND tkind,
      ICreateTypeInfo FAR* FAR* lplpictinfo) PURE;

    STDMETHOD(SetName)(THIS_ OLECHAR FAR* szName) PURE;

    STDMETHOD(SetVersion)(THIS_
      unsigned short wMajorVerNum, unsigned short wMinorVerNum) PURE;

    STDMETHOD(SetGuid) (THIS_ REFGUID guid) PURE;

    STDMETHOD(SetDocString)(THIS_ OLECHAR FAR* szDoc) PURE;

    STDMETHOD(SetHelpFileName)(THIS_ OLECHAR FAR* szHelpFileName) PURE;

    STDMETHOD(SetHelpContext)(THIS_ unsigned long dwHelpContext) PURE;

    STDMETHOD(SetLcid)(THIS_ LCID lcid) PURE;

    STDMETHOD(SetLibFlags)(THIS_ unsigned int uLibFlags) PURE;

    STDMETHOD(SaveAllChanges)(THIS) PURE;
};

typedef ICreateTypeLib FAR* LPCREATETYPELIB;


/*---------------------------------------------------------------------*/
/*                   ICreateTypeLib2 interface                         */
/*---------------------------------------------------------------------*/


#undef  INTERFACE
#define INTERFACE ICreateTypeLib2

DECLARE_INTERFACE_(ICreateTypeLib2, ICreateTypeLib)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ICreateTypeLib methods */
    STDMETHOD(CreateTypeInfo)(THIS_
                              OLECHAR *             szName,
                              TYPEKIND              tkind,
                              ICreateTypeInfo **    lplpictinfo) PURE;

    STDMETHOD(SetName)(THIS_ OLECHAR *szName) PURE;

    STDMETHOD(SetVersion)(THIS_
                          unsigned short wMajorVerNum,
                          unsigned short wMinorVerNum) PURE;

    STDMETHOD(SetGuid) (THIS_ REFGUID guid) PURE;

    STDMETHOD(SetDocString)(THIS_ OLECHAR *szDoc) PURE;

    STDMETHOD(SetHelpFileName)(THIS_ OLECHAR *szHelpFileName) PURE;

    STDMETHOD(SetHelpContext)(THIS_ unsigned long dwHelpContext) PURE;

    STDMETHOD(SetLcid)(THIS_ LCID lcid) PURE;

    STDMETHOD(SetLibFlags)(THIS_ unsigned int uLibFlags) PURE;

    STDMETHOD(SaveAllChanges)(THIS) PURE;

    /* ICreateTypeLib2 methods */
    STDMETHOD(DeleteTypeInfo)(THIS_ LPOLESTR szName) PURE;

    STDMETHOD(SetCustData)(THIS_ REFGUID guid, VARIANT *pVarVal) PURE;

    STDMETHOD(SetHelpStringContext)(THIS_
                                    unsigned long dwHelpStringContext) PURE;
    STDMETHOD(SetHelpStringDll)(THIS_ LPOLESTR szFileName) PURE;
};

typedef ICreateTypeLib2 *LPCREATETYPELIB2;


/*---------------------------------------------------------------------*/
/*                         ICreateTypeInfo                             */
/*---------------------------------------------------------------------*/

#undef  INTERFACE
#define INTERFACE ICreateTypeInfo

DECLARE_INTERFACE_(ICreateTypeInfo, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ICreateTypeInfo methods */
    STDMETHOD(SetGuid)(THIS_ REFGUID guid) PURE;

    STDMETHOD(SetTypeFlags)(THIS_ unsigned int uTypeFlags) PURE;

    STDMETHOD(SetDocString)(THIS_ OLECHAR FAR* pstrDoc) PURE;

    STDMETHOD(SetHelpContext)(THIS_ unsigned long dwHelpContext) PURE;

    STDMETHOD(SetVersion)(THIS_
      unsigned short wMajorVerNum, unsigned short wMinorVerNum) PURE;

    STDMETHOD(AddRefTypeInfo)(THIS_
      ITypeInfo FAR* ptinfo, HREFTYPE FAR* phreftype) PURE;

    STDMETHOD(AddFuncDesc)(THIS_
      unsigned int index, FUNCDESC FAR* pfuncdesc) PURE;

    STDMETHOD(AddImplType)(THIS_
      unsigned int index, HREFTYPE hreftype) PURE;

    STDMETHOD(SetImplTypeFlags)(THIS_
      unsigned int index, int impltypeflags) PURE;

    STDMETHOD(SetAlignment)(THIS_ unsigned short cbAlignment) PURE;

    STDMETHOD(SetSchema)(THIS_ OLECHAR FAR* lpstrSchema) PURE;

    STDMETHOD(AddVarDesc)(THIS_
      unsigned int index, VARDESC FAR* pvardesc) PURE;

    STDMETHOD(SetFuncAndParamNames)(THIS_
      unsigned int index, OLECHAR FAR* FAR* rgszNames, unsigned int cNames) PURE;

    STDMETHOD(SetVarName)(THIS_
      unsigned int index, OLECHAR FAR* szName) PURE;

    STDMETHOD(SetTypeDescAlias)(THIS_
      TYPEDESC FAR* ptdescAlias) PURE;

    STDMETHOD(DefineFuncAsDllEntry)(THIS_
      unsigned int index, OLECHAR FAR* szDllName, OLECHAR FAR* szProcName) PURE;

    STDMETHOD(SetFuncDocString)(THIS_
      unsigned int index, OLECHAR FAR* szDocString) PURE;

    STDMETHOD(SetVarDocString)(THIS_
      unsigned int index, OLECHAR FAR* szDocString) PURE;

    STDMETHOD(SetFuncHelpContext)(THIS_
      unsigned int index, unsigned long dwHelpContext) PURE;

    STDMETHOD(SetVarHelpContext)(THIS_
      unsigned int index, unsigned long dwHelpContext) PURE;

    STDMETHOD(SetMops)(THIS_
      unsigned int index, BSTR bstrMops) PURE;

    STDMETHOD(SetTypeIdldesc)(THIS_
      IDLDESC FAR* pidldesc) PURE;

    STDMETHOD(LayOut)(THIS) PURE;
};

typedef ICreateTypeInfo FAR* LPCREATETYPEINFO;


/*---------------------------------------------------------------------*/
/*                   ICreateTypeInfo2                                  */
/*---------------------------------------------------------------------*/

#undef  INTERFACE
#define INTERFACE ICreateTypeInfo2

DECLARE_INTERFACE_(ICreateTypeInfo2, ICreateTypeInfo)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ICreateTypeInfo methods */
    STDMETHOD(SetGuid)(THIS_ REFGUID guid) PURE;

    STDMETHOD(SetTypeFlags)(THIS_ unsigned int uTypeFlags) PURE;

    STDMETHOD(SetDocString)(THIS_ OLECHAR *pstrDoc) PURE;

    STDMETHOD(SetHelpContext)(THIS_ unsigned long dwHelpContext) PURE;

    STDMETHOD(SetVersion)(THIS_
                          unsigned short wMajorVerNum,
                          unsigned short wMinorVerNum) PURE;

    STDMETHOD(AddRefTypeInfo)(THIS_
                              ITypeInfo *   ptinfo,
                              HREFTYPE *    phreftype) PURE;

    STDMETHOD(AddFuncDesc)(THIS_
                           unsigned int index,
                           FUNCDESC *   pfuncdesc) PURE;

    STDMETHOD(AddImplType)(THIS_
                           unsigned int index,
                           HREFTYPE     hreftype) PURE;

    STDMETHOD(SetImplTypeFlags)(THIS_
                                unsigned int    index,
                                int             impltypeflags) PURE;

    STDMETHOD(SetAlignment)(THIS_ unsigned short cbAlignment) PURE;

    STDMETHOD(SetSchema)(THIS_ OLECHAR *lpstrSchema) PURE;

    STDMETHOD(AddVarDesc)(THIS_
                          unsigned int  index,
                          VARDESC *     pvardesc) PURE;

    STDMETHOD(SetFuncAndParamNames)(THIS_
                                    unsigned int    index,
                                    OLECHAR **      rgszNames, 
                                    unsigned int    cNames) PURE;

    STDMETHOD(SetVarName)(THIS_
                          unsigned int  index,
                          OLECHAR *     szName) PURE;

    STDMETHOD(SetTypeDescAlias)(THIS_
                                TYPEDESC *ptdescAlias) PURE;

    STDMETHOD(DefineFuncAsDllEntry)(THIS_
                                    unsigned int    index,
                                    OLECHAR *       szDllName,
                                    OLECHAR *       szProcName) PURE;

    STDMETHOD(SetFuncDocString)(THIS_
                                unsigned int    index,
                                OLECHAR *       szDocString) PURE;

    STDMETHOD(SetVarDocString)(THIS_
                               unsigned int     index,
                               OLECHAR *        szDocString) PURE;

    STDMETHOD(SetFuncHelpContext)(THIS_
                                  unsigned int  index,
                                  unsigned long dwHelpContext) PURE;

    STDMETHOD(SetVarHelpContext)(THIS_
                                 unsigned int   index,
                                 unsigned long  dwHelpContext) PURE;

    STDMETHOD(SetMops)(THIS_
                       unsigned int index,
                       BSTR         bstrMops) PURE;

    STDMETHOD(SetTypeIdldesc)(THIS_
                              IDLDESC *pidldesc) PURE;

    STDMETHOD(LayOut)(THIS) PURE;

    /* ICreateTypeInfo2 methods */
    STDMETHOD(DeleteFuncDesc)(THIS_ unsigned int index) PURE;

    STDMETHOD(DeleteFuncDescByMemId)(THIS_
                                     MEMBERID   memid,
                                     INVOKEKIND invkind) PURE;

    STDMETHOD(DeleteVarDesc)(THIS_ unsigned int index) PURE;

    STDMETHOD(DeleteVarDescByMemId)(THIS_ MEMBERID memid) PURE;

    STDMETHOD(DeleteImplType)(THIS_ unsigned int index) PURE;

    STDMETHOD(SetCustData)(THIS_ REFGUID guid, VARIANT *pVarVal) PURE;

    STDMETHOD(SetFuncCustData)(THIS_ 
                               unsigned int index, 
                               REFGUID      guid, 
                               VARIANT *    pVarVal) PURE;
    
    STDMETHOD(SetParamCustData)(THIS_ 
                                unsigned int    indexFunc, 
                                unsigned int    indexParam, 
                                REFGUID         guid, 
                                VARIANT *       pVarVal) PURE;

    STDMETHOD(SetVarCustData)(THIS_ 
                              unsigned int  index, 
                              REFGUID       guid, 
                              VARIANT *     pVarVal) PURE;

    STDMETHOD(SetImplTypeCustData)(THIS_ 
                              unsigned int  index, 
                              REFGUID       guid, 
                              VARIANT *     pVarVal) PURE;

    STDMETHOD(SetHelpStringContext)(THIS_
                                    unsigned long dwHelpStringContext) PURE;

    STDMETHOD(SetFuncHelpStringContext)(THIS_
                                        unsigned int    index,
                                        unsigned long   dwHelpStringContext) PURE;

    STDMETHOD(SetVarHelpStringContext)(THIS_
                                        unsigned int    index,
                                        unsigned long   dwHelpStringContext) PURE;

    STDMETHOD(Invalidate)(THIS) PURE;

    STDMETHOD(SetName)(THIS_ OLECHAR FAR* szName) PURE;

};

typedef ICreateTypeInfo2 *LPCREATETYPEINFO2;


/*---------------------------------------------------------------------*/
/*                         TypeInfo APIs                               */
/*---------------------------------------------------------------------*/
/* compute a 32bit hash value for the given name  based on the lcid and system kind
 */
#ifdef WIN32
STDAPI_(unsigned long)
LHashValOfNameSysA(SYSKIND syskind, LCID lcid, const char FAR* szName);
#endif //WIN32

STDAPI_(unsigned long)
LHashValOfNameSys(SYSKIND syskind, LCID lcid, const OLECHAR FAR* szName);

/* Macro to compute a 32bit hash value for the given name based on the LCID
 */
#ifdef _MAC
#define LHashValOfName(lcid, szName) \
	LHashValOfNameSys(SYS_MAC, lcid, szName)
#else
#define LHashValOfName(lcid, szName) \
	LHashValOfNameSys(SYS_WIN32, lcid, szName)
#endif

/* compute a 16bit hash value from 32 bit hash value
 */
#define WHashValOfLHashVal(lhashval) \
	 ((unsigned short) (0x0000ffff & (lhashval)))

/* Check if the hash values are compatible.
*/
#define IsHashValCompatible(lhashval1, lhashval2) \
	((BOOL) ((0x00ff0000 & (lhashval1)) == (0x00ff0000 & (lhashval2))))

/* load the typelib from the file with the given filename
 */
STDAPI
LoadTypeLib(const OLECHAR FAR* szFile, ITypeLib FAR* FAR* pptlib);

// Control how a type library is registered
typedef enum tagREGKIND
{
    REGKIND_DEFAULT,
    REGKIND_REGISTER,
    REGKIND_NONE
} REGKIND;

STDAPI
LoadTypeLibEx(LPCOLESTR szFile, REGKIND regkind, ITypeLib ** pptlib);

/* load registered typelib
 */
STDAPI
LoadRegTypeLib(
    REFGUID rguid,
    unsigned short wVerMajor,
    unsigned short wVerMinor,
    LCID lcid,
    ITypeLib FAR* FAR* pptlib);

/* get path to registered typelib
 */
STDAPI
QueryPathOfRegTypeLib(
    REFGUID guid,
    unsigned short wMaj,
    unsigned short wMin,
    LCID lcid,
    LPBSTR lpbstrPathName);

/* add typelib to registry
 */
STDAPI
RegisterTypeLib(
    ITypeLib FAR* ptlib,
    OLECHAR FAR* szFullPath,
    OLECHAR FAR* szHelpDir);

STDAPI UnRegisterTypeLib(
    REFGUID        libID,
    unsigned short wMajorVerNum,
    unsigned short wMinorVerNum,
    LCID           lcid,
    SYSKIND        syskind);

STDAPI
CreateTypeLib(SYSKIND syskind, LPCOLESTR szFile, ICreateTypeLib FAR* FAR* ppctlib);

STDAPI
CreateTypeLib2(SYSKIND syskind, LPCOLESTR szFile, ICreateTypeLib2 **ppctlib);


#ifdef _MAC
/* load the typelib from the file with the given FSSPEC
 */
STDAPI
LoadTypeLibFSp(const FSSpec *pfsspec, ITypeLib FAR* FAR* pptlib);

/* register the location of the standard typelib folder
 */
STDAPI
RegisterTypeLibFolder(OLECHAR FAR* szFullPath);

/* obtain the path to the registered typelib folder
 */
STDAPI
QueryTypeLibFolder(LPBSTR pbstr);
#endif // _MAC


/*---------------------------------------------------------------------*/
/*                          IEnumVARIANT                               */
/*---------------------------------------------------------------------*/

#undef  INTERFACE
#define INTERFACE IEnumVARIANT

DECLARE_INTERFACE_(IEnumVARIANT, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* IEnumVARIANT methods */
    STDMETHOD(Next)(
      THIS_ unsigned long celt, VARIANT FAR* rgvar, unsigned long FAR* pceltFetched) PURE;
    STDMETHOD(Skip)(THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset)(THIS) PURE;
    STDMETHOD(Clone)(THIS_ IEnumVARIANT FAR* FAR* ppenum) PURE;
};

typedef IEnumVARIANT FAR* LPENUMVARIANT;


/*---------------------------------------------------------------------*/
/*                             IDispatch                               */
/*---------------------------------------------------------------------*/


#undef  INTERFACE
#define INTERFACE IDispatch

DECLARE_INTERFACE_(IDispatch, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(THIS_ unsigned int FAR* pctinfo) PURE;

    STDMETHOD(GetTypeInfo)(
      THIS_
      unsigned int itinfo,
      LCID lcid,
      ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetIDsOfNames)(
      THIS_
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      unsigned int cNames,
      LCID lcid,
      DISPID FAR* rgdispid) PURE;

    STDMETHOD(Invoke)(
      THIS_
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      unsigned short wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      unsigned int FAR* puArgErr) PURE;
};

typedef IDispatch FAR* LPDISPATCH;


/* DISPID reserved for the standard "value" property */
#define DISPID_VALUE	0

/* DISPID reserved to indicate an "unknown" name */
#define DISPID_UNKNOWN	-1

/* The following DISPID is reserved to indicate the param
 * that is the right-hand-side (or "put" value) of a PropertyPut
 */
#define DISPID_PROPERTYPUT -3

/* DISPID reserved for the standard "NewEnum" method */
#define DISPID_NEWENUM	-4

/* DISPID reserved for the standard "Evaluate" method */
#define DISPID_EVALUATE	-5

#define DISPID_CONSTRUCTOR -6
#define DISPID_DESTRUCTOR -7
#define DISPID_COLLECT -8

/* The range -500 through -999 is reserved for Controls */
/* The range 0x80010000 through 0x8001FFFF is reserved for Controls */
/* The range -5000 through -5499 is reserved for ActiveX Accessability */
/* The range -2000 through -2499 is reserved for VB5 */
/* The range -2700 through -2799 is reserved for VBScript/JavaScript */
/* The range -3900 through -3999 is reserved for Forms */
/* The range -5500 through -5550 is reserved for Forms */
/* The remainder of the negative DISPIDs are reserved for future use */

//---------------------------------------------------------------------
//                      IErrorInfo, et al
//---------------------------------------------------------------------

#undef  INTERFACE
#define INTERFACE  IErrorInfo
DECLARE_INTERFACE_(IErrorInfo, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* IErrorInfo methods */
    STDMETHOD(GetGUID)(THIS_ GUID FAR* pguid) PURE;
    STDMETHOD(GetSource)(THIS_ BSTR FAR* pbstrSource) PURE;
    STDMETHOD(GetDescription)(THIS_ BSTR FAR* pbstrDescription) PURE;
    STDMETHOD(GetHelpFile)(THIS_ BSTR FAR* pbstrHelpFile) PURE;
    STDMETHOD(GetHelpContext)(THIS_ unsigned long FAR* pdwHelpContext) PURE;
};

#undef  INTERFACE
#define INTERFACE  ICreateErrorInfo
DECLARE_INTERFACE_(ICreateErrorInfo, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ICreateErrorInfo methods */
    STDMETHOD(SetGUID)(THIS_ REFGUID rguid) PURE;
    STDMETHOD(SetSource)(THIS_ LPOLESTR szSource) PURE;
    STDMETHOD(SetDescription)(THIS_ LPOLESTR szDescription) PURE;
    STDMETHOD(SetHelpFile)(THIS_ LPOLESTR szHelpFile) PURE;
    STDMETHOD(SetHelpContext)(THIS_ unsigned long dwHelpContext) PURE;
};

#undef  INTERFACE
#define INTERFACE  ISupportErrorInfo
DECLARE_INTERFACE_(ISupportErrorInfo, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void FAR* FAR* ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ISupportErrorInfo methods */
    STDMETHOD(InterfaceSupportsErrorInfo)(THIS_ REFIID riid) PURE;
};

STDAPI SetErrorInfo(unsigned long dwReserved, IErrorInfo FAR* perrinfo);
STDAPI GetErrorInfo(unsigned long dwReserved, IErrorInfo FAR* FAR* pperrinfo);
STDAPI CreateErrorInfo(ICreateErrorInfo FAR* FAR* pperrinfo);


/*---------------------------------------------------------------------*/
/*                   ITypeChangeEvents                                 */
/*---------------------------------------------------------------------*/

// notification messages used by the dynamic typeinfo protocol.
typedef enum tagCHANGEKIND {
    CHANGEKIND_ADDMEMBER,
    CHANGEKIND_DELETEMEMBER,
    CHANGEKIND_SETNAMES,
    CHANGEKIND_SETDOCUMENTATION,
    CHANGEKIND_GENERAL,
    CHANGEKIND_INVALIDATE,
    CHANGEKIND_CHANGEFAILED,
    CHANGEKIND_MAX
} CHANGEKIND;

#undef  INTERFACE
#define INTERFACE ITypeChangeEvents

DECLARE_INTERFACE_(ITypeChangeEvents, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObj) PURE;
    STDMETHOD_(unsigned long, AddRef)(THIS) PURE;
    STDMETHOD_(unsigned long, Release)(THIS) PURE;

    /* ITypeChangeEvents methods */
    STDMETHOD(RequestTypeChange)(THIS_
                                 CHANGEKIND     changekind,
                                 ITypeInfo *    ptinfoBefore,
                                 LPOLESTR       lpstrName,
                                 int *          pfCancel) PURE;
    
    STDMETHOD(AfterTypeChange)(THIS_ 
                               CHANGEKIND   changekind,
                               ITypeInfo *  ptinfoAfter,
                               LPOLESTR     lpstrName) PURE;
};

typedef ITypeChangeEvents *LPTYPECHANGEEVENTS;


/*---------------------------------------------------------------------*/
/*                   IDispatch implementation support                  */
/*---------------------------------------------------------------------*/

#if defined(_MAC)
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2) // These structures require this packing, which was the
			// compiler default in OLE 2.02, but not anymore.	
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif
#endif

typedef struct FARSTRUCT tagPARAMDATA {
    OLECHAR FAR* szName;		/* parameter name */
    VARTYPE vt;			/* parameter type */
} PARAMDATA, FAR* LPPARAMDATA;

#if defined(_MAC)
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)		// restore default packing
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif
#endif

typedef struct FARSTRUCT tagMETHODDATA {
    OLECHAR FAR* szName;		/* method name */
    PARAMDATA FAR* ppdata;	/* pointer to an array of PARAMDATAs */
    DISPID dispid;		/* method ID */
    unsigned int iMeth;		/* method index */
    CALLCONV cc;		/* calling convention */
    unsigned int cArgs;		/* count of arguments */
    unsigned short wFlags;	/* same wFlags as on IDispatch::Invoke() */
    VARTYPE vtReturn;
} METHODDATA, FAR* LPMETHODDATA;

typedef struct FARSTRUCT tagINTERFACEDATA {
    METHODDATA FAR* pmethdata;	/* pointer to an array of METHODDATAs */
    unsigned int cMembers;	/* count of members */
} INTERFACEDATA, FAR* LPINTERFACEDATA;



/* Locate the parameter indicated by the given position, and
 * return it coerced to the given target VARTYPE (vtTarg).
 */
STDAPI
DispGetParam(
    DISPPARAMS FAR* pdispparams,
    unsigned int position,
    VARTYPE vtTarg,
    VARIANT FAR* pvarResult,
    unsigned int FAR* puArgErr);

/* Automatic TypeInfo driven implementation of IDispatch::GetIDsOfNames()
 */ 
STDAPI
DispGetIDsOfNames(
    ITypeInfo FAR* ptinfo,
    OLECHAR FAR* FAR* rgszNames,
    unsigned int cNames,
    DISPID FAR* rgdispid);

/* Automatic TypeInfo driven implementation of IDispatch::Invoke()
 */
STDAPI
DispInvoke(
    void FAR* _this,
    ITypeInfo FAR* ptinfo,
    DISPID dispidMember,
    unsigned short wFlags,
    DISPPARAMS FAR* pparams,
    VARIANT FAR* pvarResult,
    EXCEPINFO FAR* pexcepinfo,
    unsigned int FAR* puArgErr);

/* Construct a TypeInfo from an interface data description
 */
STDAPI
CreateDispTypeInfo(
    INTERFACEDATA FAR* pidata,
    LCID lcid,
    ITypeInfo FAR* FAR* pptinfo);

/* Create an instance of the standard TypeInfo driven IDispatch
 * implementation.
 */
STDAPI
CreateStdDispatch(
    IUnknown FAR* punkOuter,
    void FAR* pvThis,
    ITypeInfo FAR* ptinfo,
    IUnknown FAR* FAR* ppunkStdDisp);

// Low-level helper for IDispatch::Invoke() provides machine independence
// for customized Invoke().
STDAPI
DispCallFunc(
    void *        pvInstance,
    unsigned long oVft,
    CALLCONV      cc,
    VARTYPE       vtReturn,
    unsigned int  cActuals,
    VARTYPE *     prgvt,
    VARIANTARG ** prgpvarg,
    VARIANT *     pvargResult);

/*---------------------------------------------------------------------*/
/*                    Active Object Registration API                   */
/*---------------------------------------------------------------------*/

/* flags for RegisterActiveObject */
#define ACTIVEOBJECT_STRONG 0x0
#define ACTIVEOBJECT_WEAK 0x1

STDAPI
RegisterActiveObject(
    IUnknown FAR* punk,
    REFCLSID rclsid,
    unsigned long dwFlags,
    unsigned long FAR* pdwRegister);

STDAPI
RevokeActiveObject(
    unsigned long dwRegister,
    void FAR* pvReserved);

STDAPI
GetActiveObject(
    REFCLSID rclsid,
    void FAR* pvReserved,
    IUnknown FAR* FAR* ppunk);


#undef UNION_NAME

/*---------------------------------------------------------------------*/
/*                   New date functions
/*---------------------------------------------------------------------*/

#define VAR_VALIDDATE                0x0004    /* VarDateFromUdate() only */
#define VAR_CALENDAR_HIJRI           0x0008    /* use Hijri calender */
#define VARIANT_CALENDAR_HIJRI VAR_CALENDAR_HIJRI
// The SYSTEMTIME structure is copied directly from the Win32 API.  It
// is embedded in the UDATE structure (see below).
//
#if !defined(_WINBASE_)		// winbase.h defines this
#if !defined(_WORD_DEFINED)
#define _WORD_DEFINED
typedef unsigned short WORD;
#endif	// !defined(_WORD_DEFINED)

#if !defined(_SYSTEMTIME_)
#define _SYSTEMTIME_
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;
#endif // !defined(_SYSTEMTIME_)
#endif // !defined(_WINBASE_)


// The UDATE structure is used with VarDateFromUdate() and VarUdateFromDate().
// It represents an "unpacked date".
//
typedef struct {
    SYSTEMTIME      st;
    unsigned short  wDayOfYear;
} UDATE;

// APIs to "pack" and "unpack" dates.
//
STDAPI VarDateFromUdate(UDATE *pudateIn, unsigned long dwFlags, DATE *pdateOut);
STDAPI VarUdateFromDate(DATE dateIn, unsigned long dwFlags, UDATE *pudateOut);

/* API to retrieve the secondary(altername) month names
   Useful for Hijri, Polish and Russian alternate month names
*/   
STDAPI GetAltMonthNames(LCID lcid, LPOLESTR * * prgp);


/*---------------------------------------------------------------------*/
/*                           MISC API                                  */
/*---------------------------------------------------------------------*/

STDAPI_(unsigned long) OaBuildVersion(void);

STDAPI_(void) ClearCustData(LPCUSTDATA pCustData);

#endif /* _DISPATCH_H_ */

