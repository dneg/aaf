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

all : targets

MAKEFILE = maketargets.mak

BLD_CFG_DIR = ../ref-impl/bld_cfg
include $(BLD_CFG_DIR)/common.mk

DODO = ./tool/dodo.$(EXE)
UPDATE = ../update.ksh

.SUFFIXES: .cpp .h .comc .comcx .comh .dod .exp .idl .fidl .implc .implh .comt .cppt .refh .frefh

# This file contains the list of all of the targets to be built...
include targets.mk
include aafobjects.mk


INCLUDE_DIR = ../ref-impl/include
IMPL_DIR = ../ref-impl/src/impl
COMAPI_DIR = ../ref-impl/src/com-api
TEST_DIR = ../test/com/ComModTestAAF/ModuleTests

targets: macros/base.mac
targets: $(DODO_TARGETS)

targets: $(INCLUDE_DIR)/ref-api/AAF.h
targets: $(INCLUDE_DIR)/ref-api/AAF_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFPlugin.h
targets: $(INCLUDE_DIR)/ref-api/AAFPlugin_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFPluginTypes.h
targets: $(INCLUDE_DIR)/ref-api/AAFPrivate.h
targets: $(INCLUDE_DIR)/ref-api/AAFPrivate_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFTypes.h

targets: $(INCLUDE_DIR)/com-api/AAF.idl
targets: $(INCLUDE_DIR)/com-api/AAFPlugin.idl
targets: $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
targets: $(INCLUDE_DIR)/com-api/AAFPrivate.idl
targets: $(INCLUDE_DIR)/com-api/AAFTypes.idl

targets: $(INCLUDE_DIR)/com-api/AAF.h
targets: $(INCLUDE_DIR)/com-api/AAF_i.c
targets: $(INCLUDE_DIR)/com-api/AAFPlugin.h
targets: $(INCLUDE_DIR)/com-api/AAFPlugin_i.c
targets: $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
targets: $(INCLUDE_DIR)/com-api/AAFPrivate.h
targets: $(INCLUDE_DIR)/com-api/AAFPrivate_i.c
targets: $(INCLUDE_DIR)/com-api/AAFTypes.h

targets: $(IMPL_DIR)/AAFClassIDs.h
targets: $(COMAPI_DIR)/AAFCLSIDs.h
targets: $(COMAPI_DIR)/AAFObjectTable.h
targets: $(COMAPI_DIR)/AAFObjectTable_i.cpp
targets: $(COMAPI_DIR)/CAAFEnumValidation.h
targets: $(COMAPI_DIR)/CAAFEnumValidation.cpp


PRIVATE_FIDL_DOD_FILES  = $(PRIVATE_FIDL_TARGETS:%.fidl=%.dod)
FIDL_DOD_FILES         	= $(FIDL_TARGETS:%.fidl=%.dod)
PLUGIN_FIDL_DOD_FILES   = $(PLUGIN_FIDL_TARGETS:%.fidl=%.dod)
PRIVATE_FREFH_DOD_FILES = $(PRIVATE_FREFH_TARGETS:%.frefh=%.dod)
FREFH_DOD_FILES        	= $(FREFH_TARGETS:%.frefh=%.dod)
PLUGIN_FREFH_DOD_FILES  = $(PLUGIN_FREFH_TARGETS:%.frefh=%.dod)


# Build rule to make sure that the copyright text in base.mac is always in sync
# with the contents of CopyrightMessage.txt.
macros/base.mac : CopyrightMessage.txt sync_copyright.pl
	$(PERL) sync_copyright.pl --copyright CopyrightMessage.txt --macro macros/base.mac > base.mac.tmp
	@$(UPDATE) base.mac.tmp macros/base.mac
	$(RM) -f base.mac.tmp

