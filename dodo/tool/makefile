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

all: tool

BLD_CFG_DIR = ../../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)


tool: $(DODO)

test_only: dodo.test.$(EXE)

run:
	$(SH_PREFIX) ./dodo.test.$(EXE) $(SH_SUFFIX)

.SUFFIXES: .cpp .$(OBJ) .h

INCLUDES = -I../../ref-impl/bld_cfg

DEPEND_INCLUDES = $(INCLUDES) -I/usr/include/CC -I/usr/include/g++

OURCFLAGS = -Dbool=int -Dtrue=1 -Dfalse=0

DODO_COMMON_OBJS = SourceInfo.$(OBJ) TextStream.$(OBJ) ArgDef.$(OBJ) \
	ArgSet.$(OBJ) MacroDef.$(OBJ) MacroSet.$(OBJ)

DODO_APP_OBJS = $(DODO_COMMON_OBJS) DodoMain.$(OBJ)

DODO_TEST_OBJS = $(DODO_COMMON_OBJS) TestMain.$(OBJ) \
	SourceInfoTest.$(OBJ) TextStreamTest.$(OBJ) \
	ArgDefTest.$(OBJ) ArgSetTest.$(OBJ) MacroDefTest.$(OBJ) MacroSetTest.$(OBJ)

DODO_SOURCES = DodoMain.cpp TestMain.cpp \
	SourceInfo.cpp SourceInfoTest.cpp \
	TextStream.cpp TextStreamTest.cpp \
	ArgDef.cpp ArgDefTest.cpp \
	ArgSet.cpp ArgSetTest.cpp \
	MacroDef.cpp MacroDefTest.cpp \
	MacroSet.cpp MacroSetTest.cpp

.cpp.$(OBJ):
	$(CC) -c $(OURCFLAGS) $(INCLUDES) -o $*.$(OBJ) $*.cpp

$(DODO): $(DODO_APP_OBJS)
	$(CC) -o $(DODO) $(LDFLAGS) $(DODO_APP_OBJS)

dodo.test.$(EXE): $(DODO_TEST_OBJS)
	$(CC) -o dodo.test.$(EXE) $(LDFLAGS) $(DODO_TEST_OBJS)

depend:
	makedepend $(DEPEND_INCLUDES) $(DODO_SOURCES)

clean: baseclean
	$(RM) -rf dodo.test.* core *.core makefile.bak test.dod ii_files

# DO NOT DELETE
