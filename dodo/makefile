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
	AAFAttribute.all \
	AAFAttributeArray.all \
	AAFComponent.all \
	AAFCompositionMob.all \
	AAFConstValue.all \
	AAFControlPoint.all \
	AAFDataDefinition.all \
	AAFEdgecode.all \
	AAFFiller.all \
	AAFInt64.all \
	AAFIntegerAttribute.all \
	AAFLocator.all \
	AAFMediaDescriptor.all \
	AAFMediaFileDescriptor.all \
	AAFMediaTapeDescriptor.all \
	AAFMediaTapeDescriptor.all \
	AAFMediaGroup.all  \
	AAFObject.all \
	AAFMob.all  \
	AAFMasterMob.all \
	AAFMobSlot.all \
	AAFParameter.all \
	AAFTimelineMobSlot.all \
	AAFObjectAttribute.all \
	AAFParameterSlot.all \
	AAFSegment.all \
	AAFSequence.all \
	AAFSourceClip.all \
	AAFSourceReference.all \
	AAFString.all \
	AAFStringAttribute.all \
	AAFTimecode.all \
	AAFTimecodeStream.all \
	AAFTimecodeStream12M.all \
	AAFTransition.all \
	AAFUInt64.all \
	AAFUnitTest.all \
	AAFUnixLocator.all \
	AAFVaryingValue.all \
	AAFWindowsLocator.all \
	AAFMedia.all

# Jeff hasn't finished with these yet
#	AAFMediaTapeDescriptor.all \
#

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

AAFAttribute.all : AAFAttribute.cpp AAFAttribute.h
AAFAttribute.all : AAFAttribute.comc AAFAttribute.comh
AAFAttribute.all : AAFAttribute.exp AAFAttribute.idl
AAFAttribute.all : AAFAttribute.implc AAFAttribute.implh
AAFAttribute.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFAttribute.h : AAFAttribute.exp macros/h.mac
AAFAttribute.cpp : AAFAttribute.exp macros/cpp.mac
AAFAttribute.idl : AAFAttribute.exp macros/idl.mac
AAFAttribute.comc : AAFAttribute.exp macros/comc.mac
AAFAttribute.comh : AAFAttribute.exp macros/comh.mac
AAFAttribute.implc : AAFAttribute.exp macros/implc.mac
AAFAttribute.implh : AAFAttribute.exp macros/implh.mac

AAFAttributeArray.all : AAFAttributeArray.cpp AAFAttributeArray.h
AAFAttributeArray.all : AAFAttributeArray.comc AAFAttributeArray.comh
AAFAttributeArray.all : AAFAttributeArray.exp AAFAttributeArray.idl
AAFAttributeArray.all : AAFAttributeArray.implc AAFAttributeArray.implh
AAFAttributeArray.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFAttributeArray.h : AAFAttributeArray.exp macros/h.mac
AAFAttributeArray.cpp : AAFAttributeArray.exp macros/cpp.mac
AAFAttributeArray.idl : AAFAttributeArray.exp macros/idl.mac
AAFAttributeArray.comc : AAFAttributeArray.exp macros/comc.mac
AAFAttributeArray.comh : AAFAttributeArray.exp macros/comh.mac
AAFAttributeArray.implc : AAFAttributeArray.exp macros/implc.mac
AAFAttributeArray.implh : AAFAttributeArray.exp macros/implh.mac

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

AAFControlPoint.all : AAFControlPoint.cpp AAFControlPoint.h
AAFControlPoint.all : AAFControlPoint.comc AAFControlPoint.comh
AAFControlPoint.all : AAFControlPoint.exp AAFControlPoint.idl
AAFControlPoint.all : AAFControlPoint.implc AAFControlPoint.implh
AAFControlPoint.exp : AAFUnitTest.exp macros/exp.mac
AAFControlPoint.h : AAFControlPoint.exp macros/h.mac
AAFControlPoint.cpp : AAFControlPoint.exp macros/cpp.mac
AAFControlPoint.idl : AAFControlPoint.exp macros/idl.mac
AAFControlPoint.comc : AAFControlPoint.exp macros/comc.mac
AAFControlPoint.comh : AAFControlPoint.exp macros/comh.mac
AAFControlPoint.implc : AAFControlPoint.exp macros/implc.mac
AAFControlPoint.implh : AAFControlPoint.exp macros/implh.mac

