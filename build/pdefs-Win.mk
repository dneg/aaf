###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
#
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

#------------------------------------------------------------------------------
#
# pdefs-Win.mk
#
#	This file contains makefile definitions for the WinNT platform family,
#	and is intended to support native g++ (MinGW) and MSVC builds.
#
#
# Uses:
#	COMPILER, CC
#	
# Requires:
#	CC
#
# Sets:
#	MIPS_ABI, COMPILER, PLATFORM_CFLAGS,
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIBEXT, DLL,
#	UUIDLIB
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Include common Unix definitions.
#------------------------------------------------------------------------------
include $(AAFBASE)/build/pdefs-Unix.mk
SHELL = /bin/sh

#------------------------------------------------------------------------------
# Compiler-specific definitions
#------------------------------------------------------------------------------
COMPILER ?= cl
include $(AAFBASE)/build/cdefs-$(COMPILER).mk


#------------------------------------------------------------------------------
# Platform specific compiler options
#------------------------------------------------------------------------------
PLATFORM_CFLAGS = -DUNICODE -D_UNICODE

# FIXME: Once the ULONG* OMUInt32* incompatibility when building OM is fixed,
# assign USE_SS=1 and add -DOM_USE_WINDOWS_SS to PLATFORM_CFLAGS.
# The MS ole32 Structured Storage implementation will then be available when
# building with g++.

#------------------------------------------------------------------------------
# Linker command and options
#------------------------------------------------------------------------------

# Command to link executable.
LD = $(CC) -ldl -rdynamic
LD_STAT_OUTPUT = $@
LD_DYN_OUTPUT = -o $@

# Command to link static library
ifndef LD_STAT_LIB
    # Note: CC is invoked here to use IRIX specific 
    # compiler option -ar.
    LD_STAT_LIB = ar -cr 
endif

# Command to link dynamic library
ifndef LD_DYN_LIB
    LD_DYN_LIB = $(CC) -shared
endif

# UUID library to use
ifndef UUIDLIB
  ifeq ($(COMPILER),g++)
    UUIDLIB = -lole32 -luuid
  endif
endif


#------------------------------------------------------------------------------
# Select UNICODE or ansi API's:
#   U_OPTS=use_unicode
#   U_OPTS=no_unicode
#------------------------------------------------------------------------------
U_OPTS=no_unicode


#------------------------------------------------------------------------------
# Binary file extensions
#------------------------------------------------------------------------------
OBJ ?= .o
EXE ?= .exe
LIBEXT ?= .a
DLL ?= .dll

# The Microsoft MSVC++ compiler (cl) is very different to other compilers and linkers
# and uses different commands to compile, link static libs and link DLLs.
ifeq ($(COMPILER),cl)
    LD_STAT_LIB = lib.exe /NOLOGO
    LD_STAT_OUTPUT = /OUT:$@
    LD_DEF = /DEF:
    LD_DYN_OUTPUT = /OUT:$@

    ifeq ($(AAFTARGET),Release)
      MSVC_LINK_OPT = /NODEFAULTLIB:"msvcrtd.lib"
    else
      MSVC_LINK_OPT = /DEBUG /NODEFAULTLIB:"msvcrt.lib"
    endif

    LD_DYN_LIB = link.exe /NOLOGO /DLL /INCREMENTAL:NO /SUBSYSTEM:CONSOLE /MACHINE:X86 /NODEFAULTLIB:"libc.lib" /NODEFAULTLIB:"libcmt.lib" $(MSVC_LINK_OPT) /NODEFAULTLIB:"libcd.lib" /NODEFAULTLIB:"libcmtd.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
    LD = link.exe /NOLOGO /INCREMENTAL:NO /SUBSYSTEM:CONSOLE /MACHINE:X86 /NODEFAULTLIB:"libc.lib" /NODEFAULTLIB:"libcmt.lib" $(MSVC_LINK_OPT) /NODEFAULTLIB:"libcd.lib" /NODEFAULTLIB:"libcmtd.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
    OBJ = .obj
    LIBEXT = .lib
	LINK_AAF_APP = $(AAFSDKLIBDIR)/libaaflib$(LIBEXT) $(AAFSDKLIBDIR)/libaafiid$(LIBEXT) /OUT:$@
endif


#----------------------------------------------------------
# APP_RUN_ENV - Command to setup environment for running
# an AAF application from the command line (by setting up
# paths to dynamically loaded AAF libraries).
#----------------------------------------------------------
APP_RUN_ENV = env PATH=$(AAFSDKBINDIR):"$(PATH)"
