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
AAF_LIB = lib
EXE = exe

CC = cl
LD = cl
AAF_CMD_LINK = link

AAF_LINK_FLAGS = -lib -nologo
AAF_LINK_OUTPUT_FLAG = -out:

LFLAGS = 
CFLAGS = /nologo /Gi- /MDd /W3 /GX /Z7 /Od /Yd /D "WIN32" /D "_WIN32" /D "_DEBUG" /D "_WINDOWS"\
	/D "_UNICODE" /D "UNICODE"
#CFLAGS = /nologo /Gi-
OBJFILE = /Fo$*.${OBJ}
EXEFLAG = /Fo
DLL_LINK_FLAGS = kernel32.lib user32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib\
	/nologo /DLL /subsystem:windows /debug /pdb:none 
ARFLAGS = -r

SH_PREFIX = 
SH_SUFFIX = 
SHELL = c:/mksnt/sh.exe

MIDL = midl
MIDL_SYS_CFLAGS = /server none /client none
