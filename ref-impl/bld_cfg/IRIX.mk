############################################
#                                          #
# Advanced Authoring Format                #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

#
# Contains elements for SGI/IRIX builds.
#

PTFM_SUFX = ix
OBJ = o${PTFM_SUFX}
LIB = a${PTFM_SUFX}
EXE = x${PTFM_SUFX}
CC = CC
LD = CC
LFLAGS = -Olimit 1026
CFLAGS = -Olimit 1026
OBJFILE = -o $*.${OBJ}
EXEFILE = -o unittest.${EXE}

SH_PREFIX = 
SH_SUFFIX =
