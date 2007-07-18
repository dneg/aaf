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

ANALYZE = $(AAFSDKBINDIR)/AAFAnalyzer$(EXE) -analyze -reqs ../../Requirements/AAFRequirements.xml

include ../../aafanalyzer.mk

.PHONY : clean
clean::
	rm -f *.aaf *.actual

.PHONY : check
check : clean $(AAFSDKBINDIR)/BuildTestFile$(EXE)

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPAcyclicTest.xml EPAcyclicTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Acyclic Analysis Test" -verbose EPAcyclicTest.aaf > EPAcyclicTest.actual
	@echo "Comparing EPAcyclicTest Expected Results (EPAcyclicTest.expected) to Actual Results (EPAcyclicTest.actual)"
	diff EPAcyclicTest.expected EPAcyclicTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDerivationTest.xml EPDerivationTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Mob Derivation Test" -verbose EPDerivationTest.aaf > EPDerivationTest.actual
	@echo "Comparing EPDerivationTest Expected Results (EPDerivationTest.expected) to Actual Results (EPDerivationTest.actual)"
	diff EPDerivationTest.expected EPDerivationTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPNameTest.xml EPNameTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Naming Test" -verbose EPNameTest.aaf > EPNameTest.actual
	@echo "Comparing EPNameTest Expected Results (EPNameTest.expected) to Actual Results (EPNameTest.actual)"
	diff EPNameTest.expected EPNameTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPContainedTrackTest.xml EPContainedTrackTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Mob Contained Tracks Test" -verbose EPContainedTrackTest.aaf > EPContainedTrackTest.actual
	@echo "Comparing EPContainedTrackTest Expected Results (EPContainedTrackTest.expected) to Actual Results (EPContainedTrackTest.actual)"
	diff EPContainedTrackTest.expected EPContainedTrackTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPTrackContentsTest.xml EPTrackContentsTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Track Contents Test" -verbose EPTrackContentsTest.aaf > EPTrackContentsTest.actual
	@echo "Comparing EPTrackContentsTest Expected Results (EPTrackContentsTest.expected) to Actual Results (EPTrackContentsTest.actual)"
	diff EPTrackContentsTest.expected EPTrackContentsTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest.xml EPEditRateTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Edit Rate Test" -verbose EPEditRateTest.aaf > EPEditRateTest.actual
	@echo "Comparing EPEditRateTest Expected Results (EPEditRateTest.expected) to Actual Results (EPEditRateTest.actual)"
	diff EPEditRateTest.expected EPEditRateTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest2.xml EPEditRateTest2.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Edit Rate Test" -verbose EPEditRateTest2.aaf > EPEditRateTest2.actual
	@echo "Comparing EPEditRateTest2 Expected Results (EPEditRateTest2.expected) to Actual Results (EPEditRateTest2.actual)"
	diff EPEditRateTest2.expected EPEditRateTest2.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest3.xml EPEditRateTest3.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Edit Rate Test" -verbose EPEditRateTest3.aaf > EPEditRateTest3.actual
	@echo "Comparing EPEditRateTest3 Expected Results (EPEditRateTest3.expected) to Actual Results (EPEditRateTest3.actual)"
	diff EPEditRateTest3.expected EPEditRateTest3.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest4.xml EPEditRateTest4.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Edit Rate Test" -verbose EPEditRateTest4.aaf > EPEditRateTest4.actual
	@echo "Comparing EPEditRateTest4 Expected Results (EPEditRateTest4.expected) to Actual Results (EPEditRateTest4.actual)"
	diff EPEditRateTest4.expected EPEditRateTest4.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest5.xml EPEditRateTest5.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Edit Rate Test" -verbose EPEditRateTest5.aaf > EPEditRateTest5.actual
	@echo "Comparing EPEditRateTest5 Expected Results (EPEditRateTest5.expected) to Actual Results (EPEditRateTest5.actual)"
	diff EPEditRateTest5.expected EPEditRateTest5.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEditRateTest6.xml EPEditRateTest6.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Edit Rate Test" -verbose EPEditRateTest6.aaf > EPEditRateTest6.actual
	@echo "Comparing EPEditRateTest6 Expected Results (EPEditRateTest6.expected) to Actual Results (EPEditRateTest6.actual)"
	diff EPEditRateTest6.expected EPEditRateTest6.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPEffectTest.xml EPEffectTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Effect Test" -verbose EPEffectTest.aaf > EPEffectTest.actual
	@echo "Comparing EPEffectTest Expected Results (EPEffectTest.expected) to Actual Results (EPEffectTest.actual)"
	diff EPEffectTest.expected EPEffectTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPAnnotationTest.xml EPAnnotationTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Annotation Test" -verbose EPAnnotationTest.aaf > EPAnnotationTest.actual
	@echo "Comparing EPAnnotationTest Expected Results (EPAnnotationTest.expected) to Actual Results (EPAnnotationTest.actual)"
	diff EPAnnotationTest.expected EPAnnotationTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPAnnotationTest2.xml EPAnnotationTest2.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Annotation Test" -verbose EPAnnotationTest2.aaf > EPAnnotationTest2.actual
	@echo "Comparing EPAnnotationTest 2 Expected Results (EPAnnotationTest2.expected) to Actual Results (EPAnnotationTest2.actual)"
	diff EPAnnotationTest2.expected EPAnnotationTest2.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDefinitionTest.xml EPDefinitionTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Definition Test" -verbose EPDefinitionTest.aaf > EPDefinitionTest.actual
	@echo "Comparing EPDefinitionTest Expected Results (EPDefinitionTest.expected) to Actual Results (EPDefinitionTest.actual)"
	diff EPDefinitionTest.expected EPDefinitionTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPDefinitionTest2.xml EPDefinitionTest2.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Definition Test" -verbose EPDefinitionTest2.aaf > EPDefinitionTest2.actual
	@echo "Comparing EPDefinitionTest Expected Results (EPDefinitionTest2.expected) to Actual Results (EPDefinitionTest2.actual)"
	diff EPDefinitionTest2.expected EPDefinitionTest2.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPHeaderTest1.xml EPHeaderTest1.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Header Test" -verbose EPHeaderTest1.aaf > EPHeaderTest1.actual
	@echo "Comparing EPHeaderTest 1 Expected Results (EPHeaderTest1.expected) to Actual Results (EPHeaderTest1.actual)"
	diff EPHeaderTest1.expected EPHeaderTest1.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPHeaderTest2.xml EPHeaderTest2.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Header Test" -verbose EPHeaderTest2.aaf > EPHeaderTest2.actual
	@echo "Comparing EPHeaderTest 2 Expected Results (EPHeaderTest2.expected) to Actual Results (EPHeaderTest2.actual)"
	diff EPHeaderTest2.expected EPHeaderTest2.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPHeaderTest3.xml EPHeaderTest3.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Header Test" -verbose EPHeaderTest3.aaf > EPHeaderTest3.actual
	@echo "Comparing EPHeaderTest 3 Expected Results (EPHeaderTest3.expected) to Actual Results (EPHeaderTest3.actual)"
	diff EPHeaderTest3.expected EPHeaderTest3.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPParameterTest.xml EPParameterTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Parameter Test" -verbose EPParameterTest.aaf > EPParameterTest.actual
	@echo "Comparing EPParameterTest Expected Results (EPParameterTest.expected) to Actual Results (EPParameterTest.actual)"
	diff EPParameterTest.expected EPParameterTest.actual
	@echo "End of Test"

	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/BuildTestFile$(EXE) EPMultiChannelAudioTest.xml EPMultiChannelAudioTest.aaf
	-env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(ANALYZE) -filter "Edit Protocol Multi-Channel Audio Test" -verbose EPMultiChannelAudioTest.aaf > EPMultiChannelAudioTest.actual
	@echo "Comparing EPMultiChannelAudioTest Expected Results (EPMultiChannelAudioTest.expected) to Actual Results (EPMultiChannelAudioTest.actual)"
	diff EPMultiChannelAudioTest.expected EPMultiChannelAudioTest.actual
	@echo "End of Test"
