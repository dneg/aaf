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


LIBTARGET = $(BINDIR)/libcom-api$(DLL)


# The following two lines initialize CXXFILES2 to contain the list
# of source files from ./com-dll directory.
CXXFILES_TMP = $(wildcard com-dll/*.$(CPP_EXTENSION)) $(wildcard com-dll/*.c)
CXXFILES2 = $(CXXFILES_TMP:com-dll/%=%)


.PHONY : all
all : com-api


.PHONY : com-api
com-api : $(LIBTARGET)


.PHONY : com-dll
com-dll :
	cd com-dll && $(MAKE)


$(LIBTARGET) : $(OBJDIR) $(BINDIR) $(CXXOBJS) com-dll
	$(LD_DYN_LIB) $(CXXOBJS) $(CXXOBJS2) -L$(LIBDIR) -o $@ \
	-limpl -lom -lrefstg -lm


.PHONY : clean
clean:
	cd com-dll && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(LIBTARGET)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd com-dll && $(MAKE) GENDEPS=0 $@


include $(AAFBASE)/build/rules.mk

