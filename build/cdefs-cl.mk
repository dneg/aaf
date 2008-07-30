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
# cdefs-cl.mk
#
#	This file contains makefile definitions for 
#	Microsoft's Visual C++ Compiler
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
CC = cl
CCVERSION = $(shell cl 2>&1 | head -1 | sed 's/.*Version \([^ ]*\).*/\1/')
MSVCVERSION = $(shell echo $(CCVERSION) | sed -e 's/^12\..*/vs6/' -e 's/^13\..*/vs7/' -e 's/^14\..*/vs8/' -e 's/^15\..*/vs9/')

# Header dependencies are taken care of by PDB or precompiled headers.
# Therefore don't generate dependencies to be included by make.
GENDEPS=0

#------------------------------------------------------------------------------
# Debug info generation
#------------------------------------------------------------------------------
DBG_FLAGS = /nologo /EHsc /RTC1 /MDd /W3 /WX /Z7 /Gs /GR /D "_DEBUG" /D "OM_DEBUG" /D "OM_STACK_TRACE_ON_ASSERT" /D "OM_USE_SCHEMASOFT_SS" /D "OM_USE_WINDOWS_SS" /D "_WIN32" /D "WIN32" /D "_CONSOLE" -DUNICODE /D "OM_USE_STORAGE_EX" /D "COM_NO_WINDOWS_H" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_SECURE_NO_WARNINGS" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "_SECURE_SCL=0"

#------------------------------------------------------------------------------
# Optimisation flags when building a Release
#------------------------------------------------------------------------------
REL_FLAGS = /nologo /EHsc /MD /O2 /W3 /WX /Z7 /Gs /GR /D "NDEBUG" /D "OM_USE_SCHEMASOFT_SS" /D "OM_USE_WINDOWS_SS" /D "_WIN32" /D "WIN32" /D "_CONSOLE" -DUNICODE /D "OM_USE_STORAGE_EX" /D "COM_NO_WINDOWS_H" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_SECURE_NO_WARNINGS" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "_SECURE_SCL=0"

#------------------------------------------------------------------------------
# Compiler flags
#
#------------------------------------------------------------------------------
ADD_CFLAGS =
