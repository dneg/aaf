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

#
# Contains elements for Linux builds.
#

PTFM_SUFX = ix
OBJ = o${PTFM_SUFX}
AAF_LIB = a${PTFM_SUFX}
EXE = x${PTFM_SUFX}

RM = rm
ECHO = echo -e
CP = cp
CAT = cat
CHMOD = chmod
MV = mv
SH = sh
TOUCH = touch
PERL = perl

CC = gcc
LD = gcc
AAF_CMD_LINK = ld

AAF_LINK_FLAGS = 
AAF_LINK_OUTPUT_FLAG = -o
LFLAGS = -Olimit 1026
# CFLAGS = -Wall -Werror
CFLAGS = -Werror
ARFLAGS = -r
OBJFILE = -o $*.${OBJ}
EXEFILE = -o unittest.${EXE}
CP = cp
MAKE_SUFFIX =

UNICODE = 1
SH_PREFIX = 
SH_SUFFIX =


#
# Some platforms (which do their own makedepend generation) may
# require that DEPEND_INCLUDES and SOURCES be already defined in order
# for depend.mk to function properly.  That should not be a problem if
# Clearcase dependency checking is used.
#
include $(BLD_CFG_DIR)/depend.mk


