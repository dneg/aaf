
PACKAGE = com-api
export PACKAGE

AAFBASE ?= ../../..

include $(AAFBASE)/build/defs.mk

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

LIBTARGET = $(LIBDIR)/libcom-api$(LIB_SUFFIX)

.PHONY : all
all : com-dll com-api
	echo $(CXXOBJS)

.PHONY : com-dll
com-dll :
	cd com-dll && $(MAKE)

.PHONY : com-api
com-api : $(LIBTARGET)

# Link object files created from com-dll directory source.
# These file are created building com-dll target above.
CXXFILES_COMDLL = $(wildcard ./com-dll/*.$(CPP_EXTENSION))
CXXOBJS_COMDLL = $(CXXFILES_COMDLL:./com-dll/%.$(CPP_EXTENSION)=$(OBJDIR)/../../com-dll/obj/%.o)
CXXOBJS += $(CXXOBJS_COMDLL)

$(LIBTARGET) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) \
	$(LIBDIR) $(CC2LDOPT) -shared -L$(LIBDIR) $(CXXOBJS) -o $@ -limpl

.PHONY : clean
clean:
	cd com-dll && $(MAKE) clean
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBTARGET)

include $(AAFBASE)/build/rules.mk
