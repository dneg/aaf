############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : tool targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

tool:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)

run:
	 $(SH_PREIX) cd tool ; $(MAKE) run $(SH_SUFFIX)

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl

DODO_TARGETS = \
	AAFSourceClip.all \
	AAFSourceReference.all \
	AAFComponent.all \
	AAFUnitTest.all \
	AAFCompositionMob.all

targets: $(DODO_TARGETS)

.dod.exp :
	$(SH_PREFIX) $(RM) -f $*.exp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/exp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.exp $(SH_SUFFIX)

.dod.h :
	$(SH_PREFIX) $(RM) -f $*.h $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/h.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.h $(SH_SUFFIX)

.dod.comh :
	$(SH_PREFIX) $(RM) -f $*.comh $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comh $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comh C$*.h $(SH_SUFFIX)

.dod.comc :
	$(SH_PREFIX) $(RM) -f $*.comc $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comc $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comc C$*.cpp $(SH_SUFFIX)

.dod.cpp :
	$(SH_PREFIX) $(RM) -f $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.cpp $(SH_SUFFIX)

.dod.idl :
	$(SH_PREFIX) $(RM) -f $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.idl $(SH_SUFFIX)

clean:
	$(SH_PREFIX) cd tool ; $(MAKE) clean $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.cpp *.h *.idl *.exp *.comc *.comh $(SH_SUFFIX)

AAFUnitTest.all : AAFUnitTest.cpp AAFUnitTest.h
AAFUnitTest.all : AAFUnitTest.comc AAFUnitTest.comh
AAFUnitTest.all : AAFUnitTest.exp AAFUnitTest.idl
AAFUnitTest.exp : AAFUnitTest.dod macros/exp.mac
AAFUnitTest.h : AAFUnitTest.exp macros/h.mac
AAFUnitTest.cpp : AAFUnitTest.exp macros/cpp.mac
AAFUnitTest.idl : AAFUnitTest.exp macros/idl.mac
AAFUnitTest.comc : AAFUnitTest.exp macros/comc.mac
AAFUnitTest.comh : AAFUnitTest.exp macros/comh.mac

AAFComponent.all : AAFComponent.cpp AAFComponent.h
AAFComponent.all : AAFComponent.comc AAFComponent.comh
AAFComponent.all : AAFComponent.exp AAFComponent.idl
AAFComponent.exp : AAFUnitTest.exp macros/exp.mac
AAFComponent.h : AAFComponent.exp macros/h.mac
AAFComponent.cpp : AAFComponent.exp macros/cpp.mac
AAFComponent.idl : AAFComponent.exp macros/idl.mac
AAFComponent.comc : AAFComponent.exp macros/comc.mac
AAFComponent.comh : AAFComponent.exp macros/comh.mac

AAFSourceReference.all : AAFSourceReference.cpp AAFSourceReference.h
AAFSourceReference.all : AAFSourceReference.comc AAFSourceReference.comh
AAFSourceReference.all : AAFSourceReference.exp AAFSourceReference.idl
AAFSourceReference.exp : AAFSourceReference.dod AAFUnitTest.exp macros/exp.mac
AAFSourceReference.h : AAFSourceReference.exp macros/h.mac
AAFSourceReference.cpp : AAFSourceReference.exp macros/cpp.mac
AAFSourceReference.idl : AAFSourceReference.exp macros/idl.mac
AAFSourceReference.comc : AAFSourceReference.exp macros/comc.mac
AAFSourceReference.comh : AAFSourceReference.exp macros/comh.mac

AAFSourceClip.all : AAFSourceClip.cpp AAFSourceClip.h
AAFSourceClip.all : AAFSourceClip.comc AAFSourceClip.comh
AAFSourceClip.all : AAFSourceClip.exp AAFSourceClip.idl
AAFSourceClip.exp : AAFSourceReference.exp AAFUnitTest.exp macros/exp.mac
AAFSourceClip.h : AAFSourceClip.exp macros/h.mac
AAFSourceClip.cpp : AAFSourceClip.exp macros/cpp.mac
AAFSourceClip.idl : AAFSourceClip.exp macros/idl.mac
AAFSourceClip.comc : AAFSourceClip.exp macros/comc.mac
AAFSourceClip.comh : AAFSourceClip.exp macros/comh.mac

AAFCompositionMob.all : AAFCompositionMob.cpp AAFCompositionMob.h
AAFCompositionMob.all : AAFCompositionMob.comc AAFCompositionMob.comh
AAFCompositionMob.all : AAFCompositionMob.exp AAFCompositionMob.idl
AAFCompositionMob.exp : AAFUnitTest.exp macros/exp.mac
AAFCompositionMob.h : AAFCompositionMob.exp macros/h.mac
AAFCompositionMob.cpp : AAFCompositionMob.exp macros/cpp.mac
AAFCompositionMob.idl : AAFCompositionMob.exp macros/idl.mac
AAFCompositionMob.comc : AAFCompositionMob.exp macros/comc.mac
AAFCompositionMob.comh : AAFCompositionMob.exp macros/comh.mac