# Build rule to make sure that the C++ style copyright text is always in sync
# with the contents of CopyrightMessage.txt.
CopyrightMessage.cpp : CopyrightMessage.txt sync_copyright.pl
	$(PERL) sync_copyright.pl --copyright CopyrightMessage.txt --cpp_style > CopyrightMessage.cpp


# Build rule to make sure that the IDL style copyright text is always in sync
# with the contents of CopyrightMessage.txt.
CopyrightMessage.idl : CopyrightMessage.txt sync_copyright.pl
	$(PERL) sync_copyright.pl --copyright CopyrightMessage.txt --idl_style > CopyrightMessage.idl


$(INCLUDE_DIR)/com-api/AAFPrivate.idl : $(PRIVATE_FIDL_DOD_FILES) macros/fidl.mac macros/base.mac aafobjects.mk CopyrightMessage.txt GenAafPrivateIdl.sh
	@$(ECHO) Generating AAFPrivate.idl...
	$(MAKE) -f $(MAKEFILE) AAFPrivate.idl.tmp
	@$(UPDATE) AAFPrivate.idl.tmp $(INCLUDE_DIR)/com-api/AAFPrivate.idl
	$(RM) -f AAFPrivate.idl.tmp


AAFPrivate.idl.tmp : $(PRIVATE_FIDL_TARGETS) aafobjects.mk CopyrightMessage.idl GenAafPrivateIdl.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafPrivateIdl.sh >> tmp.sh
	$(CAT) CopyrightMessage.idl > AAFPrivate.idl.tmp
	$(SH) tmp.sh >> AAFPrivate.idl.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/com-api/AAF.idl : $(FIDL_DOD_FILES) macros/fidl.mac macros/base.mac aafobjects.mk CopyrightMessage.txt GenAafIdl.sh
	@ $(ECHO) Generating AAF.idl...
	$(MAKE) -f $(MAKEFILE) AAF.idl.tmp
	@$(UPDATE) AAF.idl.tmp $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) -f AAF.idl.tmp


AAF.idl.tmp : $(FIDL_TARGETS) aafobjects.mk CopyrightMessage.idl GenAafIdl.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafIdl.sh >> tmp.sh
	$(CAT) CopyrightMessage.idl > AAF.idl.tmp
	$(SH) tmp.sh >> AAF.idl.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/com-api/AAFPlugin.idl : $(PLUGIN_FIDL_DOD_FILES) macros/fidl.mac macros/base.mac aafobjects.mk CopyrightMessage.txt GenPluginIdl.sh
	@$(ECHO) Generating Plugin.idl...
	$(MAKE) -f $(MAKEFILE) AAFPlugin.idl.tmp
	@$(UPDATE) AAFPlugin.idl.tmp $(INCLUDE_DIR)/com-api/AAFPlugin.idl
	$(RM) -f AAFPlugin.idl.tmp


AAFPlugin.idl.tmp : $(PLUGIN_FIDL_TARGETS) aafobjects.mk CopyrightMessage.idl GenPluginIdl.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPluginIdl.sh >> tmp.sh
	$(CAT) CopyrightMessage.idl > AAFPlugin.idl.tmp
	$(SH) tmp.sh >> AAFPlugin.idl.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/ref-api/AAFPrivate.h : aafobjects.mk $(PRIVATE_FREFH_DOD_FILES) macros/frefh.mac macros/base.mac CopyrightMessage.txt GenAafPrivateh.sh
	@$(ECHO) Generating reference AAFPrivate.h...
	$(MAKE) -f $(MAKEFILE) AAFPrivate.h.tmp
	@$(UPDATE) AAFPrivate.h.tmp $(INCLUDE_DIR)/ref-api/AAFPrivate.h
	$(RM) -f $AAFPrivate.h.tmp


