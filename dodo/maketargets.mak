#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
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
targets: $(INCLUDE_DIR)/com-api/AAFPrivate.idl
targets: $(INCLUDE_DIR)/com-api/AAF.idl
targets: $(INCLUDE_DIR)/com-api/AAFPlugin.idl
targets: $(INCLUDE_DIR)/ref-api/AAFPrivate.h
targets: $(INCLUDE_DIR)/ref-api/AAF.h
targets: $(UUID_DIR)/AAFPrivate_i.c
targets: $(UUID_DIR)/AAF_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFPlugin.h
targets: $(UUID_DIR)/AAFPlugin_i.c
targets: $(COMAPI_DIR)/AAFCLSIDs.h
targets: $(IMPL_DIR)/AAFClassIDs.h
targets: $(COMAPI_DIR)/AAFObjectTable.h
targets: $(COMAPI_DIR)/AAFObjectTable_i.cpp
targets: $(COMAPI_DIR)/CAAFEnumValidation.h
targets: $(COMAPI_DIR)/CAAFEnumValidation.cpp
targets: $(INCLUDE_DIR)/com-api/AAF.h $(INCLUDE_DIR)/com-api/AAF_i.c
targets: $(INCLUDE_DIR)/com-api/AAFPlugin.h $(INCLUDE_DIR)/com-api/AAFPlugin_i.c
targets: $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
targets: $(INCLUDE_DIR)/com-api/AAFPrivate.h $(INCLUDE_DIR)/com-api/AAFPrivate_i.c
targets: $(INCLUDE_DIR)/com-api/AAFTypes.h


$(INCLUDE_DIR)/com-api/AAFPrivate.idl : $(PRIVATE_FIDL_TARGETS) aafobjects.mk CopyrightMessage.txt GenAafPrivateIdl.sh
	@ $(ECHO) Generating AAFPrivate.idl...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafPrivateIdl.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/com-api/AAFPrivate.tmp
	$(RM) tmp.sh
	@if [ -f $(INCLUDE_DIR)/com-api/AAFPrivate.idl ] && cmp -s $(INCLUDE_DIR)/com-api/AAFPrivate.tmp $(INCLUDE_DIR)/com-api/AAFPrivate.idl; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPrivate.idl did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPrivate.idl; \
		$(CP) -f $(INCLUDE_DIR)/com-api/AAFPrivate.tmp $(INCLUDE_DIR)/com-api/AAFPrivate.idl; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPrivate.idl; \
	fi
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPrivate.tmp


$(INCLUDE_DIR)/com-api/AAF.idl : $(FIDL_TARGETS) aafobjects.mk CopyrightMessage.txt GenAafIdl.sh
	@ $(ECHO) Generating AAF.idl...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafIdl.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/com-api/AAF.tmp
	$(RM) tmp.sh
	@if [ -f $(INCLUDE_DIR)/com-api/AAF.idl ] && cmp -s $(INCLUDE_DIR)/com-api/AAF.tmp $(INCLUDE_DIR)/com-api/AAF.idl; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAF.idl did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAF.idl; \
		$(CP) -f $(INCLUDE_DIR)/com-api/AAF.tmp $(INCLUDE_DIR)/com-api/AAF.idl; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAF.idl; \
	fi
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.tmp


$(INCLUDE_DIR)/com-api/AAFPlugin.idl : $(PLUGIN_FIDL_TARGETS) aafobjects.mk CopyrightMessage.txt GenPluginIdl.sh
	@ $(ECHO) Generating Plugin.idl...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPluginIdl.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/com-api/AAFPlugin.tmp
	$(RM) tmp.sh
	@if [ -f $(INCLUDE_DIR)/com-api/AAFPlugin.idl ] && cmp -s $(INCLUDE_DIR)/com-api/AAFPlugin.tmp $(INCLUDE_DIR)/com-api/AAFPlugin.idl;\
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPlugin.idl did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPlugin.idl; \
		$(CP) -f $(INCLUDE_DIR)/com-api/AAFPlugin.tmp $(INCLUDE_DIR)/com-api/AAFPlugin.idl; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPlugin.idl; \
	fi
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPlugin.tmp


