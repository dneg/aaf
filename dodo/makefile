############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : dodo-app targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)

run:
	 $(SH_PREIX) cd tool ; $(MAKE) run $(SH_SUFFIX)

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .implc .implh

DODO_TARGETS = \
	AAFComponent.all \
	AAFCompositionMob.all \
	AAFInt64.all \
	AAFLocator.all \
	AAFMediaDescriptor.all \
	AAFObject.all \
	AAFSegment.all \
	AAFSourceClip.all \
	AAFSourceReference.all \
	AAFString.all \
	AAFUInt64.all \
	AAFUnitTest.all \
	AAFUnixLocator.all \
	AAFWindowsLocator.all

# Jeff hasn't finished with these yet
#
#	AAFConstValue.all
#	AAFMedia.all
#	AAFMediaFileDescriptor.all
#	AAFMediaFilmDescriptor.all
#	AAFMediaGroup.all

targets: $(DODO_TARGETS)

SRC_DIR = ../ref-impl/src

INCLUDE_DIR = ../ref-impl/include

.dod.exp :
	$(SH_PREFIX) $(RM) -f $*.exp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/exp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.exp $(SH_SUFFIX)

.dod.h :
	$(SH_PREFIX) $(RM) -f $*.h $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/h.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.h $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.h $(INCLUDE_DIR) $(SH_SUFFIX)

.dod.comh :
	$(SH_PREFIX) $(RM) -f $*.comh $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comh $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comh $(SRC_DIR)/com-api/C$*.h $(SH_SUFFIX)

.dod.comc :
	$(SH_PREFIX) $(RM) -f $*.comc $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comc $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comc $(SRC_DIR)/com-api/C$*.cpp $(SH_SUFFIX)

.dod.implh :
	$(SH_PREFIX) $(RM) -f $*.implh $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.implh $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.implh $(SRC_DIR)/impl/Impl$*.h $(SH_SUFFIX)

.dod.implc :
	$(SH_PREFIX) $(RM) -f $*.implc $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.implc $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.implc $(SRC_DIR)/impl/Impl$*.cpp $(SH_SUFFIX)

.dod.cpp :
	$(SH_PREFIX) $(RM) -f $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.cpp $(SRC_DIR)/cpp-api $(SH_SUFFIX)

.dod.idl :
	$(SH_PREFIX) $(RM) -f $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.idl $(INCLUDE_DIR) $(SH_SUFFIX)

clean:
	$(SH_PREFIX) cd tool ; $(MAKE) clean $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.cpp *.h *.idl *.exp *.comc *.comh *.implc *.implh $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/cpp-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/C*.{cpp,h} $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/Impl*.{cpp,h} $(SH_SUFFIX)

AAFComponent.all : AAFComponent.cpp AAFComponent.h
AAFComponent.all : AAFComponent.comc AAFComponent.comh
AAFComponent.all : AAFComponent.exp AAFComponent.idl
AAFComponent.all : AAFComponent.implc AAFComponent.implh
AAFComponent.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFComponent.h : AAFComponent.exp macros/h.mac
AAFComponent.cpp : AAFComponent.exp macros/cpp.mac
AAFComponent.idl : AAFComponent.exp macros/idl.mac
AAFComponent.comc : AAFComponent.exp macros/comc.mac
AAFComponent.comh : AAFComponent.exp macros/comh.mac
AAFComponent.implc : AAFComponent.exp macros/implc.mac
AAFComponent.implh : AAFComponent.exp macros/implh.mac

AAFCompositionMob.all : AAFCompositionMob.cpp AAFCompositionMob.h
AAFCompositionMob.all : AAFCompositionMob.comc AAFCompositionMob.comh
AAFCompositionMob.all : AAFCompositionMob.exp AAFCompositionMob.idl
AAFCompositionMob.all : AAFCompositionMob.implc AAFCompositionMob.implh
AAFCompositionMob.exp : AAFUnitTest.exp macros/exp.mac
AAFCompositionMob.h : AAFCompositionMob.exp macros/h.mac
AAFCompositionMob.cpp : AAFCompositionMob.exp macros/cpp.mac
AAFCompositionMob.idl : AAFCompositionMob.exp macros/idl.mac
AAFCompositionMob.comc : AAFCompositionMob.exp macros/comc.mac
AAFCompositionMob.comh : AAFCompositionMob.exp macros/comh.mac
AAFCompositionMob.implc : AAFCompositionMob.exp macros/implc.mac
AAFCompositionMob.implh : AAFCompositionMob.exp macros/implh.mac

