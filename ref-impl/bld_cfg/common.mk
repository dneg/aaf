############################################
#                                          #
# Advanced Authoring Format                #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

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
