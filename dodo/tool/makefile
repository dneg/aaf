############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all: tool

BLD_CFG_DIR = ../../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)


tool: $(DODO)

test_only: dodo.test.$(EXE)

run:
	$(SH_PREFIX) ./dodo.test.$(EXE) $(SH_SUFFIX)

.SUFFIXES: .cpp .$(OBJ) .h

DEPEND_INCLUDES = -I/usr/include/CC -I/usr/include/g++

OURCFLAGS = -Dbool=int -Dtrue=1 -Dfalse=0

DODO_COMMON_OBJS = SourceInfo.$(OBJ) TextLine.$(OBJ) TextStream.$(OBJ) ArgDef.$(OBJ) \
	ArgSet.$(OBJ) MacroDef.$(OBJ) MacroSet.$(OBJ)

DODO_APP_OBJS = $(DODO_COMMON_OBJS) DodoMain.$(OBJ)

DODO_TEST_OBJS = $(DODO_COMMON_OBJS) TestMain.$(OBJ) \
	SourceInfoTest.$(OBJ) TextLineTest.$(OBJ) TextStreamTest.$(OBJ) \
	ArgDefTest.$(OBJ) ArgSetTest.$(OBJ) MacroDefTest.$(OBJ) MacroSetTest.$(OBJ)

DODO_SOURCES = DodoMain.cpp TestMain.cpp \
	SourceInfo.cpp SourceInfoTest.cpp \
	TextLine.cpp TextLineTest.cpp \
	TextStream.cpp TextStreamTest.cpp \
	ArgDef.cpp ArgDefTest.cpp \
	ArgSet.cpp ArgSetTest.cpp \
	MacroDef.cpp MacroDefTest.cpp \
	MacroSet.cpp MacroSetTest.cpp

.cpp.$(OBJ):
	$(CC) -c $(OURCFLAGS) -o $*.$(OBJ) $*.cpp

$(DODO): $(DODO_APP_OBJS)
	$(CC) -o $(DODO) $(LDFLAGS) $(DODO_APP_OBJS)

dodo.test.$(EXE): $(DODO_TEST_OBJS)
	$(CC) -o dodo.test.$(EXE) $(LDFLAGS) $(DODO_TEST_OBJS)

depend:
	makedepend $(DEPEND_INCLUDES) $(DODO_SOURCES)

clean: baseclean
	$(RM) -rf dodo.test.* core *.core makefile.bak test.dod ii_files

# DO NOT DELETE