$(INCLUDE_DIR)/ref-api/AAFPrivate.h : aafobjects.mk $(PRIVATE_FREFH_TARGETS) CopyrightMessage.txt GenAafPrivateh.sh
	@ $(ECHO) Generating reference AAFPrivate.h...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafPrivateh.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/ref-api/AAFPrivate.tmp
	$(RM) tmp.sh
	@if [ -f $(INCLUDE_DIR)/ref-api/AAFPrivate.h ] && cmp -s $(INCLUDE_DIR)/ref-api/AAFPrivate.tmp $(INCLUDE_DIR)/ref-api/AAFPrivate.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/ref-api/AAFPrivate.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/ref-api/AAFPrivate.h; \
		$(CP) -f $(INCLUDE_DIR)/ref-api/AAFPrivate.tmp $(INCLUDE_DIR)/ref-api/AAFPrivate.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/ref-api/AAFPrivate.h; \
	fi
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPrivate.tmp


$(INCLUDE_DIR)/ref-api/AAF.h : $(FREFH_TARGETS) aafobjects.mk CopyrightMessage.txt GenAafh.sh
	@ $(ECHO) Generating reference AAF.h...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafh.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/ref-api/AAF.tmp
	$(RM) tmp.sh
	@if [ -f $(INCLUDE_DIR)/ref-api/AAF.h ] && cmp -s $(INCLUDE_DIR)/ref-api/AAF.tmp $(INCLUDE_DIR)/ref-api/AAF.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/ref-api/AAF.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/ref-api/AAF.h; \
		$(CP) -f $(INCLUDE_DIR)/ref-api/AAF.tmp $(INCLUDE_DIR)/ref-api/AAF.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/ref-api/AAF.h; \
	fi
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.tmp


$(INCLUDE_DIR)/ref-api/AAFPlugin.h : $(PLUGIN_FREFH_TARGETS) aafobjects.mk CopyrightMessage.txt GenPluginh.sh
	@ $(ECHO) Generating reference AAFPlugin.h...
	$(CP) aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPluginh.sh >> tmp.sh
	$(SH) tmp.sh > $(INCLUDE_DIR)/ref-api/AAFPlugin.tmp
	$(RM) tmp.sh
	@if [ -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h ] && cmp -s $(INCLUDE_DIR)/ref-api/AAFPlugin.tmp $(INCLUDE_DIR)/ref-api/AAFPlugin.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/ref-api/AAFPlugin.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/ref-api/AAFPlugin.h; \
		$(CP) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.tmp $(INCLUDE_DIR)/ref-api/AAFPlugin.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/ref-api/AAFPlugin.h; \
	fi
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.tmp


$(UUID_DIR)/AAF_i.c : aafobjects.mk dod2iid.awk CopyrightMessage.txt GenAaf_i.sh
	@ $(ECHO) Generating reference AAF_i.c...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAaf_i.sh >> tmp.sh
	$(SH) tmp.sh > $(UUID_DIR)/AAF_i.tmp
	$(RM) tmp.sh
	@if [ -f $(UUID_DIR)/AAF_i.c ] && cmp -s $(UUID_DIR)/AAF_i.tmp $(UUID_DIR)/AAF_i.c; \
	then \
		$(ECHO) $(UUID_DIR)/AAF_i.c did not change; \
	else \
		$(ECHO) Updating $(UUID_DIR)/AAF_i.c; \
		$(CP) -f $(UUID_DIR)/AAF_i.tmp $(UUID_DIR)/AAF_i.c; \
		$(CHMOD) +w $(UUID_DIR)/AAF_i.c; \
	fi
	$(RM) -f $(UUID_DIR)/AAF_i.tmp


$(UUID_DIR)/AAFPlugin_i.c : aafobjects.mk dod2iid.awk CopyrightMessage.txt GenPlugin_i.sh
	@ $(ECHO) Generating reference AAFPlugin_i.c...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenPlugin_i.sh >> tmp.sh
	$(SH) tmp.sh > $(UUID_DIR)/AAFPlugin_i.tmp
	$(RM) tmp.sh
	@if [ -f $(UUID_DIR)/AAFPlugin_i.c ] && cmp -s $(UUID_DIR)/AAFPlugin_i.tmp $(UUID_DIR)/AAFPlugin_i.c; \
	then \
		$(ECHO) $(UUID_DIR)/AAFPlugin_i.c did not change; \
	else \
		$(ECHO) Updating $(UUID_DIR)/AAFPlugin_i.c; \
		$(CP) -f $(UUID_DIR)/AAFPlugin_i.tmp $(UUID_DIR)/AAFPlugin_i.c; \
		$(CHMOD) +w $(UUID_DIR)/AAFPlugin_i.c; \
	fi
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.tmp


