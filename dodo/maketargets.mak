#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

all : targets

BLD_CFG_DIR = ../ref-impl/bld_cfg
PLUGIN_DIR = ../ref-impl/plugins

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	cd tool ;  $(MAKE)

test_only:
	cd tool ; $(MAKE) test_only

run:
	 cd tool ; $(MAKE) run

.SUFFIXES: .cpp .h .comc .comcx .comh .dod .exp .idl .fidl .implc .implh .comt .cppt .refh .frefh

# This file contains the list of all of the targets to be built...
include targets.mk
include aafobjects.mk


INCLUDE_DIR = ../ref-impl/include
IMPL_DIR = ../ref-impl/src/impl
UUID_DIR = ../ref-impl/include/ref-api
COMAPI_DIR = ../ref-impl/src/com-api
TEST_DIR = ../test/com/ComModTestAAF/ModuleTests

targets: $(DODO_TARGETS)
targets: $(PLUGIN_TARGETS)
targets: $(INCLUDE_DIR)/com-api/AAF.idl
targets: $(PLUGIN_DIR)/AAFPlugin.idl
targets: $(INCLUDE_DIR)/ref-api/AAF.h
targets: $(UUID_DIR)/AAFRoot_i.c
targets: $(UUID_DIR)/AAF_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFPlugin.h
targets: $(UUID_DIR)/AAFPlugin_i.c
targets: $(COMAPI_DIR)/AAFCLSIDs.h
targets: $(IMPL_DIR)/AAFClassIDs.h
targets: $(COMAPI_DIR)/AAFObjectTable.h
targets: $(COMAPI_DIR)/AAFObjectTable_i.cpp
targets: $(COMAPI_DIR)/CAAFEnumValidation.h
targets: $(COMAPI_DIR)/CAAFEnumValidation.cpp


$(INCLUDE_DIR)/com-api/AAF.idl : $(FIDL_TARGETS)
	@ $(ECHO) Generating AAF.idl...
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAAFIdl.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) tmp.sh
	$(CHMOD) -w $(INCLUDE_DIR)/com-api/AAF.idl


$(PLUGIN_DIR)/AAFPlugin.idl : $(PLUGIN_FIDL_TARGETS)
	@ $(ECHO) Generating Plugin.idl...
	$(RM) -f $(PLUGIN_DIR)/AAFPlugin.idl
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPluginIdl.sh >> tmp.sh
	$(SH) tmp.sh > $(PLUGIN_DIR)/AAFPlugin.idl
	$(RM) tmp.sh
	$(CHMOD) -w $(PLUGIN_DIR)/AAFPlugin.idl


$(INCLUDE_DIR)/ref-api/AAF.h : $(FREFH_TARGETS)
	@ $(ECHO) Generating reference AAF.h...
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafh.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) tmp.sh
	$(CHMOD) -w $(INCLUDE_DIR)/ref-api/AAF.h


$(INCLUDE_DIR)/ref-api/AAFPlugin.h : $(PLUGIN_FREFH_TARGETS)
	@ $(ECHO) Generating reference AAFPlugin.h...
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(CP) aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPluginh.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(RM) tmp.sh
	$(CHMOD) -w $(INCLUDE_DIR)/ref-api/AAFPlugin.h


$(UUID_DIR)/AAF_i.c : aafobjects.mk dod2iid.awk
	@ $(ECHO) Generating reference AAF_i.c...
	$(RM) -f $(UUID_DIR)/AAF_i.c
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAaf_i.sh >> tmp.sh
	$(SH) tmp.sh > $(UUID_DIR)/AAF_i.c
	$(RM) tmp.sh
	$(CHMOD) -w $(UUID_DIR)/AAF_i.c


$(UUID_DIR)/AAFPlugin_i.c : aafobjects.mk dod2iid.awk
	@ $(ECHO) Generating reference AAFPlugin_i.c...
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPlugin_i.sh >> tmp.sh
	$(SH) tmp.sh > $(UUID_DIR)/AAFPlugin_i.c
	$(RM) tmp.sh
	$(CHMOD) -w $(UUID_DIR)/AAFPlugin_i.c


