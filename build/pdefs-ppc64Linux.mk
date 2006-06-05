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
# pdefs-ppc64Linux.mk
#
#	This file contains makefile definitions for the Linux platform.
#
#
# Uses:
#	COMPILER, XL, RPATH, CC
#	
# Requires:
#	XL, RPATH, CC
#
# Sets:
#	MIPS_ABI, COMPILER, PLATFORM_CFLAGS, RPATH_OPT, 
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIB, DLL,
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Include common Unix definitions.
#------------------------------------------------------------------------------
include $(AAFBASE)/build/pdefs-Unix.mk


#------------------------------------------------------------------------------
# Compiler-specific definitions
#------------------------------------------------------------------------------
COMPILER ?= g++
include $(AAFBASE)/build/cdefs-$(COMPILER).mk


#------------------------------------------------------------------------------
# Platform specific compiler options
#------------------------------------------------------------------------------
PLATFORM_CFLAGS =

# turn on PPC64 specific optimisations for Release builds
REL_FLAGS += -mcpu=G5 -mpowerpc64


# Platform specific compiler options
# Large File Support definitions (see libc documentation)
#
# _LARGEFILE_SOURCE   - declares the functions fseeko() and ftello()
# _LARGEFILE64_SOURCE - enables 32 bit systems to use files larger than 2GB.
# _FILE_OFFSET_BITS   - determines which file system interface shall be used.
PLATFORM_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

# Platform specific compiler options
#
# When using glibc-2.1.2 defining _LARGEFILE_SOURCE (as libc documentation
# suggests) is not enough to get fseeko() and ftello() declared. To have
# fseeko/ftello we must request Single Unix Specification, version 2
# definitions which is done by defining the macro _XOPEN_SOURCE having the
# value of 500.
# Note that for the later versions of the library, like glibc-2.2.2 for
# example, _LARGEFILE_SOURCE will do the job.
PLATFORM_CFLAGS += -D_XOPEN_SOURCE=500


#------------------------------------------------------------------------------
# Linker command and options
#------------------------------------------------------------------------------
RPATH_OPT = $(XL)-rpath $(XL)$(RPATH)

# Command to link executable.
LD = $(CC) -ldl -rdynamic

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

# For GNU/Linux the UUID library is provided by e2fsprogs
ifndef UUIDLIB
    UUIDLIB_PATH := $(shell for f in /usr/local/lib /usr/lib /lib /usr/lib64 /lib64; do test -e $$f/libuuid.a && echo $$f && break; done)
    ifeq "$(UUIDLIB_PATH)" ""
        $(error Required library libuuid.a not found - install the e2fsprogs development libraries)
    endif

    UUIDLIB = -luuid
endif

# For GNU/Linux always attempt to use the libdv library.
# If this detection fails, LIBDV_PATH can be passed on the make command line.
ifndef LIBDV_PATH
	TMP_LIBDV_PATH := $(shell for f in /usr/local/lib /usr/lib /lib /usr/lib64 /lib64; do test -e $$f/libdv.a && echo $$f && break; done)
	ifneq "$(TMP_LIBDV_PATH)" ""
		LIBDV_PATH = $(TMP_LIBDV_PATH)
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
EXE ?= 
LIB ?= .a
DLL ?= .so

