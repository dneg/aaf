###############################################################################
#
# $Id$
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
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
###############################################################################


PROGNAME = AAFAnalyzerRegressionTest

PROGLIBS = -lBaseTest \
           -lTestPhase \
           -lRequirement \
           -lAAFAnalyzerBase \
           -lAnalyzerBase \
           -lexpat

AAFBASE ?= ../../../..
INCLUDES ?= -I$(AAFSDKINCLUDEDIR) \
	   -I../../AnalyzerBase \
	   -I../../AAFAnalyzerBase \
	   -I../../EPAnalyzerBase \
	   -I../../BaseTestImpl \
	   -I../../EditProtocolTestImpl \
	   -I../../TestPhaseLib \
	   -I../../RequirementLib \
	   -I$(AAFBASE)/examples2/axLib
include ../../aafanalyzer.mk

.PHONY : check
check : $(AAFSDKBINDIR)/AAFAnalyzerRegressionTest$(EXE)
	rm -f *.aaf
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/BuildTestFile$(EXE) GraphBuilder-ResolveRefTest.xml GraphBuilder-ResolveRefTest.aaf
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/AAFAnalyzerRegressionTest$(EXE) \
				-reqs $(AAFBASE)/Utilities/AAFAnalyzer/Requirements/AAFRequirements.xml \
                                -file GraphBuilder-ResolveRefTest.aaf > AAFAnalyzerRegressionTest.actual
	diff AAFAnalyzerRegressionTest.actual AAFAnalyzerRegressionTest.expected > /dev/null
	@echo Analyzer regression test expected versus actual output compared correctly.
