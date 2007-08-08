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
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################


EXAMPLE = ImportDV

include ../comexamplerules.mk

TEST_CMD = $(APP_RUN_ENV) $(AAFSDKBINDIR)/ImportDV$(EXE)
.PHONY : check
check : $(AAFSDKBINDIR)/ImportDV$(EXE)
	@# Only run libdv compression tests if libdv available
ifdef LIBDV_PATH
	@rm -f raw.yuv
	$(TEST_CMD) -compressionEnable ../ExportDV/ExportDV.aaf && test -f raw.yuv
	@rm -f raw.yuv
	$(TEST_CMD) -compressionEnable ../ExportDV/ExportDV.mxf && test -f raw.yuv
	@rm -f raw.yuv
	$(TEST_CMD) -compressionEnable ../ExportDV/ExportDV_legacyDV.aaf && test -f raw.yuv
	@rm -f raw.yuv
	$(TEST_CMD) -compressionEnable ../ExportDV/ExportDV_DVBased.aaf && test -f raw.yuv
endif
	@# Always run test using DV-compressed frame (doesn't require libdv)
	@rm -f raw.dv
	$(TEST_CMD) ../ExportDV/ExportDV_NoCompression.aaf && test -f raw.dv
