#------------------------------------------------------------------------------
#
# Advanced Authoring Format
#
# Copyright (c) 2000 Avid Technology, Inc.
#
#
#	This file contains makefile definitions for 
#	the Irix MIPSpro C++ compiler.
#
# Uses:
#	MIPS_ABI -	application binary interface. It's set in including 
#			platform definition file (like pdefs-MipsIrix.mk).
#
# Requires:
#       MIPS_ABI
#
# Sets:
#	CC -		C++ compiler command.
#	ADD_CFLAGS -	compiler options.
#	DBG_FLAGS -	compiler options to generate debug info.
#	XL - 		compiler option to pass an argument to a linker.
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Compiler command
# MIPS_ABI is defined in the platform definition file
#------------------------------------------------------------------------------
CC = CC -$(MIPS_ABI)


#------------------------------------------------------------------------------
# Debug info generation
#------------------------------------------------------------------------------
DBG_FLAGS = -g 


#------------------------------------------------------------------------------
# Compiler flags
# See the explanation of -G 0 in the gp_overflow(5) manpage.
#------------------------------------------------------------------------------
ADD_CFLAGS = -no_auto_include -prelink -xansi -G 0


#------------------------------------------------------------------------------
# This option passes its argument to a linker
#------------------------------------------------------------------------------
XL = -Wl,


