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

#
# Contains elements for FreeBSD/i386 builds.
#
# The inclusion of depend.mk requires that DEPEND_INCLUDES and SOURCES
# be already defined.
#

PTFM_SUFX = 86
OBJ = o${PTFM_SUFX}
AAF_LIB = a${PTFM_SUFX}
EXE = x${PTFM_SUFX}

CC = gcc
LD = gcc
AAF_CMD_LINK = ld

AAF_LINK_FLAGS = -r
AAF_LINK_OUTPUT_FLAG = -o
LFLAGS = -lg++
CFLAGS = -Wall -Werror
ARFLAGS = -c -q
OBJFILE = -o $*.${OBJ}
EXEFILE = -o unittest.${EXE}
CP = cp
PERL = perl
MAKE_SUFFIX = .gmake

UNICODE = 1
SH_PREFIX = 
SH_SUFFIX =


#
# Some platforms (which do their own makedepend generation) may
# require that DEPEND_INCLUDES and SOURCES be already defined in order
# for depend.mk to function properly.  That should not be a problem if
# Clearcase dependency checking is used.
#
include $(BLD_CFG_DIR)/depend.mk
