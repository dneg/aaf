

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Mar 24 17:10:16 2006
 */
/* Compiler settings for C:\cygwin\home\jesse\SDK\AAF\ref-impl\include\com-api\AAFPrivate.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IAAFRoot,0x7C3712C2,0x390D,0x11D2,0x84,0x1B,0x00,0x60,0x08,0x32,0xAC,0xB8);


MIDL_DEFINE_GUID(IID, IID_IAAFHTMLClip,0xe684d784,0xb935,0x11d2,0xbf,0x9d,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFHTMLDescriptor,0xe684d770,0xb935,0x11d2,0xbf,0x9d,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFMetaDictionary,0xe1085eeb,0x0cc3,0x11d4,0x80,0x14,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFPropValData,0x106bb6e2,0xf0c1,0x11d2,0x84,0x2c,0x00,0x60,0x08,0x32,0xac,0xb8);


MIDL_DEFINE_GUID(IID, IID_IAAFStreamPropertyValue,0xF06bb6e2,0xf0c1,0x11d2,0x84,0x2c,0x00,0x60,0x08,0x32,0xac,0xb8);


MIDL_DEFINE_GUID(IID, IID_IAAFStrongRefArrayValue,0xf2973a82,0x6895,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xf3);


MIDL_DEFINE_GUID(IID, IID_IAAFStrongRefSetValue,0x84537782,0x6897,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xf3);


MIDL_DEFINE_GUID(IID, IID_IAAFStrongRefValue,0x4142d582,0x6892,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xf3);


MIDL_DEFINE_GUID(IID, IID_IAAFTextClip,0xe684d786,0xb935,0x11d2,0xbf,0x9d,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFWeakRefArrayValue,0x02fbe502,0x6897,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xe3);


MIDL_DEFINE_GUID(IID, IID_IAAFWeakRefSetValue,0xb1078f02,0x6897,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xe3);


MIDL_DEFINE_GUID(IID, IID_IAAFWeakRefValue,0x32a8ee02,0x6893,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xe3);


MIDL_DEFINE_GUID(IID, IID_IEnumAAFStorablePropVals,0x5096c202,0x831b,0x11d4,0xa8,0x12,0x8a,0x70,0xdf,0x17,0xed,0x53);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Mar 24 17:10:16 2006
 */
/* Compiler settings for C:\cygwin\home\jesse\SDK\AAF\ref-impl\include\com-api\AAFPrivate.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IAAFRoot,0x7C3712C2,0x390D,0x11D2,0x84,0x1B,0x00,0x60,0x08,0x32,0xAC,0xB8);


MIDL_DEFINE_GUID(IID, IID_IAAFHTMLClip,0xe684d784,0xb935,0x11d2,0xbf,0x9d,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFHTMLDescriptor,0xe684d770,0xb935,0x11d2,0xbf,0x9d,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFMetaDictionary,0xe1085eeb,0x0cc3,0x11d4,0x80,0x14,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFPropValData,0x106bb6e2,0xf0c1,0x11d2,0x84,0x2c,0x00,0x60,0x08,0x32,0xac,0xb8);


MIDL_DEFINE_GUID(IID, IID_IAAFStreamPropertyValue,0xF06bb6e2,0xf0c1,0x11d2,0x84,0x2c,0x00,0x60,0x08,0x32,0xac,0xb8);


MIDL_DEFINE_GUID(IID, IID_IAAFStrongRefArrayValue,0xf2973a82,0x6895,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xf3);


MIDL_DEFINE_GUID(IID, IID_IAAFStrongRefSetValue,0x84537782,0x6897,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xf3);


MIDL_DEFINE_GUID(IID, IID_IAAFStrongRefValue,0x4142d582,0x6892,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xf3);


MIDL_DEFINE_GUID(IID, IID_IAAFTextClip,0xe684d786,0xb935,0x11d2,0xbf,0x9d,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFWeakRefArrayValue,0x02fbe502,0x6897,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xe3);


MIDL_DEFINE_GUID(IID, IID_IAAFWeakRefSetValue,0xb1078f02,0x6897,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xe3);


MIDL_DEFINE_GUID(IID, IID_IAAFWeakRefValue,0x32a8ee02,0x6893,0x11d4,0xa8,0x12,0x8f,0x71,0xe1,0x57,0xe3,0xe3);


MIDL_DEFINE_GUID(IID, IID_IEnumAAFStorablePropVals,0x5096c202,0x831b,0x11d4,0xa8,0x12,0x8a,0x70,0xdf,0x17,0xed,0x53);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

