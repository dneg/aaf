
PACKAGE = com-dll

AAFBASE ?= ../../../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../../include/ref-api \
	   -I../../../include/OM \
	   -I../../../src/OM \
	   -I../../../include -Icom-dll \
	   -I../../../include/platform \
	   -I../test -I.. -I. -I../../impl

OBJDIR = $(PACKDIR)/obj

CFLAGS += -DDONT_DELETE_LAST_REFERENCE

.PHONY : all
all : com-dll

.PHONY : com-dll
com-dll : $(OBJDIR) $(LIBDIR) $(CXXOBJS)

.PHONY : clean
clean:
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBTARGET)

include $(AAFBASE)/build/rules.mk

