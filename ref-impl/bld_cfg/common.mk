###############################################################################
#
# $Id$ $Name$
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
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

#
# Contains elements common to all makefiles in this project.  Requires
# that BLD_CFG_DIR already be set by the including makefile to point
# to this directory.
#
# Some platforms (which do their own makedepend generation) may
# require that DEPEND_INCLUDES and SOURCES be already defined.
#

.SUFFIXES: .o .o86 .oix .ont .obj .c .cpp .idl

#
# Get host type, and include appropriate host-specific makefile
#
UNAME := $(shell uname)

# Newer cygwin environments return "CYGWIN_NT" instead of Windows_NT
ifneq (,$(findstring CYGWIN_NT,$(UNAME)))
	UNAME = Windows_NT
endif 

include $(BLD_CFG_DIR)/$(UNAME).mk


#
# Other macros
#
AR = ar

RM = rm

#
# common targets
#
baseclean:
	${RM} -f *.x86 *.xix *.xnt *.exe
	${RM} -f *.o *.o86 *.oix *.ont *.obj
	${RM} -f *.a *.a86 *.aix *.ant *.lib
	${RM} -f *.dll *.ilk *.pdb *.exp
	${RM} -f vc50.*
	${RM} -f *~ .*~
	${RM} -f .depend.mk*
	${RM} -f .objects.*.txt .testobjs.*.txt
	${RM} -f .comtestobjs.*.txt
	${RM} -f *.core core
