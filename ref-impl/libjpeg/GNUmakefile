PACKAGE = ref-impl

include $(AAFBASE)/build/defs.mk

INCLUDES = -I.

CFILES = jdapimin.c \
	 jdapistd.c \
	 jdtrans.c \
	 jdatasrc.c \
	 jdmaster.c \
         jdinput.c \
	 jdmarker.c \
	 jdhuff.c \
	 jdphuff.c \
	 jdmainct.c \
	 jdcoefct.c \
         jdpostct.c \
	 jddctmgr.c \
	 jidctfst.c \
	 jidctflt.c \
	 jidctint.c \
	 jidctred.c \
         jdsample.c \
	 jdcolor.c \
	 jquant1.c \
	 jquant2.c \
	 jdmerge.c \
	 jerror.c \
	 jcapimin.c \
	 jcparam.c \
	 jcapistd.c \
	 jutils.c \
	 jmemmgr.c \
	 jcomapi.c \
	 jcmarker.c \
	 jcinit.c \
	 jmemansi.c \
	 jcmaster.c \
	 jccolor.c \
	 jcsample.c \
	 jcprepct.c \
	 jcdctmgr.c \
	 jfdctint.c \
	 jfdctfst.c \
	 jfdctflt.c \
	 jchuff.c \
	 jcphuff.c \
	 jccoefct.c \
	 jcmainct.c

.PHONY : all
all : jpeg

.PHONY : jpeg
jpeg : $(LIBDIR)/libjpeg$(LIB_SUFFIX)

$(LIBDIR)/libjpeg$(LIB_SUFFIX) :  $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB)  $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) \
	-L$(LIBDIR) -L$(AAFSDK)/ss-impl/lib  $(CXXOBJS) -o $@

.PHONY : clean
clean :
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBDIR)/libjpeg$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk