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
	@rm -f targets.mk
	@echo # This file automatically generated make. > targets.mk
	@echo # Special case AAFTypes since no object is to be built only headers... > depend.mk
	@echo # special case the utility classes since they will not be exposed by com > depend.mk
	@echo DODO_TARGETS = AAFTypes.all \>> targets.mk
	$(SH_PREFIX) for base in $(DODO_TARGET_NAMES) ;  do \
		echo \\t$$base.all \\>> targets.mk ; \
	done $(SH_SUFFIX)
	@echo "Done with targets.mk."


clean :
	$(SH_PREFIX) $(RM) -rf targets.mk $(SH_SUFFIX)
