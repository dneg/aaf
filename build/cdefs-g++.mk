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
# cdefs-g++.mk
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
#
#------------------------------------------------------------------------------
ADD_CFLAGS =

#------------------------------------------------------------------------------
# Compiler flags
# 
# Make all char declarations to be signed char declarations.
#------------------------------------------------------------------------------
ADD_CFLAGS += -fsigned-char -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64


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


