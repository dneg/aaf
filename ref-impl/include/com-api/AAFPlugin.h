

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Mar 24 17:09:31 2006
 */
/* Compiler settings for C:\cygwin\home\jesse\SDK\AAF\ref-impl\include\com-api\AAFPlugin.idl:
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

#ifndef __AAFPlugin_h__
#define __AAFPlugin_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAAFClassExtension_FWD_DEFINED__
#define __IAAFClassExtension_FWD_DEFINED__
typedef interface IAAFClassExtension IAAFClassExtension;
#endif 	/* __IAAFClassExtension_FWD_DEFINED__ */


#ifndef __IAAFEssenceCodec_FWD_DEFINED__
#define __IAAFEssenceCodec_FWD_DEFINED__
typedef interface IAAFEssenceCodec IAAFEssenceCodec;
#endif 	/* __IAAFEssenceCodec_FWD_DEFINED__ */


#ifndef __IAAFEssenceCodec2_FWD_DEFINED__
#define __IAAFEssenceCodec2_FWD_DEFINED__
typedef interface IAAFEssenceCodec2 IAAFEssenceCodec2;
#endif 	/* __IAAFEssenceCodec2_FWD_DEFINED__ */


#ifndef __IAAFEssenceContainer_FWD_DEFINED__
#define __IAAFEssenceContainer_FWD_DEFINED__
typedef interface IAAFEssenceContainer IAAFEssenceContainer;
#endif 	/* __IAAFEssenceContainer_FWD_DEFINED__ */


#ifndef __IAAFEssenceDataStream_FWD_DEFINED__
#define __IAAFEssenceDataStream_FWD_DEFINED__
typedef interface IAAFEssenceDataStream IAAFEssenceDataStream;
#endif 	/* __IAAFEssenceDataStream_FWD_DEFINED__ */


#ifndef __IAAFEssenceStream_FWD_DEFINED__
#define __IAAFEssenceStream_FWD_DEFINED__
typedef interface IAAFEssenceStream IAAFEssenceStream;
#endif 	/* __IAAFEssenceStream_FWD_DEFINED__ */


#ifndef __IAAFInterpolator_FWD_DEFINED__
#define __IAAFInterpolator_FWD_DEFINED__
typedef interface IAAFInterpolator IAAFInterpolator;
#endif 	/* __IAAFInterpolator_FWD_DEFINED__ */


#ifndef __IAAFMultiEssenceCodec_FWD_DEFINED__
#define __IAAFMultiEssenceCodec_FWD_DEFINED__
typedef interface IAAFMultiEssenceCodec IAAFMultiEssenceCodec;
#endif 	/* __IAAFMultiEssenceCodec_FWD_DEFINED__ */


#ifndef __IAAFPlugin_FWD_DEFINED__
#define __IAAFPlugin_FWD_DEFINED__
typedef interface IAAFPlugin IAAFPlugin;
#endif 	/* __IAAFPlugin_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"
#include "AAF.h"
#include "AAFPluginTypes.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_AAFPlugin_0000 */
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
// AAF Interfaces.
//=--------------------------------------------------------------------------=
//











extern RPC_IF_HANDLE __MIDL_itf_AAFPlugin_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AAFPlugin_0000_v0_0_s_ifspec;

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
            /* [in] */ IAAFDictionary *pDictionary) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFClassExtensionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFClassExtension * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFClassExtension * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFClassExtension * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterDefinitions )( 
            IAAFClassExtension * This,
            /* [in] */ IAAFDictionary *pDictionary);
        
        END_INTERFACE
    } IAAFClassExtensionVtbl;

    interface IAAFClassExtension
    {
        CONST_VTBL struct IAAFClassExtensionVtbl *lpVtbl;
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
    IAAFClassExtension * This,
    /* [in] */ IAAFDictionary *pDictionary);