$(UUID_DIR)/AAFPrivate_i.c : aafobjects.mk dod2iid.awk CopyrightMessage.txt GenAafPrivate_i.sh
	@ $(ECHO) Generating reference AAFPrivate_i.c...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenAafPrivate_i.sh >> tmp.sh
	$(SH) tmp.sh > $(UUID_DIR)/AAFPrivate_i.tmp
	$(RM) tmp.sh
	@if [ -f $(UUID_DIR)/AAFPrivate_i.c ] && cmp -s $(UUID_DIR)/AAFPrivate_i.tmp $(UUID_DIR)/AAFPrivate_i.c; \
	then \
		$(ECHO) $(UUID_DIR)/AAFPrivate_i.c did not change; \
	else \
		$(ECHO) Updating $(UUID_DIR)/AAFPrivate_i.c; \
		$(CP) -f $(UUID_DIR)/AAFPrivate_i.tmp $(UUID_DIR)/AAFPrivate_i.c; \
		$(CHMOD) +w $(UUID_DIR)/AAFPrivate_i.c; \
	fi
	$(RM) -f $(UUID_DIR)/AAFPrivate_i.tmp


$(IMPL_DIR)/AAFClassIDs.h : aafobjects.mk CopyrightMessage.txt GenClassIDs.sh
	@ $(ECHO) Generating reference AAFClassIDs.h...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenClassIDs.sh >> tmp.sh
	$(SH) tmp.sh > $(IMPL_DIR)/AAFClassIDs.tmp
	$(RM) tmp.sh
	@if [ -f $(IMPL_DIR)/AAFClassIDs.h ] && cmp -s $(IMPL_DIR)/AAFClassIDs.tmp $(IMPL_DIR)/AAFClassIDs.h; \
	then \
		$(ECHO) $(IMPL_DIR)/AAFClassIDs.h did not change; \
	else \
		$(ECHO) Updating $(IMPL_DIR)/AAFClassIDs.h; \
		$(CP) -f $(IMPL_DIR)/AAFClassIDs.tmp $(IMPL_DIR)/AAFClassIDs.h; \
		$(CHMOD) +w $(IMPL_DIR)/AAFClassIDs.h; \
	fi
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.tmp


$(COMAPI_DIR)/AAFCLSIDs.h : aafobjects.mk CopyrightMessage.txt GenCLSIDs.sh
	@ $(ECHO) Generating reference AAFCLSIDs.h...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenCLSIDs.sh >> tmp.sh
	$(SH) tmp.sh > $(COMAPI_DIR)/AAFCLSIDs.tmp
	$(RM) tmp.sh
	@if [ -f $(COMAPI_DIR)/AAFCLSIDs.h ] && cmp -s $(COMAPI_DIR)/AAFCLSIDs.tmp $(COMAPI_DIR)/AAFCLSIDs.h; \
	then \
		$(ECHO) $(COMAPI_DIR)/AAFCLSIDs.h did not change; \
	else \
		$(ECHO) Updating $(COMAPI_DIR)/AAFCLSIDs.h; \
		$(CP) -f $(COMAPI_DIR)/AAFCLSIDs.tmp $(COMAPI_DIR)/AAFCLSIDs.h; \
		$(CHMOD) +w $(COMAPI_DIR)/AAFCLSIDs.h; \
	fi
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.tmp


$(COMAPI_DIR)/AAFObjectTable.h : aafobjects.mk CopyrightMessage.txt GenObjectTable.sh
	@ $(ECHO) Generating reference AAFObjectTable.h...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenObjectTable.sh >> tmp.sh
	$(SH) tmp.sh > $(COMAPI_DIR)/AAFObjectTable.tmp
	$(RM) tmp.sh
	@if [ -f  $(COMAPI_DIR)/AAFObjectTable.h ] && cmp -s $(COMAPI_DIR)/AAFObjectTable.tmp $(COMAPI_DIR)/AAFObjectTable.h; \
	then \
		$(ECHO) $(COMAPI_DIR)/AAFObjectTable.h did not change; \
	else \
		$(ECHO) Updating $(COMAPI_DIR)/AAFObjectTable.h; \
		$(CP) -f $(COMAPI_DIR)/AAFObjectTable.tmp $(COMAPI_DIR)/AAFObjectTable.h; \
		$(CHMOD) +w $(COMAPI_DIR)/AAFObjectTable.h; \
	fi
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable.tmp