$(UUID_DIR)/AAFRoot_i.c : aafobjects.mk dod2iid.awk
	@ $(ECHO) Generating reference AAFRoot_i.c...
	$(RM) -f $(UUID_DIR)/AAFRoot_i.c
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenRoot_i.sh >> tmp.sh
	$(SH) tmp.sh > $(UUID_DIR)/AAFRoot_i.c
	$(RM) tmp.sh
	$(CHMOD) -w $(UUID_DIR)/AAFRoot_i.c


$(IMPL_DIR)/AAFClassIDs.h : aafobjects.mk
	@ $(ECHO) Generating reference AAFClassIDs.h...
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenClassIDs.sh >> tmp.sh
	$(SH) tmp.sh > $(IMPL_DIR)/AAFClassIDs.h
	$(RM) tmp.sh
	$(CHMOD) -w $(IMPL_DIR)/AAFClassIDs.h


$(COMAPI_DIR)/AAFCLSIDs.h : aafobjects.mk
	@ $(ECHO) Generating reference AAFCLSIDs.h...
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenCLSIDs.sh >> tmp.sh
	$(SH) tmp.sh > $(COMAPI_DIR)/AAFCLSIDs.h
	$(RM) tmp.sh
	$(CHMOD) -w $(COMAPI_DIR)/AAFCLSIDs.h


$(COMAPI_DIR)/AAFObjectTable.h : aafobjects.mk
	@ $(ECHO) Generating reference AAFObjectTable.h...
	@ $(RM) -f $(COMAPI_DIR)/AAFObjectTable.tmp
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenObjectTable.sh >> tmp.sh
	$(SH) tmp.sh > $(COMAPI_DIR)/AAFObjectTable.tmp
	$(RM) tmp.sh
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable.h
	$(MV) $(COMAPI_DIR)/AAFObjectTable.tmp $(COMAPI_DIR)/AAFObjectTable.h
	$(CHMOD) -w $(COMAPI_DIR)/AAFObjectTable.h


