/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at TIMESTAMP REMOVED
 */
/* Compiler settings for ../ref-impl/include/com-api/AAFPlugin.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __AAFPlugin_h__
#define __AAFPlugin_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IAAFPlugin_FWD_DEFINED__
#define __IAAFPlugin_FWD_DEFINED__
typedef interface IAAFPlugin IAAFPlugin;
#endif 	/* __IAAFPlugin_FWD_DEFINED__ */


#ifndef __IAAFClassExtension_FWD_DEFINED__
#define __IAAFClassExtension_FWD_DEFINED__
typedef interface IAAFClassExtension IAAFClassExtension;
#endif 	/* __IAAFClassExtension_FWD_DEFINED__ */


#ifndef __IAAFEssenceStream_FWD_DEFINED__
#define __IAAFEssenceStream_FWD_DEFINED__
typedef interface IAAFEssenceStream IAAFEssenceStream;
#endif 	/* __IAAFEssenceStream_FWD_DEFINED__ */


#ifndef __IAAFEssenceDataStream_FWD_DEFINED__
#define __IAAFEssenceDataStream_FWD_DEFINED__
typedef interface IAAFEssenceDataStream IAAFEssenceDataStream;
#endif 	/* __IAAFEssenceDataStream_FWD_DEFINED__ */


#ifndef __IAAFEssenceCodec_FWD_DEFINED__
#define __IAAFEssenceCodec_FWD_DEFINED__
typedef interface IAAFEssenceCodec IAAFEssenceCodec;
#endif 	/* __IAAFEssenceCodec_FWD_DEFINED__ */


#ifndef __IAAFMultiEssenceCodec_FWD_DEFINED__
#define __IAAFMultiEssenceCodec_FWD_DEFINED__
typedef interface IAAFMultiEssenceCodec IAAFMultiEssenceCodec;
#endif 	/* __IAAFMultiEssenceCodec_FWD_DEFINED__ */


#ifndef __IAAFEssenceContainer_FWD_DEFINED__
#define __IAAFEssenceContainer_FWD_DEFINED__
typedef interface IAAFEssenceContainer IAAFEssenceContainer;
#endif 	/* __IAAFEssenceContainer_FWD_DEFINED__ */


#ifndef __IAAFInterpolator_FWD_DEFINED__
#define __IAAFInterpolator_FWD_DEFINED__
typedef interface IAAFInterpolator IAAFInterpolator;
#endif 	/* __IAAFInterpolator_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"
#include "AAF.h"
#include "AAFPluginTypes.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_AAFPlugin_0000 */
/* [local] */ 

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
//=--------------------------------------------------------------------------=
// AAF Interfaces.
//=--------------------------------------------------------------------------=
//










extern RPC_IF_HANDLE __MIDL_itf_AAFPlugin_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AAFPlugin_0000_v0_0_s_ifspec;

#ifndef __IAAFPlugin_INTERFACE_DEFINED__
#define __IAAFPlugin_INTERFACE_DEFINED__