$(COMAPI_DIR)/AAFObjectTable_i.cpp : aafobjects.mk CopyrightMessage.txt GenObjectTable_i.sh
	@ $(ECHO) Generating reference AAFObjectTable_i.cpp...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenObjectTable_i.sh >> tmp.sh
	$(SH) tmp.sh > $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(RM) tmp.sh
	@if [ -f $(COMAPI_DIR)/AAFObjectTable_i.cpp ] && cmp -s $(COMAPI_DIR)/AAFObjectTable_i.tmp $(COMAPI_DIR)/AAFObjectTable_i.cpp; \
	then \
		$(ECHO) $(COMAPI_DIR)/AAFObjectTable_i.cpp did not change; \
	else \
		$(ECHO) Updating $(COMAPI_DIR)/AAFObjectTable_i.cpp; \
		$(CP) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp $(COMAPI_DIR)/AAFObjectTable_i.cpp; \
		$(CHMOD) +w $(COMAPI_DIR)/AAFObjectTable_i.cpp; \
	fi
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp

$(COMAPI_DIR)/CAAFEnumValidation.cpp $(COMAPI_DIR)/CAAFEnumValidation.h : GenEnumValidation.pl AAFPluginTypes.dod AAFTypes.dod
	@ $(ECHO) Generating references CAAFEnumValidation.cpp and CAAFEnumValidation.h ...
	$(PERL) GenEnumValidation.pl AAFTypes.dod AAFPluginTypes.dod > GenEnumValidationLog.txt
	@if [ -f $(COMAPI_DIR)/CAAFEnumValidation.cpp ] && cmp -s CAAFEnumValidation.cpp $(COMAPI_DIR)/CAAFEnumValidation.cpp; \
	then \
		$(ECHO) $(COMAPI_DIR)/CAAFEnumValidation.cpp did not change; \
	else \
		$(ECHO) Updating $(COMAPI_DIR)/CAAFEnumValidation.cpp; \
		$(CP) -f CAAFEnumValidation.cpp $(COMAPI_DIR); \
		$(CHMOD) +w $(COMAPI_DIR)/CAAFEnumValidation.cpp; \
	fi
	@if [ -f $(COMAPI_DIR)/CAAFEnumValidation.h ] && cmp -s CAAFEnumValidation.h $(COMAPI_DIR)/CAAFEnumValidation.h; \
	then \
		$(ECHO) $(COMAPI_DIR)/CAAFEnumValidation.h did not change; \
	else \
		$(ECHO) Updating $(COMAPI_DIR)/CAAFEnumValidation.h; \
		$(CP) -f CAAFEnumValidation.h $(COMAPI_DIR); \
		$(CHMOD) +w $(COMAPI_DIR)/CAAFEnumValidation.h; \
	fi
	$(RM) -f CAAFEnumValidation.cpp CAAFEnumValidation.h


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
	@if [ -f  $(INCLUDE_DIR)/com-api/AAF.h ] && cmp -s AAF.h.tmp $(INCLUDE_DIR)/com-api/AAF.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAF.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAF.h; \
		$(CP) -f AAF.h.tmp $(INCLUDE_DIR)/com-api/AAF.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAF.h; \
	fi
	@if [ -f  $(INCLUDE_DIR)/com-api/AAF_i.c ] && cmp -s AAF_i.c.tmp $(INCLUDE_DIR)/com-api/AAF_i.c; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAF_i.c did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAF_i.c; \
		$(CP) -f AAF_i.c.tmp $(INCLUDE_DIR)/com-api/AAF_i.c; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAF_i.c; \
	fi
	$(RM) AAF.h AAF_i.c AAF_p.c AAF.h.tmp AAF_i.c.tmp dlldata.c

