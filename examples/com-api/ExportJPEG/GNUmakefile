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


EXAMPLE = ExportJPEG

include ../comexamplerules.mk

TEST_CMD = env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/ExportJPEG$(EXE)
.PHONY : check
check : $(AAFSDKBINDIR)/ExportJPEG$(EXE)
	@if [ -r 4frames.mjpeg ] ; then $(TEST_CMD) -res 15:1 -rawJPEG 4frames.mjpeg ; fi
	@if [ -r sdlake_inst.uyvy ] ; then for resid in 2:1 3:1 10:1 20:1 15:1 ; do $(TEST_CMD) -res $$resid -rawUYVY sdlake_inst.uyvy -o sdlake_uyvy_$$resid.aaf || exit $$?; done ; fi
	@if [ -r sdlake_inst.uyvy ] ; then for resid in 2:1 3:1 10:1 20:1 15:1 ; do $(TEST_CMD) -ntsc -res $$resid -rawUYVY sdlake_inst.uyvy -o sdlake_uyvy_ntsc_$$resid.aaf || exit $$?; done ; fi
	for resid in 2:1 3:1 10:1 20:1 15:1 ; do $(TEST_CMD) -res $$resid -o colorbars_$$resid.aaf || exit $$?; done
	for resid in 2:1 3:1 10:1 20:1 15:1 ; do $(TEST_CMD) -ntsc -res $$resid -o colorbars_ntsc_$$resid.aaf || exit $$?; done