AAFPrivate.h.tmp : aafobjects.mk $(PRIVATE_FREFH_TARGETS) CopyrightMessage.cpp GenAafPrivateh.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafPrivateh.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFPrivate.h.tmp
	$(SH) tmp.sh >> AAFPrivate.h.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/ref-api/AAF.h : $(FREFH_DOD_FILES) macros/frefh.mac macros/base.mac aafobjects.mk CopyrightMessage.txt GenAafh.sh
	@ $(ECHO) Generating reference AAF.h...
	$(MAKE) -f $(MAKEFILE) AAF.h.tmp
	@$(UPDATE) AAF.h.tmp $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) -f AAF.h.tmp


AAF.h.tmp : $(FREFH_TARGETS) aafobjects.mk CopyrightMessage.cpp GenAafh.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafh.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAF.h.tmp
	$(SH) tmp.sh >> AAF.h.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/ref-api/AAFPlugin.h : $(PLUGIN_FREFH_DOD_FILES) macros/frefh.mac macros/base.mac aafobjects.mk CopyrightMessage.txt GenPluginh.sh
	@ $(ECHO) Generating reference AAFPlugin.h...
	$(MAKE) -f $(MAKEFILE) AAFPlugin.h.tmp
	@$(UPDATE) AAFPlugin.h.tmp $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(RM) -f AAFPlugin.h.tmp


AAFPlugin.h.tmp : $(PLUGIN_FREFH_TARGETS) aafobjects.mk CopyrightMessage.cpp GenPluginh.sh
	$(CP) aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPluginh.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFPlugin.h.tmp
	$(SH) tmp.sh >> AAFPlugin.h.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/ref-api/AAF_i.c : aafobjects.mk dod2iid.awk CopyrightMessage.txt GenAaf_i.sh
	@ $(ECHO) Generating reference AAF_i.c...
	$(MAKE) -f $(MAKEFILE) AAF_i.refh.tmp
	@$(UPDATE) AAF_i.refh.tmp $(INCLUDE_DIR)/ref-api/AAF_i.c
	$(RM) -f AAF_i.refh.tmp


AAF_i.refh.tmp : aafobjects.mk dod2iid.awk CopyrightMessage.cpp GenAaf_i.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAaf_i.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAF_i.refh.tmp
	$(SH) tmp.sh >> AAF_i.refh.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/ref-api/AAFPlugin_i.c : aafobjects.mk dod2iid.awk CopyrightMessage.txt GenPlugin_i.sh
	@ $(ECHO) Generating reference AAFPlugin_i.c...
	$(MAKE) -f $(MAKEFILE) AAFPlugin_i.refh.tmp
	@$(UPDATE) AAFPlugin_i.refh.tmp $(INCLUDE_DIR)/ref-api/AAFPlugin_i.c
	$(RM) -f AAFPlugin_i.refh.tmp


AAFPlugin_i.refh.tmp : aafobjects.mk dod2iid.awk CopyrightMessage.cpp GenPlugin_i.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPlugin_i.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFPlugin_i.refh.tmp
	$(SH) tmp.sh >> AAFPlugin_i.refh.tmp
	$(RM) tmp.sh


$(INCLUDE_DIR)/ref-api/AAFPrivate_i.c : aafobjects.mk dod2iid.awk CopyrightMessage.txt GenAafPrivate_i.sh
	@ $(ECHO) Generating reference AAFPrivate_i.c...
	$(MAKE) -f $(MAKEFILE) AAFPrivate_i.refh.tmp
	@$(UPDATE) AAFPrivate_i.refh.tmp $(INCLUDE_DIR)/ref-api/AAFPrivate_i.c
	$(RM) -f AAFPrivate_i.refh.tmp


AAFPrivate_i.refh.tmp : aafobjects.mk dod2iid.awk CopyrightMessage.cpp GenAafPrivate_i.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafPrivate_i.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFPrivate_i.refh.tmp
	$(SH) tmp.sh >> AAFPrivate_i.refh.tmp
	$(RM) tmp.sh


