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
	cd tool ;  $(MAKE)

test_only:
	cd tool ; $(MAKE) test_only
	$(MAKE) -f dodotargets.mak
	$(MAKE) -f dododepend.mak
	$(MAKE) -f maketargets.mak

run:
	$(MAKE) -f dodotargets.mak
	$(MAKE) -f dododepend.mak
	$(MAKE) -f maketargets.mak
	cd tool ; $(MAKE) run



# Make sure our list of objects in the include src directories is up-to-date.
# (TomR) We should probably move aafobjects.mk and the following behavior to 
# the ref-impl make file...
aafobjects.all : \
	$(INCLUDE_DIR)/com-api/aafobjects.mk \
	$(SRC_DIR)/com-api/aafobjects.mk \
	$(SRC_DIR)/cpp-api/aafobjects.mk

$(INCLUDE_DIR)/com-api/aafobjects.mk : aafobjects.mk
	$(RM) -f $(INCLUDE_DIR)/com-api/aafobjects.mk
	cp aafobjects.mk $(INCLUDE_DIR)/com-api

$(SRC_DIR)/com-api/aafobjects.mk : aafobjects.mk
	$(RM) -f $(SRC_DIR)/com-api/aafobjects.mk
	cp aafobjects.mk $(SRC_DIR)/com-api

$(SRC_DIR)/cpp-api/aafobjects.mk : aafobjects.mk
	$(RM) -f $(SRC_DIR)/cpp-api/aafobjects.mk
	cp aafobjects.mk $(SRC_DIR)/cpp-api




#targets: aafobjects.mk targets.mk depend.mk

targets: 
	$(MAKE) -f maketargets.mak


targets.mk : aafobjects.mk
	$(MAKE) -f dodotargets.mak

depend.mk : aafobjects.mk
	$(MAKE) -f dododepend.mak

depend : targets.mk depend.mk

clean:
	cd tool ; $(MAKE) clean
	$(MAKE) -f dodotargets.mak clean
	$(MAKE) -f dododepend.mak clean
	touch targets.mk depend.mk
	$(MAKE) -f maketargets.mak clean
	$(RM) -f targets.mk depend.mk
