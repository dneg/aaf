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
	@echo Creating targets.mk ...
	@$(SH_PREFIX) rm -f targets.tmp $(SH_SUFFIX)
	@$(SH_PREFIX) echo # This file automatically generated make. > targets.tmp $(SH_SUFFIX)
	@$(SH_PREFIX) echo # Special case AAFTypes since no object is to be built only headers... > targets.tmp $(SH_SUFFIX)
	@$(SH_PREFIX) echo # special case the utility classes since they will not be exposed by com > targets.tmp  $(SH_SUFFIX)
	@$(SH_PREFIX) echo DODO_TARGETS = \\>> targets.tmp  $(SH_SUFFIX)
	@$(SH_PREFIX) echo \\tAAFTypes.all \\c>> targets.tmp  $(SH_SUFFIX)
	$(SH_PREFIX) for base in $(AAFOBJECTS) ;  do \
		echo \\>> targets.tmp ; \
		echo \\t$$base.all \\c>> targets.tmp ; \
	done $(SH_SUFFIX)
	@$(SH_PREFIX) echo \\n>> targets.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv targets.tmp targets.mk $(SH_SUFFIX)
	@echo "Done with targets.mk."


clean :
	$(SH_PREFIX) $(RM) -rf targets.mk $(SH_SUFFIX)