$(IMPL_DIR)/AAFClassIDs.h : aafobjects.mk CopyrightMessage.txt GenClassIDs.sh
	@ $(ECHO) Generating reference AAFClassIDs.h...
	$(MAKE) -f $(MAKEFILE) AAFClassIDs.impl.tmp
	@$(UPDATE) AAFClassIDs.impl.tmp $(IMPL_DIR)/AAFClassIDs.h
	$(RM) -f AAFClassIDs.impl.tmp


AAFClassIDs.impl.tmp : aafobjects.mk CopyrightMessage.cpp GenClassIDs.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenClassIDs.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFClassIDs.impl.tmp
	$(SH) tmp.sh >> AAFClassIDs.impl.tmp
	$(RM) tmp.sh


$(COMAPI_DIR)/AAFCLSIDs.h : aafobjects.mk CopyrightMessage.txt GenCLSIDs.sh
	@ $(ECHO) Generating reference AAFCLSIDs.h...
	$(MAKE) -f $(MAKEFILE) AAFCLSIDs.comh.tmp
	@$(UPDATE) AAFCLSIDs.comh.tmp $(COMAPI_DIR)/AAFCLSIDs.h
	$(RM) -f AAFCLSIDs.comh.tmp


AAFCLSIDs.comh.tmp : aafobjects.mk CopyrightMessage.cpp GenCLSIDs.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenCLSIDs.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFCLSIDs.comh.tmp
	$(SH) tmp.sh >> AAFCLSIDs.comh.tmp
	$(RM) tmp.sh


$(COMAPI_DIR)/AAFObjectTable.h : aafobjects.mk CopyrightMessage.txt GenObjectTable.sh
	@ $(ECHO) Generating reference AAFObjectTable.h...
	$(MAKE) -f $(MAKEFILE) AAFObjectTable.comh.tmp
	@$(UPDATE) AAFObjectTable.comh.tmp $(COMAPI_DIR)/AAFObjectTable.h
	$(RM) -f AAFObjectTable.comh.tmp


AAFObjectTable.comh.tmp : aafobjects.mk CopyrightMessage.cpp GenObjectTable.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenObjectTable.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFObjectTable.comh.tmp
	$(SH) tmp.sh >> AAFObjectTable.comh.tmp
	$(RM) tmp.sh


$(COMAPI_DIR)/AAFObjectTable_i.cpp : aafobjects.mk CopyrightMessage.txt GenObjectTable_i.sh
	@ $(ECHO) Generating reference AAFObjectTable_i.cpp...
	$(MAKE) -f $(MAKEFILE) AAFObjectTable_i.refh.tmp
	@$(UPDATE) AAFObjectTable_i.refh.tmp $(COMAPI_DIR)/AAFObjectTable_i.cpp
	$(RM) -f AAFObjectTable_i.refh.tmp


AAFObjectTable_i.refh.tmp : aafobjects.mk CopyrightMessage.cpp GenObjectTable_i.sh
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenObjectTable_i.sh >> tmp.sh
	$(CAT) CopyrightMessage.cpp > AAFObjectTable_i.refh.tmp
	$(SH) tmp.sh >> AAFObjectTable_i.refh.tmp
	$(RM) tmp.sh


$(COMAPI_DIR)/CAAFEnumValidation.cpp : GenEnumValidation.pl AAFPluginTypes.dod AAFTypes.dod CopyrightMessage.txt
	@ $(ECHO) Generating CAAFEnumValidation.cpp...
	$(MAKE) -f $(MAKEFILE) CAAFEnumValidation.cpp.tmp
	@$(UPDATE) CAAFEnumValidation.cpp.tmp $(COMAPI_DIR)/CAAFEnumValidation.cpp
	$(RM) -f CAAFEnumValidation.cpp.tmp


