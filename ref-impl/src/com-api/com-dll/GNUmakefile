
PACKAGE = com-dll

AAFBASE ?= ../../../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../../include/ref-api \
	   -I../../../include/OM \
	   -I../../../src/OM \
	   -I../../../include -Icom-dll \
	   -I../../../include/platform \
	   -I../test -I.. -I. -I../../impl

OBJDIR = $(PACKDIR)/obj/com-dll

CFLAGS += -DDONT_DELETE_LAST_REFERENCE

LIBTARGET = $(LIBDIR)/libcom-dll$(LIB_SUFFIX)

.PHONY : all
all : com-dll

.PHONY : com-dll
com-dll : $(LIBTARGET)

$(LIBTARGET) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) \
	$(LIBDIR) -L$(LIBDIR) $(CXXOBJS) -o $@

.PHONY : clean
clean:
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBTARGET)

include $(AAFBASE)/build/rules.mk

