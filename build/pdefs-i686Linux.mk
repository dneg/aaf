#------------------------------------------------------------------------------
#
# Advanced Authoring Format
#
# Copyright (c) 2000 Avid Technology, Inc.
#
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
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIB, DLL, BYTE_ORDER
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Include common Unix definitions.
#------------------------------------------------------------------------------
include $(AAFBASE)/build/pdefs-i586Linux.mk


