############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : dodo-app aafobjects.all targets.mk depend.mk targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

SRC_DIR = ../ref-impl/src

INCLUDE_DIR = ../ref-impl/include


include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f dodotargets.mak $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f dododepend.mak $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f maketargets.mak $(SH_SUFFIX)

run:
	$(SH_PREFIX) $(MAKE) -f dodotargets.mak $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f dododepend.mak $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f maketargets.mak $(SH_SUFFIX)
	$(SH_PREFIX) cd tool ; $(MAKE) run $(SH_SUFFIX)



# Make sure our list of objects in the include src directories is up-to-date.
# (TomR) We should probably move aafobjects.mk and the following behavior to 
# the ref-impl make file...
aafobjects.all : \
	$(INCLUDE_DIR)/com-api/aafobjects.mk \
	$(SRC_DIR)/com-api/aafobjects.mk \
	$(SRC_DIR)/cpp-api/aafobjects.mk

$(INCLUDE_DIR)/com-api/aafobjects.mk : aafobjects.mk
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/com-api/aafobjects.mk $(SH_SUFFIX)
	$(SH_PREFIX) cp -p aafobjects.mk $(INCLUDE_DIR)/com-api $(SH_SUFFIX)

$(SRC_DIR)/com-api/aafobjects.mk : aafobjects.mk
	$(SH_PREFIX) $(RM) -f $(SRC_DIR)/com-api/aafobjects.mk $(SH_SUFFIX)
	$(SH_PREFIX) cp -p aafobjects.mk $(SRC_DIR)/com-api $(SH_SUFFIX)

$(SRC_DIR)/cpp-api/aafobjects.mk : aafobjects.mk
	$(SH_PREFIX) $(RM) -f $(SRC_DIR)/cpp-api/aafobjects.mk $(SH_SUFFIX)
	$(SH_PREFIX) cp -p aafobjects.mk $(SRC_DIR)/cpp-api $(SH_SUFFIX)




#targets: aafobjects.mk targets.mk depend.mk

targets: 
	$(SH_PREFIX) $(MAKE) -f maketargets.mak $(SH_SUFFIX)


targets.mk : aafobjects.mk
	$(SH_PREFIX) $(MAKE) -f dodotargets.mak $(SH_SUFFIX)

depend.mk : aafobjects.mk
	$(SH_PREFIX) $(MAKE) -f dododepend.mak $(SH_SUFFIX)

depend : targets.mk depend.mk

clean:
	$(SH_PREFIX) cd tool ; $(MAKE) clean $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f dodotargets.mak clean $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f dododepend.mak clean $(SH_SUFFIX)
	$(SH_PREFIX) touch targets.mk $(SH_SUFFIX)
	$(SH_PREFIX) touch depend.mk $(SH_SUFFIX)
	$(SH_PREFIX) $(MAKE) -f maketargets.mak clean $(SH_SUFFIX)