void __RPC_STUB IAAFClassExtension_RegisterDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFClassExtension_INTERFACE_DEFINED__ */


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
            /* [in] */ IAAFEssenceAccess *pEssenceAccess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountFlavours( 
            /* [out] */ aafUInt32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedFlavourID( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pVariant) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountDataDefinitions( 
            /* [out] */ aafUInt32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDataDefinition( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pDataDefID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxCodecDisplayNameLength( 
            /* [out] */ aafUInt32 *pBufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCodecDisplayName( 
            /* [ref][in] */ aafUID_constref flavour,
            /* [size_is][string][out] */ aafCharacter *pName,
            /* [in] */ aafUInt32 bufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountChannels( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafUInt16 *pNumChannels) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectInfo( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafSelectInfo_t *pSelectInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateEssence( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCheckVerbose_t verbose,
            /* [in] */ aafCheckWarnings_t outputWarnings,
            /* [in] */ aafUInt32 bufSize,
            /* [length_is][size_is][out] */ aafCharacter *pErrorText,
            /* [out] */ aafUInt32 *pBytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [ref][in] */ aafRational_constref sampleRate,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountSamples( 
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [out] */ aafLength_t *pNumSamples) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSamples( 
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [size_is][in] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesWritten,
            /* [ref][out] */ aafUInt32 *bytesWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadSamples( 
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesRead,
            /* [ref][out] */ aafUInt32 *bytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ aafPosition_t sampleFrame) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompleteWrite( 
            /* [in] */ IAAFSourceMob *pFileMob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDescriptorFromStream( 
            /* [in] */ IAAFEssenceStream *pStream,
            /* [in] */ IAAFSourceMob *pSourceMob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentEssenceStream( 
            /* [out] */ IAAFEssenceStream **ppStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutEssenceFormat( 
            /* [in] */ IAAFEssenceFormat *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceFormat( 
            /* [in] */ IAAFEssenceFormat *pFormatTemplate,
            /* [out] */ IAAFEssenceFormat **ppNewFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultEssenceFormat( 
            /* [out] */ IAAFEssenceFormat **ppNewFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceDescriptorID( 
            /* [out] */ aafUID_t *pDescriptorID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceDataID( 
            /* [out] */ aafUID_t *pEssenceDataID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedSampleSize( 
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [in] */ aafPosition_t sampleOffset,
            /* [out] */ aafLength_t *pLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLargestSampleSize( 
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [out] */ aafLength_t *pLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceCodecVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFEssenceCodec * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFEssenceCodec * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFEssenceCodec * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetEssenceAccess )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFEssenceAccess *pEssenceAccess);
        
        HRESULT ( STDMETHODCALLTYPE *CountFlavours )( 
            IAAFEssenceCodec * This,
            /* [out] */ aafUInt32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedFlavourID )( 
            IAAFEssenceCodec * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pVariant);
        
        HRESULT ( STDMETHODCALLTYPE *CountDataDefinitions )( 
            IAAFEssenceCodec * This,
            /* [out] */ aafUInt32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedDataDefinition )( 
            IAAFEssenceCodec * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pDataDefID);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxCodecDisplayNameLength )( 
            IAAFEssenceCodec * This,
            /* [out] */ aafUInt32 *pBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetCodecDisplayName )( 
            IAAFEssenceCodec * This,
            /* [ref][in] */ aafUID_constref flavour,
            /* [size_is][string][out] */ aafCharacter *pName,
            /* [in] */ aafUInt32 bufSize);
        
        HRESULT ( STDMETHODCALLTYPE *CountChannels )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafUInt16 *pNumChannels);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelectInfo )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafSelectInfo_t *pSelectInfo);
        
        HRESULT ( STDMETHODCALLTYPE *ValidateEssence )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCheckVerbose_t verbose,
            /* [in] */ aafCheckWarnings_t outputWarnings,
            /* [in] */ aafUInt32 bufSize,
            /* [length_is][size_is][out] */ aafCharacter *pErrorText,
            /* [out] */ aafUInt32 *pBytesRead);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [ref][in] */ aafRational_constref sampleRate,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE *Open )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE *CountSamples )( 
            IAAFEssenceCodec * This,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [out] */ aafLength_t *pNumSamples);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSamples )( 
            IAAFEssenceCodec * This,
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [size_is][in] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesWritten,
            /* [ref][out] */ aafUInt32 *bytesWritten);
        
        HRESULT ( STDMETHODCALLTYPE *ReadSamples )( 
            IAAFEssenceCodec * This,
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesRead,
            /* [ref][out] */ aafUInt32 *bytesRead);
        
        HRESULT ( STDMETHODCALLTYPE *Seek )( 
            IAAFEssenceCodec * This,
            /* [in] */ aafPosition_t sampleFrame);
        
        HRESULT ( STDMETHODCALLTYPE *CompleteWrite )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFSourceMob *pFileMob);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDescriptorFromStream )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFEssenceStream *pStream,
            /* [in] */ IAAFSourceMob *pSourceMob);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentEssenceStream )( 
            IAAFEssenceCodec * This,
            /* [out] */ IAAFEssenceStream **ppStream);
        
        HRESULT ( STDMETHODCALLTYPE *PutEssenceFormat )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFEssenceFormat *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetEssenceFormat )( 
            IAAFEssenceCodec * This,
            /* [in] */ IAAFEssenceFormat *pFormatTemplate,
            /* [out] */ IAAFEssenceFormat **ppNewFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetDefaultEssenceFormat )( 
            IAAFEssenceCodec * This,
            /* [out] */ IAAFEssenceFormat **ppNewFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetEssenceDescriptorID )( 
            IAAFEssenceCodec * This,
            /* [out] */ aafUID_t *pDescriptorID);
        
        HRESULT ( STDMETHODCALLTYPE *GetEssenceDataID )( 
            IAAFEssenceCodec * This,
            /* [out] */ aafUID_t *pEssenceDataID);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedSampleSize )( 
            IAAFEssenceCodec * This,
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [in] */ aafPosition_t sampleOffset,
            /* [out] */ aafLength_t *pLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetLargestSampleSize )( 
            IAAFEssenceCodec * This,
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [out] */ aafLength_t *pLength);
        
        END_INTERFACE
    } IAAFEssenceCodecVtbl;

    interface IAAFEssenceCodec
    {
        CONST_VTBL struct IAAFEssenceCodecVtbl *lpVtbl;
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
    IAAFEssenceCodec * This,
    /* [in] */ IAAFEssenceAccess *pEssenceAccess);


void __RPC_STUB IAAFEssenceCodec_SetEssenceAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountFlavours_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ aafUInt32 *pCount);


void __RPC_STUB IAAFEssenceCodec_CountFlavours_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetIndexedFlavourID_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t *pVariant);


void __RPC_STUB IAAFEssenceCodec_GetIndexedFlavourID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountDataDefinitions_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ aafUInt32 *pCount);


void __RPC_STUB IAAFEssenceCodec_CountDataDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetIndexedDataDefinition_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t *pDataDefID);


void __RPC_STUB IAAFEssenceCodec_GetIndexedDataDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetMaxCodecDisplayNameLength_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ aafUInt32 *pBufSize);


void __RPC_STUB IAAFEssenceCodec_GetMaxCodecDisplayNameLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetCodecDisplayName_Proxy( 
    IAAFEssenceCodec * This,
    /* [ref][in] */ aafUID_constref flavour,
    /* [size_is][string][out] */ aafCharacter *pName,
    /* [in] */ aafUInt32 bufSize);


void __RPC_STUB IAAFEssenceCodec_GetCodecDisplayName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountChannels_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [in] */ IAAFEssenceStream *stream,
    /* [out] */ aafUInt16 *pNumChannels);


void __RPC_STUB IAAFEssenceCodec_CountChannels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetSelectInfo_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ IAAFEssenceStream *stream,
    /* [out] */ aafSelectInfo_t *pSelectInfo);


void __RPC_STUB IAAFEssenceCodec_GetSelectInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_ValidateEssence_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCheckVerbose_t verbose,
    /* [in] */ aafCheckWarnings_t outputWarnings,
    /* [in] */ aafUInt32 bufSize,
    /* [length_is][size_is][out] */ aafCharacter *pErrorText,
    /* [out] */ aafUInt32 *pBytesRead);


void __RPC_STUB IAAFEssenceCodec_ValidateEssence_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_Create_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [ref][in] */ aafUID_constref flavour,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [ref][in] */ aafRational_constref sampleRate,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFEssenceCodec_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_Open_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ aafMediaOpenMode_t openMode,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFEssenceCodec_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CountSamples_Proxy( 
    IAAFEssenceCodec * This,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [out] */ aafLength_t *pNumSamples);


void __RPC_STUB IAAFEssenceCodec_CountSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_WriteSamples_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ aafUInt32 nSamples,
    /* [in] */ aafUInt32 buflen,
    /* [size_is][in] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 *samplesWritten,
    /* [ref][out] */ aafUInt32 *bytesWritten);


