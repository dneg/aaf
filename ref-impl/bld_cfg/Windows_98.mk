############################################
#                                          #
# Advanced Authoring Format                #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

#
# Contains elements for Windows 98 builds.
#

#
# Hack! Windows doesn't return the proper uname, so when common.mk
# includes $(UNAME).mk, on WinNT it will only include ".mk", not
# "Windows_NT.mk".  That's why a file called ".mk" exists in this
# directory, which includes this file.
#

PTFM_SUFX = w98
OBJ = obj
AAF_LIB = lib
EXE = exe

CMD_DIR = c:/packages/mksnt
VC_DIR = c:/Progra~1/Micros~8/VC98/bin

RM = ${CMD_DIR}/rm.exe
ECHO = ${CMD_DIR}/echo.exe
CP = ${CMD_DIR}/cp.exe
CAT = ${CMD_DIR}/cat.exe
CHMOD = ${CMD_DIR}/chmod.exe
MV = ${CMD_DIR}/mv.exe
SH = ${CMD_DIR}/sh.exe
TOUCH = ${CMD_DIR}/touch.exe
PERL = ${CMD_DIR}/perl.exe

SHELL = ${CMD_DIR}/sh.exe
MAKE_SUFFIX =

CC = ${VC_DIR}/cl
LD = ${VC_DIR}/cl
AAF_CMD_LINK = ${VC_DIR}/link

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

MIDL = midl
MIDL_SYS_CFLAGS = /server none /client none