AAFDataDefinition.all : AAFDataDefinition.cpp AAFDataDefinition.h
AAFDataDefinition.all : AAFDataDefinition.comc AAFDataDefinition.comh
AAFDataDefinition.all : AAFDataDefinition.exp AAFDataDefinition.idl
AAFDataDefinition.all : AAFDataDefinition.implc AAFDataDefinition.implh
AAFDataDefinition.exp : AAFUnitTest.exp macros/exp.mac
AAFDataDefinition.h : AAFDataDefinition.exp macros/h.mac
AAFDataDefinition.cpp : AAFDataDefinition.exp macros/cpp.mac
AAFDataDefinition.idl : AAFDataDefinition.exp macros/idl.mac
AAFDataDefinition.comc : AAFDataDefinition.exp macros/comc.mac
AAFDataDefinition.comh : AAFDataDefinition.exp macros/comh.mac
AAFDataDefinition.implc : AAFDataDefinition.exp macros/implc.mac
AAFDataDefinition.implh : AAFDataDefinition.exp macros/implh.mac

AAFEdgecode.all : AAFEdgecode.cpp AAFEdgecode.h
AAFEdgecode.all : AAFEdgecode.comc AAFEdgecode.comh
AAFEdgecode.all : AAFEdgecode.exp AAFEdgecode.idl
AAFEdgecode.all : AAFEdgecode.implc AAFEdgecode.implh
AAFEdgecode.exp : AAFSegment.exp AAFUnitTest.exp macros/exp.mac
AAFEdgecode.h : AAFEdgecode.exp macros/h.mac
AAFEdgecode.cpp : AAFEdgecode.exp macros/cpp.mac
AAFEdgecode.idl : AAFEdgecode.exp macros/idl.mac
AAFEdgecode.comc : AAFEdgecode.exp macros/comc.mac
AAFEdgecode.comh : AAFEdgecode.exp macros/comh.mac
AAFEdgecode.implc : AAFEdgecode.exp macros/implc.mac
AAFEdgecode.implh : AAFEdgecode.exp macros/implh.mac

AAFFiller.all : AAFFiller.cpp AAFFiller.h
AAFFiller.all : AAFFiller.comc AAFFiller.comh
AAFFiller.all : AAFFiller.exp AAFFiller.idl
AAFFiller.all : AAFFiller.implc AAFFiller.implh
AAFFiller.exp : AAFSegment.exp AAFUnitTest.exp macros/exp.mac
AAFFiller.h : AAFFiller.exp macros/h.mac
AAFFiller.cpp : AAFFiller.exp macros/cpp.mac
AAFFiller.idl : AAFFiller.exp macros/idl.mac
AAFFiller.comc : AAFFiller.exp macros/comc.mac
AAFFiller.comh : AAFFiller.exp macros/comh.mac
AAFFiller.implc : AAFFiller.exp macros/implc.mac
AAFFiller.implh : AAFFiller.exp macros/implh.mac

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

AAFIntegerAttribute.all : AAFIntegerAttribute.cpp AAFIntegerAttribute.h
AAFIntegerAttribute.all : AAFIntegerAttribute.comc AAFIntegerAttribute.comh
AAFIntegerAttribute.all : AAFIntegerAttribute.exp AAFIntegerAttribute.idl
AAFIntegerAttribute.all : AAFIntegerAttribute.implc AAFIntegerAttribute.implh
AAFIntegerAttribute.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFIntegerAttribute.h : AAFIntegerAttribute.exp macros/h.mac
AAFIntegerAttribute.cpp : AAFIntegerAttribute.exp macros/cpp.mac
AAFIntegerAttribute.idl : AAFIntegerAttribute.exp macros/idl.mac
AAFIntegerAttribute.comc : AAFIntegerAttribute.exp macros/comc.mac
AAFIntegerAttribute.comh : AAFIntegerAttribute.exp macros/comh.mac
AAFIntegerAttribute.implc : AAFIntegerAttribute.exp macros/implc.mac
AAFIntegerAttribute.implh : AAFIntegerAttribute.exp macros/implh.mac

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

