PACKAGE = com-api
export PACKAGE

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../include/ref-api \
	   -I../../include/OM \
	   -I../../src/OM \
	   -I../../include \
	   -Icom-dll \
	   -I../../../include/platform \
	   -I../impl \
	   -I.. -I. \
	   -I../../../ss-impl/ref/h \
	   -I../../aaflib


LIBTARGET = $(LIBDIR)/libcom-api$(DLL)


CXXFILES2 = $(wildcard ./com-dll/*.$(CPP_EXTENSION))


.PHONY : all
all : com-api


.PHONY : com-api
com-api : $(LIBTARGET)


.PHONY : com-dll
com-dll :
	cd com-dll && $(MAKE)


$(LIBTARGET) : $(OBJDIR) $(LIBDIR) $(CXXOBJS) com-dll
	$(LD_DYN_LIB) $(CXXOBJS) $(CXXOBJS2) -L$(LIBDIR) -o $@ \
	-limpl -lom -lrefstg -lm


.PHONY : clean
clean:
	cd com-dll && $(MAKE) clean
	$(RMDIR) $(OBJDIR)/*
	$(RM) $(LIBTARGET)


include $(AAFBASE)/build/rules.mk