CAAFEnumValidation.cpp.tmp : GenEnumValidation.pl AAFPluginTypes.dod AAFTypes.dod CopyrightMessage.cpp
	$(PERL) GenEnumValidation.pl --source AAFTypes.dod AAFPluginTypes.dod > GenEnumValidationLog.cpp.txt
	$(CAT) CopyrightMessage.cpp > CAAFEnumValidation.cpp.tmp
	$(CAT) CAAFEnumValidation.cpp >> CAAFEnumValidation.cpp.tmp
	$(RM) -f CAAFEnumValidation.cpp


$(COMAPI_DIR)/CAAFEnumValidation.h : GenEnumValidation.pl AAFPluginTypes.dod AAFTypes.dod CopyrightMessage.txt
	@ $(ECHO) Generating CAAFEnumValidation.h...
	$(MAKE) -f $(MAKEFILE) CAAFEnumValidation.h.tmp
	@$(UPDATE) CAAFEnumValidation.h.tmp $(COMAPI_DIR)/CAAFEnumValidation.h
	$(RM) -f CAAFEnumValidation.h.tmp


CAAFEnumValidation.h.tmp : GenEnumValidation.pl AAFPluginTypes.dod AAFTypes.dod CopyrightMessage.cpp
	$(PERL) GenEnumValidation.pl --include AAFTypes.dod AAFPluginTypes.dod > GenEnumValidationLog.h.txt
	$(CAT) CopyrightMessage.cpp > CAAFEnumValidation.h.tmp
	$(CAT) CAAFEnumValidation.h >> CAAFEnumValidation.h.tmp
	$(RM) -f CAAFEnumValidation.h



#
#
# Rules for files generated in ref-impl/include/com-api directory by IDL compiler.
# Currently it only runs on NT.
#
#
$(INCLUDE_DIR)/com-api/AAF.h $(INCLUDE_DIR)/com-api/AAF_i.c : $(INCLUDE_DIR)/com-api/AAF.idl
	@ $(ECHO) Generating AAF.h AAF_i.c ...
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAF.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAF.h > AAF.h.tmp
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAF_i.c > AAF_i.c.tmp
	$(UPDATE) AAF.h.tmp $(INCLUDE_DIR)/com-api/AAF.h
	$(UPDATE) AAF_i.c.tmp $(INCLUDE_DIR)/com-api/AAF_i.c
	$(RM) AAF.h AAF_i.c AAF_p.c AAF.h.tmp AAF_i.c.tmp dlldata.c

$(INCLUDE_DIR)/com-api/AAFPlugin.h $(INCLUDE_DIR)/com-api/AAFPlugin_i.c : $(INCLUDE_DIR)/com-api/AAFPlugin.idl
	@ $(ECHO) Generating AAFPlugin.h AAFPlugin_i.c ...
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFPlugin.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPlugin.h > AAFPlugin.h.tmp
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPlugin_i.c > AAFPlugin_i.c.tmp
	$(UPDATE) AAFPlugin.h.tmp $(INCLUDE_DIR)/com-api/AAFPlugin.h
	$(UPDATE) AAFPlugin_i.c.tmp $(INCLUDE_DIR)/com-api/AAFPlugin_i.c
	$(RM) AAFPlugin.h AAFPlugin_i.c AAFPlugin_p.c AAFPlugin.h.tmp AAFPlugin_i.c.tmp dlldata.c

$(INCLUDE_DIR)/com-api/AAFPluginTypes.h : $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	@ $(ECHO) Generating AAFPluginTypes.h
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPluginTypes.h > AAFPluginTypes.h.tmp
	$(UPDATE) AAFPluginTypes.h.tmp $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
	$(RM) AAFPluginTypes.h AAFPluginTypes.h.tmp

