

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AAFPrivate_h__
#define __AAFPrivate_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAAFRoot_FWD_DEFINED__
#define __IAAFRoot_FWD_DEFINED__
typedef interface IAAFRoot IAAFRoot;
#endif 	/* __IAAFRoot_FWD_DEFINED__ */


#ifndef __IAAFHTMLClip_FWD_DEFINED__
#define __IAAFHTMLClip_FWD_DEFINED__
typedef interface IAAFHTMLClip IAAFHTMLClip;
#endif 	/* __IAAFHTMLClip_FWD_DEFINED__ */


#ifndef __IAAFHTMLDescriptor_FWD_DEFINED__
#define __IAAFHTMLDescriptor_FWD_DEFINED__
typedef interface IAAFHTMLDescriptor IAAFHTMLDescriptor;
#endif 	/* __IAAFHTMLDescriptor_FWD_DEFINED__ */


#ifndef __IAAFMetaDictionary_FWD_DEFINED__
#define __IAAFMetaDictionary_FWD_DEFINED__
typedef interface IAAFMetaDictionary IAAFMetaDictionary;
#endif 	/* __IAAFMetaDictionary_FWD_DEFINED__ */


#ifndef __IAAFPropValData_FWD_DEFINED__
#define __IAAFPropValData_FWD_DEFINED__
typedef interface IAAFPropValData IAAFPropValData;
#endif 	/* __IAAFPropValData_FWD_DEFINED__ */


#ifndef __IAAFStreamPropertyValue_FWD_DEFINED__
#define __IAAFStreamPropertyValue_FWD_DEFINED__
typedef interface IAAFStreamPropertyValue IAAFStreamPropertyValue;
#endif 	/* __IAAFStreamPropertyValue_FWD_DEFINED__ */


#ifndef __IAAFStrongRefArrayValue_FWD_DEFINED__
#define __IAAFStrongRefArrayValue_FWD_DEFINED__
typedef interface IAAFStrongRefArrayValue IAAFStrongRefArrayValue;
#endif 	/* __IAAFStrongRefArrayValue_FWD_DEFINED__ */


#ifndef __IAAFStrongRefSetValue_FWD_DEFINED__
#define __IAAFStrongRefSetValue_FWD_DEFINED__
typedef interface IAAFStrongRefSetValue IAAFStrongRefSetValue;
#endif 	/* __IAAFStrongRefSetValue_FWD_DEFINED__ */


#ifndef __IAAFStrongRefValue_FWD_DEFINED__
#define __IAAFStrongRefValue_FWD_DEFINED__
typedef interface IAAFStrongRefValue IAAFStrongRefValue;
#endif 	/* __IAAFStrongRefValue_FWD_DEFINED__ */


#ifndef __IAAFTextClip_FWD_DEFINED__
#define __IAAFTextClip_FWD_DEFINED__
typedef interface IAAFTextClip IAAFTextClip;
#endif 	/* __IAAFTextClip_FWD_DEFINED__ */


#ifndef __IAAFWeakRefArrayValue_FWD_DEFINED__
#define __IAAFWeakRefArrayValue_FWD_DEFINED__
typedef interface IAAFWeakRefArrayValue IAAFWeakRefArrayValue;
#endif 	/* __IAAFWeakRefArrayValue_FWD_DEFINED__ */


#ifndef __IAAFWeakRefSetValue_FWD_DEFINED__
#define __IAAFWeakRefSetValue_FWD_DEFINED__
typedef interface IAAFWeakRefSetValue IAAFWeakRefSetValue;
#endif 	/* __IAAFWeakRefSetValue_FWD_DEFINED__ */


#ifndef __IAAFWeakRefValue_FWD_DEFINED__
#define __IAAFWeakRefValue_FWD_DEFINED__
typedef interface IAAFWeakRefValue IAAFWeakRefValue;
#endif 	/* __IAAFWeakRefValue_FWD_DEFINED__ */


#ifndef __IEnumAAFStorablePropVals_FWD_DEFINED__
#define __IEnumAAFStorablePropVals_FWD_DEFINED__
typedef interface IEnumAAFStorablePropVals IEnumAAFStorablePropVals;
#endif 	/* __IEnumAAFStorablePropVals_FWD_DEFINED__ */


