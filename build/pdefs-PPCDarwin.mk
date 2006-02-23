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
# pdefs-PPCDarwin.mk
#
#	This file contains makefile definitions for the Darwin (Mac OS X)
#	platform.
#
#
# Uses:
#	COMPILER, XL, RPATH, CC
#
# Requires:
#	XL, RPATH, CC
#
# Sets:
#	COMPILER, PLATFORM_CFLAGS, RPATH_OPT,
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIB, DLL,
#	UUIDLIB, PLATFORMLIBS
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Include common Unix definitions.
#------------------------------------------------------------------------------
include $(AAFBASE)/build/pdefs-Unix.mk

## Now redifine copy so we don't blow out the TOC on static .a libs.
CP = ditto

#------------------------------------------------------------------------------
# Compiler-specific definitions
#------------------------------------------------------------------------------
COMPILER ?= g++
include $(AAFBASE)/build/cdefs-$(COMPILER).mk
CC = $(COMPILER)

#------------------------------------------------------------------------------
# Platform specific compiler options
#------------------------------------------------------------------------------
PLATFORM_CFLAGS = -arch ppc


#------------------------------------------------------------------------------
# Linker command and options
#------------------------------------------------------------------------------
RPATH_OPT =

# Command to link executable.
LD = $(COMPILER)

# Command to link static library
ifndef LD_STAT_LIB
    LD_STAT_LIB = libtool -static -o
endif

# Command to link dynamic library
ifndef LD_DYN_LIB
    LD_DYN_LIB = $(COMPILER) -dynamiclib
endif

# UUID library to use
# CoreFoundation contains definitions for CFUUIDCreate(), CFUUIDGetUUIDBytes()
# UUIDLIB is currently only used for static builds (AAFTARGET=Debug-static)
ifndef UUIDLIB
    UUIDLIB = -framework CoreFoundation
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
DLL ?= .dylib


#------------------------------------------------------------------------------
#  Get OS revision and add version specific libs as needed.
#------------------------------------------------------------------------------
DARWIN_REV = $(shell uname -r  | cut -c 1 )
ifeq ($(DARWIN_REV),6)
    PLATFORMLIBS = -ldl -lwchar
else
    # assuming >6
    PLATFORMLIBS =
endif