/* interface IAAFPlugin */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3631F7A4-9121-11d2-8088-006008143e6f")
    IAAFPlugin : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CountDefinitions( 
            /* [out] */ aafUInt32 __RPC_FAR *pDefCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDefinitionID( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t __RPC_FAR *pPluginID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPluginDescriptorID( 
            /* [out] */ aafUID_t __RPC_FAR *pPluginID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDefinitionObject( 
            /* [in] */ aafUInt32 index,
            /* [in] */ IAAFDictionary __RPC_FAR *pDictionary,
            /* [out] */ IAAFDefObject __RPC_FAR *__RPC_FAR *pDefObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDescriptor( 
            /* [in] */ IAAFDictionary __RPC_FAR *pDictionary,
            /* [out] */ IAAFPluginDef __RPC_FAR *__RPC_FAR *pPluginDef) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFPlugin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFPlugin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFPlugin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CountDefinitions )( 
            IAAFPlugin __RPC_FAR * This,
            /* [out] */ aafUInt32 __RPC_FAR *pDefCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIndexedDefinitionID )( 
            IAAFPlugin __RPC_FAR * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t __RPC_FAR *pPluginID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPluginDescriptorID )( 
            IAAFPlugin __RPC_FAR * This,
            /* [out] */ aafUID_t __RPC_FAR *pPluginID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIndexedDefinitionObject )( 
            IAAFPlugin __RPC_FAR * This,
            /* [in] */ aafUInt32 index,
            /* [in] */ IAAFDictionary __RPC_FAR *pDictionary,
            /* [out] */ IAAFDefObject __RPC_FAR *__RPC_FAR *pDefObject);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDescriptor )( 
            IAAFPlugin __RPC_FAR * This,
            /* [in] */ IAAFDictionary __RPC_FAR *pDictionary,
            /* [out] */ IAAFPluginDef __RPC_FAR *__RPC_FAR *pPluginDef);
        
        END_INTERFACE
    } IAAFPluginVtbl;

    interface IAAFPlugin
    {
        CONST_VTBL struct IAAFPluginVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFPlugin_CountDefinitions(This,pDefCount)	\
    (This)->lpVtbl -> CountDefinitions(This,pDefCount)

#define IAAFPlugin_GetIndexedDefinitionID(This,index,pPluginID)	\
    (This)->lpVtbl -> GetIndexedDefinitionID(This,index,pPluginID)

#define IAAFPlugin_GetPluginDescriptorID(This,pPluginID)	\
    (This)->lpVtbl -> GetPluginDescriptorID(This,pPluginID)

#define IAAFPlugin_GetIndexedDefinitionObject(This,index,pDictionary,pDefObject)	\
    (This)->lpVtbl -> GetIndexedDefinitionObject(This,index,pDictionary,pDefObject)

#define IAAFPlugin_CreateDescriptor(This,pDictionary,pPluginDef)	\
    (This)->lpVtbl -> CreateDescriptor(This,pDictionary,pPluginDef)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFPlugin_CountDefinitions_Proxy( 
    IAAFPlugin __RPC_FAR * This,
    /* [out] */ aafUInt32 __RPC_FAR *pDefCount);


void __RPC_STUB IAAFPlugin_CountDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_GetIndexedDefinitionID_Proxy( 
    IAAFPlugin __RPC_FAR * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t __RPC_FAR *pPluginID);


void __RPC_STUB IAAFPlugin_GetIndexedDefinitionID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_GetPluginDescriptorID_Proxy( 
    IAAFPlugin __RPC_FAR * This,
    /* [out] */ aafUID_t __RPC_FAR *pPluginID);


void __RPC_STUB IAAFPlugin_GetPluginDescriptorID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_GetIndexedDefinitionObject_Proxy( 
    IAAFPlugin __RPC_FAR * This,
    /* [in] */ aafUInt32 index,
    /* [in] */ IAAFDictionary __RPC_FAR *pDictionary,
    /* [out] */ IAAFDefObject __RPC_FAR *__RPC_FAR *pDefObject);


void __RPC_STUB IAAFPlugin_GetIndexedDefinitionObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_CreateDescriptor_Proxy( 
    IAAFPlugin __RPC_FAR * This,
    /* [in] */ IAAFDictionary __RPC_FAR *pDictionary,
    /* [out] */ IAAFPluginDef __RPC_FAR *__RPC_FAR *pPluginDef);


void __RPC_STUB IAAFPlugin_CreateDescriptor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFPlugin_INTERFACE_DEFINED__ */


#ifndef __IAAFClassExtension_INTERFACE_DEFINED__
#define __IAAFClassExtension_INTERFACE_DEFINED__

/* interface IAAFClassExtension */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFClassExtension;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65C07344-DF36-11d3-8008-00104bc9156d")
    IAAFClassExtension : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterDefinitions( 
            /* [in] */ IAAFDictionary __RPC_FAR *pDictionary) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFClassExtensionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFClassExtension __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFClassExtension __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFClassExtension __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterDefinitions )( 
            IAAFClassExtension __RPC_FAR * This,
            /* [in] */ IAAFDictionary __RPC_FAR *pDictionary);
        
        END_INTERFACE
    } IAAFClassExtensionVtbl;

    interface IAAFClassExtension
    {
        CONST_VTBL struct IAAFClassExtensionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFClassExtension_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFClassExtension_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFClassExtension_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFClassExtension_RegisterDefinitions(This,pDictionary)	\
    (This)->lpVtbl -> RegisterDefinitions(This,pDictionary)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFClassExtension_RegisterDefinitions_Proxy( 
    IAAFClassExtension __RPC_FAR * This,
    /* [in] */ IAAFDictionary __RPC_FAR *pDictionary);


void __RPC_STUB IAAFClassExtension_RegisterDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFClassExtension_INTERFACE_DEFINED__ */


#ifndef __IAAFEssenceStream_INTERFACE_DEFINED__
#define __IAAFEssenceStream_INTERFACE_DEFINED__

/* interface IAAFEssenceStream */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFEssenceStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("83402902-9146-11d2-8088-006008143e6f")
    IAAFEssenceStream : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ aafUInt32 bytes,
            /* [size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *bytesWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t pBuffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *pBytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ aafPosition_t byteOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SeekRelative( 
            /* [in] */ aafInt32 byteOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ aafPosition_t __RPC_FAR *pPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLength( 
            /* [out] */ aafLength_t __RPC_FAR *pLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FlushCache( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCacheSize( 
            /* [in] */ aafUInt32 itsSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFEssenceStream __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFEssenceStream __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [in] */ aafUInt32 bytes,
            /* [size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *bytesWritten);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t pBuffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *pBytesRead);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Seek )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [in] */ aafPosition_t byteOffset);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SeekRelative )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [in] */ aafInt32 byteOffset);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPosition )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [out] */ aafPosition_t __RPC_FAR *pPosition);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLength )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [out] */ aafLength_t __RPC_FAR *pLength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FlushCache )( 
            IAAFEssenceStream __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCacheSize )( 
            IAAFEssenceStream __RPC_FAR * This,
            /* [in] */ aafUInt32 itsSize);
        
        END_INTERFACE
    } IAAFEssenceStreamVtbl;

    interface IAAFEssenceStream
    {
        CONST_VTBL struct IAAFEssenceStreamVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFEssenceStream_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFEssenceStream_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFEssenceStream_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFEssenceStream_Write(This,bytes,buffer,bytesWritten)	\
    (This)->lpVtbl -> Write(This,bytes,buffer,bytesWritten)

#define IAAFEssenceStream_Read(This,buflen,pBuffer,pBytesRead)	\
    (This)->lpVtbl -> Read(This,buflen,pBuffer,pBytesRead)

#define IAAFEssenceStream_Seek(This,byteOffset)	\
    (This)->lpVtbl -> Seek(This,byteOffset)

#define IAAFEssenceStream_SeekRelative(This,byteOffset)	\
    (This)->lpVtbl -> SeekRelative(This,byteOffset)

#define IAAFEssenceStream_GetPosition(This,pPosition)	\
    (This)->lpVtbl -> GetPosition(This,pPosition)

#define IAAFEssenceStream_GetLength(This,pLength)	\
    (This)->lpVtbl -> GetLength(This,pLength)

#define IAAFEssenceStream_FlushCache(This)	\
    (This)->lpVtbl -> FlushCache(This)

#define IAAFEssenceStream_SetCacheSize(This,itsSize)	\
    (This)->lpVtbl -> SetCacheSize(This,itsSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFEssenceStream_Write_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [in] */ aafUInt32 bytes,
    /* [size_is][out] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 __RPC_FAR *bytesWritten);


void __RPC_STUB IAAFEssenceStream_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_Read_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [in] */ aafUInt32 buflen,
    /* [length_is][size_is][out] */ aafDataBuffer_t pBuffer,
    /* [ref][out] */ aafUInt32 __RPC_FAR *pBytesRead);


void __RPC_STUB IAAFEssenceStream_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_Seek_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [in] */ aafPosition_t byteOffset);