AAFMacLocator.all : AAFMacLocator.cpp AAFMacLocator.h
AAFMacLocator.all : AAFMacLocator.comc AAFMacLocator.comh
AAFMacLocator.all : AAFMacLocator.exp AAFMacLocator.idl
AAFMacLocator.all : AAFMacLocator.implc AAFMacLocator.implh
AAFMacLocator.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac
AAFMacLocator.h : AAFMacLocator.exp macros/h.mac
AAFMacLocator.cpp : AAFMacLocator.exp macros/cpp.mac
AAFMacLocator.idl : AAFMacLocator.exp macros/idl.mac
AAFMacLocator.implc : AAFMacLocator.exp macros/implc.mac
AAFMacLocator.implh : AAFMacLocator.exp macros/implh.mac

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

AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.cpp AAFMediaTapeDescriptor.h
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.comc AAFMediaTapeDescriptor.comh
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.exp AAFMediaTapeDescriptor.idl
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.implc AAFMediaTapeDescriptor.implh
AAFMediaTapeDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac
AAFMediaTapeDescriptor.h : AAFMediaTapeDescriptor.exp macros/h.mac
AAFMediaTapeDescriptor.cpp : AAFMediaTapeDescriptor.exp macros/cpp.mac
AAFMediaTapeDescriptor.idl : AAFMediaTapeDescriptor.exp macros/idl.mac
AAFMediaTapeDescriptor.comc : AAFMediaTapeDescriptor.exp macros/comc.mac
AAFMediaTapeDescriptor.comh : AAFMediaTapeDescriptor.exp macros/comh.mac
AAFMediaTapeDescriptor.implc : AAFMediaTapeDescriptor.exp macros/implc.mac
AAFMediaTapeDescriptor.implh : AAFMediaTapeDescriptor.exp macros/implh.mac

AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.cpp AAFMediaTapeDescriptor.h
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.comc AAFMediaTapeDescriptor.comh
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.exp AAFMediaTapeDescriptor.idl
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.implc AAFMediaTapeDescriptor.implh
AAFMediaTapeDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac
AAFMediaTapeDescriptor.h : AAFMediaTapeDescriptor.exp macros/h.mac
AAFMediaTapeDescriptor.cpp : AAFMediaTapeDescriptor.exp macros/cpp.mac
AAFMediaTapeDescriptor.idl : AAFMediaTapeDescriptor.exp macros/idl.mac
AAFMediaTapeDescriptor.comc : AAFMediaTapeDescriptor.exp macros/comc.mac
AAFMediaTapeDescriptor.comh : AAFMediaTapeDescriptor.exp macros/comh.mac
AAFMediaTapeDescriptor.implc : AAFMediaTapeDescriptor.exp macros/implc.mac
AAFMediaTapeDescriptor.implh : AAFMediaTapeDescriptor.exp macros/implh.mac

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

AAFMob.all : AAFMob.cpp AAFMob.h
AAFMob.all : AAFMob.comc AAFMob.comh
AAFMob.all : AAFMob.exp AAFMob.idl
AAFMob.all : AAFMob.implc AAFMob.implh
AAFMob.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFMob.h : AAFMob.exp macros/h.mac
AAFMob.cpp : AAFMob.exp macros/cpp.mac
AAFMob.idl : AAFMob.exp macros/idl.mac
AAFMob.comc : AAFMob.exp macros/comc.mac
AAFMob.comh : AAFMob.exp macros/comh.mac
AAFMob.implc : AAFMob.exp macros/implc.mac
AAFMob.implh : AAFMob.exp macros/implh.mac

AAFMasterMob.all : AAFMasterMob.cpp AAFMasterMob.h
AAFMasterMob.all : AAFMasterMob.comc AAFMasterMob.comh
AAFMasterMob.all : AAFMasterMob.exp AAFMasterMob.idl
AAFMasterMob.all : AAFMasterMob.implc AAFMasterMob.implh
AAFMasterMob.exp : AAFUnitTest.exp AAFMob.exp macros/exp.mac
AAFMasterMob.h : AAFMasterMob.exp macros/h.mac
AAFMasterMob.cpp : AAFMasterMob.exp macros/cpp.mac
AAFMasterMob.idl : AAFMasterMob.exp macros/idl.mac
AAFMasterMob.comc : AAFMasterMob.exp macros/comc.mac
AAFMasterMob.comh : AAFMasterMob.exp macros/comh.mac
AAFMasterMob.implc : AAFMasterMob.exp macros/implc.mac
AAFMasterMob.implh : AAFMasterMob.exp macros/implh.mac

