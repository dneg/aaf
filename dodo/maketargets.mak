############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)

run:
	 $(SH_PREFIX) cd tool ; $(MAKE) run $(SH_SUFFIX)

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .implc .implh .comt .cppt

# This file contains the list of all of the targets to be built...								   
include targets.mk


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
	$(SH_PREFIX) cp $*.h $(INCLUDE_DIR)/cpp-api/ $(SH_SUFFIX)

.dod.cppt :
	$(SH_PREFIX) $(RM) -f $*.cppt $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.cppt $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.cppt $(SRC_DIR)/cpp-api/test/$*Test.cpp $(SH_SUFFIX)

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

.dod.comt :
	$(SH_PREFIX) $(RM) -f $*.comt $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comt.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comt $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comt $(SRC_DIR)/com-api/test/C$*Test.cpp $(SH_SUFFIX)

.dod.implh :
	if test -z "`cleartool ls -vob $(SRC_DIR)/impl/Impl$*.h`" ; then ; \
		$(RM) -f $*.implh ; \
		./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ; \
		mv $*.tmp $*.implh ; \
		cp $*.implh $(SRC_DIR)/impl/Impl$*.h ; \
	fi

.dod.implc :
	if test -z "`cleartool ls -vob $(SRC_DIR)/impl/Impl$*.h`" ; then ; \
		$(RM) -f $*.implc ; \
		./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ; \
		mv $*.tmp $*.implc ; \
		cp $*.implc $(SRC_DIR)/impl/Impl$*.cpp ; \
	fi

.dod.cpp :
	$(SH_PREFIX) $(RM) -f $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.cpp $(SRC_DIR)/cpp-api $(SH_SUFFIX)

.dod.idl :
	$(SH_PREFIX) $(RM) -f $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.idl $(INCLUDE_DIR)/com-api/ $(SH_SUFFIX)



clean:
	$(SH_PREFIX) cd tool ; $(MAKE) clean $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.cpp *.h *.idl *.exp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.comc *.comh *.comt $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.implc *.implh $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/cpp-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/com-api/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/com-api/*.idl $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/cpp-api/*.h $(SH_SUFFIX)


# This file contains the list of all dependents...
include depend.mk
