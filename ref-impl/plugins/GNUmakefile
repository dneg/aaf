
PACKAGE = ref-impl

AAFBASE ?= ../..

include $(AAFBASE)/build/defs.mk

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
	   CAAFWAVECodec.cpp \
	   CAAFAIFCCodec.cpp \
	   jpegesdatadst.cpp \
	   jpegesdatasrc.cpp

SUPOBJS = $(OBJDIR)/aafCvt.o \
	  $(OBJDIR)/AAFUtils.o \
	  $(OBJDIR)/../../com-dll/obj/com-dll/CAAFClassFactory.o \
	  $(OBJDIR)/../../com-dll/obj/com-dll/CAAFServer.o \
	  $(OBJDIR)/../../com-dll/obj/com-dll/CAAFUnknown.o \
	  $(OBJDIR)/../../com-dll/obj/com-dll/CAAFInProcServer.o

.PHONY : all
all : aafpgapi aafintp

.PHONY : aafpgapi
aafpgapi : $(LIBDIR)/libaafpgapi$(LIB_SUFFIX)

.PHONY : aafintp
aafintp :  $(LIBDIR)/libaafintp$(LIB_SUFFIX)

$(LIBDIR)/libaafpgapi$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB)  $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) \
	-L$(LIBDIR) -L$(AAFSDK)/ss-impl/lib $(SUPOBJS) $(CXXOBJS) -o \
	$@ -laaflib -laafiid -ljpeg

CXXFILES2 = AAFInterpCOM.cpp \
	    AAFRational.cpp \
	    CAAFBasicInterp.cpp

$(LIBDIR)/libaafintp$(LIB_SUFFIX) :  $(OBJDIR) $(LIBDIR) $(CXXOBJS2)
	$(LD_DYN_LIB)  $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) \
	-L$(LIBDIR) -L$(AAFSDK)/ss-impl/lib  $(SUPOBJS) $(CXXOBJS2) -o \
	$@ -laaflib -laafiid

.PHONY : clean
clean :
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBDIR)/libaafpgapi$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk

