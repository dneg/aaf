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


EXAMPLE = ExportPCM

include ../comexamplerules.mk

# Add include path to EssenceTestData.h for Laser audio samples
INCLUDES += -I../../../test/com/ComModTestAAF/ModuleTests

TEST_CMD = $(APP_RUN_ENV) $(AAFSDKBINDIR)/ExportPCM$(EXE)
.PHONY : check
check : $(AAFSDKBINDIR)/ExportPCM$(EXE)
	@rm -f ExportPCM.aaf Laser.wav Laser.pcm Laser.aaf
	$(TEST_CMD) && test -f ExportPCM.aaf && test -f Laser.wav
	@rm -f ExportPCM.aaf Laser.wav Laser.pcm Laser.aaf
	$(TEST_CMD) -File && test -f ExportPCM.aaf && test -f Laser.pcm
	@rm -f ExportPCM.aaf Laser.wav Laser.pcm Laser.aaf
	$(TEST_CMD) -AAF && test -f ExportPCM.aaf && test -f Laser.aaf
	@rm -f ExportPCM.aaf Laser.wav Laser.pcm Laser.aaf
	$(TEST_CMD) -embed && test -f ExportPCM.aaf
