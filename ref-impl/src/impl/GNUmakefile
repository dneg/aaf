
PACKAGE = ref-impl
export PACKAGE

AAFBASE ?= ../../..

include $(AAFBASE)/build/defs.mk

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
impl : $(LIBDIR)/libimpl$(LIB_SUFFIX)

$(LIBDIR)/libimpl$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB)  $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) \
	$(LIBDIR) -L$(LIBDIR) $(CXXOBJS) -o $@ -lrefstg -lom -lm

.PHONY : clean
clean :
	-@rm -rf $(OBJDIR)/*.o
	-@rm -f $(LIBDIR)/libimpl$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk



