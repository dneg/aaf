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
# The inclusion of depend.mk requires that DEPEND_INCLUDES and SOURCES
# be already defined.
#

PTFM_SUFX = 86
OBJ = o${PTFM_SUFX}
AAF_LIB = a${PTFM_SUFX}
EXE = x${PTFM_SUFX}
CC = gcc
LD = gcc
LFLAGS = -lstdc++
CFLAGS = -Wall -Werror
ARFLAGS = -c -q
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
