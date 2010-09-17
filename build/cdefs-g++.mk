###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
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
CCVERSION = $(shell g++ -dumpversion)

#------------------------------------------------------------------------------
# gcc 4.x always gives warnings about non-virtual destructor, e.g.
#   warning: 'struct IUnknown' has virtual functions but non-virtual destructor
# This warning cannot be fixed by providing an empty virtual destructor since
# it will change the binary substitutability of the libcom-api.so library.
# Since binary substitutability is a requirement of the SDK we will have to
# live with this warning forever or supress it using
#   -Wno-non-virtual-dtor
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Debug info generation
#------------------------------------------------------------------------------
DBG_FLAGS = -g -Wall -Wundef -Wno-non-virtual-dtor -Werror -Wno-reorder -Wno-unused

#------------------------------------------------------------------------------
# Optimisation flags when building a Release
#------------------------------------------------------------------------------
REL_FLAGS = -O2 -Wall -Wundef -Wno-non-virtual-dtor -Werror -Wno-reorder -Wno-unused

# Turn off no-strict-aliasing warning as a workaround for g++ 4.1.x and above
# "dereferencing type-punned pointer will break strict-aliasing rules" warnings.
# This warning is available only when optimization level -O2 or above is used.
# g++ 4.1.x gives this warning when a pointer is cast to (void**) since
# according to ISO C99 the two types are incompatible.  The correct fix for
# the SDK is to change the APIs involved to pass (void*) instead of (void**).
REL_FLAGS += -Wno-strict-aliasing

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
ADD_CFLAGS += -fsigned-char


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
