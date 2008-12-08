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


EXAMPLE = ExportDV

include ../comexamplerules.mk

TEST_CMD = $(APP_RUN_ENV) $(AAFSDKBINDIR)/ExportDV$(EXE)
.PHONY : check
check : $(AAFSDKBINDIR)/ExportDV$(EXE)
	@# Only run compression tests if ffmpeg available
ifdef FFMPEG_PATH
	@# Test default args
	$(TEST_CMD)
	@# Test mxf output
	$(TEST_CMD) -o ExportDV.mxf -mxf
	@# Test compression using all codec flavours
	@for f in \
	LegacyDV_625_50 \
	LegacyDV_525_60 \
	IEC_DV_625_50 \
	IEC_DV_525_60 \
	DV_Based_25Mbps_625_50 \
	DV_Based_25Mbps_525_60 \
	DV_Based_50Mbps_625_50 \
	DV_Based_50Mbps_525_60 \
	DVbased_1080x50I_100Mbps \
	DVbased_1080x5994I_100Mbps \
	DVbased_720x50P_100Mbps \
	DVbased_720x5994P_100Mbps \
	SMPTE_D10_625x50I_50Mbps \
	SMPTE_D10_525x5994I_50Mbps \
	SMPTE_D10_625x50I_40Mbps \
	SMPTE_D10_525x5994I_40Mbps \
	SMPTE_D10_625x50I_30Mbps \
	SMPTE_D10_525x5994I_30Mbps ; do \
	  rm -f $$f.aaf; echo $(TEST_CMD) -o ExportDV_$$f.aaf -f $$f; $(TEST_CMD) -o ExportDV_$$f.aaf -f $$f || exit 1; \
	done
endif
	@# Run test using pre-compressed DV frame (default IEC_DV_625_50 flavour)
	rm -f ExportDV_NoCompression.aaf
	$(TEST_CMD) -o ExportDV_NoCompression.aaf -raw ../../../test/com/ScaleTest/1frame.dv && test -f ExportDV_NoCompression.aaf
