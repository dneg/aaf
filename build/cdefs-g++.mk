#------------------------------------------------------------------------------
#
# Advanced Authoring Format
#
# Copyright (c) 2000 Avid Technology, Inc.
#
#
#	This file contains makefile definitions for 
#	the GNU C++ compiler.
#
# Uses:
#	none
#
# Requires:
#	none
#
# Sets:
#	CC -		C++ compiler command.
#	ADD_CFLAGS -	compiler options.
#	DBG_FLAGS -	compiler options to generate debug info.
#	XL -		compiler option to pass an argument to a linker.
#
# Optional:
#	LD_STAT_LIB -	linker command to create a static library.
#			Usually it's set in platform definition file to
#			use compiler as a linker.
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Compiler command
#------------------------------------------------------------------------------
CC = g++


#------------------------------------------------------------------------------
# Debug info generation
#------------------------------------------------------------------------------
DBG_FLAGS = -g 


#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
ADD_CFLAGS = -O3


#------------------------------------------------------------------------------
# This option passes its argument to a linker
# Note: Space is required between this option
#	and its argument
#------------------------------------------------------------------------------
XL = -Xlinker 


#------------------------------------------------------------------------------
# Static library linker command
#------------------------------------------------------------------------------
#LD_STAT_LIB = ar -cr


