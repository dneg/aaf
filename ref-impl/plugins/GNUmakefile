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

PACKAGE = ref-impl

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I. \
		-I../include \
		-I../include/ref-api \
		-I../src/impl \
		-I../src/com-api \
		-I../src/com-api/com-dll \
		-I../libjpeg \
		-I../include/com-api


CXXFILES = AAFPluginCOM.cpp \
		CAAFEssenceDataStream.cpp \
		CAAFEssenceFileStream.cpp \
		CAAFEssenceFileContainer.cpp \
		CAAFEssenceRIFFWAVEStream.cpp \
		CAAFEssenceRIFFWAVEContainer.cpp \
		CAAFJPEGCodec.cpp \
		CAAFJPEGDescriptorHelper.cpp \
		CAAFCDCICodec.cpp \
		CAAFCDCIDescriptorHelper.cpp \
		CAAFWAVECodec.cpp \
		CAAFAIFCCodec.cpp \
		CAAFPCMCodec.cpp \
		jpegesdatadst.cpp \
		jpegesdatasrc.cpp


SUPOBJS = $(OBJDIR)/AAFUtils$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFClassFactory$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFServer$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFUnknown$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFInProcServer$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/utf8$(OBJ)


.PHONY : all
all : aafpgapi aafintp


DYN_PGAPI_LIB = $(LIBDIR)/libaafpgapi$(DLL)
DYN_INTP_LIB = $(LIBDIR)/libaafintp$(DLL)
STAT_PGAPI_LIB = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libaafpgapi$(LIB)
STAT_INTP_LIB = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libaafintp$(LIB)

# Default targets are dynamic libraries
PGAPI_LIB = $(DYN_PGAPI_LIB)
INTP_LIB = $(DYN_INTP_LIB)
ifeq ($(AAFTARGET),Debug-static)
	PGAPI_LIB = $(STAT_PGAPI_LIB)
	INTP_LIB = $(STAT_INTP_LIB)
endif


.PHONY : aafpgapi
aafpgapi : $(OBJDIR) $(LIBDIR) $(PGAPI_LIB)

.PHONY : aafintp
aafintp : $(OBJDIR) $(LIBDIR) $(INTP_LIB)


$(STAT_PGAPI_LIB) : $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)

$(DYN_PGAPI_LIB) : $(CXXOBJS)
	$(LD_DYN_LIB) \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS) -o $@ \
	$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIB) \
	$(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR)/libaafiid$(LIB) \
	-ljpeg $(OPT_CODEC_LIBS)


CXXFILES2 = AAFInterpCOM.cpp \
		AAFRational.cpp \
		CAAFBasicInterp.cpp


$(STAT_INTP_LIB) : $(CXXOBJS2)
	$(LD_STAT_LIB) $@ $(CXXOBJS2)

$(DYN_INTP_LIB) : $(SUPOBJS) $(CXXOBJS2)
	$(LD_DYN_LIB) \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS2) -o $@ \
	$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIB) \
	$(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR)/libaafiid$(LIB)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(PGAPI_LIB)
	@$(RM) $(INTP_LIB)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
