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

SRC_DIR = ../ref-impl/src
INCLUDE_DIR = ../ref-impl/include

depend.mk : aafobjects.mk
	@ echo Creating depend.tmp ...
	@ rm -f depend.tmp
	@ echo # This file automatically generated make. > depend.tmp
	@ echo # Special case AAFTypes since no object is to be built only headers... >> depend.tmp
	@ echo AAFTypes.all...
	@ echo AAFTypes.all : $(INCLUDE_DIR)/cpp-api/AAFTypes.h >> depend.tmp
	@ echo AAFTypes.all : $(INCLUDE_DIR)/com-api/AAFTypes.idl >> depend.tmp
	@ echo AAFTypes.all : $(INCLUDE_DIR)/ref-api/AAFTypes.h >> depend.tmp
	@ echo $(INCLUDE_DIR)/ref-api/AAFTypes.h : macros/refh.mac macros/base.mac >> depend.tmp
	@ echo $(INCLUDE_DIR)/cpp-api/AAFTypes.h : macros/h.mac macros/base.mac >> depend.tmp
	@ echo $(INCLUDE_DIR)/com-api/AAFTypes.idl : macros/idl.mac macros/base.mac >> depend.tmp
	@ echo # >> depend.tmp
	@ echo # >> depend.tmp
	@ echo #special case the utility classes since they will not be exposed by com >> depend.tmp
	@ for base in $(DODO_TARGET_NAMES) ; do \
		echo $$base.all... ; \
		echo '' >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/cpp-api/$$base.cpp >> depend.tmp ; \
		echo $$base.all : $(INCLUDE_DIR)/cpp-api/$$base.h >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/cpp-api/test/$$baseTest.cpp >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/com-api/C$$base.cpp >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/com-api/C$$base.h >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/com-api/test/C$$baseTest.cpp >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/impl/Impl$$base.cpp >> depend.tmp ; \
		echo $$base.all : $(SRC_DIR)/impl/Impl$$base.h >> depend.tmp ; \
		echo $$base.all : $(INCLUDE_DIR)/com-api/$$base.idl >> depend.tmp ; \
		echo $$base.all : $(INCLUDE_DIR)/ref-api/$$base.h >> depend.tmp ; \
		echo $$base.all : $$base.exp >> depend.tmp ; \
		echo $(SRC_DIR)/cpp-api/$$base.cpp : macros/cpp.mac macros/base.mac >> depend.tmp ; \
		echo $(INCLUDE_DIR)/cpp-api/$$base.h : macros/h.mac macros/base.mac >> depend.tmp ; \
		echo $(SRC_DIR)/cpp-api/test/$$baseTest.cpp : macros/cppt.mac macros/base.mac >> depend.tmp ; \
		echo $(SRC_DIR)/com-api/C$$base.cpp : macros/comc.mac macros/base.mac >> depend.tmp ; \
		echo $(SRC_DIR)/com-api/C$$base.h : macros/comh.mac macros/base.mac >> depend.tmp ; \
		echo $(SRC_DIR)/com-api/test/C$$baseTest.cpp : macros/comt.mac macros/base.mac >> depend.tmp ; \
		echo $(SRC_DIR)/impl/Impl$$base.cpp : macros/implc.mac macros/base.mac >> depend.tmp ; \
		echo $(SRC_DIR)/impl/Impl$$base.h : macros/implh.mac macros/base.mac >> depend.tmp ; \
		echo $(INCLUDE_DIR)/com-api/$$base.idl : macros/idl.mac macros/base.mac >> depend.tmp ; \
		echo $(INCLUDE_DIR)/ref-api/$$base.h : macros/refh.mac macros/base.mac >> depend.tmp ; \
		echo $$base.exp : macros/exp.mac macros/base.mac >> depend.tmp ; \
		for import in `grep '^#import' $$base.dod | sed -e 's,#import,,' | sed -e 's,.*/,,'` ; do \
			echo $(SRC_DIR)/cpp-api/$$base.cpp : $$import >> depend.tmp ; \
			echo $(INCLUDE_DIR)/cpp-api/$$base.h : $$import >> depend.tmp ; \
			echo $(SRC_DIR)/cpp-api/test/$$baseTest.cpp : $$import >> depend.tmp ; \
			echo $(SRC_DIR)/com-api/C$$base.cpp : $$import >> depend.tmp ; \
			echo $(SRC_DIR)/com-api/C$$base.h : $$import >> depend.tmp ; \
			echo $(SRC_DIR)/com-api/test/C$$baseTest.cpp : $$import >> depend.tmp ; \
			echo $(SRC_DIR)/impl/Impl$$base.cpp : $$import >> depend.tmp ; \
			echo $(SRC_DIR)/impl/Impl$$base.h : $$import >> depend.tmp ; \
			echo $(INCLUDE_DIR)/com-api/$$base.idl : $$import >> depend.tmp ; \
			echo $$base.refh : $$import >> depend.tmp ; \
			echo $$base.exp : $$import >> depend.tmp ; \
		done ; \
	done
	@ echo \\n>> depend.tmp
	mv depend.tmp depend.mk
	@ echo "Done with depend.mk."


clean :
	$(RM) -rf depend.mk
	$(SH_PREFIX) touch depend.mk $(SH_SUFFIX)
