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

ALL = AAFAnalyzerBase \
      RequirementLib \
      TestPhaseLib \
      BaseTestImpl \
      RegressionTest \
      EditProtocolTestImpl \
      AAFAnalyzer

all: $(ALL)

.PHONY: AAFAnalyzerBase
AAFAnalyzerBase:
	cd AAFAnalyzerBase && $(MAKE)

.PHONY: RequirementLib
RequirementLib:
	cd RequirementLib && $(MAKE)	

.PHONY: TestPhaseLib
TestPhaseLib: AAFAnalyzerBase RequirementLib
	cd TestPhaseLib && $(MAKE)

.PHONY: BaseTestImpl
BaseTestImpl: AAFAnalyzerBase RequirementLib TestPhaseLib
	cd BaseTestImpl && $(MAKE)

.PHONY: RegressionTest
RegressionTest: AAFAnalyzerBase RequirementLib TestPhaseLib BaseTestImpl
	cd RegressionTest && $(MAKE)

.PHONY: EditProtocolTestImpl
EditProtocolTestImpl: AAFAnalyzerBase RequirementLib TestPhaseLib BaseTestImpl
	cd EditProtocolTestImpl && $(MAKE)

.PHONY: AAFAnalyzer
AAFAnalyzer: AAFAnalyzerBase RequirementLib TestPhaseLib BaseTestImpl EditProtocolTestImpl
	cd AAFAnalyzer && $(MAKE)

.PHONY : check
check : RegressionTest
	cd RegressionTest && $(MAKE) $@

.PHONY: clean
clean:
	cd RequirementLib && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd TestPhaseLib && $(MAKE) GENDEPS=0 $@
	cd RegressionTest && $(MAKE) GENDEPS=0 $@
	cd BaseTestImpl && $(MAKE) GENDEPS=0 $@
	cd EditProtocolTestImpl && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzer && $(MAKE) GENDEPS=0 $@

.PHONY: realclean
realclean:
	cd AAFAnalyzerBase && $(MAKE) GENDEPS=0 $@
	cd TestPhaseLib && $(MAKE) GENDEPS=0 $@
	cd RegressionTest && $(MAKE) GENDEPS=0 $@
	cd BaseTestImpl && $(MAKE) GENDEPS=0 $@
	cd EditProtocolTestImpl && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzer && $(MAKE) GENDEPS=0 $@