$(COMAPI_DIR)/AAFObjectTable_i.cpp : aafobjects.mk
	@ $(ECHO) Generating reference AAFObjectTable_i.cpp...
	@ $(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenObjectTable_i.sh >> tmp.sh
	$(SH) tmp.sh > $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(RM) tmp.sh
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.cpp
	$(MV) $(COMAPI_DIR)/AAFObjectTable_i.tmp $(COMAPI_DIR)/AAFObjectTable_i.cpp
	$(CHMOD) -w $(COMAPI_DIR)/AAFObjectTable_i.cpp


$(COMAPI_DIR)/CAAFEnumValidation.cpp $(COMAPI_DIR)/CAAFEnumValidation.h : GenEnumValidation.pl AAFPluginTypes.dod AAFTypes.dod
	@ $(ECHO) Generating references CAAFEnumValidation.cpp and CAAFEnumValidation.h ...
	$(PERL) GenEnumValidation.pl AAFTypes.dod AAFPluginTypes.dod > GenEnumValidationLog.txt
	$(MV) -f CAAFEnumValidation.cpp CAAFEnumValidation.h $(COMAPI_DIR)/
	$(CHMOD) -w $(COMAPI_DIR)/CAAFEnumValidation.cpp $(COMAPI_DIR)/CAAFEnumValidation.h


SRC_DIR = ../ref-impl/src

.dod.exp :
	$(RM) -f $*.exp
	./tool/$(DODO) -f macros/exp.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.exp
	$(CHMOD) -w $*.exp

.dod.h :
	$(RM) -f $*.h
	./tool/$(DODO) -f macros/h.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.h
	$(CHMOD) -w $*.h
	$(CP) -f $*.h $(INCLUDE_DIR)/cpp-api/
	$(CHMOD) -w $(INCLUDE_DIR)/cpp-api/$*.h

.dod.cppt :
	$(RM) -f $*.cppt
	./tool/$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.cppt
	$(CHMOD) -w $*.cppt
	$(CP) -f $*.cppt $(TEST_DIR)/$*Test.cpp
	$(CHMOD) -w $(TEST_DIR)//$*Test.cpp

.dod.comh :
	$(RM) -f $*.comh
	./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comh
	$(CHMOD) -w $*.comh
	$(CP) -f $*.comh $(SRC_DIR)/com-api/C$*.h
	$(CHMOD) -w $(SRC_DIR)/com-api/C$*.h

.dod.comc :
	$(RM) -f $*.comc
	./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comc
	$(CHMOD) -w $*.comc
	$(CP) -f $*.comc $(SRC_DIR)/com-api/C$*.cpp
	$(CHMOD) -w $(SRC_DIR)/com-api/C$*.cpp

.dod.comcx :
	$(RM) -f $*.comcx
	./tool/$(DODO) -f macros/comcx.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comcx
	$(CHMOD) -w $*.comcx
	$(CP) -f $*.comcx $(SRC_DIR)/com-api/C$*.cpp
	$(CHMOD) -w $(SRC_DIR)/com-api/C$*.cpp

.dod.comt :
	$(RM) -f $*.comt
	./tool/$(DODO) -f macros/comt.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comt
	$(CHMOD) -w $*.comt
	$(CP) -f $*.comt $(TEST_DIR)/C$*Test.cpp
	$(CHMOD) -w $(TEST_DIR)/C$*Test.cpp

.dod.implh :
	$(RM) -f $*.implh ;
	./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ;
	$(MV) $*.tmp $*.implh ;
	$(CHMOD) -w $*.implh ;
	$(CP) -f $*.implh $(SRC_DIR)/impl/Impl$*.h ;
	$(CHMOD) -w $(SRC_DIR)/impl/Impl$*.h ;

.dod.implc :
	$(RM) -f $*.implc ;
	./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ;
	$(MV) $*.tmp $*.implc ;
	$(CHMOD) -w $*.implc ;
	$(CP) -f $*.implc $(SRC_DIR)/impl/Impl$*.cpp ;
	$(CHMOD) -w $(SRC_DIR)/impl/Impl$*.cpp ;

.dod.cpp :
	$(RM) -f $*.cpp
	./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.cpp
	$(CHMOD) -w $*.cpp
	$(CP) -f $*.cpp $(SRC_DIR)/cpp-api
	$(CHMOD) -w $(SRC_DIR)/cpp-api/$*.cpp

.dod.idl :
	$(RM) -f $*.idl
	./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.idl
	$(CHMOD) -w $*.idl
	$(CP) -f $*.idl $(INCLUDE_DIR)/com-api/
	$(CHMOD) -w $(INCLUDE_DIR)/com-api/$*.idl

.dod.fidl :
	$(RM) -f $*.fidl
	./tool/$(DODO) -f macros/fidl.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.fidl
	$(CHMOD) -w $*.fidl

.dod.refh :
	$(RM) -f $*.refh
	./tool/$(DODO) -f macros/refh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.refh
	$(CHMOD) -w $*.refh
	$(CP) -f $*.refh $(INCLUDE_DIR)/ref-api/$*.h
	$(CHMOD) -w $(INCLUDE_DIR)/ref-api/$*.h

.dod.frefh :
	$(RM) -f $*.frefh
	./tool/$(DODO) -f macros/frefh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.frefh
	$(CHMOD) -w $*.frefh



clean:
	$(RM) -f *.idl *.fidl *.exp
	$(RM) -f *.comc *.comcx *.comh *.comt *.refh *.frefh
	$(RM) -f *.implc *.implh
	$(RM) -f core
#	$(RM) -f $(TEST_DIR)/CAAF*Test.cpp
#	$(RM) -f $(TEST_DIR)/CEnumAAF*Test.cpp
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFRoot.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFRoot.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	$(RM) -f $(UUID_DIR)/AAFRoot_i.c
	$(RM) -f $(UUID_DIR)/AAF_i.c
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable.h
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.cpp
	$(RM) -f $(COMAPI_DIR)/CAAFEnumValidation.h
	$(RM) -f $(COMAPI_DIR)/CAAFEnumValidation_i.cpp
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) DelTargets.sh >> tmp.sh
	$(SH) tmp.sh
	$(RM) tmp.sh

# This file contains the list of all dependents...
include depend.mk

