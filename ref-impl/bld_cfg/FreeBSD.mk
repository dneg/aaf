############################################
#                                          #
# Advanced Authoring Format                #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

#
# Contains elements for FreeBSD/i386 builds.
#

PTFM_SUFX = 86
OBJ = o${PTFM_SUFX}
LIB = a${PTFM_SUFX}
EXE = x${PTFM_SUFX}
CC = gcc
LD = gcc
LFLAGS = -lstdc++
CFLAGS = -Wall -Werror
OBJFILE = -o $*.${OBJ}
EXEFILE = -o unittest.${EXE}

SH_PREFIX = 
SH_SUFFIX =