$(INCLUDE_DIR)/com-api/AAFPlugin.h $(INCLUDE_DIR)/com-api/AAFPlugin_i.c : $(INCLUDE_DIR)/com-api/AAFPlugin.idl
	@ $(ECHO) Generating AAFPlugin.h AAFPlugin_i.c ...
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFPlugin.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPlugin.h > AAFPlugin.h.tmp
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPlugin_i.c > AAFPlugin_i.c.tmp
	@if [ -f  $(INCLUDE_DIR)/com-api/AAFPlugin.h ] && cmp -s AAFPlugin.h.tmp $(INCLUDE_DIR)/com-api/AAFPlugin.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPlugin.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPlugin.h; \
		$(CP) -f AAFPlugin.h.tmp $(INCLUDE_DIR)/com-api/AAFPlugin.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPlugin.h; \
	fi
	@if [ -f  $(INCLUDE_DIR)/com-api/AAFPlugin_i.c ] && cmp -s AAFPlugin_i.c.tmp $(INCLUDE_DIR)/com-api/AAFPlugin_i.c; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPlugin_i.c did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPlugin_i.c; \
		$(CP) -f AAFPlugin_i.c.tmp $(INCLUDE_DIR)/com-api/AAFPlugin_i.c; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPlugin_i.c; \
	fi
	$(RM) AAFPlugin.h AAFPlugin_i.c AAFPlugin_p.c AAFPlugin.h.tmp AAFPlugin_i.c.tmp dlldata.c

$(INCLUDE_DIR)/com-api/AAFPluginTypes.h : $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	@ $(ECHO) Generating AAFPluginTypes.h
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPluginTypes.h > AAFPluginTypes.h.tmp
	@if [ -f  $(INCLUDE_DIR)/com-api/AAFPluginTypes.h ] && cmp -s AAFPluginTypes.h.tmp $(INCLUDE_DIR)/com-api/AAFPluginTypes.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPluginTypes.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPluginTypes.h; \
		$(CP) -f AAFPluginTypes.h.tmp $(INCLUDE_DIR)/com-api/AAFPluginTypes.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPluginTypes.h; \
	fi
	$(RM) AAFPluginTypes.h AAFPluginTypes.h.tmp

$(INCLUDE_DIR)/com-api/AAFPrivate.h $(INCLUDE_DIR)/com-api/AAFPrivate_i.c : $(INCLUDE_DIR)/com-api/AAFPrivate.idl
	@ $(ECHO) Generating AAFPrivate.h AAFPrivate_i.c ...
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFPrivate.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPrivate.h > AAFPrivate.h.tmp
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFPrivate_i.c > AAFPrivate_i.c.tmp
	@if [ -f  $(INCLUDE_DIR)/com-api/AAFPrivate.h ] && cmp -s AAFPrivate.h.tmp $(INCLUDE_DIR)/com-api/AAFPrivate.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPrivate.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPrivate.h; \
		$(CP) -f AAFPrivate.h.tmp $(INCLUDE_DIR)/com-api/AAFPrivate.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPrivate.h; \
	fi
	@if [ -f  $(INCLUDE_DIR)/com-api/AAFPrivate_i.c ] && cmp -s AAFPrivate_i.c.tmp $(INCLUDE_DIR)/com-api/AAFPrivate_i.c; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFPrivate_i.c did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFPrivate_i.c; \
		$(CP) -f AAFPrivate_i.c.tmp $(INCLUDE_DIR)/com-api/AAFPrivate_i.c; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFPrivate_i.c; \
	fi
	$(RM) AAFPrivate.h AAFPrivate_i.c AAFPrivate_p.c AAFPrivate.h.tmp AAFPrivate_i.c.tmp dlldata.c

$(INCLUDE_DIR)/com-api/AAFTypes.h : $(INCLUDE_DIR)/com-api/AAFTypes.idl
	@ $(ECHO) Generating AAFTypes.h
	$(MIDL) -I $(INCLUDE_DIR)/com-api -out . $(INCLUDE_DIR)/com-api/AAFTypes.idl
	@ $(ECHO) Removing timestamp from generated files ...
	sed -e '/^\/\* File created by MIDL compiler version /{' -e 'N' -e 's/\/\* at .*/\/\* at TIMESTAMP REMOVED/' -e '}' AAFTypes.h > AAFTypes.h.tmp
	@if [ -f  $(INCLUDE_DIR)/com-api/AAFTypes.h ] && cmp -s AAFTypes.h.tmp $(INCLUDE_DIR)/com-api/AAFTypes.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/AAFTypes.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/AAFTypes.h; \
		$(CP) -f AAFTypes.h.tmp $(INCLUDE_DIR)/com-api/AAFTypes.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/AAFTypes.h; \
	fi
	$(RM) AAFTypes.h AAFTypes.h.tmp



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
	@if [ -f $(INCLUDE_DIR)/cpp-api/$*.h ] && cmp -s $*.h $(INCLUDE_DIR)/cpp-api/$*.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/cpp-api/$*.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/cpp-api/$*.h; \
		$(CP) -f $*.h $(INCLUDE_DIR)/cpp-api; \
		$(CHMOD) -w $(INCLUDE_DIR)/cpp-api/$*.h; \
	fi