void __RPC_STUB IAAFEssenceStream_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_SeekRelative_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [in] */ aafInt32 byteOffset);


void __RPC_STUB IAAFEssenceStream_SeekRelative_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_GetPosition_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [out] */ aafPosition_t __RPC_FAR *pPosition);


void __RPC_STUB IAAFEssenceStream_GetPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_GetLength_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [out] */ aafLength_t __RPC_FAR *pLength);


void __RPC_STUB IAAFEssenceStream_GetLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_FlushCache_Proxy( 
    IAAFEssenceStream __RPC_FAR * This);


void __RPC_STUB IAAFEssenceStream_FlushCache_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_SetCacheSize_Proxy( 
    IAAFEssenceStream __RPC_FAR * This,
    /* [in] */ aafUInt32 itsSize);


void __RPC_STUB IAAFEssenceStream_SetCacheSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceStream_INTERFACE_DEFINED__ */


#ifndef __IAAFEssenceDataStream_INTERFACE_DEFINED__
#define __IAAFEssenceDataStream_INTERFACE_DEFINED__

/* interface IAAFEssenceDataStream */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFEssenceDataStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDDB6AB1-98DC-11d2-808a-006008143e6f")
    IAAFEssenceDataStream : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IUnknown __RPC_FAR *essenceData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceDataStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFEssenceDataStream __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFEssenceDataStream __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFEssenceDataStream __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IAAFEssenceDataStream __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *essenceData);
        
        END_INTERFACE
    } IAAFEssenceDataStreamVtbl;

    interface IAAFEssenceDataStream
    {
        CONST_VTBL struct IAAFEssenceDataStreamVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFEssenceDataStream_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFEssenceDataStream_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFEssenceDataStream_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFEssenceDataStream_Init(This,essenceData)	\
    (This)->lpVtbl -> Init(This,essenceData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFEssenceDataStream_Init_Proxy( 
    IAAFEssenceDataStream __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *essenceData);


void __RPC_STUB IAAFEssenceDataStream_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceDataStream_INTERFACE_DEFINED__ */


#ifndef __IAAFEssenceCodec_INTERFACE_DEFINED__
#define __IAAFEssenceCodec_INTERFACE_DEFINED__

/* interface IAAFEssenceCodec */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFEssenceCodec;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3631F7A2-9121-11d2-8088-006008143e6f")
    IAAFEssenceCodec : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetEssenceAccess( 
            /* [in] */ IAAFEssenceAccess __RPC_FAR *pEssenceAccess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountFlavours( 
            /* [out] */ aafUInt32 __RPC_FAR *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedFlavourID( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t __RPC_FAR *pVariant) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountDataDefinitions( 
            /* [out] */ aafUInt32 __RPC_FAR *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDataDefinition( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t __RPC_FAR *pDataDefID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxCodecDisplayNameLength( 
            /* [out] */ aafUInt32 __RPC_FAR *pBufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCodecDisplayName( 
            /* [ref][in] */ aafUID_constref flavour,
            /* [size_is][string][out] */ aafCharacter __RPC_FAR *pName,
            /* [in] */ aafUInt32 bufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountChannels( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [out] */ aafUInt16 __RPC_FAR *pNumChannels) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectInfo( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [out] */ aafSelectInfo_t __RPC_FAR *pSelectInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateEssence( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCheckVerbose_t verbose,
            /* [in] */ aafCheckWarnings_t outputWarnings,
            /* [in] */ aafUInt32 bufSize,
            /* [length_is][size_is][out] */ aafCharacter __RPC_FAR *pErrorText,
            /* [out] */ aafUInt32 __RPC_FAR *pBytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [ref][in] */ aafRational_constref sampleRate,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountSamples( 
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [out] */ aafLength_t __RPC_FAR *pNumSamples) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSamples( 
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [size_is][in] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *samplesWritten,
            /* [ref][out] */ aafUInt32 __RPC_FAR *bytesWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadSamples( 
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *samplesRead,
            /* [ref][out] */ aafUInt32 __RPC_FAR *bytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ aafPosition_t sampleFrame) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompleteWrite( 
            /* [in] */ IAAFSourceMob __RPC_FAR *pFileMob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDescriptorFromStream( 
            /* [in] */ IAAFEssenceStream __RPC_FAR *pStream,
            /* [in] */ IAAFSourceMob __RPC_FAR *pSourceMob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentEssenceStream( 
            /* [out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutEssenceFormat( 
            /* [in] */ IAAFEssenceFormat __RPC_FAR *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceFormat( 
            /* [in] */ IAAFEssenceFormat __RPC_FAR *pFormatTemplate,
            /* [out] */ IAAFEssenceFormat __RPC_FAR *__RPC_FAR *ppNewFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultEssenceFormat( 
            /* [out] */ IAAFEssenceFormat __RPC_FAR *__RPC_FAR *ppNewFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceDescriptorID( 
            /* [out] */ aafUID_t __RPC_FAR *pDescriptorID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceDataID( 
            /* [out] */ aafUID_t __RPC_FAR *pEssenceDataID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedSampleSize( 
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [in] */ aafPosition_t sampleOffset,
            /* [out] */ aafLength_t __RPC_FAR *pLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLargestSampleSize( 
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [out] */ aafLength_t __RPC_FAR *pLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceCodecVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFEssenceCodec __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFEssenceCodec __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEssenceAccess )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFEssenceAccess __RPC_FAR *pEssenceAccess);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CountFlavours )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ aafUInt32 __RPC_FAR *pCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIndexedFlavourID )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t __RPC_FAR *pVariant);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CountDataDefinitions )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ aafUInt32 __RPC_FAR *pCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIndexedDataDefinition )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t __RPC_FAR *pDataDefID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMaxCodecDisplayNameLength )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ aafUInt32 __RPC_FAR *pBufSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCodecDisplayName )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [ref][in] */ aafUID_constref flavour,
            /* [size_is][string][out] */ aafCharacter __RPC_FAR *pName,
            /* [in] */ aafUInt32 bufSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CountChannels )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [out] */ aafUInt16 __RPC_FAR *pNumChannels);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelectInfo )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [out] */ aafSelectInfo_t __RPC_FAR *pSelectInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateEssence )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCheckVerbose_t verbose,
            /* [in] */ aafCheckWarnings_t outputWarnings,
            /* [in] */ aafUInt32 bufSize,
            /* [length_is][size_is][out] */ aafCharacter __RPC_FAR *pErrorText,
            /* [out] */ aafUInt32 __RPC_FAR *pBytesRead);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Create )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [ref][in] */ aafRational_constref sampleRate,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CountSamples )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [out] */ aafLength_t __RPC_FAR *pNumSamples);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteSamples )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [size_is][in] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *samplesWritten,
            /* [ref][out] */ aafUInt32 __RPC_FAR *bytesWritten);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadSamples )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 __RPC_FAR *samplesRead,
            /* [ref][out] */ aafUInt32 __RPC_FAR *bytesRead);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Seek )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ aafPosition_t sampleFrame);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompleteWrite )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *pFileMob);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDescriptorFromStream )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFEssenceStream __RPC_FAR *pStream,
            /* [in] */ IAAFSourceMob __RPC_FAR *pSourceMob);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentEssenceStream )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutEssenceFormat )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFEssenceFormat __RPC_FAR *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEssenceFormat )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFEssenceFormat __RPC_FAR *pFormatTemplate,
            /* [out] */ IAAFEssenceFormat __RPC_FAR *__RPC_FAR *ppNewFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultEssenceFormat )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ IAAFEssenceFormat __RPC_FAR *__RPC_FAR *ppNewFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEssenceDescriptorID )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ aafUID_t __RPC_FAR *pDescriptorID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEssenceDataID )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [out] */ aafUID_t __RPC_FAR *pEssenceDataID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIndexedSampleSize )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [in] */ aafPosition_t sampleOffset,
            /* [out] */ aafLength_t __RPC_FAR *pLength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLargestSampleSize )( 
            IAAFEssenceCodec __RPC_FAR * This,
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [out] */ aafLength_t __RPC_FAR *pLength);
        
        END_INTERFACE
    } IAAFEssenceCodecVtbl;

    interface IAAFEssenceCodec
    {
        CONST_VTBL struct IAAFEssenceCodecVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFEssenceCodec_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFEssenceCodec_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFEssenceCodec_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFEssenceCodec_SetEssenceAccess(This,pEssenceAccess)	\
    (This)->lpVtbl -> SetEssenceAccess(This,pEssenceAccess)

#define IAAFEssenceCodec_CountFlavours(This,pCount)	\
    (This)->lpVtbl -> CountFlavours(This,pCount)

#define IAAFEssenceCodec_GetIndexedFlavourID(This,index,pVariant)	\
    (This)->lpVtbl -> GetIndexedFlavourID(This,index,pVariant)

#define IAAFEssenceCodec_CountDataDefinitions(This,pCount)	\
    (This)->lpVtbl -> CountDataDefinitions(This,pCount)

#define IAAFEssenceCodec_GetIndexedDataDefinition(This,index,pDataDefID)	\
    (This)->lpVtbl -> GetIndexedDataDefinition(This,index,pDataDefID)

#define IAAFEssenceCodec_GetMaxCodecDisplayNameLength(This,pBufSize)	\
    (This)->lpVtbl -> GetMaxCodecDisplayNameLength(This,pBufSize)

#define IAAFEssenceCodec_GetCodecDisplayName(This,flavour,pName,bufSize)	\
    (This)->lpVtbl -> GetCodecDisplayName(This,flavour,pName,bufSize)

#define IAAFEssenceCodec_CountChannels(This,fileMob,essenceKind,stream,pNumChannels)	\
    (This)->lpVtbl -> CountChannels(This,fileMob,essenceKind,stream,pNumChannels)

#define IAAFEssenceCodec_GetSelectInfo(This,fileMob,stream,pSelectInfo)	\
    (This)->lpVtbl -> GetSelectInfo(This,fileMob,stream,pSelectInfo)

#define IAAFEssenceCodec_ValidateEssence(This,fileMob,stream,verbose,outputWarnings,bufSize,pErrorText,pBytesRead)	\
    (This)->lpVtbl -> ValidateEssence(This,fileMob,stream,verbose,outputWarnings,bufSize,pErrorText,pBytesRead)

#define IAAFEssenceCodec_Create(This,fileMob,flavour,essenceKind,sampleRate,stream,compEnable)	\
    (This)->lpVtbl -> Create(This,fileMob,flavour,essenceKind,sampleRate,stream,compEnable)

#define IAAFEssenceCodec_Open(This,fileMob,openMode,stream,compEnable)	\
    (This)->lpVtbl -> Open(This,fileMob,openMode,stream,compEnable)

#define IAAFEssenceCodec_CountSamples(This,essenceKind,pNumSamples)	\
    (This)->lpVtbl -> CountSamples(This,essenceKind,pNumSamples)

#define IAAFEssenceCodec_WriteSamples(This,nSamples,buflen,buffer,samplesWritten,bytesWritten)	\
    (This)->lpVtbl -> WriteSamples(This,nSamples,buflen,buffer,samplesWritten,bytesWritten)

#define IAAFEssenceCodec_ReadSamples(This,nSamples,buflen,buffer,samplesRead,bytesRead)	\
    (This)->lpVtbl -> ReadSamples(This,nSamples,buflen,buffer,samplesRead,bytesRead)

#define IAAFEssenceCodec_Seek(This,sampleFrame)	\
    (This)->lpVtbl -> Seek(This,sampleFrame)

#define IAAFEssenceCodec_CompleteWrite(This,pFileMob)	\
    (This)->lpVtbl -> CompleteWrite(This,pFileMob)

#define IAAFEssenceCodec_CreateDescriptorFromStream(This,pStream,pSourceMob)	\
    (This)->lpVtbl -> CreateDescriptorFromStream(This,pStream,pSourceMob)

#define IAAFEssenceCodec_GetCurrentEssenceStream(This,ppStream)	\
    (This)->lpVtbl -> GetCurrentEssenceStream(This,ppStream)

#define IAAFEssenceCodec_PutEssenceFormat(This,pFormat)	\
    (This)->lpVtbl -> PutEssenceFormat(This,pFormat)

#define IAAFEssenceCodec_GetEssenceFormat(This,pFormatTemplate,ppNewFormat)	\
    (This)->lpVtbl -> GetEssenceFormat(This,pFormatTemplate,ppNewFormat)

#define IAAFEssenceCodec_GetDefaultEssenceFormat(This,ppNewFormat)	\
    (This)->lpVtbl -> GetDefaultEssenceFormat(This,ppNewFormat)

#define IAAFEssenceCodec_GetEssenceDescriptorID(This,pDescriptorID)	\
    (This)->lpVtbl -> GetEssenceDescriptorID(This,pDescriptorID)

#define IAAFEssenceCodec_GetEssenceDataID(This,pEssenceDataID)	\
    (This)->lpVtbl -> GetEssenceDataID(This,pEssenceDataID)

#define IAAFEssenceCodec_GetIndexedSampleSize(This,essenceDefID,sampleOffset,pLength)	\
    (This)->lpVtbl -> GetIndexedSampleSize(This,essenceDefID,sampleOffset,pLength)

#define IAAFEssenceCodec_GetLargestSampleSize(This,essenceDefID,pLength)	\
    (This)->lpVtbl -> GetLargestSampleSize(This,essenceDefID,pLength)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_SetEssenceAccess_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFEssenceAccess __RPC_FAR *pEssenceAccess);


