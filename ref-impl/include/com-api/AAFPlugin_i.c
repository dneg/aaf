

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 6.00.0361 */
/* at TIMESTAMP REMOVED
 */
/* Compiler settings for ../ref-impl/include/com-api/AAFPlugin.idl:
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

MIDL_DEFINE_GUID(IID, IID_IAAFClassExtension,0x65C07344,0xDF36,0x11d3,0x80,0x08,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceCodec,0x3631F7A2,0x9121,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceCodec2,0x8888F7A2,0x9121,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceContainer,0xa7337031,0xc103,0x11d2,0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceDataStream,0xCDDB6AB1,0x98DC,0x11d2,0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceDataStream2,0xF8C9C2A1,0xDD6B,0x4e10,0x88,0x4F,0x01,0x2A,0xF4,0x35,0x50,0xBC);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceStream,0x83402902,0x9146,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFInterpolator,0x75C6CDF2,0x0D67,0x11d3,0x80,0xA9,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFMultiEssenceCodec,0x7F93992D,0xDDA3,0x11d3,0x80,0x08,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFPlugin,0x3631F7A4,0x9121,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 6.00.0361 */
/* at TIMESTAMP REMOVED
 */
/* Compiler settings for ../ref-impl/include/com-api/AAFPlugin.idl:
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

MIDL_DEFINE_GUID(IID, IID_IAAFClassExtension,0x65C07344,0xDF36,0x11d3,0x80,0x08,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceCodec,0x3631F7A2,0x9121,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceCodec2,0x8888F7A2,0x9121,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceContainer,0xa7337031,0xc103,0x11d2,0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceDataStream,0xCDDB6AB1,0x98DC,0x11d2,0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceDataStream2,0xF8C9C2A1,0xDD6B,0x4e10,0x88,0x4F,0x01,0x2A,0xF4,0x35,0x50,0xBC);


MIDL_DEFINE_GUID(IID, IID_IAAFEssenceStream,0x83402902,0x9146,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFInterpolator,0x75C6CDF2,0x0D67,0x11d3,0x80,0xA9,0x00,0x60,0x08,0x14,0x3e,0x6f);


MIDL_DEFINE_GUID(IID, IID_IAAFMultiEssenceCodec,0x7F93992D,0xDDA3,0x11d3,0x80,0x08,0x00,0x10,0x4b,0xc9,0x15,0x6d);


MIDL_DEFINE_GUID(IID, IID_IAAFPlugin,0x3631F7A4,0x9121,0x11d2,0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

