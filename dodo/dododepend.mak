#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

all : depend.mk


BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk


# Include the file that lists all of the dodo target names.
include aafobjects.mk


depend.mk : aafobjects.mk
	@ $(ECHO) Creating depend.tmp ...
	@ $(RM) -f depend.tmp
	$(ECHO) Doing first depend step...
	$(CP)  aafobjects.mk tmp.sh
	$(CAT) GenDepend.sh >> tmp.sh
	$(SH) tmp.sh > depend.tmp
	$(ECHO) Doing second depend step...
	$(CP)  aafobjects.mk tmp2.sh
	$(CAT) GenDepend2.sh >> tmp2.sh
	$(SH) tmp2.sh
	$(RM) tmp.sh tmp2.sh
	@ $(MV) depend.tmp depend.mk
	@ $(ECHO) "Done with depend.mk."


clean :
	$(RM) -rf depend.mk