void __RPC_STUB IAAFEssenceCodec_WriteSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_ReadSamples_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ aafUInt32 nSamples,
    /* [in] */ aafUInt32 buflen,
    /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 *samplesRead,
    /* [ref][out] */ aafUInt32 *bytesRead);


void __RPC_STUB IAAFEssenceCodec_ReadSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_Seek_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ aafPosition_t sampleFrame);


void __RPC_STUB IAAFEssenceCodec_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CompleteWrite_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFSourceMob *pFileMob);


void __RPC_STUB IAAFEssenceCodec_CompleteWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_CreateDescriptorFromStream_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFEssenceStream *pStream,
    /* [in] */ IAAFSourceMob *pSourceMob);


void __RPC_STUB IAAFEssenceCodec_CreateDescriptorFromStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetCurrentEssenceStream_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ IAAFEssenceStream **ppStream);


void __RPC_STUB IAAFEssenceCodec_GetCurrentEssenceStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_PutEssenceFormat_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFEssenceFormat *pFormat);


void __RPC_STUB IAAFEssenceCodec_PutEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetEssenceFormat_Proxy( 
    IAAFEssenceCodec * This,
    /* [in] */ IAAFEssenceFormat *pFormatTemplate,
    /* [out] */ IAAFEssenceFormat **ppNewFormat);


void __RPC_STUB IAAFEssenceCodec_GetEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetDefaultEssenceFormat_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ IAAFEssenceFormat **ppNewFormat);


void __RPC_STUB IAAFEssenceCodec_GetDefaultEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetEssenceDescriptorID_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ aafUID_t *pDescriptorID);


void __RPC_STUB IAAFEssenceCodec_GetEssenceDescriptorID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetEssenceDataID_Proxy( 
    IAAFEssenceCodec * This,
    /* [out] */ aafUID_t *pEssenceDataID);


void __RPC_STUB IAAFEssenceCodec_GetEssenceDataID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetIndexedSampleSize_Proxy( 
    IAAFEssenceCodec * This,
    /* [ref][in] */ aafUID_constref essenceDefID,
    /* [in] */ aafPosition_t sampleOffset,
    /* [out] */ aafLength_t *pLength);


void __RPC_STUB IAAFEssenceCodec_GetIndexedSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec_GetLargestSampleSize_Proxy( 
    IAAFEssenceCodec * This,
    /* [ref][in] */ aafUID_constref essenceDefID,
    /* [out] */ aafLength_t *pLength);


void __RPC_STUB IAAFEssenceCodec_GetLargestSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceCodec_INTERFACE_DEFINED__ */


#ifndef __IAAFEssenceCodec2_INTERFACE_DEFINED__
#define __IAAFEssenceCodec2_INTERFACE_DEFINED__