AAFConstValue.all : AAFConstValue.cpp AAFConstValue.h
AAFConstValue.all : AAFConstValue.comc AAFConstValue.comh
AAFConstValue.all : AAFConstValue.exp AAFConstValue.idl
AAFConstValue.all : AAFConstValue.implc AAFConstValue.implh
AAFConstValue.exp : AAFUnitTest.exp macros/exp.mac
AAFConstValue.h : AAFConstValue.exp macros/h.mac
AAFConstValue.cpp : AAFConstValue.exp macros/cpp.mac
AAFConstValue.idl : AAFConstValue.exp macros/idl.mac
AAFConstValue.comc : AAFConstValue.exp macros/comc.mac
AAFConstValue.comh : AAFConstValue.exp macros/comh.mac
AAFConstValue.implc : AAFConstValue.exp macros/implc.mac
AAFConstValue.implh : AAFConstValue.exp macros/implh.mac

AAFInt64.all : AAFInt64.cpp AAFInt64.h
AAFInt64.all : AAFInt64.comc AAFInt64.comh
AAFInt64.all : AAFInt64.exp AAFInt64.idl
AAFInt64.all : AAFInt64.implc AAFInt64.implh
AAFInt64.exp : AAFUnitTest.exp macros/exp.mac
AAFInt64.h : AAFInt64.exp macros/h.mac
AAFInt64.cpp : AAFInt64.exp macros/cpp.mac
AAFInt64.idl : AAFInt64.exp macros/idl.mac
AAFInt64.comc : AAFInt64.exp macros/comc.mac
AAFInt64.comh : AAFInt64.exp macros/comh.mac
AAFInt64.implc : AAFInt64.exp macros/implc.mac
AAFInt64.implh : AAFInt64.exp macros/implh.mac

AAFLocator.all : AAFLocator.cpp AAFLocator.h
AAFLocator.all : AAFLocator.comc AAFLocator.comh
AAFLocator.all : AAFLocator.exp AAFLocator.idl
AAFLocator.all : AAFLocator.implc AAFLocator.implh
AAFLocator.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFLocator.h : AAFLocator.exp macros/h.mac
AAFLocator.cpp : AAFLocator.exp macros/cpp.mac
AAFLocator.idl : AAFLocator.exp macros/idl.mac
AAFLocator.implc : AAFLocator.exp macros/implc.mac
AAFLocator.implh : AAFLocator.exp macros/implh.mac

AAFMedia.all : AAFMedia.cpp AAFMedia.h
AAFMedia.all : AAFMedia.comc AAFMedia.comh
AAFMedia.all : AAFMedia.exp AAFMedia.idl
AAFMedia.all : AAFMedia.implc AAFMedia.implh
AAFMedia.exp : AAFUnitTest.exp macros/exp.mac
AAFMedia.h : AAFMedia.exp macros/h.mac
AAFMedia.cpp : AAFMedia.exp macros/cpp.mac
AAFMedia.idl : AAFMedia.exp macros/idl.mac
AAFMedia.comc : AAFMedia.exp macros/comc.mac
AAFMedia.comh : AAFMedia.exp macros/comh.mac
AAFMedia.implc : AAFMedia.exp macros/implc.mac
AAFMedia.implh : AAFMedia.exp macros/implh.mac