void __RPC_STUB IAAFEssenceCodec_SetEssenceAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountFlavours_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ aafUInt32 __RPC_FAR *pCount);


void __RPC_STUB IAAFEssenceCodec_CountFlavours_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetIndexedFlavourID_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t __RPC_FAR *pVariant);


void __RPC_STUB IAAFEssenceCodec_GetIndexedFlavourID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountDataDefinitions_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ aafUInt32 __RPC_FAR *pCount);


void __RPC_STUB IAAFEssenceCodec_CountDataDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetIndexedDataDefinition_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t __RPC_FAR *pDataDefID);


void __RPC_STUB IAAFEssenceCodec_GetIndexedDataDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetMaxCodecDisplayNameLength_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ aafUInt32 __RPC_FAR *pBufSize);


void __RPC_STUB IAAFEssenceCodec_GetMaxCodecDisplayNameLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetCodecDisplayName_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [ref][in] */ aafUID_constref flavour,
    /* [size_is][string][out] */ aafCharacter __RPC_FAR *pName,
    /* [in] */ aafUInt32 bufSize);


void __RPC_STUB IAAFEssenceCodec_GetCodecDisplayName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountChannels_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [out] */ aafUInt16 __RPC_FAR *pNumChannels);


void __RPC_STUB IAAFEssenceCodec_CountChannels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetSelectInfo_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [out] */ aafSelectInfo_t __RPC_FAR *pSelectInfo);