AAFMobSlot.all : AAFMobSlot.cpp AAFMobSlot.h
AAFMobSlot.all : AAFMobSlot.comc AAFMobSlot.comh
AAFMobSlot.all : AAFMobSlot.exp AAFMobSlot.idl
AAFMobSlot.all : AAFMobSlot.implc AAFMobSlot.implh
AAFMobSlot.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac
AAFMobSlot.h : AAFMobSlot.exp macros/h.mac
AAFMobSlot.cpp : AAFMobSlot.exp macros/cpp.mac
AAFMobSlot.idl : AAFMobSlot.exp macros/idl.mac
AAFMobSlot.comc : AAFMobSlot.exp macros/comc.mac
AAFMobSlot.comh : AAFMobSlot.exp macros/comh.mac
AAFMobSlot.implc : AAFMobSlot.exp macros/implc.mac
AAFMobSlot.implh : AAFMobSlot.exp macros/implh.mac

AAFTimelineMobSlot.all : AAFTimelineMobSlot.cpp AAFTimelineMobSlot.h
AAFTimelineMobSlot.all : AAFTimelineMobSlot.comc AAFTimelineMobSlot.comh
AAFTimelineMobSlot.all : AAFTimelineMobSlot.exp AAFTimelineMobSlot.idl
AAFTimelineMobSlot.all : AAFTimelineMobSlot.implc AAFTimelineMobSlot.implh
AAFTimelineMobSlot.exp : AAFUnitTest.exp AAFMobSlot.exp macros/exp.mac
AAFTimelineMobSlot.h : AAFTimelineMobSlot.exp macros/h.mac
AAFTimelineMobSlot.cpp : AAFTimelineMobSlot.exp macros/cpp.mac
AAFTimelineMobSlot.idl : AAFTimelineMobSlot.exp macros/idl.mac
AAFTimelineMobSlot.comc : AAFTimelineMobSlot.exp macros/comc.mac
AAFTimelineMobSlot.comh : AAFTimelineMobSlot.exp macros/comh.mac
AAFTimelineMobSlot.implc : AAFTimelineMobSlot.exp macros/implc.mac
AAFTimelineMobSlot.implh : AAFTimelineMobSlot.exp macros/implh.mac

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

AAFObjectAttribute.all : AAFObjectAttribute.cpp AAFObjectAttribute.h
AAFObjectAttribute.all : AAFObjectAttribute.comc AAFObjectAttribute.comh
AAFObjectAttribute.all : AAFObjectAttribute.exp AAFObjectAttribute.idl
AAFObjectAttribute.all : AAFObjectAttribute.implc AAFObjectAttribute.implh
AAFObjectAttribute.exp : AAFUnitTest.exp macros/exp.mac
AAFObjectAttribute.h : AAFObjectAttribute.exp macros/h.mac
AAFObjectAttribute.cpp : AAFObjectAttribute.exp macros/cpp.mac
AAFObjectAttribute.idl : AAFObjectAttribute.exp macros/idl.mac
AAFObjectAttribute.comc : AAFObjectAttribute.exp macros/comc.mac
AAFObjectAttribute.comh : AAFObjectAttribute.exp macros/comh.mac
AAFObjectAttribute.implc : AAFObjectAttribute.exp macros/implc.mac
AAFObjectAttribute.implh : AAFObjectAttribute.exp macros/implh.mac

AAFParameter.all : AAFParameter.cpp AAFParameter.h
AAFParameter.all : AAFParameter.comc AAFParameter.comh
AAFParameter.all : AAFParameter.exp AAFParameter.idl
AAFParameter.all : AAFParameter.implc AAFParameter.implh
AAFParameter.exp : AAFUnitTest.exp macros/exp.mac
AAFParameter.h : AAFParameter.exp macros/h.mac
AAFParameter.cpp : AAFParameter.exp macros/cpp.mac
AAFParameter.idl : AAFParameter.exp macros/idl.mac
AAFParameter.comc : AAFParameter.exp macros/comc.mac
AAFParameter.comh : AAFParameter.exp macros/comh.mac
AAFParameter.implc : AAFParameter.exp macros/implc.mac
AAFParameter.implh : AAFParameter.exp macros/implh.mac

