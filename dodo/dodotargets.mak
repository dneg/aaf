############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : targets.mk


BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk


# Include the file that lists all of the dodo target names.
include aafobjects.mk


targets.mk : aafobjects.mk
	@ echo Creating targets.mk ...
	@ rm -f targets.tmp
	@ echo "#" This file automatically generated make. > targets.tmp
	@ echo "#" Special case AAFTypes since no object is to be built only headers... >> targets.tmp
	@ echo "#" special case the utility classes since they will not be exposed by com >> targets.tmp 
	@ echo DODO_TARGETS = '\' >> targets.tmp 
	@ echo '	'AAFTypes.all' \' >> targets.tmp 
	@ echo '	'AAFModuleTest.all \\\c>> targets.tmp 
	@ for base in $(AAFOBJECTS) $(AAFCOMINTERFACESONLY) ;  do \
		echo '\' >> targets.tmp ; \
		echo '	'$$base.all \\\c>> targets.tmp ; \
	  done
	@ echo '\' >> targets.tmp
	@ echo '	'AAFModule.all \\\c>> targets.tmp ;
	@ echo '' >> targets.tmp
	@ echo '' >> targets.tmp
	@ echo FIDL_TARGETS = \\\c >> targets.tmp 
	@ for base in $(AAFOBJECTS) $(AAFCOMINTERFACESONLY) ;  do \
		echo '\' >> targets.tmp ; \
		echo '	'$$base.fidl \\\c>> targets.tmp ; \
	  done
	@ echo '\' >> targets.tmp
	@ echo '	'AAFModule.fidl \\\c>> targets.tmp ;
	@ echo '' >> targets.tmp
	@ echo '' >> targets.tmp
	@ echo FREFH_TARGETS = \\\c >> targets.tmp 
	@ for base in $(AAFOBJECTS) $(AAFCOMINTERFACESONLY) ;  do \
		echo '\' >> targets.tmp ; \
		echo '	'$$base.frefh \\\c>> targets.tmp ; \
	  done
	@ echo '\' >> targets.tmp
	@ echo '	'AAFModule.frefh \\\c>> targets.tmp ;
	@ echo '' >> targets.tmp
	@ echo '' >> targets.tmp
	@ echo PLUGIN_TARGETS = '\' >> targets.tmp 
	@ echo '	'AAFTypes.all' \' >> targets.tmp 
	@ echo '	'AAFPluginTypes.all' \' >> targets.tmp 
	@ echo '	'AAFModuleTest.all \\\c>> targets.tmp 
	@ for base in $(PLUGIN_OBJECTS) ;  do \
		echo '\' >> targets.tmp ; \
		echo '	'$$base.all \\\c>> targets.tmp ; \
	  done
	@ echo '' >> targets.tmp
	@ echo '' >> targets.tmp
	@ echo PLUGIN_FIDL_TARGETS = \\\c >> targets.tmp 
	@ for base in $(PLUGIN_OBJECTS) ;  do \
		echo '\' >> targets.tmp ; \
		echo '	'$$base.fidl \\\c>> targets.tmp ; \
	  done
	@ echo '' >> targets.tmp
	@ echo '' >> targets.tmp
	@ echo PLUGIN_FREFH_TARGETS = \\\c >> targets.tmp 
	@ for base in $(PLUGIN_OBJECTS) ;  do \
		echo '\' >> targets.tmp ; \
		echo '	'$$base.frefh \\\c>> targets.tmp ; \
	  done
	@ echo '' >> targets.tmp
	@ mv targets.tmp targets.mk
	@ echo "Done with targets.mk."


clean :
	$(RM) -rf targets.mk