/* interface IAAFEssenceCodec2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAAFEssenceCodec2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8888F7A2-9121-11d2-8088-006008143e6f")
    IAAFEssenceCodec2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetEssenceAccess( 
            /* [in] */ IAAFEssenceAccess *pEssenceAccess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountFlavours( 
            /* [out] */ aafUInt32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedFlavourID( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pVariant) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountDataDefinitions( 
            /* [out] */ aafUInt32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDataDefinition( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pDataDefID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxCodecDisplayNameLength( 
            /* [out] */ aafUInt32 *pBufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCodecDisplayName( 
            /* [ref][in] */ aafUID_constref flavour,
            /* [size_is][string][out] */ aafCharacter *pName,
            /* [in] */ aafUInt32 bufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountChannels( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafUInt16 *pNumChannels) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectInfo( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafSelectInfo_t *pSelectInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateEssence( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCheckVerbose_t verbose,
            /* [in] */ aafCheckWarnings_t outputWarnings,
            /* [in] */ aafUInt32 bufSize,
            /* [length_is][size_is][out] */ aafCharacter *pErrorText,
            /* [out] */ aafUInt32 *pBytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [ref][in] */ aafRational_constref sampleRate,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CountSamples( 
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [out] */ aafLength_t *pNumSamples) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSamples( 
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [size_is][in] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesWritten,
            /* [ref][out] */ aafUInt32 *bytesWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadSamples( 
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesRead,
            /* [ref][out] */ aafUInt32 *bytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ aafPosition_t sampleFrame) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompleteWrite( 
            /* [in] */ IAAFSourceMob *pFileMob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDescriptorFromStream( 
            /* [in] */ IAAFEssenceStream *pStream,
            /* [in] */ IAAFSourceMob *pSourceMob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentEssenceStream( 
            /* [out] */ IAAFEssenceStream **ppStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutEssenceFormat( 
            /* [in] */ IAAFEssenceFormat *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceFormat( 
            /* [in] */ IAAFEssenceFormat *pFormatTemplate,
            /* [out] */ IAAFEssenceFormat **ppNewFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultEssenceFormat( 
            /* [out] */ IAAFEssenceFormat **ppNewFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceDescriptorID( 
            /* [out] */ aafUID_t *pDescriptorID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEssenceDataID( 
            /* [out] */ aafUID_t *pEssenceDataID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedSampleSize( 
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [in] */ aafPosition_t sampleOffset,
            /* [out] */ aafLength_t *pLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLargestSampleSize( 
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [out] */ aafLength_t *pLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFlavour( 
            /* [ref][in] */ aafUID_constref flavour) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceCodec2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFEssenceCodec2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFEssenceCodec2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetEssenceAccess )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFEssenceAccess *pEssenceAccess);
        
        HRESULT ( STDMETHODCALLTYPE *CountFlavours )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ aafUInt32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedFlavourID )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pVariant);
        
        HRESULT ( STDMETHODCALLTYPE *CountDataDefinitions )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ aafUInt32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedDataDefinition )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pDataDefID);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxCodecDisplayNameLength )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ aafUInt32 *pBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetCodecDisplayName )( 
            IAAFEssenceCodec2 * This,
            /* [ref][in] */ aafUID_constref flavour,
            /* [size_is][string][out] */ aafCharacter *pName,
            /* [in] */ aafUInt32 bufSize);
        
        HRESULT ( STDMETHODCALLTYPE *CountChannels )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafUInt16 *pNumChannels);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelectInfo )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [out] */ aafSelectInfo_t *pSelectInfo);
        
        HRESULT ( STDMETHODCALLTYPE *ValidateEssence )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCheckVerbose_t verbose,
            /* [in] */ aafCheckWarnings_t outputWarnings,
            /* [in] */ aafUInt32 bufSize,
            /* [length_is][size_is][out] */ aafCharacter *pErrorText,
            /* [out] */ aafUInt32 *pBytesRead);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [ref][in] */ aafRational_constref sampleRate,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE *Open )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE *CountSamples )( 
            IAAFEssenceCodec2 * This,
            /* [ref][in] */ aafUID_constref essenceKind,
            /* [out] */ aafLength_t *pNumSamples);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSamples )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [size_is][in] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesWritten,
            /* [ref][out] */ aafUInt32 *bytesWritten);
        
        HRESULT ( STDMETHODCALLTYPE *ReadSamples )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ aafUInt32 nSamples,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *samplesRead,
            /* [ref][out] */ aafUInt32 *bytesRead);
        
        HRESULT ( STDMETHODCALLTYPE *Seek )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ aafPosition_t sampleFrame);
        
        HRESULT ( STDMETHODCALLTYPE *CompleteWrite )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFSourceMob *pFileMob);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDescriptorFromStream )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFEssenceStream *pStream,
            /* [in] */ IAAFSourceMob *pSourceMob);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentEssenceStream )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ IAAFEssenceStream **ppStream);
        
        HRESULT ( STDMETHODCALLTYPE *PutEssenceFormat )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFEssenceFormat *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetEssenceFormat )( 
            IAAFEssenceCodec2 * This,
            /* [in] */ IAAFEssenceFormat *pFormatTemplate,
            /* [out] */ IAAFEssenceFormat **ppNewFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetDefaultEssenceFormat )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ IAAFEssenceFormat **ppNewFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetEssenceDescriptorID )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ aafUID_t *pDescriptorID);
        
        HRESULT ( STDMETHODCALLTYPE *GetEssenceDataID )( 
            IAAFEssenceCodec2 * This,
            /* [out] */ aafUID_t *pEssenceDataID);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedSampleSize )( 
            IAAFEssenceCodec2 * This,
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [in] */ aafPosition_t sampleOffset,
            /* [out] */ aafLength_t *pLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetLargestSampleSize )( 
            IAAFEssenceCodec2 * This,
            /* [ref][in] */ aafUID_constref essenceDefID,
            /* [out] */ aafLength_t *pLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetFlavour )( 
            IAAFEssenceCodec2 * This,
            /* [ref][in] */ aafUID_constref flavour);
        
        END_INTERFACE
    } IAAFEssenceCodec2Vtbl;

    interface IAAFEssenceCodec2
    {
        CONST_VTBL struct IAAFEssenceCodec2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAAFEssenceCodec2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAAFEssenceCodec2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAAFEssenceCodec2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAAFEssenceCodec2_SetEssenceAccess(This,pEssenceAccess)	\
    (This)->lpVtbl -> SetEssenceAccess(This,pEssenceAccess)

#define IAAFEssenceCodec2_CountFlavours(This,pCount)	\
    (This)->lpVtbl -> CountFlavours(This,pCount)

#define IAAFEssenceCodec2_GetIndexedFlavourID(This,index,pVariant)	\
    (This)->lpVtbl -> GetIndexedFlavourID(This,index,pVariant)

#define IAAFEssenceCodec2_CountDataDefinitions(This,pCount)	\
    (This)->lpVtbl -> CountDataDefinitions(This,pCount)

#define IAAFEssenceCodec2_GetIndexedDataDefinition(This,index,pDataDefID)	\
    (This)->lpVtbl -> GetIndexedDataDefinition(This,index,pDataDefID)

#define IAAFEssenceCodec2_GetMaxCodecDisplayNameLength(This,pBufSize)	\
    (This)->lpVtbl -> GetMaxCodecDisplayNameLength(This,pBufSize)

#define IAAFEssenceCodec2_GetCodecDisplayName(This,flavour,pName,bufSize)	\
    (This)->lpVtbl -> GetCodecDisplayName(This,flavour,pName,bufSize)

#define IAAFEssenceCodec2_CountChannels(This,fileMob,essenceKind,stream,pNumChannels)	\
    (This)->lpVtbl -> CountChannels(This,fileMob,essenceKind,stream,pNumChannels)

#define IAAFEssenceCodec2_GetSelectInfo(This,fileMob,stream,pSelectInfo)	\
    (This)->lpVtbl -> GetSelectInfo(This,fileMob,stream,pSelectInfo)

#define IAAFEssenceCodec2_ValidateEssence(This,fileMob,stream,verbose,outputWarnings,bufSize,pErrorText,pBytesRead)	\
    (This)->lpVtbl -> ValidateEssence(This,fileMob,stream,verbose,outputWarnings,bufSize,pErrorText,pBytesRead)

#define IAAFEssenceCodec2_Create(This,fileMob,flavour,essenceKind,sampleRate,stream,compEnable)	\
    (This)->lpVtbl -> Create(This,fileMob,flavour,essenceKind,sampleRate,stream,compEnable)

#define IAAFEssenceCodec2_Open(This,fileMob,openMode,stream,compEnable)	\
    (This)->lpVtbl -> Open(This,fileMob,openMode,stream,compEnable)

#define IAAFEssenceCodec2_CountSamples(This,essenceKind,pNumSamples)	\
    (This)->lpVtbl -> CountSamples(This,essenceKind,pNumSamples)

#define IAAFEssenceCodec2_WriteSamples(This,nSamples,buflen,buffer,samplesWritten,bytesWritten)	\
    (This)->lpVtbl -> WriteSamples(This,nSamples,buflen,buffer,samplesWritten,bytesWritten)

#define IAAFEssenceCodec2_ReadSamples(This,nSamples,buflen,buffer,samplesRead,bytesRead)	\
    (This)->lpVtbl -> ReadSamples(This,nSamples,buflen,buffer,samplesRead,bytesRead)

#define IAAFEssenceCodec2_Seek(This,sampleFrame)	\
    (This)->lpVtbl -> Seek(This,sampleFrame)

#define IAAFEssenceCodec2_CompleteWrite(This,pFileMob)	\
    (This)->lpVtbl -> CompleteWrite(This,pFileMob)

#define IAAFEssenceCodec2_CreateDescriptorFromStream(This,pStream,pSourceMob)	\
    (This)->lpVtbl -> CreateDescriptorFromStream(This,pStream,pSourceMob)

#define IAAFEssenceCodec2_GetCurrentEssenceStream(This,ppStream)	\
    (This)->lpVtbl -> GetCurrentEssenceStream(This,ppStream)

#define IAAFEssenceCodec2_PutEssenceFormat(This,pFormat)	\
    (This)->lpVtbl -> PutEssenceFormat(This,pFormat)

#define IAAFEssenceCodec2_GetEssenceFormat(This,pFormatTemplate,ppNewFormat)	\
    (This)->lpVtbl -> GetEssenceFormat(This,pFormatTemplate,ppNewFormat)

#define IAAFEssenceCodec2_GetDefaultEssenceFormat(This,ppNewFormat)	\
    (This)->lpVtbl -> GetDefaultEssenceFormat(This,ppNewFormat)

#define IAAFEssenceCodec2_GetEssenceDescriptorID(This,pDescriptorID)	\
    (This)->lpVtbl -> GetEssenceDescriptorID(This,pDescriptorID)

#define IAAFEssenceCodec2_GetEssenceDataID(This,pEssenceDataID)	\
    (This)->lpVtbl -> GetEssenceDataID(This,pEssenceDataID)

#define IAAFEssenceCodec2_GetIndexedSampleSize(This,essenceDefID,sampleOffset,pLength)	\
    (This)->lpVtbl -> GetIndexedSampleSize(This,essenceDefID,sampleOffset,pLength)

#define IAAFEssenceCodec2_GetLargestSampleSize(This,essenceDefID,pLength)	\
    (This)->lpVtbl -> GetLargestSampleSize(This,essenceDefID,pLength)

#define IAAFEssenceCodec2_SetFlavour(This,flavour)	\
    (This)->lpVtbl -> SetFlavour(This,flavour)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_SetEssenceAccess_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFEssenceAccess *pEssenceAccess);