.dod.cppt :
	$(RM) -f $*.cppt
	./tool/$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.cppt
	$(CHMOD) -w $*.cppt
	@if [ -f $(TEST_DIR)/$*Test.cpp ] && cmp -s $*.cppt $(TEST_DIR)/$*Test.cpp; \
	then \
		$(ECHO) $(TEST_DIR)/$*Test.cpp did not change; \
	else \
		$(ECHO) Updating $(TEST_DIR)/$*Test.cpp; \
		$(CP) -f $*.cppt $(TEST_DIR)/$*Test.cpp; \
		$(CHMOD) +w $(TEST_DIR)/$*Test.cpp; \
	fi

.dod.comh :
	$(RM) -f $*.comh
	./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comh
	$(CHMOD) -w $*.comh
	@if [ -f $(SRC_DIR)/com-api/C$*.h ] && cmp -s $*.comh $(SRC_DIR)/com-api/C$*.h; \
	then \
		$(ECHO) $(SRC_DIR)/com-api/C$*.h did not change; \
	else \
		$(ECHO) Updating $(SRC_DIR)/com-api/C$*.h; \
		$(CP) -f $*.comh $(SRC_DIR)/com-api/C$*.h; \
		$(CHMOD) +w $(SRC_DIR)/com-api/C$*.h; \
	fi

.dod.comc :
	$(RM) -f $*.comc
	./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comc
	$(CHMOD) -w $*.comc
	@if [ -f $(SRC_DIR)/com-api/C$*.cpp ] && cmp -s $*.comc $(SRC_DIR)/com-api/C$*.cpp; \
	then \
		$(ECHO) $(SRC_DIR)/com-api/C$*.cpp did not change; \
	else \
		$(ECHO) Updating $(SRC_DIR)/com-api/C$*.cpp; \
		$(CP) -f $*.comc $(SRC_DIR)/com-api/C$*.cpp; \
		$(CHMOD) +w $(SRC_DIR)/com-api/C$*.cpp; \
	fi

.dod.comcx :
	$(RM) -f $*.comcx
	./tool/$(DODO) -f macros/comcx.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comcx
	$(CHMOD) -w $*.comcx
	@if [ -f $(SRC_DIR)/com-api/C$*.cpp ] && cmp -s $*.comcx $(SRC_DIR)/com-api/C$*.cpp; \
	then \
		$(ECHO) $(SRC_DIR)/com-api/C$*.cpp did not change; \
	else \
		$(ECHO) Updating $(SRC_DIR)/com-api/C$*.cpp; \
		$(CP) -f $*.comcx $(SRC_DIR)/com-api/C$*.cpp; \
		$(CHMOD) +w $(SRC_DIR)/com-api/C$*.cpp; \
	fi

.dod.comt :
	$(RM) -f $*.comt
	./tool/$(DODO) -f macros/comt.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.comt
	$(CHMOD) -w $*.comt
	@if [ -f $(TEST_DIR)/C$*Test.cpp ] && cmp -s $*.comt $(TEST_DIR)/C$*Test.cpp; \
	then \
		$(ECHO) $(TEST_DIR)/C$*Test.cpp did not change; \
	else \
		$(ECHO) Updating $(TEST_DIR)/C$*Test.cpp; \
		$(CP) -f $*.comt $(TEST_DIR)/C$*Test.cpp; \
		$(CHMOD) +w $(TEST_DIR)/C$*Test.cpp; \
	fi

