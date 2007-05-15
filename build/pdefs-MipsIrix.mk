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
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIBEXT, DLL,
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
# 
# g++ is supported in addition to MIPSpro which is the default compiler.
# To select g++ use "make COMPILER=g++"
#------------------------------------------------------------------------------
COMPILER ?= MIPSpro
include $(AAFBASE)/build/cdefs-$(COMPILER).mk


#------------------------------------------------------------------------------
# Platform specific compiler options
# SGI native compiler defines the following macros if
# -xansi option is specified. Other compilers need them to be 
# set explicitly.
#------------------------------------------------------------------------------
#
# 1152 - variable set but never referenced
# 1174 - variable declared but never referenced
# 1107 - A signed bit field has a length of 1 bit.
# 1681 - virtual function override intended?
# 1460 - function decalred inline after being called
# 1047 - macro has an incompatible redefinition
# 1183 - unsigned integer compared to zero

PLATFORM_CFLAGS = -Dsgi -woff 1552,1174,1107,1681,1460,1047,1183 $(MODULE_PLATFORM_CFLAGS) 

# iostream support on Irix is provided by libCio
# Include this as part of the link command.
LIBCIO = -lCio

#------------------------------------------------------------------------------
# Linker command and options
#------------------------------------------------------------------------------
RPATH_OPT = $(XL)-rpath $(XL)$(RPATH)

# Command to link executable.
LD = $(CC) $(XL)-wall $(MODULE_PLATFORM_LDFLAGS)
LD_STAT_OUTPUT = $@
LD_DYN_OUTPUT = -o $@

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

#-------------------------------------------------------
# g++ specific overrides when invoked with COMPILER=g++
#-------------------------------------------------------
ifeq "$(COMPILER)" "g++"
    PLATFORM_CFLAGS = -fPIC
    LD_DYN_LIB = $(CC) -Xlinker -LD_LAYOUT:lgot_buffer=50 -fPIC -shared -g
    LIBCIO =
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
LIBEXT ?= .a
DLL ?= .so