void __RPC_STUB IAAFEssenceCodec2_SetEssenceAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_CountFlavours_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ aafUInt32 *pCount);


void __RPC_STUB IAAFEssenceCodec2_CountFlavours_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetIndexedFlavourID_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t *pVariant);


void __RPC_STUB IAAFEssenceCodec2_GetIndexedFlavourID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_CountDataDefinitions_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ aafUInt32 *pCount);


void __RPC_STUB IAAFEssenceCodec2_CountDataDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetIndexedDataDefinition_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t *pDataDefID);


void __RPC_STUB IAAFEssenceCodec2_GetIndexedDataDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetMaxCodecDisplayNameLength_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ aafUInt32 *pBufSize);


void __RPC_STUB IAAFEssenceCodec2_GetMaxCodecDisplayNameLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetCodecDisplayName_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [ref][in] */ aafUID_constref flavour,
    /* [size_is][string][out] */ aafCharacter *pName,
    /* [in] */ aafUInt32 bufSize);


void __RPC_STUB IAAFEssenceCodec2_GetCodecDisplayName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_CountChannels_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [in] */ IAAFEssenceStream *stream,
    /* [out] */ aafUInt16 *pNumChannels);


void __RPC_STUB IAAFEssenceCodec2_CountChannels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetSelectInfo_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ IAAFEssenceStream *stream,
    /* [out] */ aafSelectInfo_t *pSelectInfo);


void __RPC_STUB IAAFEssenceCodec2_GetSelectInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_ValidateEssence_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCheckVerbose_t verbose,
    /* [in] */ aafCheckWarnings_t outputWarnings,
    /* [in] */ aafUInt32 bufSize,
    /* [length_is][size_is][out] */ aafCharacter *pErrorText,
    /* [out] */ aafUInt32 *pBytesRead);


void __RPC_STUB IAAFEssenceCodec2_ValidateEssence_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_Create_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [ref][in] */ aafUID_constref flavour,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [ref][in] */ aafRational_constref sampleRate,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFEssenceCodec2_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_Open_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ aafMediaOpenMode_t openMode,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFEssenceCodec2_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_CountSamples_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [ref][in] */ aafUID_constref essenceKind,
    /* [out] */ aafLength_t *pNumSamples);


void __RPC_STUB IAAFEssenceCodec2_CountSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_WriteSamples_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ aafUInt32 nSamples,
    /* [in] */ aafUInt32 buflen,
    /* [size_is][in] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 *samplesWritten,
    /* [ref][out] */ aafUInt32 *bytesWritten);


void __RPC_STUB IAAFEssenceCodec2_WriteSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_ReadSamples_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ aafUInt32 nSamples,
    /* [in] */ aafUInt32 buflen,
    /* [length_is][size_is][out] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 *samplesRead,
    /* [ref][out] */ aafUInt32 *bytesRead);


void __RPC_STUB IAAFEssenceCodec2_ReadSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_Seek_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ aafPosition_t sampleFrame);


void __RPC_STUB IAAFEssenceCodec2_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_CompleteWrite_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFSourceMob *pFileMob);


void __RPC_STUB IAAFEssenceCodec2_CompleteWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_CreateDescriptorFromStream_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFEssenceStream *pStream,
    /* [in] */ IAAFSourceMob *pSourceMob);


void __RPC_STUB IAAFEssenceCodec2_CreateDescriptorFromStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetCurrentEssenceStream_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ IAAFEssenceStream **ppStream);


void __RPC_STUB IAAFEssenceCodec2_GetCurrentEssenceStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_PutEssenceFormat_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFEssenceFormat *pFormat);


void __RPC_STUB IAAFEssenceCodec2_PutEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetEssenceFormat_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [in] */ IAAFEssenceFormat *pFormatTemplate,
    /* [out] */ IAAFEssenceFormat **ppNewFormat);


void __RPC_STUB IAAFEssenceCodec2_GetEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetDefaultEssenceFormat_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ IAAFEssenceFormat **ppNewFormat);


void __RPC_STUB IAAFEssenceCodec2_GetDefaultEssenceFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetEssenceDescriptorID_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ aafUID_t *pDescriptorID);


void __RPC_STUB IAAFEssenceCodec2_GetEssenceDescriptorID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetEssenceDataID_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [out] */ aafUID_t *pEssenceDataID);


void __RPC_STUB IAAFEssenceCodec2_GetEssenceDataID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetIndexedSampleSize_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [ref][in] */ aafUID_constref essenceDefID,
    /* [in] */ aafPosition_t sampleOffset,
    /* [out] */ aafLength_t *pLength);