/* header files for imported files */
#include "AAF.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_AAFPrivate_0000 */
/* [local] */ 

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
//=--------------------------------------------------------------------------=
// Private AAF Interfaces.
//=--------------------------------------------------------------------------=
//
















extern RPC_IF_HANDLE __MIDL_itf_AAFPrivate_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AAFPrivate_0000_v0_0_s_ifspec;

#ifndef __IAAFRoot_INTERFACE_DEFINED__
#define __IAAFRoot_INTERFACE_DEFINED__

/* interface IAAFRoot */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFRoot;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7C3712C2-390D-11D2-841B-00600832ACB8")
    IAAFRoot : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetImplRep( 
            /* [retval][out] */ void **__MIDL_0008) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE InitializeExtension( 
            REFCLSID clsid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFRootVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFRoot * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFRoot * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFRoot * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetImplRep )( 
            IAAFRoot * This,
            /* [retval][out] */ void **__MIDL_0008);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *InitializeExtension )( 
            IAAFRoot * This,
            REFCLSID clsid);
        
        END_INTERFACE
    } IAAFRootVtbl;

    interface IAAFRoot
    {
        CONST_VTBL struct IAAFRootVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFRoot_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFRoot_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFRoot_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFRoot_GetImplRep(This,__MIDL_0008)	\
    (This)->lpVtbl -> GetImplRep(This,__MIDL_0008)

#define IAAFRoot_InitializeExtension(This,clsid)	\
    (This)->lpVtbl -> InitializeExtension(This,clsid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local] */ HRESULT STDMETHODCALLTYPE IAAFRoot_GetImplRep_Proxy( 
    IAAFRoot * This,
    /* [retval][out] */ void **__MIDL_0008);


void __RPC_STUB IAAFRoot_GetImplRep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local] */ HRESULT STDMETHODCALLTYPE IAAFRoot_InitializeExtension_Proxy( 
    IAAFRoot * This,
    REFCLSID clsid);


void __RPC_STUB IAAFRoot_InitializeExtension_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFRoot_INTERFACE_DEFINED__ */


#ifndef __IAAFHTMLClip_INTERFACE_DEFINED__
#define __IAAFHTMLClip_INTERFACE_DEFINED__

