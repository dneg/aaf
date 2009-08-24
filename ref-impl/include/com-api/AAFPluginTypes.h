

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 6.00.0361 */
/* at TIMESTAMP REMOVED
 */
/* Compiler settings for ../ref-impl/include/com-api/AAFPluginTypes.idl:
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


#ifndef __AAFPluginTypes_h__
#define __AAFPluginTypes_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "wtypes.h"
#include "AAFTypes.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_AAFPluginTypes_0000 */
/* [local] */ 

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


extern RPC_IF_HANDLE __MIDL_itf_AAFPluginTypes_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AAFPluginTypes_0000_v0_0_s_ifspec;

#ifndef __IAAFPluginTypes_INTERFACE_DEFINED__
#define __IAAFPluginTypes_INTERFACE_DEFINED__

/* interface IAAFPluginTypes */
/* [auto_handle][unique][uuid] */ 

typedef aafInt32 aafDeinterleave_t;

typedef 
enum _aafDeinterleave_e
    {	kAAFdeinterleave	= 0,
	kAAFleaveInterleaved	= kAAFdeinterleave + 1
    } 	aafDeinterleave_e;

typedef aafInt32 aafCheckVerbose_t;

typedef 
enum _aafCheckVerbose_e
    {	kAAFCheckVerbose	= 0,
	kAAFCheckQuiet	= kAAFCheckVerbose + 1
    } 	aafCheckVerbose_e;

typedef aafInt32 aafCheckWarnings_t;

typedef 
enum _aafCheckWarnings_e
    {	kAAFCheckPrintWarnings	= 0,
	kAAFCheckNoWarnings	= kAAFCheckPrintWarnings + 1
    } 	aafCheckWarnings_e;

typedef struct _aafSelectInfo_t
    {
    aafBoolean_t hwAssisted;
    aafBoolean_t isNative;
    aafBoolean_t willHandleMDES;
    aafInt16 relativeLoss;
    aafUInt32 avgBitsPerSec;
    } 	aafSelectInfo_t;



extern RPC_IF_HANDLE IAAFPluginTypes_v0_0_c_ifspec;
extern RPC_IF_HANDLE IAAFPluginTypes_v0_0_s_ifspec;
#endif /* __IAAFPluginTypes_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