void __RPC_STUB IAAFEssenceCodec2_GetIndexedSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_GetLargestSampleSize_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [ref][in] */ aafUID_constref essenceDefID,
    /* [out] */ aafLength_t *pLength);


void __RPC_STUB IAAFEssenceCodec2_GetLargestSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceCodec2_SetFlavour_Proxy( 
    IAAFEssenceCodec2 * This,
    /* [ref][in] */ aafUID_constref flavour);


void __RPC_STUB IAAFEssenceCodec2_SetFlavour_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceCodec2_INTERFACE_DEFINED__ */


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
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEssenceStreamWriteOnly( 
            /* [string][in] */ aafCharacter_constptr pPath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenEssenceStreamReadOnly( 
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenEssenceStreamAppend( 
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceContainerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFEssenceContainer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFEssenceContainer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFEssenceContainer * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateEssenceStream )( 
            IAAFEssenceContainer * This,
            /* [string][in] */ aafCharacter_constptr pName,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);
        
        HRESULT ( STDMETHODCALLTYPE *CreateEssenceStreamWriteOnly )( 
            IAAFEssenceContainer * This,
            /* [string][in] */ aafCharacter_constptr pPath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);
        
        HRESULT ( STDMETHODCALLTYPE *OpenEssenceStreamReadOnly )( 
            IAAFEssenceContainer * This,
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);
        
        HRESULT ( STDMETHODCALLTYPE *OpenEssenceStreamAppend )( 
            IAAFEssenceContainer * This,
            /* [string][in] */ aafCharacter_constptr pFilePath,
            /* [in] */ aafMobID_constptr pMobID,
            /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);
        
        END_INTERFACE
    } IAAFEssenceContainerVtbl;

    interface IAAFEssenceContainer
    {
        CONST_VTBL struct IAAFEssenceContainerVtbl *lpVtbl;
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
    IAAFEssenceContainer * This,
    /* [string][in] */ aafCharacter_constptr pName,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_CreateEssenceStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_CreateEssenceStreamWriteOnly_Proxy( 
    IAAFEssenceContainer * This,
    /* [string][in] */ aafCharacter_constptr pPath,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_CreateEssenceStreamWriteOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_OpenEssenceStreamReadOnly_Proxy( 
    IAAFEssenceContainer * This,
    /* [string][in] */ aafCharacter_constptr pFilePath,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_OpenEssenceStreamReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceContainer_OpenEssenceStreamAppend_Proxy( 
    IAAFEssenceContainer * This,
    /* [string][in] */ aafCharacter_constptr pFilePath,
    /* [in] */ aafMobID_constptr pMobID,
    /* [retval][out] */ IAAFEssenceStream **ppEssenceStream);


void __RPC_STUB IAAFEssenceContainer_OpenEssenceStreamAppend_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceContainer_INTERFACE_DEFINED__ */


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
            /* [in] */ IUnknown *essenceData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceDataStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFEssenceDataStream * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFEssenceDataStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFEssenceDataStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *Init )( 
            IAAFEssenceDataStream * This,
            /* [in] */ IUnknown *essenceData);
        
        END_INTERFACE
    } IAAFEssenceDataStreamVtbl;

    interface IAAFEssenceDataStream
    {
        CONST_VTBL struct IAAFEssenceDataStreamVtbl *lpVtbl;
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
    IAAFEssenceDataStream * This,
    /* [in] */ IUnknown *essenceData);


void __RPC_STUB IAAFEssenceDataStream_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceDataStream_INTERFACE_DEFINED__ */


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
            /* [ref][out] */ aafUInt32 *bytesWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t pBuffer,
            /* [ref][out] */ aafUInt32 *pBytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ aafPosition_t byteOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SeekRelative( 
            /* [in] */ aafInt32 byteOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ aafPosition_t *pPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLength( 
            /* [out] */ aafLength_t *pLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FlushCache( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCacheSize( 
            /* [in] */ aafUInt32 itsSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFEssenceStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFEssenceStream * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFEssenceStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFEssenceStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *Write )( 
            IAAFEssenceStream * This,
            /* [in] */ aafUInt32 bytes,
            /* [size_is][out] */ aafDataBuffer_t buffer,
            /* [ref][out] */ aafUInt32 *bytesWritten);
        
        HRESULT ( STDMETHODCALLTYPE *Read )( 
            IAAFEssenceStream * This,
            /* [in] */ aafUInt32 buflen,
            /* [length_is][size_is][out] */ aafDataBuffer_t pBuffer,
            /* [ref][out] */ aafUInt32 *pBytesRead);
        
        HRESULT ( STDMETHODCALLTYPE *Seek )( 
            IAAFEssenceStream * This,
            /* [in] */ aafPosition_t byteOffset);
        
        HRESULT ( STDMETHODCALLTYPE *SeekRelative )( 
            IAAFEssenceStream * This,
            /* [in] */ aafInt32 byteOffset);
        
        HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IAAFEssenceStream * This,
            /* [out] */ aafPosition_t *pPosition);
        
        HRESULT ( STDMETHODCALLTYPE *GetLength )( 
            IAAFEssenceStream * This,
            /* [out] */ aafLength_t *pLength);
        
        HRESULT ( STDMETHODCALLTYPE *FlushCache )( 
            IAAFEssenceStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetCacheSize )( 
            IAAFEssenceStream * This,
            /* [in] */ aafUInt32 itsSize);
        
        END_INTERFACE
    } IAAFEssenceStreamVtbl;

    interface IAAFEssenceStream
    {
        CONST_VTBL struct IAAFEssenceStreamVtbl *lpVtbl;
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
    IAAFEssenceStream * This,
    /* [in] */ aafUInt32 bytes,
    /* [size_is][out] */ aafDataBuffer_t buffer,
    /* [ref][out] */ aafUInt32 *bytesWritten);


void __RPC_STUB IAAFEssenceStream_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_Read_Proxy( 
    IAAFEssenceStream * This,
    /* [in] */ aafUInt32 buflen,
    /* [length_is][size_is][out] */ aafDataBuffer_t pBuffer,
    /* [ref][out] */ aafUInt32 *pBytesRead);


void __RPC_STUB IAAFEssenceStream_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_Seek_Proxy( 
    IAAFEssenceStream * This,
    /* [in] */ aafPosition_t byteOffset);


void __RPC_STUB IAAFEssenceStream_Seek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_SeekRelative_Proxy( 
    IAAFEssenceStream * This,
    /* [in] */ aafInt32 byteOffset);


void __RPC_STUB IAAFEssenceStream_SeekRelative_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_GetPosition_Proxy( 
    IAAFEssenceStream * This,
    /* [out] */ aafPosition_t *pPosition);


void __RPC_STUB IAAFEssenceStream_GetPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_GetLength_Proxy( 
    IAAFEssenceStream * This,
    /* [out] */ aafLength_t *pLength);


void __RPC_STUB IAAFEssenceStream_GetLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_FlushCache_Proxy( 
    IAAFEssenceStream * This);


void __RPC_STUB IAAFEssenceStream_FlushCache_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFEssenceStream_SetCacheSize_Proxy( 
    IAAFEssenceStream * This,
    /* [in] */ aafUInt32 itsSize);


void __RPC_STUB IAAFEssenceStream_SetCacheSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFEssenceStream_INTERFACE_DEFINED__ */


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
            /* [out] */ aafUInt32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedSupportedType( 
            /* [in] */ aafUInt32 index,
            /* [out] */ IAAFTypeDef **ppType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeDefinition( 
            /* [out] */ IAAFTypeDef **ppTypeDef) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTypeDefinition( 
            /* [in] */ IAAFTypeDef *pTypeDef) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParameter( 
            /* [out] */ IAAFParameter **ppParameter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetParameter( 
            /* [in] */ IAAFParameter *pParameter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InterpolateOne( 
            /* [in] */ aafRational_t *pInputValue,
            /* [in] */ aafUInt32 valueSize,
            /* [length_is][size_is][out] */ aafDataBuffer_t pValue,
            /* [out] */ aafUInt32 *bytesRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InterpolateMany( 
            /* [in] */ aafRational_t *pStartInputValue,
            /* [in] */ aafRational_t *pInputStep,
            /* [in] */ aafUInt32 generateCount,
            /* [out] */ aafMemPtr_t pOutputValue,
            /* [out] */ aafUInt32 *pResultCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFInterpolatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFInterpolator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFInterpolator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFInterpolator * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetNumTypesSupported )( 
            IAAFInterpolator * This,
            /* [out] */ aafUInt32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedSupportedType )( 
            IAAFInterpolator * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ IAAFTypeDef **ppType);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeDefinition )( 
            IAAFInterpolator * This,
            /* [out] */ IAAFTypeDef **ppTypeDef);
        
        HRESULT ( STDMETHODCALLTYPE *SetTypeDefinition )( 
            IAAFInterpolator * This,
            /* [in] */ IAAFTypeDef *pTypeDef);
        
        HRESULT ( STDMETHODCALLTYPE *GetParameter )( 
            IAAFInterpolator * This,
            /* [out] */ IAAFParameter **ppParameter);
        
        HRESULT ( STDMETHODCALLTYPE *SetParameter )( 
            IAAFInterpolator * This,
            /* [in] */ IAAFParameter *pParameter);
        
        HRESULT ( STDMETHODCALLTYPE *InterpolateOne )( 
            IAAFInterpolator * This,
            /* [in] */ aafRational_t *pInputValue,
            /* [in] */ aafUInt32 valueSize,
            /* [length_is][size_is][out] */ aafDataBuffer_t pValue,
            /* [out] */ aafUInt32 *bytesRead);
        
        HRESULT ( STDMETHODCALLTYPE *InterpolateMany )( 
            IAAFInterpolator * This,
            /* [in] */ aafRational_t *pStartInputValue,
            /* [in] */ aafRational_t *pInputStep,
            /* [in] */ aafUInt32 generateCount,
            /* [out] */ aafMemPtr_t pOutputValue,
            /* [out] */ aafUInt32 *pResultCount);
        
        END_INTERFACE
    } IAAFInterpolatorVtbl;

    interface IAAFInterpolator
    {
        CONST_VTBL struct IAAFInterpolatorVtbl *lpVtbl;
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
    IAAFInterpolator * This,
    /* [out] */ aafUInt32 *pCount);


void __RPC_STUB IAAFInterpolator_GetNumTypesSupported_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetIndexedSupportedType_Proxy( 
    IAAFInterpolator * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ IAAFTypeDef **ppType);


void __RPC_STUB IAAFInterpolator_GetIndexedSupportedType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetTypeDefinition_Proxy( 
    IAAFInterpolator * This,
    /* [out] */ IAAFTypeDef **ppTypeDef);


void __RPC_STUB IAAFInterpolator_GetTypeDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_SetTypeDefinition_Proxy( 
    IAAFInterpolator * This,
    /* [in] */ IAAFTypeDef *pTypeDef);


void __RPC_STUB IAAFInterpolator_SetTypeDefinition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_GetParameter_Proxy( 
    IAAFInterpolator * This,
    /* [out] */ IAAFParameter **ppParameter);


void __RPC_STUB IAAFInterpolator_GetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_SetParameter_Proxy( 
    IAAFInterpolator * This,
    /* [in] */ IAAFParameter *pParameter);


void __RPC_STUB IAAFInterpolator_SetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_InterpolateOne_Proxy( 
    IAAFInterpolator * This,
    /* [in] */ aafRational_t *pInputValue,
    /* [in] */ aafUInt32 valueSize,
    /* [length_is][size_is][out] */ aafDataBuffer_t pValue,
    /* [out] */ aafUInt32 *bytesRead);


void __RPC_STUB IAAFInterpolator_InterpolateOne_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFInterpolator_InterpolateMany_Proxy( 
    IAAFInterpolator * This,
    /* [in] */ aafRational_t *pStartInputValue,
    /* [in] */ aafRational_t *pInputStep,
    /* [in] */ aafUInt32 generateCount,
    /* [out] */ aafMemPtr_t pOutputValue,
    /* [out] */ aafUInt32 *pResultCount);


void __RPC_STUB IAAFInterpolator_InterpolateMany_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFInterpolator_INTERFACE_DEFINED__ */


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
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable,
            /* [in] */ aafUInt32 numParms,
            /* [size_is][in] */ aafmMultiCreate_t *createParms) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MultiOpen( 
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteBlocks( 
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][in] */ aafmMultiXfer_t *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t *pResultParm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadBlocks( 
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][out] */ aafmMultiXfer_t *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t *pResultParm) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFMultiEssenceCodecVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFMultiEssenceCodec * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFMultiEssenceCodec * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFMultiEssenceCodec * This);
        
        HRESULT ( STDMETHODCALLTYPE *MultiCreate )( 
            IAAFMultiEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [ref][in] */ aafUID_constref flavour,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable,
            /* [in] */ aafUInt32 numParms,
            /* [size_is][in] */ aafmMultiCreate_t *createParms);
        
        HRESULT ( STDMETHODCALLTYPE *MultiOpen )( 
            IAAFMultiEssenceCodec * This,
            /* [in] */ IAAFSourceMob *fileMob,
            /* [in] */ aafMediaOpenMode_t openMode,
            /* [in] */ IAAFEssenceStream *stream,
            /* [in] */ aafCompressEnable_t compEnable);
        
        HRESULT ( STDMETHODCALLTYPE *WriteBlocks )( 
            IAAFMultiEssenceCodec * This,
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][in] */ aafmMultiXfer_t *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t *pResultParm);
        
        HRESULT ( STDMETHODCALLTYPE *ReadBlocks )( 
            IAAFMultiEssenceCodec * This,
            /* [in] */ aafDeinterleave_t inter,
            /* [in] */ aafUInt16 xferBlockCount,
            /* [size_is][out] */ aafmMultiXfer_t *pTransferParm,
            /* [size_is][out] */ aafmMultiResult_t *pResultParm);
        
        END_INTERFACE
    } IAAFMultiEssenceCodecVtbl;

    interface IAAFMultiEssenceCodec
    {
        CONST_VTBL struct IAAFMultiEssenceCodecVtbl *lpVtbl;
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
    IAAFMultiEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [ref][in] */ aafUID_constref flavour,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCompressEnable_t compEnable,
    /* [in] */ aafUInt32 numParms,
    /* [size_is][in] */ aafmMultiCreate_t *createParms);


void __RPC_STUB IAAFMultiEssenceCodec_MultiCreate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_MultiOpen_Proxy( 
    IAAFMultiEssenceCodec * This,
    /* [in] */ IAAFSourceMob *fileMob,
    /* [in] */ aafMediaOpenMode_t openMode,
    /* [in] */ IAAFEssenceStream *stream,
    /* [in] */ aafCompressEnable_t compEnable);


void __RPC_STUB IAAFMultiEssenceCodec_MultiOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_WriteBlocks_Proxy( 
    IAAFMultiEssenceCodec * This,
    /* [in] */ aafDeinterleave_t inter,
    /* [in] */ aafUInt16 xferBlockCount,
    /* [size_is][in] */ aafmMultiXfer_t *pTransferParm,
    /* [size_is][out] */ aafmMultiResult_t *pResultParm);


void __RPC_STUB IAAFMultiEssenceCodec_WriteBlocks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFMultiEssenceCodec_ReadBlocks_Proxy( 
    IAAFMultiEssenceCodec * This,
    /* [in] */ aafDeinterleave_t inter,
    /* [in] */ aafUInt16 xferBlockCount,
    /* [size_is][out] */ aafmMultiXfer_t *pTransferParm,
    /* [size_is][out] */ aafmMultiResult_t *pResultParm);


void __RPC_STUB IAAFMultiEssenceCodec_ReadBlocks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFMultiEssenceCodec_INTERFACE_DEFINED__ */


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
            /* [out] */ aafUInt32 *pDefCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDefinitionID( 
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pPluginID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPluginDescriptorID( 
            /* [out] */ aafUID_t *pPluginID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndexedDefinitionObject( 
            /* [in] */ aafUInt32 index,
            /* [in] */ IAAFDictionary *pDictionary,
            /* [out] */ IAAFDefObject **pDefObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDescriptor( 
            /* [in] */ IAAFDictionary *pDictionary,
            /* [out] */ IAAFPluginDef **pPluginDef) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAAFPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAAFPlugin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAAFPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAAFPlugin * This);
        
        HRESULT ( STDMETHODCALLTYPE *CountDefinitions )( 
            IAAFPlugin * This,
            /* [out] */ aafUInt32 *pDefCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedDefinitionID )( 
            IAAFPlugin * This,
            /* [in] */ aafUInt32 index,
            /* [out] */ aafUID_t *pPluginID);
        
        HRESULT ( STDMETHODCALLTYPE *GetPluginDescriptorID )( 
            IAAFPlugin * This,
            /* [out] */ aafUID_t *pPluginID);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndexedDefinitionObject )( 
            IAAFPlugin * This,
            /* [in] */ aafUInt32 index,
            /* [in] */ IAAFDictionary *pDictionary,
            /* [out] */ IAAFDefObject **pDefObject);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDescriptor )( 
            IAAFPlugin * This,
            /* [in] */ IAAFDictionary *pDictionary,
            /* [out] */ IAAFPluginDef **pPluginDef);
        
        END_INTERFACE
    } IAAFPluginVtbl;

    interface IAAFPlugin
    {
        CONST_VTBL struct IAAFPluginVtbl *lpVtbl;
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
    IAAFPlugin * This,
    /* [out] */ aafUInt32 *pDefCount);


void __RPC_STUB IAAFPlugin_CountDefinitions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_GetIndexedDefinitionID_Proxy( 
    IAAFPlugin * This,
    /* [in] */ aafUInt32 index,
    /* [out] */ aafUID_t *pPluginID);


void __RPC_STUB IAAFPlugin_GetIndexedDefinitionID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_GetPluginDescriptorID_Proxy( 
    IAAFPlugin * This,
    /* [out] */ aafUID_t *pPluginID);


void __RPC_STUB IAAFPlugin_GetPluginDescriptorID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_GetIndexedDefinitionObject_Proxy( 
    IAAFPlugin * This,
    /* [in] */ aafUInt32 index,
    /* [in] */ IAAFDictionary *pDictionary,
    /* [out] */ IAAFDefObject **pDefObject);


void __RPC_STUB IAAFPlugin_GetIndexedDefinitionObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAAFPlugin_CreateDescriptor_Proxy( 
    IAAFPlugin * This,
    /* [in] */ IAAFDictionary *pDictionary,
    /* [out] */ IAAFPluginDef **pPluginDef);


void __RPC_STUB IAAFPlugin_CreateDescriptor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAAFPlugin_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


