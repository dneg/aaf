PACKAGE = ref-impl

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
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
jpeg : $(OBJDIR) $(LIBDIR) $(LIBDIR)/libjpeg$(LIB)


$(LIBDIR)/libjpeg$(LIB) :  $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(LIBDIR)/libjpeg$(LIB)


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

