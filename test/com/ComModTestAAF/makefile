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

all: app

BLD_CFG_DIR = ../../ref-impl/bld_cfg

SOURCES = \
	aafcomtst.cpp

OBJECTS = \
	aafcomtst.obj

INCLUDES = -I. -I../../ref-impl/include/com-api \
	-I../../ref-impl/src/com-api/com-dll

# DEPEND_INCLUDES = ${INCLUDES} -I/usr/include/CC -I/usr/include/g++
DEPEND_INCLUDES = ${INCLUDES}



#
# Some platforms (which do their own makedepend generation) may
# require that DEPEND_INCLUDES and SOURCES be already defined when you
# include $(BLD_CFG_DIR)/common.mk .  Examine the
# $(BLD_CFG_DIR)/$(UNAME).mk file for each platform to be sure.
#
include $(BLD_CFG_DIR)/common.mk


app : aafcomtst.$(EXE)


# LINK_FLAGS = kernel32.lib advapi32.lib ole32.lib uuid.lib \
# 	/out: $(DLL_LINK_FLAGS) /def:AAFCOM.DEF 

LINK_FLAGS = ole32.lib

.cpp.${OBJ}:
	${CC} -c ${INCLUDES} -DWIN32=1 -GX $*.cpp

aafcomtst.$(EXE) : $(OUTDIR) $(DEF_FILE) $(OBJECTS)
	$(LD) $(LINK_FLAGS) $(OBJECTS)

clean: baseclean

depend: basedepend
