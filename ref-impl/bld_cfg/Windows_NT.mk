############################################
#                                          #
# Advanced Authoring Format                #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

#
# Contains elements for Windows NT/i386 builds.
#

#
# Hack! Windows NT doesn't return the proper uname, so when common.mk
# includes $(UNAME).mk, on WinNT it will only include ".mk", not
# "Windows_NT.mk".  That's why a file called ".mk" exists in this
# directory, which includes this file.
#

PTFM_SUFX = nt
OBJ = obj
LIB = lib
EXE = exe
CC = cl
LD = cl
LFLAGS = 
CFLAGS = /nologo /Gi-
OBJFILE = /Fo$*.${OBJ}
EXEFLAG = /Fo

SH_PREFIX = 
SH_SUFFIX = 
SHELL = c:/mksnt/sh

IDL = midl
