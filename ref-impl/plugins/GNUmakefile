PACKAGE = ref-impl

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


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
	  $(AAFSDK)/com-api/$(AAFTARGETDIR)/CAAFClassFactory$(OBJ) \
	  $(AAFSDK)/com-api/$(AAFTARGETDIR)/CAAFServer$(OBJ) \
	  $(AAFSDK)/com-api/$(AAFTARGETDIR)/CAAFUnknown$(OBJ) \
	  $(AAFSDK)/com-api/$(AAFTARGETDIR)/CAAFInProcServer$(OBJ)


.PHONY : all
all : aafpgapi aafintp


.PHONY : aafpgapi
aafpgapi : $(BINDIR)/libaafpgapi$(DLL)


.PHONY : aafintp
aafintp :  $(BINDIR)/libaafintp$(DLL)


$(BINDIR)/libaafpgapi$(DLL) : $(OBJDIR) $(BINDIR) $(CXXOBJS)
	$(LD_DYN_LIB)  \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS) -o $@ -laaflib -laafiid -ljpeg



CXXFILES2 = AAFInterpCOM.cpp \
	    AAFRational.cpp \
	    CAAFBasicInterp.cpp


$(BINDIR)/libaafintp$(DLL) :  $(OBJDIR) $(BINDIR) $(CXXOBJS2)
	$(LD_DYN_LIB) \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS2) -o $@ -laaflib -laafiid


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(BINDIR)/libaafpgapi$(DLL)
	@$(RM) $(BINDIR)/libaafintp$(DLL)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

