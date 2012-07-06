###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################

ALL = axLib axDump axExample axMasterMobEx axFileGen axPluginInfo axPlugin

all: $(ALL)

.PHONY: axLib
axLib:
	cd axLib && $(MAKE)

.PHONY: axDump
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

.PHONY: axFileGen
axFileGen:
	cd axFileGen && $(MAKE)

# Check target builds utiltities that are required by other check
# tests possibly outside of the utilitiy directory (originally this
# was added for the ComFileExtDevUtilVerityTest.sh script).
.PHONY : check
check : axLib
	echo examples2 check dependencies build complete

.PHONY: clean
clean:
	cd axLib && $(MAKE) GENDEPS=0 $@
	cd axDump && $(MAKE) GENDEPS=0 $@
	cd axExample && $(MAKE) GENDEPS=0 $@
	cd axMasterMobEx && $(MAKE) GENDEPS=0 $@
	cd axPluginInfo && $(MAKE) GENDEPS=0 $@
	cd axPlugin && $(MAKE) GENDEPS=0 $@
	cd axFileGen && $(MAKE) GENDEPS=0 $@

.PHONY:  realclean
realclean:
	cd axLib && $(MAKE) GENDEPS=0 $@
	cd axDump && $(MAKE) GENDEPS=0 $@
	cd axExample && $(MAKE) GENDEPS=0 $@
	cd axMasterMobEx && $(MAKE) GENDEPS=0 $@
	cd axPluginInfo && $(MAKE) GENDEPS=0 $@
	cd axPlugin && $(MAKE) GENDEPS=0 $@
	cd axFileGen && $(MAKE) GENDEPS=0 $@
