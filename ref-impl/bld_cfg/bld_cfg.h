#ifndef _bld_cfg_AAFBuildConfig_h_
#define _bld_cfg_AAFBuildConfig_h_

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

 */


#if (__GNUC__ >= 2)
  #define AAF_BUILD_CONFIG_BUILTIN_BOOL	1
#endif

#endif /* ! _bld_cfg_AAFBuildConfig_h_ */
