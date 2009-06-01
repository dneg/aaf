#ifndef _aaf_tk_bld_cfg_bld_cfg_h_
#define _aaf_tk_bld_cfg_bld_cfg_h_

//=---------------------------------------------------------------------=
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

/*
 * This file contains macro definitions to be used for cross-platform
 * issues.  Users should not have to edit this file; all
 * AAF_BUILD_CONFIG_* macro definitions are automatically made
 * depending on pre-defined compiler macros.
 */

/*

Macros defined here:

AAF_BUILD_CONFIG_ASSY_POWERPC
	- is defined to be non-zero if this platform supports PowerPC
	  assembly code.

AAF_BUILD_CONFIG_ASSY_68K
	- is defined to be non-zero if this platform supports 68000
	  assembly code.

AAF_BUILD_CONFIG_ASSY_68K2
	- is defined to be non-zero if this platform supports 68020
	  assembly code.

AAF_BUILD_CONFIG_ASSY_X86
	- is defined to be non-zero if this platform supports Intel
	  x86 assembly code.

AAF_BUILD_CONFIG_ASSY_386
	- is defined to be non-zero if this platform supports Intel
	  386 assembly code.

AAF_BUILD_CONFIG_EXTERN_C_HEADER
	- Evaluates to the appropriate "extern C" lead statement for
	  shared C and C++ header files.  Should be put at the top of
	  such a header file.

AAF_BUILD_CONFIG_EXTERN_C_TRAILER
	- Evaluates to the appropriate "extern C" trailing statement
	  for shared C and C++ header files.  Should be put at the
	  bottom of such a header file.

AAF_BUILD_CONFIG_EXPLICIT_TEMPLATES
	- Evaluates to true if this platform requires explicit template
	  instantiation.

 */


#if __cplusplus
  #define AAF_BUILD_CONFIG_CPLUSPLUS 1
#endif

#if AAF_BUILD_CONFIG_CPLUSPLUS
  #define AAF_BUILD_CONFIG_EXTERN_C_HEADER	extern "C" {
  #define AAF_BUILD_CONFIG_EXTERN_C_TRAILER	}
#else /* ! AAF_BUILD_CONFIG_CPLUSPLUS */
  #define AAF_BUILD_CONFIG_EXTERN_C_HEADER
  #define AAF_BUILD_CONFIG_EXTERN_C_TRAILER
#endif /* ! AAF_BUILD_CONFIG_CPLUSPLUS */


#if __GNUC__
  #define AAF_BUILD_CONFIG_EXPLICIT_TEMPLATES 1
#endif

#endif /* ! _aaf_tk_bld_cfg_bld_cfg_h_ */



///////////////////////////
// Emacs formatting info //
///////////////////////////


/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
