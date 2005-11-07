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
	rm -f *.aaf *.temp *.results
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDerivationTest.xml EPDerivationTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPDerivationTest.aaf ../../Requirements/AAFRequirements.xml >> EPDerivationTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Naming Test" EPDerivationTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Mob Derivation Test" EPDerivationTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Mob Derivation Test" EPDerivationTest.temp >> EPDerivationTest.results
	@echo "Comparing EPDericationTest Expected Results (EPDerivationTest.expected) to Actual Results (EPDerivationTest.results)"
	diff EPDerivationTest.expected EPDerivationTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPNameTest.xml EPNameTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPNameTest.aaf ../../Requirements/AAFRequirements.xml >> EPNameTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Mob Contained Tracks Test" EPNameTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Naming Test" EPNameTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Naming Test" EPNameTest.temp >> EPNameTest.results
	@echo "Comparing EPNameTest Expected Results (EPNameTest.expected) to Actual Results (EPNameTest.results)"
	diff EPNameTest.expected EPNameTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPContainedTrackTest.xml EPContainedTrackTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPContainedTrackTest.aaf ../../Requirements/AAFRequirements.xml >> EPContainedTrackTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Track Contents Test" EPContainedTrackTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Mob Contained Tracks Test" EPContainedTrackTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Mob Contained Tracks Test" EPContainedTrackTest.temp >> EPContainedTrackTest.results
	@echo "Comparing EPContainedTrackTest Expected Results (EPContainedTrackTest.expected) to Actual Results (EPContainedTrackTest.results)"
	diff EPContainedTrackTest.expected EPContainedTrackTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPTrackContentsTest.xml EPTrackContentsTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPTrackContentsTest.aaf ../../Requirements/AAFRequirements.xml >> EPTrackContentsTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPTrackContentsTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Track Contents Test" EPTrackContentsTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Track Contents Test" EPTrackContentsTest.temp >> EPTrackContentsTest.results
	@echo "Comparing EPTrackContentsTest Expected Results (EPTrackContentsTest.expected) to Actual Results (EPTrackContentsTest.results)"
	diff EPTrackContentsTest.expected EPTrackContentsTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest.xml EPEditRateTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest.temp
	grep -A `expr \`expr \\\`wc -l EPEditRateTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest.temp >> EPEditRateTest.results
	@echo "Comparing EPEditRateTest Expected Results (EPEditRateTest.expected) to Actual Results (EPEditRateTest.results)"
	diff EPEditRateTest.expected EPEditRateTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest2.xml EPEditRateTest2.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest2.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest2.temp
	grep -A `expr \`expr \\\`wc -l EPEditRateTest2.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest2.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest2.temp >> EPEditRateTest2.results
	@echo "Comparing EPEditRateTest2 Expected Results (EPEditRateTest2.expected) to Actual Results (EPEditRateTest2.results)"
	diff EPEditRateTest2.expected EPEditRateTest2.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest3.xml EPEditRateTest3.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest3.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest3.temp
	grep -A `expr \`expr \\\`wc -l EPEditRateTest3.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest3.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest3.temp >> EPEditRateTest3.results
	@echo "Comparing EPEditRateTest3 Expected Results (EPEditRateTest3.expected) to Actual Results (EPEditRateTest3.results)"
	diff EPEditRateTest3.expected EPEditRateTest3.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest4.xml EPEditRateTest4.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest4.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest4.temp
	grep -A `expr \`expr \\\`wc -l EPEditRateTest4.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest4.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest4.temp >> EPEditRateTest4.results
	@echo "Comparing EPEditRateTest4 Expected Results (EPEditRateTest4.expected) to Actual Results (EPEditRateTest4.results)"
	diff EPEditRateTest4.expected EPEditRateTest4.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest5.xml EPEditRateTest5.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest5.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest5.temp
	grep -A `expr \`expr \\\`wc -l EPEditRateTest5.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest5.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest5.temp >> EPEditRateTest5.results
	@echo "Comparing EPEditRateTest5 Expected Results (EPEditRateTest5.expected) to Actual Results (EPEditRateTest5.results)"
	diff EPEditRateTest5.expected EPEditRateTest5.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest6.xml EPEditRateTest6.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest6.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest6.temp
	grep -A `expr \`expr \\\`wc -l EPEditRateTest6.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest6.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest6.temp >> EPEditRateTest6.results
	@echo "Comparing EPEditRateTest6 Expected Results (EPEditRateTest6.expected) to Actual Results (EPEditRateTest6.results)"
	diff EPEditRateTest6.expected EPEditRateTest6.results
	@echo "End of Test"