PACKAGE = ref-impl
export PACKAGE

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I. \
	   -I../../include/ref-api \
	   -I../../include \
	   -I../../include/platform \
	   -I../../include/OM \
	   -I../../src/OM \
	   -I../../aaflib


.PHONY : all
all : impl


.PHONY : impl
impl : $(LIBDIR)/libimpl$(LIB)


$(LIBDIR)/libimpl$(LIB) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS) -lm


.PHONY : clean
clean :
	$(RMDIR) $(OBJDIR)/*
	$(RM) $(LIBDIR)/libimpl$(LIB)


include $(AAFBASE)/build/rules.mk

