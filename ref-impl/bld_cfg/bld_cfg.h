#ifndef _aaf_tk_bld_cfg_bld_cfg_h_
#define _aaf_tk_bld_cfg_bld_cfg_h_

/******************************************\
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

AAF_BUILD_CONFIG_BUILTIN_BOOL
	- is defined to be non-zero if this platform/compiler supports
	  a built-in bool type.

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


/*
 * So far the following compilers don't have bool:
 *  SGI
 *  MS VC++ 4.2 or earlier (_MSC_VER == 1020 for 4.2)
 */
#if ! (__sgi || (_MSC_VER <= 1020))
  #define AAF_BUILD_CONFIG_BUILTIN_BOOL	1
#endif

/*
 * However! some compilers which don't have bool have the infuriating
 * characteristic that they still reserve the bool keyword.  Most
 * notably, this occurs in MSVC++4.2.  Of course, all compilers which
 * _do_ have bool also have the keyword reserved.
 */
#if ! (__sgi)
  #define AAF_BUILD_CONFIG_BOOL_RESERVED 1
#endif

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
