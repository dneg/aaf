############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : depend.mk


BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk


# Include the file that lists all of the dodo target names.
include aafobjects.mk


depend.mk : aafobjects.mk
	@echo Creating depend.mk ...
	@rm -f depend.mk
	@echo # This file automatically generated make. > depend.mk
	@echo # Special case AAFTypes since no object is to be built only headers... > depend.mk
	@echo AAFTypes.all...
	@echo AAFTypes.all : AAFTypes.cpp AAFTypes.h AAFTypes.cppt > depend.mk
	@echo AAFTypes.all : AAFTypes.comc AAFTypes.comh > depend.mk
	@echo AAFTypes.all : AAFTypes.implc AAFTypes.implh > depend.mk
	@echo AAFTypes.all : AAFTypes.idl AAFTypes.exp > depend.mk
	@echo AAFTypes.h : macros/h.mac macros/base.mac > depend.mk
	@echo AAFTypes.comh : macros/comh.mac macros/base.mac > depend.mk
	@echo AAFTypes.implh : macros/implh.mac macros/base.mac > depend.mk
	@echo AAFTypes.idl : macros/idl.mac macros/base.mac > depend.mk
	@echo AAFTypes.exp : macros/exp.mac macros/base.mac > depend.mk
	@echo # > depend.mk
	@echo # > depend.mk
	@echo #special case the utility classes since they will not be exposed by com > depend.mk
	@echo AAFUInt64.all...
	@echo AAFUInt64.all : AAFUInt64.cpp AAFUInt64.h AAFUInt64.cppt > depend.mk
	@echo AAFUInt64.all : AAFUInt64.implc AAFUInt64.implh > depend.mk
	@echo AAFUInt64.all : AAFUInt64.exp > depend.mk
	@echo AAFUInt64.cpp : macros/cpp.mac macros/base.mac > depend.mk
	@echo AAFUInt64.h : macros/h.mac macros/base.mac > depend.mk
	@echo AAFUInt64.cppt : macros/cppt.mac macros/base.mac > depend.mk
	@echo AAFUInt64.implc : macros/implc.mac macros/base.mac > depend.mk
	@echo AAFUInt64.implh : macros/implh.mac macros/base.mac > depend.mk
	@echo AAFUInt64.exp : macros/exp.mac macros/base.mac > depend.mk
	@echo # > depend.mk
	@echo AAFInt64.all...
	@echo AAFInt64.all : AAFInt64.cpp AAFInt64.h AAFInt64.cppt > depend.mk
	@echo AAFInt64.all : AAFInt64.implc AAFInt64.implh > depend.mk
	@echo AAFInt64.all : AAFInt64.exp > depend.mk
	@echo AAFInt64.cpp : macros/cpp.mac macros/base.mac > depend.mk
	@echo AAFInt64.h : macros/h.mac macros/base.mac > depend.mk
	@echo AAFInt64.cppt : macros/cppt.mac macros/base.mac > depend.mk
	@echo AAFInt64.implc : macros/implc.mac macros/base.mac > depend.mk
	@echo AAFInt64.implh : macros/implh.mac macros/base.mac > depend.mk
	@echo AAFInt64.exp : macros/exp.mac macros/base.mac > depend.mk
	@echo # > depend.mk
	@echo AAFString.all...
	@echo AAFString.all : AAFString.cpp AAFString.h AAFString.cppt > depend.mk
	@echo AAFString.all : AAFString.implc AAFString.implh > depend.mk
	@echo AAFString.all : AAFString.exp > depend.mk
	@echo AAFString.cpp : macros/cpp.mac macros/base.mac > depend.mk
	@echo AAFString.h : macros/h.mac macros/base.mac > depend.mk
	@echo AAFString.cppt : macros/cppt.mac macros/base.mac > depend.mk
	@echo AAFString.implc : macros/implc.mac macros/base.mac > depend.mk
	@echo AAFString.implh : macros/implh.mac macros/base.mac > depend.mk
	@echo AAFString.exp : macros/exp.mac macros/base.mac > depend.mk
	$(SH_PREFIX) for base in $(DODO_TARGET_NAMES) ; do \
		echo $$base.all... ; \
		echo '' >> depend.mk ; \
		echo $$base.all : $$base.cpp $$base.h $$base.cppt >> depend.mk ; \
		echo $$base.all : $$base.comc $$base.comh $$base.comt >> depend.mk ; \
		echo $$base.all : $$base.implc $$base.implh >> depend.mk ; \
		echo $$base.all : $$base.idl $$base.exp >> depend.mk ; \
		echo $$base.cpp : macros/cpp.mac macros/base.mac >> depend.mk ; \
		echo $$base.h : macros/h.mac macros/base.mac >> depend.mk ; \
		echo $$base.cppt : macros/cppt.mac macros/base.mac >> depend.mk ; \
		echo $$base.comc : macros/comc.mac macros/base.mac >> depend.mk ; \
		echo $$base.comh : macros/comh.mac macros/base.mac >> depend.mk ; \
		echo $$base.comt : macros/comt.mac macros/base.mac >> depend.mk ; \
		echo $$base.implc : macros/implc.mac macros/base.mac >> depend.mk ; \
		echo $$base.implh : macros/implh.mac macros/base.mac >> depend.mk ; \
		echo $$base.idl : macros/idl.mac macros/base.mac >> depend.mk ; \
		echo $$base.exp : macros/exp.mac macros/base.mac >> depend.mk ; \
		for import in `grep '^#import' $$base.dod | sed -e 's,#import,,'` ; do \
			echo $$base.cpp : $$import >> depend.mk ; \
			echo $$base.h : $$import >> depend.mk ; \
			echo $$base.cppt : $$import >> depend.mk ; \
			echo $$base.comc : $$import >> depend.mk ; \
			echo $$base.comh : $$import >> depend.mk ; \
			echo $$base.comt : $$import >> depend.mk ; \
			echo $$base.implc : $$import >> depend.mk ; \
			echo $$base.implh : $$import >> depend.mk ; \
			echo $$base.idl : $$import >> depend.mk ; \
			echo $$base.exp : $$import >> depend.mk ; \
		done ; \
	done $(SH_SUFFIX)
	@echo "Done with depend.mk."


clean :
	$(SH_PREFIX) $(RM) -rf depend.mk $(SH_SUFFIX)
