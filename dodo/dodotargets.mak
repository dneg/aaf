#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

all : targets.mk


BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk


# Include the file that lists all of the dodo target names.
include aafobjects.mk


targets.mk : aafobjects.mk
	@ $(ECHO) Creating targets.mk ...
	$(CP)  aafobjects.mk tmp.sh
	$(CAT) GenTargets.sh >> tmp.sh
	sh tmp.sh > targets.tmp
	@ $(MV) targets.tmp targets.mk
	@ $(ECHO) "Done with targets.mk."


clean :
	$(RM) -rf targets.mk