AAFMediaDescriptor.all : AAFMediaDescriptor.cpp AAFMediaDescriptor.h
AAFMediaDescriptor.all : AAFMediaDescriptor.comc AAFMediaDescriptor.comh
AAFMediaDescriptor.all : AAFMediaDescriptor.exp AAFMediaDescriptor.idl
AAFMediaDescriptor.all : AAFMediaDescriptor.implc AAFMediaDescriptor.implh
AAFMediaDescriptor.exp : AAFUnitTest.exp macros/exp.mac
AAFMediaDescriptor.h : AAFMediaDescriptor.exp macros/h.mac
AAFMediaDescriptor.cpp : AAFMediaDescriptor.exp macros/cpp.mac
AAFMediaDescriptor.idl : AAFMediaDescriptor.exp macros/idl.mac
AAFMediaDescriptor.comc : AAFMediaDescriptor.exp macros/comc.mac
AAFMediaDescriptor.comh : AAFMediaDescriptor.exp macros/comh.mac
AAFMediaDescriptor.implc : AAFMediaDescriptor.exp macros/implc.mac
AAFMediaDescriptor.implh : AAFMediaDescriptor.exp macros/implh.mac

AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.cpp AAFMediaFileDescriptor.h
AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.comc AAFMediaFileDescriptor.comh
AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.exp AAFMediaFileDescriptor.idl
AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.implc AAFMediaFileDescriptor.implh
AAFMediaFileDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac
AAFMediaFileDescriptor.h : AAFMediaFileDescriptor.exp macros/h.mac
AAFMediaFileDescriptor.cpp : AAFMediaFileDescriptor.exp macros/cpp.mac
AAFMediaFileDescriptor.idl : AAFMediaFileDescriptor.exp macros/idl.mac
AAFMediaFileDescriptor.comc : AAFMediaFileDescriptor.exp macros/comc.mac
AAFMediaFileDescriptor.comh : AAFMediaFileDescriptor.exp macros/comh.mac
AAFMediaFileDescriptor.implc : AAFMediaFileDescriptor.exp macros/implc.mac
AAFMediaFileDescriptor.implh : AAFMediaFileDescriptor.exp macros/implh.mac

AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.cpp AAFMediaFilmDescriptor.h
AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.comc AAFMediaFilmDescriptor.comh
AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.exp AAFMediaFilmDescriptor.idl
AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.implc AAFMediaFilmDescriptor.implh
AAFMediaFilmDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac
AAFMediaFilmDescriptor.h : AAFMediaFilmDescriptor.exp macros/h.mac
AAFMediaFilmDescriptor.cpp : AAFMediaFilmDescriptor.exp macros/cpp.mac
AAFMediaFilmDescriptor.idl : AAFMediaFilmDescriptor.exp macros/idl.mac
AAFMediaFilmDescriptor.comc : AAFMediaFilmDescriptor.exp macros/comc.mac
AAFMediaFilmDescriptor.comh : AAFMediaFilmDescriptor.exp macros/comh.mac
AAFMediaFilmDescriptor.implc : AAFMediaFilmDescriptor.exp macros/implc.mac
AAFMediaFilmDescriptor.implh : AAFMediaFilmDescriptor.exp macros/implh.mac

AAFMediaGroup.all : AAFMediaGroup.cpp AAFMediaGroup.h
AAFMediaGroup.all : AAFMediaGroup.comc AAFMediaGroup.comh
AAFMediaGroup.all : AAFMediaGroup.exp AAFMediaGroup.idl
AAFMediaGroup.all : AAFMediaGroup.implc AAFMediaGroup.implh
AAFMediaGroup.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac
AAFMediaGroup.h : AAFMediaGroup.exp macros/h.mac
AAFMediaGroup.cpp : AAFMediaGroup.exp macros/cpp.mac
AAFMediaGroup.idl : AAFMediaGroup.exp macros/idl.mac
AAFMediaGroup.comc : AAFMediaGroup.exp macros/comc.mac
AAFMediaGroup.comh : AAFMediaGroup.exp macros/comh.mac
AAFMediaGroup.implc : AAFMediaGroup.exp macros/implc.mac
AAFMediaGroup.implh : AAFMediaGroup.exp macros/implh.mac

