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

PACKAGE = ref-impl

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk

CFLAGS +=  -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

# Include directories
INCLUDES = -I. \
	   -I../include \
	   -I../include/ref-api \
	   -I../src/impl \
	   -I../src/com-api/com-dll \
	   -I../libjpeg \
           -I../../Utilities/Include \
	   -I../include/com-api


CXXFILES = AAFPluginCOM.cpp \
	   CAAFEssenceDataStream.cpp \
	   CAAFEssenceFileStream.cpp \
	   CAAFEssenceFileContainer.cpp \
	   CAAFJPEGCodec.cpp \
	   CAAFJPEGDescriptorHelper.cpp \
	   CAAFCDCICodec.cpp \
	   CAAFCDCIDescriptorHelper.cpp \
	   CAAFWAVECodec.cpp \
	   CAAFAIFCCodec.cpp \
	   jpegesdatadst.cpp \
	   jpegesdatasrc.cpp


SUPOBJS = $(OBJDIR)/aafCvt$(OBJ) \
	  $(OBJDIR)/AAFUtils$(OBJ) \
	  $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFClassFactory$(OBJ) \
	  $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFServer$(OBJ) \
	  $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFUnknown$(OBJ) \
	  $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFInProcServer$(OBJ)


.PHONY : all
all : aafpgapi aafintp


.PHONY : aafpgapi
aafpgapi : $(OBJDIR) $(LIBDIR) $(LIBDIR)/libaafpgapi$(DLL)


.PHONY : aafintp
aafintp :  $(OBJDIR) $(LIBDIR) $(LIBDIR)/libaafintp$(DLL)


$(LIBDIR)/libaafpgapi$(DLL) : $(CXXOBJS)
	$(LD_DYN_LIB)  \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS) -o $@ \
	$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIB) \
	$(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR)/libaafiid$(LIB) \
	-ljpeg



CXXFILES2 = AAFInterpCOM.cpp \
	    AAFRational.cpp \
	    CAAFBasicInterp.cpp


$(LIBDIR)/libaafintp$(DLL) :  $(CXXOBJS2)
	$(LD_DYN_LIB) \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS2) -o $@ \
        $(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIB) \
        $(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR)/libaafiid$(LIB)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(LIBDIR)/libaafpgapi$(DLL)
	@$(RM) $(LIBDIR)/libaafintp$(DLL)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

