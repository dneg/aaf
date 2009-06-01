###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################


EXAMPLE = ExportJPEG

include ../comexamplerules.mk

TEST_CMD = $(APP_RUN_ENV) $(AAFSDKBINDIR)/ExportJPEG$(EXE)
.PHONY : check
check : $(AAFSDKBINDIR)/ExportJPEG$(EXE)
	@if [ -r 4frames.mjpeg ] ; then $(TEST_CMD) -res 15:1 -rawJPEG 4frames.mjpeg ; fi
	@if [ -r sdlake_inst.uyvy ] ; then for resid in 2:1 3:1 10:1 20:1 15:1 ; do resname=`echo $$resid | sed 's/:/_/g'`; $(TEST_CMD) -res $$resid -rawUYVY sdlake_inst.uyvy -o sdlake_uyvy_$$resname.aaf || exit $$?; done ; fi
	@if [ -r sdlake_inst.uyvy ] ; then for resid in 2:1 3:1 10:1 20:1 15:1 ; do resname=`echo $$resid | sed 's/:/_/g'`; $(TEST_CMD) -ntsc -res $$resid -rawUYVY sdlake_inst.uyvy -o sdlake_uyvy_ntsc_$$resname.aaf || exit $$?; done ; fi
	for resid in 2:1 3:1 10:1 20:1 15:1 ; do resname=`echo $$resid | sed 's/:/_/g'`; $(TEST_CMD) -res $$resid -o colorbars_$$resname.aaf || exit $$?; done
	for resid in 2:1 3:1 10:1 20:1 15:1 ; do resname=`echo $$resid | sed 's/:/_/g'`; $(TEST_CMD) -ntsc -res $$resid -o colorbars_ntsc_$$resname.aaf || exit $$?; done
