/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at TIMESTAMP REMOVED
 */
/* Compiler settings for ../ref-impl/include/com-api/AAFPluginTypes.idl:
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

#ifndef __AAFPluginTypes_h__
#define __AAFPluginTypes_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

/* header files for imported files */
#include "wtypes.h"
#include "AAFTypes.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_AAFPluginTypes_0000 */
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
    }	aafDeinterleave_e;

typedef aafInt32 aafCheckVerbose_t;

typedef 
enum _aafCheckVerbose_e
    {	kAAFCheckVerbose	= 0,
	kAAFCheckQuiet	= kAAFCheckVerbose + 1
    }	aafCheckVerbose_e;

typedef aafInt32 aafCheckWarnings_t;

typedef 
enum _aafCheckWarnings_e
    {	kAAFCheckPrintWarnings	= 0,
	kAAFCheckNoWarnings	= kAAFCheckPrintWarnings + 1
    }	aafCheckWarnings_e;

typedef struct  _aafSelectInfo_t
    {
    aafBoolean_t hwAssisted;
    aafBoolean_t isNative;
    aafBoolean_t willHandleMDES;
    aafInt16 relativeLoss;
    aafUInt32 avgBitsPerSec;
    }	aafSelectInfo_t;



extern RPC_IF_HANDLE IAAFPluginTypes_v0_0_c_ifspec;
extern RPC_IF_HANDLE IAAFPluginTypes_v0_0_s_ifspec;
#endif /* __IAAFPluginTypes_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
