#ifndef _aaf_tk_bld_cfg_bld_cfg_h_
#define _aaf_tk_bld_cfg_bld_cfg_h_

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

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
