###############################################################################
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# 
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

ALL = axLib axDump axExample axMasterMobEx axPluginInfo axPlugin

all: $(ALL)

.PHONY: axLib
axLib:
	cd axLib && $(MAKE)

.PHONY: axLib
axDump:
	cd axDump && $(MAKE)

.PHONY: axExample
axExample:
	cd axExample && $(MAKE)

.PHONY: axMasterMobEx
axMasterMobEx:
	cd axMasterMobEx && $(MAKE)

.PHONY: axPluginInfo
axPluginInfo:
	cd axPluginInfo && $(MAKE)

.PHONY: axPlugin
axPlugin:
	cd axPlugin && $(MAKE)

.PHONY: clean
clean:
	cd axLib && $(MAKE) GENDEPS=0 $@
	cd axDump && $(MAKE) GENDEPS=0 $@
	cd axExample && $(MAKE) GENDEPS=0 $@
	cd axMasterMobEx && $(MAKE) GENDEPS=0 $@
	cd axPluginInfo && $(MAKE) GENDEPS=0 $@
	cd axPlugin && $(MAKE) GENDEPS=0 $@

.PHONY:  realclean
realclean:
	cd axLib && $(MAKE) GENDEPS=0 $@
	cd axDump && $(MAKE) GENDEPS=0 $@
	cd axExample && $(MAKE) GENDEPS=0 $@
	cd axMasterMobEx && $(MAKE) GENDEPS=0 $@
	cd axPluginInfo && $(MAKE) GENDEPS=0 $@
	cd axPlugin && $(MAKE) GENDEPS=0 $@