$(INCLUDE_DIR)/com-api/AAFPrivate.h $(INCLUDE_DIR)/com-api/AAFPrivate_i.c : $(INCLUDE_DIR)/com-api/AAFPrivate.idl
	@ $(ECHO) Generating AAFPrivate.h AAFPrivate_i.c ...
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFPrivate.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPrivate.h > AAFPrivate.h.tmp
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPrivate_i.c > AAFPrivate_i.c.tmp
	$(UPDATE) AAFPrivate.h.tmp $(INCLUDE_DIR)/com-api/AAFPrivate.h
	$(UPDATE) AAFPrivate_i.c.tmp $(INCLUDE_DIR)/com-api/AAFPrivate_i.c
	$(RM) AAFPrivate.h AAFPrivate_i.c AAFPrivate_p.c AAFPrivate.h.tmp AAFPrivate_i.c.tmp dlldata.c

$(INCLUDE_DIR)/com-api/AAFTypes.h : $(INCLUDE_DIR)/com-api/AAFTypes.idl
	@ $(ECHO) Generating AAFTypes.h
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFTypes.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFTypes.h > AAFTypes.h.tmp
	$(UPDATE) AAFTypes.h.tmp $(INCLUDE_DIR)/com-api/AAFTypes.h
	$(RM) AAFTypes.h AAFTypes.h.tmp



SRC_DIR = ../ref-impl/src

$(INCLUDE_DIR)/cpp-api/%.h : %.dod macros/h.mac macros/base.mac
	$(MAKE) -f $*.h
	@$(UPDATE) $*.h $(INCLUDE_DIR)/cpp-api/$*.h

$(TEST_DIR)/%Test.cpp : %.dod macros/cppt.mac macros/base.mac
	$(MAKE) -f $*.cppt
	@$(UPDATE) $*.cppt $(TEST_DIR)/$*Test.cpp

$(COMAPI_DIR)/C%.h : %.dod macros/comh.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.comh
	@$(UPDATE) $*.comh $(SRC_DIR)/com-api/C$*.h

$(COMAPI_DIR)/C%.cpp : %.dod macros/comc.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.comc
	@$(UPDATE) $*.comc $(SRC_DIR)/com-api/C$*.cpp

$(TEST_DIR)/C%Test.cpp : %.dod macros/comt.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.comt
	@$(UPDATE) $*.comt $(TEST_DIR)/C$*Test.cpp

$(SRC_DIR)/impl/Impl%.h : %.dod macros/implh.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.implh
	@$(UPDATE) $*.implh $(SRC_DIR)/impl/Impl$*.h

$(SRC_DIR)/impl/Impl%.cpp : %.dod macros/implc.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.implc
	@$(UPDATE) $*.implc $(SRC_DIR)/impl/Impl$*.cpp

$(SRC_DIR)/cpp-api/%.cpp : %.dod macros/cpp.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.cpp
	@$(UPDATE) $*.cpp $(SRC_DIR)/cpp-api/$*.cpp

$(INCLUDE_DIR)/com-api/%.idl : %.dod macros/idl.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.idl
	@$(UPDATE) $*.idl $(INCLUDE_DIR)/com-api/$*.idl

$(INCLUDE_DIR)/ref-api/%.h : %.dod macros/refh.mac macros/base.mac
	$(MAKE) -f $(MAKEFILE) $*.refh
	@$(UPDATE) $*.refh $(INCLUDE_DIR)/ref-api/$*.h

.dod.exp :
	$(RM) -f $*.exp
	$(DODO) -f macros/exp.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.exp
	$(CHMOD) -w $*.exp

.dod.h :
	$(RM) -f $*.h
	$(DODO) -f macros/h.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.h
	$(CHMOD) -w $*.h

.dod.cppt :
	$(RM) -f $*.cppt
	$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.cppt
	$(CHMOD) -w $*.cppt

.dod.comh :
	$(RM) -f $*.comh
	$(DODO) -f macros/comh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comh
	$(CHMOD) -w $*.comh

.dod.comc :
	$(RM) -f $*.comc
	$(DODO) -f macros/comc.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comc
	$(CHMOD) -w $*.comc

.dod.comcx :
	$(RM) -f $*.comcx
	$(DODO) -f macros/comcx.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comcx
	$(CHMOD) -w $*.comcx

