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
	@ echo Creating depend.tmp ...
	@ $(SH_PREFIX) rm -f depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo # This file automatically generated make. > depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo # Special case AAFTypes since no object is to be built only headers... >> depend.tmp $(SH_SUFFIX)
	@ echo AAFTypes.all...
	@ $(SH_PREFIX) echo AAFTypes.all : AAFTypes.h >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo AAFTypes.all : AAFTypes.idl >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo AAFTypes.h : macros/h.mac macros/base.mac >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo AAFTypes.idl : macros/idl.mac macros/base.mac >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo # >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo # >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) echo #special case the utility classes since they will not be exposed by com >> depend.tmp $(SH_SUFFIX)
	@ $(SH_PREFIX) for base in $(DODO_TARGET_NAMES) ; do \
		echo $$base.all... ; \
		echo '' >> depend.tmp ; \
		echo $$base.all : $$base.cpp $$base.h $$base.cppt >> depend.tmp ; \
		echo $$base.all : $$base.comc $$base.comh $$base.comt >> depend.tmp ; \
		echo $$base.all : $$base.implc $$base.implh >> depend.tmp ; \
		echo $$base.all : $$base.idl $$base.exp >> depend.tmp ; \
		echo $$base.cpp : macros/cpp.mac macros/base.mac >> depend.tmp ; \
		echo $$base.h : macros/h.mac macros/base.mac >> depend.tmp ; \
		echo $$base.cppt : macros/cppt.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comc : macros/comc.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comh : macros/comh.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comt : macros/comt.mac macros/base.mac >> depend.tmp ; \
		echo $$base.implc : macros/implc.mac macros/base.mac >> depend.tmp ; \
		echo $$base.implh : macros/implh.mac macros/base.mac >> depend.tmp ; \
		echo $$base.idl : macros/idl.mac macros/base.mac >> depend.tmp ; \
		echo $$base.exp : macros/exp.mac macros/base.mac >> depend.tmp ; \
		for import in `grep '^#import' $$base.dod | sed -e 's,#import,,' | sed -e 's,.*/,,'` ; do \
			echo $$base.cpp : $$import >> depend.tmp ; \
			echo $$base.h : $$import >> depend.tmp ; \
			echo $$base.cppt : $$import >> depend.tmp ; \
			echo $$base.comc : $$import >> depend.tmp ; \
			echo $$base.comh : $$import >> depend.tmp ; \
			echo $$base.comt : $$import >> depend.tmp ; \
			echo $$base.implc : $$import >> depend.tmp ; \
			echo $$base.implh : $$import >> depend.tmp ; \
			echo $$base.idl : $$import >> depend.tmp ; \
			echo $$base.exp : $$import >> depend.tmp ; \
		done ; \
	done $(SH_SUFFIX)
	@ $(SH_PREFIX) echo \\n>> depend.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv depend.tmp depend.mk $(SH_SUFFIX)
	@ echo "Done with depend.mk."


clean :
	$(SH_PREFIX) $(RM) -rf depend.mk $(SH_SUFFIX)
