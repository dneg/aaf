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

PACKAGE = Examples

AAFBASE ?= ../../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR) \
	   -I../../../../ref-impl/include/ref-api \
	   -I../../../../ref-impl/include/com-api \
	   -I../../../../ref-impl/src/com-api/com-dll \
	   -I../../../../Utilities/Include \
	   -I..


.PHONY : all
all : $(OBJDIR) \
      $(AAFSDKBINDIR)/libAAFPersonnelExtension$(DLL) \
      $(AAFSDKBINDIR)/ComPersonnelPluginWrite$(EXE)


CXXFILES = AAFPersonnelExtension.cpp \
	   CAAFAdminMob.cpp \
	   CAAFPersonnelResource.cpp


SUPOBJS = $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFClassFactory$(OBJ) \
	  $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFServer$(OBJ) \
	  $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFUnknown$(OBJ) \
	  $(OBJDIR)/extensionUtils$(OBJ)


$(AAFSDKBINDIR)/libAAFPersonnelExtension$(DLL) : $(CXXOBJS)
	$(LD_DYN_LIB) $(SUPOBJS) $(CXXOBJS) -L$(AAFSDKLIBDIR) -o $@ -laaflib -laafiid


CXXFILES2 = extensionReadPlugin.cpp \
	    extensionWritePlugin.cpp \
	    writePlugin.cpp


$(AAFSDKBINDIR)/ComPersonnelPluginWrite$(EXE) : $(CXXOBJS2)
	$(LD) $(CXXOBJS2) $(OBJDIR)/extensionUtils$(OBJ) \
	-L$(AAFSDKLIBDIR) -laaflib -laafiid -o $@


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/libAAFPersonnelExtension$(DLL)
	@$(RM) $(AAFSDKBINDIR)/ComPersonnelPluginWrite$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