AAFParameterSlot.all : AAFParameterSlot.cpp AAFParameterSlot.h
AAFParameterSlot.all : AAFParameterSlot.comc AAFParameterSlot.comh
AAFParameterSlot.all : AAFParameterSlot.exp AAFParameterSlot.idl
AAFParameterSlot.all : AAFParameterSlot.implc AAFParameterSlot.implh
AAFParameterSlot.exp : AAFUnitTest.exp macros/exp.mac
AAFParameterSlot.h : AAFParameterSlot.exp macros/h.mac
AAFParameterSlot.cpp : AAFParameterSlot.exp macros/cpp.mac
AAFParameterSlot.idl : AAFParameterSlot.exp macros/idl.mac
AAFParameterSlot.comc : AAFParameterSlot.exp macros/comc.mac
AAFParameterSlot.comh : AAFParameterSlot.exp macros/comh.mac
AAFParameterSlot.implc : AAFParameterSlot.exp macros/implc.mac
AAFParameterSlot.implh : AAFParameterSlot.exp macros/implh.mac

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

AAFSequence.all : AAFSequence.cpp AAFSequence.h
AAFSequence.all : AAFSequence.comc AAFSequence.comh
AAFSequence.all : AAFSequence.exp AAFSequence.idl
AAFSequence.all : AAFSequence.implc AAFSequence.implh
AAFSequence.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac
AAFSequence.h : AAFSequence.exp macros/h.mac
AAFSequence.cpp : AAFSequence.exp macros/cpp.mac
AAFSequence.idl : AAFSequence.exp macros/idl.mac
AAFSequence.comc : AAFSequence.exp macros/comc.mac
AAFSequence.comh : AAFSequence.exp macros/comh.mac
AAFSequence.implc : AAFSequence.exp macros/implc.mac
AAFSequence.implh : AAFSequence.exp macros/implh.mac

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

AAFStringAttribute.all : AAFStringAttribute.cpp AAFStringAttribute.h
AAFStringAttribute.all : AAFStringAttribute.comc AAFStringAttribute.comh
AAFStringAttribute.all : AAFStringAttribute.exp AAFStringAttribute.idl
AAFStringAttribute.all : AAFStringAttribute.implc AAFStringAttribute.implh
AAFStringAttribute.exp : AAFUnitTest.exp macros/exp.mac
AAFStringAttribute.h : AAFStringAttribute.exp macros/h.mac
AAFStringAttribute.cpp : AAFStringAttribute.exp macros/cpp.mac
AAFStringAttribute.idl : AAFStringAttribute.exp macros/idl.mac
AAFStringAttribute.comc : AAFStringAttribute.exp macros/comc.mac
AAFStringAttribute.comh : AAFStringAttribute.exp macros/comh.mac
AAFStringAttribute.implc : AAFStringAttribute.exp macros/implc.mac
AAFStringAttribute.implh : AAFStringAttribute.exp macros/implh.mac

AAFTimecode.all : AAFTimecode.cpp AAFTimecode.h
AAFTimecode.all : AAFTimecode.comc AAFTimecode.comh
AAFTimecode.all : AAFTimecode.exp AAFTimecode.idl
AAFTimecode.all : AAFTimecode.implc AAFTimecode.implh
AAFTimecode.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac
AAFTimecode.h : AAFTimecode.exp macros/h.mac
AAFTimecode.cpp : AAFTimecode.exp macros/cpp.mac
AAFTimecode.idl : AAFTimecode.exp macros/idl.mac
AAFTimecode.comc : AAFTimecode.exp macros/comc.mac
AAFTimecode.comh : AAFTimecode.exp macros/comh.mac
AAFTimecode.implc : AAFTimecode.exp macros/implc.mac
AAFTimecode.implh : AAFTimecode.exp macros/implh.mac