void __RPC_STUB IAAFEssenceCodec_GetSelectInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_ValidateEssence_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [in] */ aafCheckVerbose_t verbose,
    /* [in] */ aafCheckWarnings_t outputWarnings,
    /* [in] */ aafUInt32 bufSize,
    /* [length_is][size_is][out] */ aafCharacter __RPC_FAR *pErrorText,
    /* [out] */ aafUInt32 __RPC_FAR *pBytesRead);


void __RPC_STUB IAAFEssenceCodec_ValidateEssence_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_Create_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [ref][in] */ aafUID_constref flavour,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [ref][in] */ aafRational_constref sampleRate,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFEssenceCodec_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_Open_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [in] */ aafMediaOpenMode_t openMode,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFEssenceCodec_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountSamples_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [out] */ aafLength_t __RPC_FAR *pNumSamples);


void __RPC_STUB IAAFEssenceCodec_CountSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_WriteSamples_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ aafUInt32 nSamples,
    /* [in] */ aafUInt32 buflen,
    /* [size_is][in] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 __RPC_FAR *samplesWritten,
    /* [ref][out] */ aafUInt32 __RPC_FAR *bytesWritten);


void __RPC_STUB IAAFEssenceCodec_WriteSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_ReadSamples_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ aafUInt32 nSamples,
    /* [in] */ aafUInt32 buflen,
    /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 __RPC_FAR *samplesRead,
    /* [ref][out] */ aafUInt32 __RPC_FAR *bytesRead);


