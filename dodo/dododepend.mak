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
	@ rm -f depend.tmp
	@ echo "#" This file automatically generated make. > depend.tmp
	@ echo "#" Special case AAFTypes since no object is to be built only headers... >> depend.tmp
	@ echo AAFTypes.all...
	@ echo AAFTypes.all : AAFTypes.h >> depend.tmp
	@ echo AAFTypes.all : AAFTypes.idl >> depend.tmp
	@ echo AAFTypes.all : AAFTypes.refh >> depend.tmp
	@ echo AAFTypes.h : macros/h.mac macros/base.mac >> depend.tmp
	@ echo AAFTypes.idl : macros/idl.mac macros/base.mac >> depend.tmp
	@ echo AAFTypes.refh : macros/refh.mac macros/base.mac >> depend.tmp
	@ echo "" >> depend.tmp
	@ echo "#" >> depend.tmp
	@ echo "#" >> depend.tmp
	@ echo "#" special case the utility classes since they will not be exposed by com >> depend.tmp
	@ echo AAFModuleTest.all...
	@ echo "" >> depend.tmp
	@ echo AAFModuleTest.all : AAFModuleTest.idl >> depend.tmp
	@ echo AAFModuleTest.all : AAFModuleTest.refh >> depend.tmp
	@ echo AAFModuleTest.idl : macros/idl.mac macros/base.mac >> depend.tmp
	@ echo AAFModuleTest.refh : macros/refh.mac macros/base.mac >> depend.tmp
	@ for base in $(DODO_TARGET_NAMES) ; do \
		echo $$base.all... ; \
		echo "" >> depend.tmp ; \
		echo $$base.all : $$base.cpp $$base.h $$base.cppt >> depend.tmp ; \
		echo $$base.all : $$base.comc $$base.comh >> depend.tmp ; \
		echo $$base.all : $$base.comt >> depend.tmp ; \
		echo $$base.all : $$base.implc $$base.implh >> depend.tmp ; \
		echo $$base.all : $$base.fidl >> depend.tmp ; \
		echo $$base.all : $$base.frefh >> depend.tmp ; \
		echo $$base.cpp : macros/cpp.mac macros/base.mac >> depend.tmp ; \
		echo $$base.h : macros/h.mac macros/base.mac >> depend.tmp ; \
		echo $$base.cppt : macros/cppt.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comc : macros/comc.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comh : macros/comh.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comt : macros/comt.mac macros/base.mac >> depend.tmp ; \
		echo $$base.implc : macros/implc.mac macros/base.mac >> depend.tmp ; \
		echo $$base.implh : macros/implh.mac macros/base.mac >> depend.tmp ; \
		echo $$base.fidl : macros/fidl.mac macros/base.mac >> depend.tmp ; \
		echo $$base.frefh : macros/frefh.mac macros/base.mac >> depend.tmp ; \
		echo $$base.exp : macros/exp.mac macros/base.mac >> depend.tmp ; \
		for import in `grep '^\#import' $$base.dod | sed -e 's,\#import,,' | sed -e 's,.*/,,'` ; do \
			echo $$base.cpp : $$import >> depend.tmp ; \
			echo $$base.h : $$import >> depend.tmp ; \
			echo $$base.cppt : $$import >> depend.tmp ; \
			echo $$base.comc : $$import >> depend.tmp ; \
			echo $$base.comh : $$import >> depend.tmp ; \
			echo $$base.comt : $$import >> depend.tmp ; \
			echo $$base.implc : $$import >> depend.tmp ; \
			echo $$base.implh : $$import >> depend.tmp ; \
			echo $$base.fidl : $$import >> depend.tmp ; \
			echo $$base.frefh : $$import >> depend.tmp ; \
			echo $$base.exp : $$import >> depend.tmp ; \
		done ; \
	  done
	@ for base in $(AAFCOMINTERFACESONLY) ; do \
		echo $$base.all... ; \
		echo "" >> depend.tmp ; \
		echo $$base.all : $$base.fidl $$base.comcx $$base.exp >> depend.tmp ; \
		echo $$base.fidl : macros/fidl.mac macros/base.mac >> depend.tmp ; \
		echo $$base.comcx : macros/comcx.mac macros/base.mac >> depend.tmp ; \
		echo $$base.exp : macros/exp.mac macros/base.mac >> depend.tmp ; \
      done
	@ echo "" >> depend.tmp
	for file in $(HUMAN_TYPED_IMPL) ; do \
		grep -v $$file\.impl depend.tmp > depend.tmp2 ; \
		rm depend.tmp ; \
		mv depend.tmp2 depend.tmp ; \
      done
	for file in $(HUMAN_TYPED_COMTEST) ; do \
		grep -v $$file\.comt depend.tmp > depend.tmp2 ; \
		rm depend.tmp ; \
		mv depend.tmp2 depend.tmp ; \
      done
	@ mv depend.tmp depend.mk
	@ echo "Done with depend.mk."


clean :
	$(RM) -rf depend.mk
	touch depend.mk