AAFTimecodeStream.all : AAFTimecodeStream.cpp AAFTimecodeStream.h
AAFTimecodeStream.all : AAFTimecodeStream.comc AAFTimecodeStream.comh
AAFTimecodeStream.all : AAFTimecodeStream.exp AAFTimecodeStream.idl
AAFTimecodeStream.all : AAFTimecodeStream.implc AAFTimecodeStream.implh
AAFTimecodeStream.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac
AAFTimecodeStream.h : AAFTimecodeStream.exp macros/h.mac
AAFTimecodeStream.cpp : AAFTimecodeStream.exp macros/cpp.mac
AAFTimecodeStream.idl : AAFTimecodeStream.exp macros/idl.mac
AAFTimecodeStream.comc : AAFTimecodeStream.exp macros/comc.mac
AAFTimecodeStream.comh : AAFTimecodeStream.exp macros/comh.mac
AAFTimecodeStream.implc : AAFTimecodeStream.exp macros/implc.mac
AAFTimecodeStream.implh : AAFTimecodeStream.exp macros/implh.mac

AAFTimecodeStream12M.all : AAFTimecodeStream12M.cpp AAFTimecodeStream12M.h
AAFTimecodeStream12M.all : AAFTimecodeStream12M.comc AAFTimecodeStream12M.comh
AAFTimecodeStream12M.all : AAFTimecodeStream12M.exp AAFTimecodeStream12M.idl
AAFTimecodeStream12M.all : AAFTimecodeStream12M.implc AAFTimecodeStream12M.implh
AAFTimecodeStream12M.exp : AAFUnitTest.exp AAFTimecodeStream.exp macros/exp.mac
AAFTimecodeStream12M.h : AAFTimecodeStream12M.exp macros/h.mac
AAFTimecodeStream12M.cpp : AAFTimecodeStream12M.exp macros/cpp.mac
AAFTimecodeStream12M.idl : AAFTimecodeStream12M.exp macros/idl.mac
AAFTimecodeStream12M.comc : AAFTimecodeStream12M.exp macros/comc.mac
AAFTimecodeStream12M.comh : AAFTimecodeStream12M.exp macros/comh.mac
AAFTimecodeStream12M.implc : AAFTimecodeStream12M.exp macros/implc.mac
AAFTimecodeStream12M.implh : AAFTimecodeStream12M.exp macros/implh.mac

AAFTransition.all : AAFTransition.cpp AAFTransition.h
AAFTransition.all : AAFTransition.comc AAFTransition.comh
AAFTransition.all : AAFTransition.exp AAFTransition.idl
AAFTransition.all : AAFTransition.implc AAFTransition.implh
AAFTransition.exp : AAFUnitTest.exp AAFTimecodeStream.exp macros/exp.mac
AAFTransition.h : AAFTransition.exp macros/h.mac
AAFTransition.cpp : AAFTransition.exp macros/cpp.mac
AAFTransition.idl : AAFTransition.exp macros/idl.mac
AAFTransition.comc : AAFTransition.exp macros/comc.mac
AAFTransition.comh : AAFTransition.exp macros/comh.mac
AAFTransition.implc : AAFTransition.exp macros/implc.mac
AAFTransition.implh : AAFTransition.exp macros/implh.mac

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

AAFVaryingValue.all : AAFVaryingValue.cpp AAFVaryingValue.h
AAFVaryingValue.all : AAFVaryingValue.comc AAFVaryingValue.comh
AAFVaryingValue.all : AAFVaryingValue.exp AAFVaryingValue.idl
AAFVaryingValue.all : AAFVaryingValue.implc AAFVaryingValue.implh
AAFVaryingValue.exp : AAFParameter.exp macros/exp.mac
AAFVaryingValue.h : AAFVaryingValue.exp macros/h.mac
AAFVaryingValue.cpp : AAFVaryingValue.exp macros/cpp.mac
AAFVaryingValue.idl : AAFVaryingValue.exp macros/idl.mac
AAFVaryingValue.comc : AAFVaryingValue.exp macros/comc.mac
AAFVaryingValue.comh : AAFVaryingValue.exp macros/comh.mac
AAFVaryingValue.implc : AAFVaryingValue.exp macros/implc.mac
AAFVaryingValue.implh : AAFVaryingValue.exp macros/implh.mac

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