void __RPC_STUB IAAFEssenceCodec_ReadSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_Seek_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ aafPosition_t sampleFrame);


void __RPC_STUB IAAFEssenceCodec_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CompleteWrite_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *pFileMob);


void __RPC_STUB IAAFEssenceCodec_CompleteWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CreateDescriptorFromStream_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFEssenceStream __RPC_FAR *pStream,
    /* [in] */ IAAFSourceMob __RPC_FAR *pSourceMob);


void __RPC_STUB IAAFEssenceCodec_CreateDescriptorFromStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetCurrentEssenceStream_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppStream);


void __RPC_STUB IAAFEssenceCodec_GetCurrentEssenceStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_PutEssenceFormat_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFEssenceFormat __RPC_FAR *pFormat);


void __RPC_STUB IAAFEssenceCodec_PutEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetEssenceFormat_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFEssenceFormat __RPC_FAR *pFormatTemplate,
    /* [out] */ IAAFEssenceFormat __RPC_FAR *__RPC_FAR *ppNewFormat);


void __RPC_STUB IAAFEssenceCodec_GetEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetDefaultEssenceFormat_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ IAAFEssenceFormat __RPC_FAR *__RPC_FAR *ppNewFormat);


void __RPC_STUB IAAFEssenceCodec_GetDefaultEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetEssenceDescriptorID_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ aafUID_t __RPC_FAR *pDescriptorID);


void __RPC_STUB IAAFEssenceCodec_GetEssenceDescriptorID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetEssenceDataID_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [out] */ aafUID_t __RPC_FAR *pEssenceDataID);


void __RPC_STUB IAAFEssenceCodec_GetEssenceDataID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetIndexedSampleSize_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [ref][in] */ aafUID_constref essenceDefID,
    /* [in] */ aafPosition_t sampleOffset,
    /* [out] */ aafLength_t __RPC_FAR *pLength);


void __RPC_STUB IAAFEssenceCodec_GetIndexedSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetLargestSampleSize_Proxy( 
    IAAFEssenceCodec __RPC_FAR * This,
    /* [ref][in] */ aafUID_constref essenceDefID,
    /* [out] */ aafLength_t __RPC_FAR *pLength);


void __RPC_STUB IAAFEssenceCodec_GetLargestSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceCodec_INTERFACE_DEFINED__ */


#ifndef __IAAFMultiEssenceCodec_INTERFACE_DEFINED__
#define __IAAFMultiEssenceCodec_INTERFACE_DEFINED__

