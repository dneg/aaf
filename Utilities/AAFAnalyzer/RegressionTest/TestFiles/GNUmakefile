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
	@echo "Comparing EPDerivationTest Expected Results (EPDerivationTest.expected) to Actual Results (EPDerivationTest.results)"
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
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEditRateTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest.temp >> EPEditRateTest.results
	@echo "Comparing EPEditRateTest Expected Results (EPEditRateTest.expected) to Actual Results (EPEditRateTest.results)"
	diff EPEditRateTest.expected EPEditRateTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest2.xml EPEditRateTest2.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest2.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest2.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEditRateTest2.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest2.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest2.temp >> EPEditRateTest2.results
	@echo "Comparing EPEditRateTest2 Expected Results (EPEditRateTest2.expected) to Actual Results (EPEditRateTest2.results)"
	diff EPEditRateTest2.expected EPEditRateTest2.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest3.xml EPEditRateTest3.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest3.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest3.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEditRateTest3.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest3.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest3.temp >> EPEditRateTest3.results
	@echo "Comparing EPEditRateTest3 Expected Results (EPEditRateTest3.expected) to Actual Results (EPEditRateTest3.results)"
	diff EPEditRateTest3.expected EPEditRateTest3.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest4.xml EPEditRateTest4.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest4.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest4.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEditRateTest4.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest4.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest4.temp >> EPEditRateTest4.results
	@echo "Comparing EPEditRateTest4 Expected Results (EPEditRateTest4.expected) to Actual Results (EPEditRateTest4.results)"
	diff EPEditRateTest4.expected EPEditRateTest4.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest5.xml EPEditRateTest5.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest5.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest5.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEditRateTest5.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest5.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest5.temp >> EPEditRateTest5.results
	@echo "Comparing EPEditRateTest5 Expected Results (EPEditRateTest5.expected) to Actual Results (EPEditRateTest5.results)"
	diff EPEditRateTest5.expected EPEditRateTest5.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest6.xml EPEditRateTest6.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEditRateTest6.aaf ../../Requirements/AAFRequirements.xml >> EPEditRateTest6.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEditRateTest6.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Edit Rate Test" EPEditRateTest6.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Edit Rate Test" EPEditRateTest6.temp >> EPEditRateTest6.results
	@echo "Comparing EPEditRateTest6 Expected Results (EPEditRateTest6.expected) to Actual Results (EPEditRateTest6.results)"
	diff EPEditRateTest6.expected EPEditRateTest6.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEffectTest.xml EPEffectTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPEffectTest.aaf ../../Requirements/AAFRequirements.xml >> EPEffectTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Annotation Test" EPEffectTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Effect Test" EPEffectTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Effect Test" EPEffectTest.temp >> EPEffectTest.results
	@echo "Comparing EPEffectTest Expected Results (EPEffectTest.expected) to Actual Results (EPEffectTest.results)"
	diff EPEffectTest.expected EPEffectTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPAnnotationTest.xml EPAnnotationTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPAnnotationTest.aaf ../../Requirements/AAFRequirements.xml >> EPAnnotationTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Definition Test" EPAnnotationTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Annotation Test" EPAnnotationTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Annotation Test" EPAnnotationTest.temp >> EPAnnotationTest.results
	@echo "Comparing EPAnnotationTest Expected Results (EPAnnotationTest.expected) to Actual Results (EPAnnotationTest.results)"
	diff EPAnnotationTest.expected EPAnnotationTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPAnnotationTest2.xml EPAnnotationTest2.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPAnnotationTest2.aaf ../../Requirements/AAFRequirements.xml >> EPAnnotationTest2.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Definition Test" EPAnnotationTest2.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Annotation Test" EPAnnotationTest2.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Annotation Test" EPAnnotationTest2.temp >> EPAnnotationTest2.results
	@echo "Comparing EPAnnotationTest 2 Expected Results (EPAnnotationTest2.expected) to Actual Results (EPAnnotationTest2.results)"
	diff EPAnnotationTest2.expected EPAnnotationTest2.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDefinitionTest.xml EPDefinitionTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPDefinitionTest.aaf ../../Requirements/AAFRequirements.xml >> EPDefinitionTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Header Test" EPDefinitionTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Definition Test" EPDefinitionTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Definition Test" EPDefinitionTest.temp >> EPDefinitionTest.results
	@echo "Comparing EPDefinitionTest Expected Results (EPDefinitionTest.expected) to Actual Results (EPDefinitionTest.results)"
	diff EPDefinitionTest.expected EPDefinitionTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDefinitionTest2.xml EPDefinitionTest2.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPDefinitionTest2.aaf ../../Requirements/AAFRequirements.xml >> EPDefinitionTest2.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Header Test" EPDefinitionTest2.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Definition Test" EPDefinitionTest2.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Definition Test" EPDefinitionTest2.temp >> EPDefinitionTest2.results
	@echo "Comparing EPDefinitionTest Expected Results (EPDefinitionTest2.expected) to Actual Results (EPDefinitionTest2.results)"
	diff EPDefinitionTest2.expected EPDefinitionTest2.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPHeaderTest1.xml EPHeaderTest1.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPHeaderTest1.aaf ../../Requirements/AAFRequirements.xml >> EPHeaderTest1.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Parameter Test" EPHeaderTest1.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Header Test" EPHeaderTest1.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Header Test" EPHeaderTest1.temp >> EPHeaderTest1.results
	@echo "Comparing EPHeaderTest 1 Expected Results (EPHeaderTest1.expected) to Actual Results (EPHeaderTest1.results)"
	diff EPHeaderTest1.expected EPHeaderTest1.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPHeaderTest2.xml EPHeaderTest2.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPHeaderTest2.aaf ../../Requirements/AAFRequirements.xml >> EPHeaderTest2.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Parameter Test" EPHeaderTest2.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Header Test" EPHeaderTest2.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Header Test" EPHeaderTest2.temp >> EPHeaderTest2.results
	@echo "Comparing EPHeaderTest 2 Expected Results (EPHeaderTest2.expected) to Actual Results (EPHeaderTest2.results)"
	diff EPHeaderTest2.expected EPHeaderTest2.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPHeaderTest3.xml EPHeaderTest3.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPHeaderTest3.aaf ../../Requirements/AAFRequirements.xml >> EPHeaderTest3.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Parameter Test" EPHeaderTest3.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Header Test" EPHeaderTest3.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Header Test" EPHeaderTest3.temp >> EPHeaderTest3.results
	@echo "Comparing EPHeaderTest 3 Expected Results (EPHeaderTest3.expected) to Actual Results (EPHeaderTest3.results)"
	diff EPHeaderTest3.expected EPHeaderTest3.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPParameterTest.xml EPParameterTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPParameterTest.aaf ../../Requirements/AAFRequirements.xml >> EPParameterTest.temp
	grep -A `expr \`expr \\\`grep -n "Name:   Edit Protocol Multi-Channel Audio Test" EPParameterTest.temp | sed 's_\([0-9]*\).*_\1_' \\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Parameter Test" EPParameterTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - 1` "Name:   Edit Protocol Parameter Test" EPParameterTest.temp >> EPParameterTest.results
	@echo "Comparing EPParameterTest Expected Results (EPParameterTest.expected) to Actual Results (EPParameterTest.results)"
	diff EPParameterTest.expected EPParameterTest.results
	@echo "End of Test"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPMultiChannelAudioTest.xml EPMultiChannelAudioTest.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):/usr/local/lib:$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) EPMultiChannelAudioTest.aaf ../../Requirements/AAFRequirements.xml >> EPMultiChannelAudioTest.temp
	grep -A `expr \`expr \\\`wc -l EPMultiChannelAudioTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\` - \`expr \\\`grep -n "Name:   Edit Protocol Multi-Channel Audio Test" EPMultiChannelAudioTest.temp | sed 's_\([0-9]*\).*_\1_'\\\`\`` "Name:   Edit Protocol Multi-Channel Audio Test" EPMultiChannelAudioTest.temp >> EPMultiChannelAudioTest.results
	@echo "Comparing EPMultiChannelAudioTest Expected Results (EPMultiChannelAudioTest.expected) to Actual Results (EPMultiChannelAudioTest.results)"
	diff EPMultiChannelAudioTest.expected EPMultiChannelAudioTest.results
	@echo "End of Test"
