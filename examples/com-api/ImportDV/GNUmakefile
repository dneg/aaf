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
	@# Only run decompression tests if ffmpeg available
ifdef FFMPEG_PATH
	@for f in \
	LegacyDV_625_50 \
	LegacyDV_525_60 \
	IEC_DV_625_50 \
	IEC_DV_525_60 \
	DV_Based_25Mbps_625_50 \
	DV_Based_25Mbps_525_60 \
	DV_Based_50Mbps_625_50 \
	DV_Based_50Mbps_525_60 \
	SMPTE_D10_625x50I_50Mbps \
	SMPTE_D10_525x5994I_50Mbps \
	SMPTE_D10_625x50I_40Mbps \
	SMPTE_D10_525x5994I_40Mbps \
	SMPTE_D10_625x50I_30Mbps \
	SMPTE_D10_525x5994I_30Mbps \
	NoCompression \
	; do rm -f raw.yuv; echo $(TEST_CMD) -d ../ExportDV/ExportDV_$$f.aaf; $(TEST_CMD) -d ../ExportDV/ExportDV_$$f.aaf || exit 1; done
endif
	@# Run unwrapping only (no decompression) tests.
	@# Tests if file is present before attempting test to avoid errors
	@# when FFMPEG_PATH is not set and FFMPEG-encoded files not present.
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
	SMPTE_D10_525x5994I_30Mbps \
	NoCompression ; do \
	  rm -f raw.cdci; if [ ! -r ../ExportDV/ExportDV_$$f.aaf ] ; then continue; fi; echo $(TEST_CMD) ../ExportDV/ExportDV_$$f.aaf; $(TEST_CMD) ../ExportDV/ExportDV_$$f.aaf || exit 1; \
	done
