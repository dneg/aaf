
AAFBASE ?= ..

include $(AAFBASE)/build/common.mk

INCLUDE_DIR = $(AAFBASE)/ref-impl/include/com-api
SRC_DIR = $(AAFBASE)/ref-impl/src/com-api

.PHONY : all
all : dodoTool aafObjects targets.mk depend.mk targets

.PHONY : dodoTool
dodoTool :
	@echo "[Building dodo/tool]\n"
	cd tool && $(MAKE)

.PHONY : aafObjects
aafObjects : $(INCLUDE_DIR)/aafobjects.mk $(SRC_DIR)/aafobjects.mk

$(INCLUDE_DIR)/aafobjects.mk : aafobjects.mk
	@echo "[Building $@]\n"
	-@rm -f $@
	cp $< $@

$(SRC_DIR)/aafobjects.mk : aafobjects.mk
	@echo "[Building $@]\n"
	-@rm -f $@
	cp $< $@

DODO = $(BINDIR)/dodo

targets.mk : aafobjects.mk
	@echo "[Building $@]\n"
	$(MAKE) -f dodotargets.mak

depend.mk : aafobjects.mk
	@echo "[Building $@]\n"
	$(MAKE) -f dododepend.mak

targets :
	@echo "[Building $@]\n"
	$(MAKE) -f maketargets_gnu.mak

.PHONY : clean
clean :
	cd tool && $(MAKE) GENDEPS=0 clean
	$(MAKE) -f dodotargets.mak clean
	$(MAKE) -f dododepend.mak clean
	touch targets.mk depend.mk
	$(MAKE) -f maketargets_gnu.mak clean

.PHONY : realclean
realclean : clean
	$(MAKE) -f maketargets_gnu.mak $@
	-@rm -f targets.mk depend.mk

# Although the following include is standard for all the makefiles 
# in this distribution, it's commented out here because this 
# directory doesn't contain any C/C++ files and doesn't need 
# building rules for them
#include $(AAFBASE)/build/rules.mk



