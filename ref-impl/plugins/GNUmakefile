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
	  $(OBJDIR)/../../com-api/obj/com-dll/CAAFClassFactory$(OBJ) \
	  $(OBJDIR)/../../com-api/obj/com-dll/CAAFServer$(OBJ) \
	  $(OBJDIR)/../../com-api/obj/com-dll/CAAFUnknown$(OBJ) \
	  $(OBJDIR)/../../com-api/obj/com-dll/CAAFInProcServer$(OBJ)


.PHONY : all
all : aafpgapi aafintp


.PHONY : aafpgapi
aafpgapi : $(LIBDIR)/libaafpgapi$(DLL)


.PHONY : aafintp
aafintp :  $(LIBDIR)/libaafintp$(DLL)


$(LIBDIR)/libaafpgapi$(DLL) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB)  \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS) -o $@ -laaflib -laafiid -ljpeg



CXXFILES2 = AAFInterpCOM.cpp \
	    AAFRational.cpp \
	    CAAFBasicInterp.cpp


$(LIBDIR)/libaafintp$(DLL) :  $(OBJDIR) $(LIBDIR) $(CXXOBJS2)
	$(LD_DYN_LIB) \
	-L$(LIBDIR) $(SUPOBJS) $(CXXOBJS2) -o $@ -laaflib -laafiid


.PHONY : clean
clean :
	$(RM) $(OBJDIR)/*$(OBJ)
	$(RM) $(LIBDIR)/libaafpgapi$(DLL)
	$(RM) $(LIBDIR)/libaafintp$(DLL)


include $(AAFBASE)/build/rules.mk