.dod.implh :
	$(RM) -f $*.implh ;
	./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ;
	$(MV) $*.tmp $*.implh ;
	$(CHMOD) -w $*.implh ;
	@if [ -f $(SRC_DIR)/impl/Impl$*.h ] && cmp -s $*.implh $(SRC_DIR)/impl/Impl$*.h; \
	then \
		$(ECHO) $(SRC_DIR)/impl/Impl$*.h did not change; \
	else \
		$(ECHO) Updating $(SRC_DIR)/impl/Impl$*.h; \
		$(CP) -f $*.implh $(SRC_DIR)/impl/Impl$*.h ; \
		$(CHMOD) +w $(SRC_DIR)/impl/Impl$*.h ; \
	fi

.dod.implc :
	$(RM) -f $*.implc ;
	./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ;
	$(MV) $*.tmp $*.implc ;
	$(CHMOD) -w $*.implc ;
	@if [ -f $(SRC_DIR)/impl/Impl$*.cpp ] && cmp -s $*.implc $(SRC_DIR)/impl/Impl$*.cpp; \
	then \
		$(ECHO) $(SRC_DIR)/impl/Impl$*.cpp did not change; \
	else \
		$(ECHO) Updating $(SRC_DIR)/impl/Impl$*.cpp; \
		$(CP) -f $*.implc $(SRC_DIR)/impl/Impl$*.cpp ; \
		$(CHMOD) +w $(SRC_DIR)/impl/Impl$*.cpp ; \
	fi

.dod.cpp :
	$(RM) -f $*.cpp
	./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.cpp
	$(CHMOD) -w $*.cpp
	@if [ -f $(SRC_DIR)/cpp-api/$*.cpp ] && cmp -s $*.cpp $(SRC_DIR)/cpp-api/$*.cpp; \
	then \
		$(ECHO) $(SRC_DIR)/cpp-api/$*.cpp did not change; \
	else \
		$(ECHO) Updating $(SRC_DIR)/cpp-api/$*.cpp; \
		$(CP) -f $*.cpp $(SRC_DIR)/cpp-api; \
		$(CHMOD) +w $(SRC_DIR)/cpp-api/$*.cpp; \
	fi

.dod.idl :
	$(RM) -f $*.idl
	./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.idl
	$(CHMOD) -w $*.idl
	@if [ -f $(INCLUDE_DIR)/com-api/$*.idl ] && cmp -s $*.idl $(INCLUDE_DIR)/com-api/$*.idl; \
	then \
		$(ECHO) $(INCLUDE_DIR)/com-api/$*.idl did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/com-api/$*.idl; \
		$(CP) -f $*.idl $(INCLUDE_DIR)/com-api; \
		$(CHMOD) +w $(INCLUDE_DIR)/com-api/$*.idl; \
	fi

.dod.fidl :
	$(RM) -f $*.fidl
	perl GenObjectInterfaces.pl $* > ObjInt.tmp
	./tool/$(DODO) -f macros/fidl.mac < $*.dod > $*.tmp
	perl -p -0 \
		-e 's/(^\[\n\s*object,\n\s*uuid)/OBJ_INT\n\1/m;'\
		-e 's/^(\n\n)\n+/\1/mg;'\
		$*.tmp > $*X.tmp
	sed -e '/OBJ_INT/r ObjInt.tmp' -e '/OBJ_INT/d' $*X.tmp > $*.fidl
	$(CHMOD) -w $*.fidl
	$(RM) -f ObjInt.tmp $*.tmp $*X.tmp

.dod.refh :
	$(RM) -f $*.refh
	./tool/$(DODO) -f macros/refh.mac < $*.dod > $*.tmp
	$(MV) $*.tmp $*.refh
	$(CHMOD) -w $*.refh
	@if [ -f $(INCLUDE_DIR)/ref-api/$*.h ] && cmp -s $*.refh $(INCLUDE_DIR)/ref-api/$*.h; \
	then \
		$(ECHO) $(INCLUDE_DIR)/ref-api/$*.h did not change; \
	else \
		$(ECHO) Updating $(INCLUDE_DIR)/ref-api/$*.h; \
		$(CP) -f $*.refh $(INCLUDE_DIR)/ref-api/$*.h; \
		$(CHMOD) +w $(INCLUDE_DIR)/ref-api/$*.h; \
	fi

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
	$(RM) -f GenEnumValidationLog.txt

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
	$(RM) -f $(UUID_DIR)/AAFPrivate_i.c
	$(RM) -f $(UUID_DIR)/AAF_i.c
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
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