/* interface IAAFHTMLClip */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFHTMLClip;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e684d784-b935-11d2-bf9d-00104bc9156d")
    IAAFHTMLClip : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetBeginAnchor( 
            /* [string][in] */ aafCharacter_constptr pBeginAnchor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBeginAnchor( 
            /* [size_is][string][out] */ aafCharacter *pBeginAnchor,
            /* [in] */ aafUInt32 bufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBeginAnchorBufLen( 
            /* [out] */ aafUInt32 *pBufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEndAnchor( 
            /* [string][in] */ aafCharacter_constptr pEndAnchor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEndAnchor( 
            /* [size_is][string][out] */ aafCharacter *pEndAnchor,
            /* [in] */ aafUInt32 bufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEndAnchorBufLen( 
            /* [out] */ aafUInt32 *pBufSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFHTMLClipVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFHTMLClip * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFHTMLClip * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFHTMLClip * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetBeginAnchor )( 
            IAAFHTMLClip * This,
            /* [string][in] */ aafCharacter_constptr pBeginAnchor);
        
        HRESULT ( STDMETHODCALLTYPE *GetBeginAnchor )( 
            IAAFHTMLClip * This,
            /* [size_is][string][out] */ aafCharacter *pBeginAnchor,
            /* [in] */ aafUInt32 bufSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBeginAnchorBufLen )( 
            IAAFHTMLClip * This,
            /* [out] */ aafUInt32 *pBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetEndAnchor )( 
            IAAFHTMLClip * This,
            /* [string][in] */ aafCharacter_constptr pEndAnchor);
        
        HRESULT ( STDMETHODCALLTYPE *GetEndAnchor )( 
            IAAFHTMLClip * This,
            /* [size_is][string][out] */ aafCharacter *pEndAnchor,
            /* [in] */ aafUInt32 bufSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetEndAnchorBufLen )( 
            IAAFHTMLClip * This,
            /* [out] */ aafUInt32 *pBufSize);
        
        END_INTERFACE
    } IAAFHTMLClipVtbl;

    interface IAAFHTMLClip
    {
        CONST_VTBL struct IAAFHTMLClipVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFHTMLClip_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFHTMLClip_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFHTMLClip_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFHTMLClip_SetBeginAnchor(This,pBeginAnchor)	\
    (This)->lpVtbl -> SetBeginAnchor(This,pBeginAnchor)

#define IAAFHTMLClip_GetBeginAnchor(This,pBeginAnchor,bufSize)	\
    (This)->lpVtbl -> GetBeginAnchor(This,pBeginAnchor,bufSize)

#define IAAFHTMLClip_GetBeginAnchorBufLen(This,pBufSize)	\
    (This)->lpVtbl -> GetBeginAnchorBufLen(This,pBufSize)

#define IAAFHTMLClip_SetEndAnchor(This,pEndAnchor)	\
    (This)->lpVtbl -> SetEndAnchor(This,pEndAnchor)

#define IAAFHTMLClip_GetEndAnchor(This,pEndAnchor,bufSize)	\
    (This)->lpVtbl -> GetEndAnchor(This,pEndAnchor,bufSize)

#define IAAFHTMLClip_GetEndAnchorBufLen(This,pBufSize)	\
    (This)->lpVtbl -> GetEndAnchorBufLen(This,pBufSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFHTMLClip_SetBeginAnchor_Proxy( 
    IAAFHTMLClip * This,
    /* [string][in] */ aafCharacter_constptr pBeginAnchor);


void __RPC_STUB IAAFHTMLClip_SetBeginAnchor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFHTMLClip_GetBeginAnchor_Proxy( 
    IAAFHTMLClip * This,
    /* [size_is][string][out] */ aafCharacter *pBeginAnchor,
    /* [in] */ aafUInt32 bufSize);


void __RPC_STUB IAAFHTMLClip_GetBeginAnchor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFHTMLClip_GetBeginAnchorBufLen_Proxy( 
    IAAFHTMLClip * This,
    /* [out] */ aafUInt32 *pBufSize);


void __RPC_STUB IAAFHTMLClip_GetBeginAnchorBufLen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFHTMLClip_SetEndAnchor_Proxy( 
    IAAFHTMLClip * This,
    /* [string][in] */ aafCharacter_constptr pEndAnchor);


void __RPC_STUB IAAFHTMLClip_SetEndAnchor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFHTMLClip_GetEndAnchor_Proxy( 
    IAAFHTMLClip * This,
    /* [size_is][string][out] */ aafCharacter *pEndAnchor,
    /* [in] */ aafUInt32 bufSize);


void __RPC_STUB IAAFHTMLClip_GetEndAnchor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFHTMLClip_GetEndAnchorBufLen_Proxy( 
    IAAFHTMLClip * This,
    /* [out] */ aafUInt32 *pBufSize);


void __RPC_STUB IAAFHTMLClip_GetEndAnchorBufLen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFHTMLClip_INTERFACE_DEFINED__ */


#ifndef __IAAFHTMLDescriptor_INTERFACE_DEFINED__
#define __IAAFHTMLDescriptor_INTERFACE_DEFINED__

/* interface IAAFHTMLDescriptor */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFHTMLDescriptor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e684d770-b935-11d2-bf9d-00104bc9156d")
    IAAFHTMLDescriptor : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFHTMLDescriptorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFHTMLDescriptor * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFHTMLDescriptor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFHTMLDescriptor * This);
        
        END_INTERFACE
    } IAAFHTMLDescriptorVtbl;

    interface IAAFHTMLDescriptor
    {
        CONST_VTBL struct IAAFHTMLDescriptorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFHTMLDescriptor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFHTMLDescriptor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFHTMLDescriptor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFHTMLDescriptor_INTERFACE_DEFINED__ */


#ifndef __IAAFMetaDictionary_INTERFACE_DEFINED__
#define __IAAFMetaDictionary_INTERFACE_DEFINED__

/* interface IAAFMetaDictionary */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFMetaDictionary;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e1085eeb-0cc3-11d4-8014-00104bc9156d")
    IAAFMetaDictionary : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFMetaDictionaryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFMetaDictionary * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFMetaDictionary * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFMetaDictionary * This);
        
        END_INTERFACE
    } IAAFMetaDictionaryVtbl;

    interface IAAFMetaDictionary
    {
        CONST_VTBL struct IAAFMetaDictionaryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFMetaDictionary_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFMetaDictionary_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFMetaDictionary_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFMetaDictionary_INTERFACE_DEFINED__ */


#ifndef __IAAFPropValData_INTERFACE_DEFINED__
#define __IAAFPropValData_INTERFACE_DEFINED__

/* interface IAAFPropValData */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFPropValData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("106bb6e2-f0c1-11d2-842c-00600832acb8")
    IAAFPropValData : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFPropValDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFPropValData * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFPropValData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFPropValData * This);
        
        END_INTERFACE
    } IAAFPropValDataVtbl;

    interface IAAFPropValData
    {
        CONST_VTBL struct IAAFPropValDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFPropValData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFPropValData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFPropValData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFPropValData_INTERFACE_DEFINED__ */


#ifndef __IAAFStreamPropertyValue_INTERFACE_DEFINED__
#define __IAAFStreamPropertyValue_INTERFACE_DEFINED__

/* interface IAAFStreamPropertyValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFStreamPropertyValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F06bb6e2-f0c1-11d2-842c-00600832acb8")
    IAAFStreamPropertyValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFStreamPropertyValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFStreamPropertyValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFStreamPropertyValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFStreamPropertyValue * This);
        
        END_INTERFACE
    } IAAFStreamPropertyValueVtbl;

    interface IAAFStreamPropertyValue
    {
        CONST_VTBL struct IAAFStreamPropertyValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFStreamPropertyValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFStreamPropertyValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFStreamPropertyValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFStreamPropertyValue_INTERFACE_DEFINED__ */


#ifndef __IAAFStrongRefArrayValue_INTERFACE_DEFINED__
#define __IAAFStrongRefArrayValue_INTERFACE_DEFINED__

/* interface IAAFStrongRefArrayValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFStrongRefArrayValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f2973a82-6895-11d4-a812-8f71e157e3f3")
    IAAFStrongRefArrayValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFStrongRefArrayValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFStrongRefArrayValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFStrongRefArrayValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFStrongRefArrayValue * This);
        
        END_INTERFACE
    } IAAFStrongRefArrayValueVtbl;

    interface IAAFStrongRefArrayValue
    {
        CONST_VTBL struct IAAFStrongRefArrayValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFStrongRefArrayValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFStrongRefArrayValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFStrongRefArrayValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFStrongRefArrayValue_INTERFACE_DEFINED__ */


#ifndef __IAAFStrongRefSetValue_INTERFACE_DEFINED__
#define __IAAFStrongRefSetValue_INTERFACE_DEFINED__

/* interface IAAFStrongRefSetValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFStrongRefSetValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("84537782-6897-11d4-a812-8f71e157e3f3")
    IAAFStrongRefSetValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFStrongRefSetValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFStrongRefSetValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFStrongRefSetValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFStrongRefSetValue * This);
        
        END_INTERFACE
    } IAAFStrongRefSetValueVtbl;

    interface IAAFStrongRefSetValue
    {
        CONST_VTBL struct IAAFStrongRefSetValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFStrongRefSetValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFStrongRefSetValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFStrongRefSetValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFStrongRefSetValue_INTERFACE_DEFINED__ */


#ifndef __IAAFStrongRefValue_INTERFACE_DEFINED__
#define __IAAFStrongRefValue_INTERFACE_DEFINED__

/* interface IAAFStrongRefValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFStrongRefValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4142d582-6892-11d4-a812-8f71e157e3f3")
    IAAFStrongRefValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFStrongRefValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFStrongRefValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFStrongRefValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFStrongRefValue * This);
        
        END_INTERFACE
    } IAAFStrongRefValueVtbl;

    interface IAAFStrongRefValue
    {
        CONST_VTBL struct IAAFStrongRefValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFStrongRefValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFStrongRefValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFStrongRefValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFStrongRefValue_INTERFACE_DEFINED__ */


#ifndef __IAAFTextClip_INTERFACE_DEFINED__
#define __IAAFTextClip_INTERFACE_DEFINED__

/* interface IAAFTextClip */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFTextClip;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e684d786-b935-11d2-bf9d-00104bc9156d")
    IAAFTextClip : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFTextClipVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFTextClip * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFTextClip * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFTextClip * This);
        
        END_INTERFACE
    } IAAFTextClipVtbl;

    interface IAAFTextClip
    {
        CONST_VTBL struct IAAFTextClipVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFTextClip_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFTextClip_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFTextClip_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFTextClip_INTERFACE_DEFINED__ */


#ifndef __IAAFWeakRefArrayValue_INTERFACE_DEFINED__
#define __IAAFWeakRefArrayValue_INTERFACE_DEFINED__

/* interface IAAFWeakRefArrayValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFWeakRefArrayValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("02fbe502-6897-11d4-a812-8f71e157e3e3")
    IAAFWeakRefArrayValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFWeakRefArrayValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFWeakRefArrayValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFWeakRefArrayValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFWeakRefArrayValue * This);
        
        END_INTERFACE
    } IAAFWeakRefArrayValueVtbl;

    interface IAAFWeakRefArrayValue
    {
        CONST_VTBL struct IAAFWeakRefArrayValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFWeakRefArrayValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFWeakRefArrayValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFWeakRefArrayValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFWeakRefArrayValue_INTERFACE_DEFINED__ */


#ifndef __IAAFWeakRefSetValue_INTERFACE_DEFINED__
#define __IAAFWeakRefSetValue_INTERFACE_DEFINED__

/* interface IAAFWeakRefSetValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFWeakRefSetValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b1078f02-6897-11d4-a812-8f71e157e3e3")
    IAAFWeakRefSetValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFWeakRefSetValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFWeakRefSetValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFWeakRefSetValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFWeakRefSetValue * This);
        
        END_INTERFACE
    } IAAFWeakRefSetValueVtbl;

    interface IAAFWeakRefSetValue
    {
        CONST_VTBL struct IAAFWeakRefSetValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFWeakRefSetValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFWeakRefSetValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFWeakRefSetValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFWeakRefSetValue_INTERFACE_DEFINED__ */


#ifndef __IAAFWeakRefValue_INTERFACE_DEFINED__
#define __IAAFWeakRefValue_INTERFACE_DEFINED__

/* interface IAAFWeakRefValue */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFWeakRefValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("32a8ee02-6893-11d4-a812-8f71e157e3e3")
    IAAFWeakRefValue : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAAFWeakRefValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFWeakRefValue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFWeakRefValue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFWeakRefValue * This);
        
        END_INTERFACE
    } IAAFWeakRefValueVtbl;

    interface IAAFWeakRefValue
    {
        CONST_VTBL struct IAAFWeakRefValueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFWeakRefValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFWeakRefValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFWeakRefValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAAFWeakRefValue_INTERFACE_DEFINED__ */


#ifndef __IEnumAAFStorablePropVals_INTERFACE_DEFINED__
#define __IEnumAAFStorablePropVals_INTERFACE_DEFINED__

/* interface IEnumAAFStorablePropVals */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumAAFStorablePropVals;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5096c202-831b-11d4-a812-8a70df17ed53")
    IEnumAAFStorablePropVals : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IEnumAAFStorablePropValsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumAAFStorablePropVals * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumAAFStorablePropVals * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumAAFStorablePropVals * This);
        
        END_INTERFACE
    } IEnumAAFStorablePropValsVtbl;

    interface IEnumAAFStorablePropVals
    {
        CONST_VTBL struct IEnumAAFStorablePropValsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumAAFStorablePropVals_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumAAFStorablePropVals_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumAAFStorablePropVals_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEnumAAFStorablePropVals_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


