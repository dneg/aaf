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


PROGNAME = BuildTestFile

PROGLIBS = -lexpat

AAFBASE ?= ../../../..
include ../../aafanalyzer.mk

.PHONY : check
check : $(AAFSDKBINDIR)/BuildTestFile$(EXE)
	rm -f EPDerivationTest.aaf EPDerivationTest.temp EPDerivationTest.results
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDerivationTest.xml EPDerivationTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPDerivationTest.aaf ../../Requirements/AAFRequirements.xml >> EPDerivationTest.temp
	grep -A `expr \`expr \\\`wc -l EPDerivationTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Mob Derivation Test" EPDerivationTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Mob Derivation Test" EPDerivationTest.temp >> EPDerivationTest.results
	@echo "Comparing EPDericationTest Expected Results (EPDerivationTest.expected) to Actual Results (EPDerivationTest.results)"
	diff EPDerivationTest.expected EPDerivationTest.results
	@echo "End of Test"
