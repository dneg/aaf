###############################################################################
#
# $Id$ $Name$
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
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

ALL = AnalyzerBase \
      AAFAnalyzerBase \
      EPAnalyzerBase \
      RequirementLib \
      TestPhaseLib \
      BaseTestImpl \
      RegressionTestStructure \
      EditProtocolTestImpl \
      AAFAnalyzer \
      RegressionTestFiles

all: $(ALL)

.PHONY: RegressionTest
RegressionTest: RegressionTestStructure RegressionTestFiles

.PHONY: AnalyzerBase
AnalyzerBase:
	cd AnalyzerBase && $(MAKE)

.PHONY: AAFAnalyzerBase
AAFAnalyzerBase: AnalyzerBase
	cd AAFAnalyzerBase && $(MAKE)

.PHONY: EPAnalyzerBase
EPAnalyzerBase: AnalyzerBase AAFAnalyzerBase
	cd EPAnalyzerBase && $(MAKE)

.PHONY: RequirementLib
RequirementLib: AnalyzerBase
	cd RequirementLib && $(MAKE)	

.PHONY: TestPhaseLib
TestPhaseLib: AnalyzerBase RequirementLib
	cd TestPhaseLib && $(MAKE)

.PHONY: BaseTestImpl
BaseTestImpl: AnalyzerBase AAFAnalyzerBase RequirementLib TestPhaseLib
	cd BaseTestImpl && $(MAKE)

.PHONY: RegressionTestStructure
RegressionTestStructure: AnalyzerBase AAFAnalyzerBase RequirementLib TestPhaseLib BaseTestImpl
	cd RegressionTest && $(MAKE) $@

.PHONY: EditProtocolTestImpl
EditProtocolTestImpl: AnalyzerBase AAFAnalyzerBase EPAnalyzerBase RequirementLib TestPhaseLib
	cd EditProtocolTestImpl && $(MAKE)

.PHONY: AAFAnalyzer
AAFAnalyzer: AAFAnalyzerBase RequirementLib TestPhaseLib BaseTestImpl EditProtocolTestImpl
	cd AAFAnalyzer && $(MAKE)
	
.PHONY: RegressionTestFiles
RegressionTestFiles:
	cd RegressionTest && $(MAKE) $@

.PHONY : check
check : RegressionTestStructure RegressionTestFiles AAFAnalyzer
	cd RegressionTest && $(MAKE) $@

.PHONY: clean
clean:
	cd RequirementLib && $(MAKE) GENDEPS=0 $@
	cd AnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd EPAnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd TestPhaseLib && $(MAKE) GENDEPS=0 $@
	cd RegressionTest && $(MAKE) GENDEPS=0 $@
	cd BaseTestImpl && $(MAKE) GENDEPS=0 $@
	cd EditProtocolTestImpl && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzer && $(MAKE) GENDEPS=0 $@

.PHONY: realclean
realclean:
	cd AnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd EPAnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd TestPhaseLib && $(MAKE) GENDEPS=0 $@
	cd RegressionTest && $(MAKE) GENDEPS=0 $@
	cd BaseTestImpl && $(MAKE) GENDEPS=0 $@
	cd EditProtocolTestImpl && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzer && $(MAKE) GENDEPS=0 $@