AAFObject.all : AAFObject.cpp AAFObject.h
AAFObject.all : AAFObject.comc AAFObject.comh
AAFObject.all : AAFObject.exp AAFObject.idl
AAFObject.all : AAFObject.implc AAFObject.implh
AAFObject.exp : AAFUnitTest.exp macros/exp.mac
AAFObject.h : AAFObject.exp macros/h.mac
AAFObject.cpp : AAFObject.exp macros/cpp.mac
AAFObject.idl : AAFObject.exp macros/idl.mac
AAFObject.comc : AAFObject.exp macros/comc.mac
AAFObject.comh : AAFObject.exp macros/comh.mac
AAFObject.implc : AAFObject.exp macros/implc.mac
AAFObject.implh : AAFObject.exp macros/implh.mac

AAFSourceClip.all : AAFSourceClip.cpp AAFSourceClip.h
AAFSourceClip.all : AAFSourceClip.comc AAFSourceClip.comh
AAFSourceClip.all : AAFSourceClip.exp AAFSourceClip.idl
AAFSourceClip.all : AAFSourceClip.implc AAFSourceClip.implh
AAFSourceClip.exp : AAFUnitTest.exp AAFSourceReference.exp macros/exp.mac
AAFSourceClip.h : AAFSourceClip.exp macros/h.mac
AAFSourceClip.cpp : AAFSourceClip.exp macros/cpp.mac
AAFSourceClip.idl : AAFSourceClip.exp macros/idl.mac
AAFSourceClip.comc : AAFSourceClip.exp macros/comc.mac
AAFSourceClip.comh : AAFSourceClip.exp macros/comh.mac
AAFSourceClip.implc : AAFSourceClip.exp macros/implc.mac
AAFSourceClip.implh : AAFSourceClip.exp macros/implh.mac

AAFSegment.all : AAFSegment.cpp AAFSegment.h
AAFSegment.all : AAFSegment.comc AAFSegment.comh
AAFSegment.all : AAFSegment.exp AAFSegment.idl
AAFSegment.all : AAFSegment.implc AAFSegment.implh
AAFSegment.exp : AAFUnitTest.exp AAFComponent.exp macros/exp.mac
AAFSegment.h : AAFSegment.exp macros/h.mac
AAFSegment.cpp : AAFSegment.exp macros/cpp.mac
AAFSegment.idl : AAFSegment.exp macros/idl.mac
AAFSegment.comc : AAFSegment.exp macros/comc.mac
AAFSegment.comh : AAFSegment.exp macros/comh.mac
AAFSegment.implc : AAFSegment.exp macros/implc.mac
AAFSegment.implh : AAFSegment.exp macros/implh.mac

AAFSourceReference.all : AAFSourceReference.cpp AAFSourceReference.h
AAFSourceReference.all : AAFSourceReference.comc AAFSourceReference.comh
AAFSourceReference.all : AAFSourceReference.exp AAFSourceReference.idl
AAFSourceReference.all : AAFSourceReference.implc AAFSourceReference.implh
AAFSourceReference.exp : AAFSourceReference.dod AAFUnitTest.exp macros/exp.mac
AAFSourceReference.h : AAFSourceReference.exp AAFSegment.exp macros/h.mac
AAFSourceReference.cpp : AAFSourceReference.exp macros/cpp.mac
AAFSourceReference.idl : AAFSourceReference.exp macros/idl.mac
AAFSourceReference.comc : AAFSourceReference.exp macros/comc.mac
AAFSourceReference.comh : AAFSourceReference.exp macros/comh.mac
AAFSourceReference.implc : AAFSourceReference.exp macros/implc.mac
AAFSourceReference.implh : AAFSourceReference.exp macros/implh.mac

AAFString.all : AAFString.cpp AAFString.h
AAFString.all : AAFString.comc AAFString.comh
AAFString.all : AAFString.exp AAFString.idl
AAFString.all : AAFString.implc AAFString.implh
AAFString.exp : AAFUnitTest.exp macros/exp.mac
AAFString.h : AAFString.exp macros/h.mac
AAFString.cpp : AAFString.exp macros/cpp.mac
AAFString.idl : AAFString.exp macros/idl.mac
AAFString.comc : AAFString.exp macros/comc.mac
AAFString.comh : AAFString.exp macros/comh.mac
AAFString.implc : AAFString.exp macros/implc.mac
AAFString.implh : AAFString.exp macros/implh.mac

