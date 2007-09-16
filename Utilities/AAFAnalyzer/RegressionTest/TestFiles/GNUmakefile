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

RUNONETEST = env AAFSDKBINDIR=$(AAFSDKBINDIR) EXE=$(EXE) ./RunOneTest.sh

include ../../aafanalyzer.mk

.PHONY: clean
clean::
	rm -f *.aaf *.actual

.PHONY: check
check:  clean $(AAFSDKBINDIR)/BuildTestFile$(EXE) alltests

.PHONY: alltests
alltests: EPAcyclicTest \
          EPDerivationTest \
          EPNameTest \
          EPContainedTrackTest \
          EPEditRateTest \
          EPEditRateTest2 \
          EPEditRateTest3 \
          EPEditRateTest4 \
          EPEditRateTest5 \
          EPEditRateTest6 \
          EPEffectTest \
          EPDefinitionTest \
          EPDefinitionTest2 \
          EPHeaderTest1 \
          EPHeaderTest2 \
          EPHeaderTest3 \
          EPParameterTest \
          EPAnnotationTest \
          EPAnnotationTest2 \
          EPMultiChannelAudioTest \
          EPTrackContentsTest

.PHONY: EPAcyclicTest
EPAcyclicTest:
	$(RUNONETEST) EPAcyclicTest "Acyclic Analysis Test"

.PHONY: EPDerivationTest
EPDerivationTest:
	$(RUNONETEST) EPDerivationTest "Mob Derivation Test"

.PHONY: EPNameTest
EPNameTest:
	$(RUNONETEST) EPNameTest "Edit Protocol Naming Test"

.PHONY: EPContainedTrackTest
EPContainedTrackTest:
	$(RUNONETEST) EPContainedTrackTest "Edit Protocol Mob Contained Tracks Test"

.PHONY: EPEditRateTest
EPEditRateTest:
	$(RUNONETEST) EPEditRateTest "Edit Protocol Edit Rate Test"

.PHONY: EPEditRateTest2
EPEditRateTest2:
	$(RUNONETEST) EPEditRateTest2 "Edit Protocol Edit Rate Test"

.PHONY: EPEditRateTest3
EPEditRateTest3:
	$(RUNONETEST) EPEditRateTest3 "Edit Protocol Edit Rate Test"

.PHONY: EPEditRateTest4
EPEditRateTest4:
	$(RUNONETEST) EPEditRateTest4 "Edit Protocol Edit Rate Test"

.PHONY: EPEditRateTest5
EPEditRateTest5:
	$(RUNONETEST) EPEditRateTest5 "Edit Protocol Edit Rate Test"

.PHONY: EPEditRateTest6
EPEditRateTest6:
	$(RUNONETEST) EPEditRateTest6 "Edit Protocol Edit Rate Test"

.PHONY: EPEffectTest
EPEffectTest:
	$(RUNONETEST) EPEffectTest "Edit Protocol Effect Test"

.PHONY: EPDefinitionTest
EPDefinitionTest:
	$(RUNONETEST) EPDefinitionTest "Edit Protocol Definition Test"

.PHONY: EPDefinitionTest2
EPDefinitionTest2:
	$(RUNONETEST) EPDefinitionTest2 "Edit Protocol Definition Test"

.PHONY: EPHeaderTest1
EPHeaderTest1:
	$(RUNONETEST) EPHeaderTest1 "Edit Protocol Header Test"

.PHONY: EPHeaderTest2
EPHeaderTest2:
	$(RUNONETEST) EPHeaderTest2 "Edit Protocol Header Test"

.PHONY: EPHeaderTest3
EPHeaderTest3:
	$(RUNONETEST) EPHeaderTest3 "Edit Protocol Header Test"

.PHONY: EPParameterTest
EPParameterTest:
	$(RUNONETEST) EPParameterTest "Edit Protocol Parameter Test"

.PHONY: EPAnnotationTest
EPAnnotationTest:
	$(RUNONETEST) EPAnnotationTest "Edit Protocol Annotation Test"

.PHONY: EPAnnotationTest2
EPAnnotationTest2:
	$(RUNONETEST) EPAnnotationTest2 "Edit Protocol Annotation Test"

.PHONY: EPMultiChannelAudioTest
EPMultiChannelAudioTest:
	$(RUNONETEST) EPMultiChannelAudioTest "Edit Protocol Multi-Channel Audio Test"

.PHONY: EPTrackContentsTest
EPTrackContentsTest:
	$(RUNONETEST) EPTrackContentsTest "Edit Protocol Track Contents Test"
