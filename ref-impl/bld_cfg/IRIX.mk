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
AAF_LIB = a${PTFM_SUFX}
EXE = x${PTFM_SUFX}

CC = CC
LD = CC
AAF_CMD_LINK = ld

AAF_LINK_FLAGS =
AAF_LINK_OUTPUT_FLAG = -o
LFLAGS = -Olimit 1026
CFLAGS = -Olimit 1026
ARFLAGS = -r
OBJFILE = -o $*.${OBJ}
EXEFILE = -o unittest.${EXE}
CP = cp

SH_PREFIX = 
SH_SUFFIX =


#
# Some platforms (which do their own makedepend generation) may
# require that DEPEND_INCLUDES and SOURCES be already defined in order
# for depend.mk to function properly.  That should not be a problem if
# Clearcase dependency checking is used.
#
include $(BLD_CFG_DIR)/depend.mk