/* interface IAAFMultiEssenceCodec */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFMultiEssenceCodec;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7F93992D-DDA3-11d3-8008-00104bc9156d")
    IAAFMultiEssenceCodec : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MultiCreate( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable,
            /* [in] */ aafUInt32 numParms,
            /* [size_is][in] */ aafmMultiCreate_t __RPC_FAR *createParms) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MultiOpen( 
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteBlocks( 
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][in] */ aafmMultiXfer_t __RPC_FAR *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t __RPC_FAR *pResultParm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadBlocks( 
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][out] */ aafmMultiXfer_t __RPC_FAR *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t __RPC_FAR *pResultParm) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFMultiEssenceCodecVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFMultiEssenceCodec __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFMultiEssenceCodec __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFMultiEssenceCodec __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiCreate )( 
            IAAFMultiEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable,
            /* [in] */ aafUInt32 numParms,
            /* [size_is][in] */ aafmMultiCreate_t __RPC_FAR *createParms);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiOpen )( 
            IAAFMultiEssenceCodec __RPC_FAR * This,
            /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteBlocks )( 
            IAAFMultiEssenceCodec __RPC_FAR * This,
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][in] */ aafmMultiXfer_t __RPC_FAR *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t __RPC_FAR *pResultParm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadBlocks )( 
            IAAFMultiEssenceCodec __RPC_FAR * This,
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][out] */ aafmMultiXfer_t __RPC_FAR *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t __RPC_FAR *pResultParm);
        
        END_INTERFACE
    } IAAFMultiEssenceCodecVtbl;

    interface IAAFMultiEssenceCodec
    {
        CONST_VTBL struct IAAFMultiEssenceCodecVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFMultiEssenceCodec_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFMultiEssenceCodec_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFMultiEssenceCodec_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFMultiEssenceCodec_MultiCreate(This,fileMob,flavour,stream,compEnable,numParms,createParms)	\
    (This)->lpVtbl -> MultiCreate(This,fileMob,flavour,stream,compEnable,numParms,createParms)

#define IAAFMultiEssenceCodec_MultiOpen(This,fileMob,openMode,stream,compEnable)	\
    (This)->lpVtbl -> MultiOpen(This,fileMob,openMode,stream,compEnable)

#define IAAFMultiEssenceCodec_WriteBlocks(This,inter,xferBlockCount,pTransferParm,pResultParm)	\
    (This)->lpVtbl -> WriteBlocks(This,inter,xferBlockCount,pTransferParm,pResultParm)

#define IAAFMultiEssenceCodec_ReadBlocks(This,inter,xferBlockCount,pTransferParm,pResultParm)	\
    (This)->lpVtbl -> ReadBlocks(This,inter,xferBlockCount,pTransferParm,pResultParm)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_MultiCreate_Proxy( 
    IAAFMultiEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [ref][in] */ aafUID_constref flavour,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [in] */ aafCompressEnable_t compEnable,
    /* [in] */ aafUInt32 numParms,
    /* [size_is][in] */ aafmMultiCreate_t __RPC_FAR *createParms);


void __RPC_STUB IAAFMultiEssenceCodec_MultiCreate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_MultiOpen_Proxy( 
    IAAFMultiEssenceCodec __RPC_FAR * This,
    /* [in] */ IAAFSourceMob __RPC_FAR *fileMob,
    /* [in] */ aafMediaOpenMode_t openMode,
    /* [in] */ IAAFEssenceStream __RPC_FAR *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFMultiEssenceCodec_MultiOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_WriteBlocks_Proxy( 
    IAAFMultiEssenceCodec __RPC_FAR * This,
    /* [in] */ aafDeinterleave_t inter,
    /* [in] */ aafUInt16 xferBlockCount,
    /* [size_is][in] */ aafmMultiXfer_t __RPC_FAR *pTransferParm,
    /* [size_is][out] */ aafmMultiResult_t __RPC_FAR *pResultParm);


void __RPC_STUB IAAFMultiEssenceCodec_WriteBlocks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_ReadBlocks_Proxy( 
    IAAFMultiEssenceCodec __RPC_FAR * This,
    /* [in] */ aafDeinterleave_t inter,
    /* [in] */ aafUInt16 xferBlockCount,
    /* [size_is][out] */ aafmMultiXfer_t __RPC_FAR *pTransferParm,
    /* [size_is][out] */ aafmMultiResult_t __RPC_FAR *pResultParm);


void __RPC_STUB IAAFMultiEssenceCodec_ReadBlocks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFMultiEssenceCodec_INTERFACE_DEFINED__ */


#ifndef __IAAFEssenceContainer_INTERFACE_DEFINED__
#define __IAAFEssenceContainer_INTERFACE_DEFINED__

/* interface IAAFEssenceContainer */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFEssenceContainer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a7337031-c103-11d2-808a-006008143e6f")
    IAAFEssenceContainer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateEssenceStream( 
            /* [string][in] */ aafCharacter_constptr pName,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEssenceStreamWriteOnly( 
            /* [string][in] */ aafCharacter_constptr pPath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenEssenceStreamReadOnly( 
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenEssenceStreamAppend( 
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceContainerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFEssenceContainer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFEssenceContainer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFEssenceContainer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEssenceStream )( 
            IAAFEssenceContainer __RPC_FAR * This,
            /* [string][in] */ aafCharacter_constptr pName,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEssenceStreamWriteOnly )( 
            IAAFEssenceContainer __RPC_FAR * This,
            /* [string][in] */ aafCharacter_constptr pPath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenEssenceStreamReadOnly )( 
            IAAFEssenceContainer __RPC_FAR * This,
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenEssenceStreamAppend )( 
            IAAFEssenceContainer __RPC_FAR * This,
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);
        
        END_INTERFACE
    } IAAFEssenceContainerVtbl;

    interface IAAFEssenceContainer
    {
        CONST_VTBL struct IAAFEssenceContainerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFEssenceContainer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFEssenceContainer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFEssenceContainer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFEssenceContainer_CreateEssenceStream(This,pName,pMobID,ppEssenceStream)	\
    (This)->lpVtbl -> CreateEssenceStream(This,pName,pMobID,ppEssenceStream)

#define IAAFEssenceContainer_CreateEssenceStreamWriteOnly(This,pPath,pMobID,ppEssenceStream)	\
    (This)->lpVtbl -> CreateEssenceStreamWriteOnly(This,pPath,pMobID,ppEssenceStream)

#define IAAFEssenceContainer_OpenEssenceStreamReadOnly(This,pFilePath,pMobID,ppEssenceStream)	\
    (This)->lpVtbl -> OpenEssenceStreamReadOnly(This,pFilePath,pMobID,ppEssenceStream)

#define IAAFEssenceContainer_OpenEssenceStreamAppend(This,pFilePath,pMobID,ppEssenceStream)	\
    (This)->lpVtbl -> OpenEssenceStreamAppend(This,pFilePath,pMobID,ppEssenceStream)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_CreateEssenceStream_Proxy( 
    IAAFEssenceContainer __RPC_FAR * This,
    /* [string][in] */ aafCharacter_constptr pName,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_CreateEssenceStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_CreateEssenceStreamWriteOnly_Proxy( 
    IAAFEssenceContainer __RPC_FAR * This,
    /* [string][in] */ aafCharacter_constptr pPath,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_CreateEssenceStreamWriteOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_OpenEssenceStreamReadOnly_Proxy( 
    IAAFEssenceContainer __RPC_FAR * This,
    /* [string][in] */ aafCharacter_constptr pFilePath,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_OpenEssenceStreamReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_OpenEssenceStreamAppend_Proxy( 
    IAAFEssenceContainer __RPC_FAR * This,
    /* [string][in] */ aafCharacter_constptr pFilePath,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream __RPC_FAR *__RPC_FAR *ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_OpenEssenceStreamAppend_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceContainer_INTERFACE_DEFINED__ */


#ifndef __IAAFInterpolator_INTERFACE_DEFINED__
#define __IAAFInterpolator_INTERFACE_DEFINED__

/* interface IAAFInterpolator */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFInterpolator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("75C6CDF2-0D67-11d3-80A9-006008143e6f")
    IAAFInterpolator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNumTypesSupported( 
            /* [out] */ aafUInt32 __RPC_FAR *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedSupportedType( 
            /* [in] */ aafUInt32 index,
            /* [out] */ IAAFTypeDef __RPC_FAR *__RPC_FAR *ppType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeDefinition( 
            /* [out] */ IAAFTypeDef __RPC_FAR *__RPC_FAR *ppTypeDef) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTypeDefinition( 
            /* [in] */ IAAFTypeDef __RPC_FAR *pTypeDef) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParameter( 
            /* [out] */ IAAFParameter __RPC_FAR *__RPC_FAR *ppParameter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetParameter( 
            /* [in] */ IAAFParameter __RPC_FAR *pParameter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InterpolateOne( 
            /* [in] */ aafRational_t __RPC_FAR *pInputValue,
            /* [in] */ aafUInt32 valueSize,
            /* [length_is][size_is][out] */ aafDataBuffer_t pValue,
            /* [out] */ aafUInt32 __RPC_FAR *bytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InterpolateMany( 
            /* [in] */ aafRational_t __RPC_FAR *pStartInputValue,
            /* [in] */ aafRational_t __RPC_FAR *pInputStep,
            /* [in] */ aafUInt32 generateCount,
            /* [out] */ aafMemPtr_t pOutputValue,
            /* [out] */ aafUInt32 __RPC_FAR *pResultCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFInterpolatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAAFInterpolator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAAFInterpolator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumTypesSupported )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [out] */ aafUInt32 __RPC_FAR *pCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIndexedSupportedType )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ IAAFTypeDef __RPC_FAR *__RPC_FAR *ppType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeDefinition )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [out] */ IAAFTypeDef __RPC_FAR *__RPC_FAR *ppTypeDef);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTypeDefinition )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [in] */ IAAFTypeDef __RPC_FAR *pTypeDef);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParameter )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [out] */ IAAFParameter __RPC_FAR *__RPC_FAR *ppParameter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetParameter )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [in] */ IAAFParameter __RPC_FAR *pParameter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InterpolateOne )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [in] */ aafRational_t __RPC_FAR *pInputValue,
            /* [in] */ aafUInt32 valueSize,
            /* [length_is][size_is][out] */ aafDataBuffer_t pValue,
            /* [out] */ aafUInt32 __RPC_FAR *bytesRead);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InterpolateMany )( 
            IAAFInterpolator __RPC_FAR * This,
            /* [in] */ aafRational_t __RPC_FAR *pStartInputValue,
            /* [in] */ aafRational_t __RPC_FAR *pInputStep,
            /* [in] */ aafUInt32 generateCount,
            /* [out] */ aafMemPtr_t pOutputValue,
            /* [out] */ aafUInt32 __RPC_FAR *pResultCount);
        
        END_INTERFACE
    } IAAFInterpolatorVtbl;

    interface IAAFInterpolator
    {
        CONST_VTBL struct IAAFInterpolatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFInterpolator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFInterpolator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFInterpolator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFInterpolator_GetNumTypesSupported(This,pCount)	\
    (This)->lpVtbl -> GetNumTypesSupported(This,pCount)

#define IAAFInterpolator_GetIndexedSupportedType(This,index,ppType)	\
    (This)->lpVtbl -> GetIndexedSupportedType(This,index,ppType)

#define IAAFInterpolator_GetTypeDefinition(This,ppTypeDef)	\
    (This)->lpVtbl -> GetTypeDefinition(This,ppTypeDef)

#define IAAFInterpolator_SetTypeDefinition(This,pTypeDef)	\
    (This)->lpVtbl -> SetTypeDefinition(This,pTypeDef)

#define IAAFInterpolator_GetParameter(This,ppParameter)	\
    (This)->lpVtbl -> GetParameter(This,ppParameter)

#define IAAFInterpolator_SetParameter(This,pParameter)	\
    (This)->lpVtbl -> SetParameter(This,pParameter)

#define IAAFInterpolator_InterpolateOne(This,pInputValue,valueSize,pValue,bytesRead)	\
    (This)->lpVtbl -> InterpolateOne(This,pInputValue,valueSize,pValue,bytesRead)

#define IAAFInterpolator_InterpolateMany(This,pStartInputValue,pInputStep,generateCount,pOutputValue,pResultCount)	\
    (This)->lpVtbl -> InterpolateMany(This,pStartInputValue,pInputStep,generateCount,pOutputValue,pResultCount)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetNumTypesSupported_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [out] */ aafUInt32 __RPC_FAR *pCount);


void __RPC_STUB IAAFInterpolator_GetNumTypesSupported_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetIndexedSupportedType_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ IAAFTypeDef __RPC_FAR *__RPC_FAR *ppType);


void __RPC_STUB IAAFInterpolator_GetIndexedSupportedType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetTypeDefinition_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [out] */ IAAFTypeDef __RPC_FAR *__RPC_FAR *ppTypeDef);


void __RPC_STUB IAAFInterpolator_GetTypeDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_SetTypeDefinition_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [in] */ IAAFTypeDef __RPC_FAR *pTypeDef);


void __RPC_STUB IAAFInterpolator_SetTypeDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetParameter_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [out] */ IAAFParameter __RPC_FAR *__RPC_FAR *ppParameter);


void __RPC_STUB IAAFInterpolator_GetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_SetParameter_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [in] */ IAAFParameter __RPC_FAR *pParameter);


void __RPC_STUB IAAFInterpolator_SetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_InterpolateOne_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [in] */ aafRational_t __RPC_FAR *pInputValue,
    /* [in] */ aafUInt32 valueSize,
    /* [length_is][size_is][out] */ aafDataBuffer_t pValue,
    /* [out] */ aafUInt32 __RPC_FAR *bytesRead);


void __RPC_STUB IAAFInterpolator_InterpolateOne_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_InterpolateMany_Proxy( 
    IAAFInterpolator __RPC_FAR * This,
    /* [in] */ aafRational_t __RPC_FAR *pStartInputValue,
    /* [in] */ aafRational_t __RPC_FAR *pInputStep,
    /* [in] */ aafUInt32 generateCount,
    /* [out] */ aafMemPtr_t pOutputValue,
    /* [out] */ aafUInt32 __RPC_FAR *pResultCount);


void __RPC_STUB IAAFInterpolator_InterpolateMany_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFInterpolator_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
