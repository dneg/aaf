###############################################################################
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
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

#------------------------------------------------------------------------------
#
# pdefs-MipsIrix.mk
#
#	This file contains makefile definitions for the Irix platform.
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
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIB, DLL, BYTE_ORDER,
#	UUIDLIB
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Include common Unix definitions.
#------------------------------------------------------------------------------
include $(AAFBASE)/build/pdefs-Unix.mk


#------------------------------------------------------------------------------
# This is application binary interface type required by the IRIX compiler. 
# It may be one of the following: n32, 64, o32. It is used to generate 
# appropriate compiler and linker options, that's why it appears before
# compiler-specific definitions. For more information see abi(5).
#------------------------------------------------------------------------------
MIPS_ABI = n32


#------------------------------------------------------------------------------
# Compiler-specific definitions
#------------------------------------------------------------------------------
COMPILER ?= MIPSpro
include $(AAFBASE)/build/cdefs-$(COMPILER).mk


#------------------------------------------------------------------------------
# Platform specific compiler options
# SGI native compiler defines the following macros if
# -xansi option is specified. Other compilers need them to be 
# set explicitly.
#------------------------------------------------------------------------------
PLATFORM_CFLAGS = -Dsgi $(MODULE_PLATFORM_CFLAGS)

#------------------------------------------------------------------------------
# Linker command and options
#------------------------------------------------------------------------------
RPATH_OPT = $(XL)-rpath $(XL)$(RPATH)

# Command to link executable.
LD = $(CC) $(XL)-wall

# Command to link static library
ifndef LD_STAT_LIB
    # Note: CC is invoked here to use IRIX specific 
    # compiler option -ar.
    LD_STAT_LIB = CC -$(MIPS_ABI) -ar -o
endif

# Command to link dynamic library
ifndef LD_DYN_LIB
    LD_DYN_LIB = $(CC) -shared $(XL)-wall $(XL)-multigot
endif

# UUID library to use
ifndef UUIDLIB
    UUIDLIB =
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


#------------------------------------------------------------------------------
# Intel machines are Little Endian (lower byte first)
# Mac, HP, SUN, etc. are Big Endian (higher byte first)
# BYTE_ORDER = -DLITTLEENDIAN=1
#------------------------------------------------------------------------------
BYTE_ORDER = -DBIGENDIAN=1

