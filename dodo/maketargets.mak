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
	cd tool ;  $(MAKE)

test_only:
	cd tool ; $(MAKE) test_only

run:
	 cd tool ; $(MAKE) run

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .implc .implh .comt .cppt .refh

# This file contains the list of all of the targets to be built...								   
include targets.mk
include aafobjects.mk


targets: $(DODO_TARGETS)

SRC_DIR = ../ref-impl/src

INCLUDE_DIR = ../ref-impl/include

.dod.exp :
	$(RM) -f $*.exp
	./tool/$(DODO) -f macros/exp.mac < $*.dod > $*.tmp
	mv $*.tmp $*.exp

.dod.h :
	$(RM) -f $*.h
	./tool/$(DODO) -f macros/h.mac < $*.dod > $*.tmp
	mv $*.tmp $*.h
	cp $*.h $(INCLUDE_DIR)/cpp-api/

.dod.cppt :
	$(RM) -f $*.cppt
	./tool/$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp
	mv $*.tmp $*.cppt
	cp $*.cppt $(SRC_DIR)/cpp-api/test/$*Test.cpp

.dod.comh :
	$(RM) -f $*.comh
	./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comh
	cp $*.comh $(SRC_DIR)/com-api/C$*.h

.dod.comc :
	$(RM) -f $*.comc
	./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comc
	cp $*.comc $(SRC_DIR)/com-api/C$*.cpp

.dod.comt :
	$(RM) -f $*.comt
	./tool/$(DODO) -f macros/comt.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comt
	cp $*.comt $(SRC_DIR)/com-api/test/C$*Test.cpp

.dod.implh :
	$(RM) -f $*.implh ;
	./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ;
	mv $*.tmp $*.implh ;
	cp $*.implh $(SRC_DIR)/impl/Impl$*.h ;

.dod.implc :
	$(RM) -f $*.implc ;
	./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ;
	mv $*.tmp $*.implc ;
	cp $*.implc $(SRC_DIR)/impl/Impl$*.cpp ;

.dod.cpp :
	$(RM) -f $*.cpp
	./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp
	mv $*.tmp $*.cpp
	cp $*.cpp $(SRC_DIR)/cpp-api

.dod.idl :
	$(RM) -f $*.idl
	./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp
	mv $*.tmp $*.idl
	cp $*.idl $(INCLUDE_DIR)/com-api/

.dod.refh :
	$(RM) -f $*.refh
	./tool/$(DODO) -f macros/refh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.refh
	cp $*.refh $(INCLUDE_DIR)/ref-api/$*.h



clean:
	cd tool ; $(MAKE) clean
	$(RM) -f *.cpp *.cppt *.h *.idl *.exp
	$(RM) -f *.comc *.comh *.comt
	$(RM) -f *.implc *.implh
	$(RM) -f core
	$(RM) -f $(SRC_DIR)/cpp-api/AAF*.cpp
	$(RM) -f $(SRC_DIR)/com-api/CAAF*.h
	$(RM) -f $(SRC_DIR)/com-api/CAAF*.cpp
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF*.h
	$(RM) -f $(INCLUDE_DIR)/com-api/EnumAAF*.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF*.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/EnumAAF*.idl
	$(RM) -f $(INCLUDE_DIR)/cpp-api/AAF*.h
	$(RM) -f $(INCLUDE_DIR)/cpp-api/EnumAAF*.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF*.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/EnumAAF*.h
	@for file in $(AUTO_GEN_IMPL) ; do \
		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
		$(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
		$(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
	done


# This file contains the list of all dependents...
include depend.mk