.dod.comt :
	$(RM) -f $*.comt
	$(DODO) -f macros/comt.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comt
	$(CHMOD) -w $*.comt

.dod.implh :
	$(RM) -f $*.implh ;
	$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ;
	$(MV) $*.tmp $*.implh ;
	$(CHMOD) -w $*.implh ;

.dod.implc :
	$(RM) -f $*.implc ;
	$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ;
	$(MV) $*.tmp $*.implc ;
	$(CHMOD) -w $*.implc ;

.dod.cpp :
	$(RM) -f $*.cpp
	$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.cpp
	$(CHMOD) -w $*.cpp

.dod.idl :
	$(RM) -f $*.idl
	$(DODO) -f macros/idl.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.idl
	$(CHMOD) -w $*.idl

.dod.fidl :
	$(RM) -f $*.fidl
	perl GenObjectInterfaces.pl $* > ObjInt.tmp
	$(DODO) -f macros/fidl.mac < $*.dod > $*.tmp
	perl -p -0 \
		-e 's/(^\[\n\s*object,\n\s*uuid)/OBJ_INT\n\1/m;'\
		-e 's/^(\n\n)\n+/\1/mg;'\
		$*.tmp > $*X.tmp
	sed -e '/OBJ_INT/r ObjInt.tmp' -e '/OBJ_INT/d' $*X.tmp > $*.fidl
	$(CHMOD) -w $*.fidl
	$(RM) -f ObjInt.tmp $*.tmp $*X.tmp

.dod.refh :
	$(RM) -f $*.refh
	$(DODO) -f macros/refh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.refh
	$(CHMOD) -w $*.refh

.dod.frefh :
	$(RM) -f $*.frefh
	$(DODO) -f macros/frefh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.frefh
	$(CHMOD) -w $*.frefh


clean:
	$(RM) -f *.idl *.fidl *.exp
	$(RM) -f *.comc *.comcx *.comh *.comt *.refh *.frefh
	$(RM) -f *.implc *.implh
	$(RM) -f core
	$(RM) -f GenEnumValidationLog.*
	$(RM) -f *.tmp
	$(RM) -f tmp.sh
	$(RM) -f CopyrightMessage.cpp
	$(RM) -f CopyrightMessage.idl

realclean : clean
#	$(RM) -f $(TEST_DIR)/CAAF*Test.cpp
#	$(RM) -f $(TEST_DIR)/CEnumAAF*Test.cpp
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.h $(INCLUDE_DIR)/com-api/AAF_i.c
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPlugin.h $(INCLUDE_DIR)/com-api/AAFPlugin_i.c
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPrivate.h $(INCLUDE_DIR)/com-api/AAFPrivate_i.c
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPrivate.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPlugin.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPrivate.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFRoot.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	$(RM) -f $(IMPL_DIR)/ImplAAFRoot.h
	$(RM) -f $(IMPL_DIR)/ImplAAFRoot.cpp
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPrivate_i.c
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF_i.c
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin_i.c
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable.h
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.cpp
	$(RM) -f $(COMAPI_DIR)/CAAFEnumValidation.h
	$(RM) -f $(COMAPI_DIR)/CAAFEnumValidation.cpp
	$(RM) -f $(COMAPI_DIR)/CAAFModule.cpp
	$(RM) -f $(COMAPI_DIR)/CAAFRoot.h
	$(RM) -f $(COMAPI_DIR)/CAAFRoot.cpp
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(ECHO) SRC_DIR=$(SRC_DIR) >> tmp.sh
	$(ECHO) INCLUDE_DIR=$(INCLUDE_DIR) >> tmp.sh
	$(ECHO) TEST_DIR=$(TEST_DIR) >> tmp.sh
	$(CAT) DelTargets.sh >> tmp.sh
	$(SH) tmp.sh
	$(RM) tmp.sh

# This file contains the list of all dependents...
include depend.mk