AAFUInt64.all : AAFUInt64.cpp AAFUInt64.h
AAFUInt64.all : AAFUInt64.comc AAFUInt64.comh
AAFUInt64.all : AAFUInt64.exp AAFUInt64.idl
AAFUInt64.all : AAFUInt64.implc AAFUInt64.implh
AAFUInt64.exp : AAFUnitTest.exp macros/exp.mac
AAFUInt64.h : AAFUInt64.exp macros/h.mac
AAFUInt64.cpp : AAFUInt64.exp macros/cpp.mac
AAFUInt64.idl : AAFUInt64.exp macros/idl.mac
AAFUInt64.comc : AAFUInt64.exp macros/comc.mac
AAFUInt64.comh : AAFUInt64.exp macros/comh.mac
AAFUInt64.implc : AAFUInt64.exp macros/implc.mac
AAFUInt64.implh : AAFUInt64.exp macros/implh.mac

AAFUnitTest.all : AAFUnitTest.cpp AAFUnitTest.h
AAFUnitTest.all : AAFUnitTest.comc AAFUnitTest.comh
AAFUnitTest.all : AAFUnitTest.exp AAFUnitTest.idl
AAFUnitTest.all : AAFUnitTest.implc AAFUnitTest.implh
AAFUnitTest.exp : AAFUnitTest.dod macros/exp.mac
AAFUnitTest.h : macros/h.mac
AAFUnitTest.cpp : AAFUnitTest.exp macros/cpp.mac
AAFUnitTest.idl : AAFUnitTest.exp macros/idl.mac
AAFUnitTest.comc : AAFUnitTest.exp macros/comc.mac
AAFUnitTest.comh : AAFUnitTest.exp macros/comh.mac
AAFUnitTest.implc : AAFUnitTest.exp macros/implc.mac
AAFUnitTest.implh : AAFUnitTest.exp macros/implh.mac

AAFUnixLocator.all : AAFUnixLocator.cpp AAFUnixLocator.h
AAFUnixLocator.all : AAFUnixLocator.comc AAFUnixLocator.comh
AAFUnixLocator.all : AAFUnixLocator.exp AAFUnixLocator.idl
AAFUnixLocator.all : AAFUnixLocator.implc AAFUnixLocator.implh
AAFUnixLocator.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac
AAFUnixLocator.h : AAFUnixLocator.exp macros/h.mac
AAFUnixLocator.cpp : AAFUnixLocator.exp macros/cpp.mac
AAFUnixLocator.idl : AAFUnixLocator.exp macros/idl.mac
AAFUnixLocator.comc : AAFUnixLocator.exp macros/comc.mac
AAFUnixLocator.comh : AAFUnixLocator.exp macros/comh.mac
AAFUnixLocator.implc : AAFUnixLocator.exp macros/implc.mac
AAFUnixLocator.implh : AAFUnixLocator.exp macros/implh.mac

AAFWindowsLocator.all : AAFWindowsLocator.cpp AAFWindowsLocator.h
AAFWindowsLocator.all : AAFWindowsLocator.comc AAFWindowsLocator.comh
AAFWindowsLocator.all : AAFWindowsLocator.exp AAFWindowsLocator.idl
AAFWindowsLocator.all : AAFWindowsLocator.implc AAFWindowsLocator.implh
AAFWindowsLocator.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac
AAFWindowsLocator.h : AAFWindowsLocator.exp macros/h.mac
AAFWindowsLocator.cpp : AAFWindowsLocator.exp macros/cpp.mac
AAFWindowsLocator.idl : AAFWindowsLocator.exp macros/idl.mac
AAFWindowsLocator.comc : AAFWindowsLocator.exp macros/comc.mac
AAFWindowsLocator.comh : AAFWindowsLocator.exp macros/comh.mac
AAFWindowsLocator.implc : AAFWindowsLocator.exp macros/implc.mac
AAFWindowsLocator.implh : AAFWindowsLocator.exp macros/implh.mac
