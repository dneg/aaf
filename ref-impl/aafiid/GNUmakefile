
PACKAGE = aafiid

AAFBASE ?= ../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../include/ref-api \
	   -I../include

.PHONY : all
all : aafiid

.PHONY : aafiid
aafiid : $(LIBDIR)/libaafiid$(LIB_SUFFIX)

$(LIBDIR)/libaafiid$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) \
	$(LIBDIR) $(CC2LDOPT) -shared -L$(LIBDIR) $(CXXOBJS) -o \
	$@ $(PROPS_LIB)

.PHONY : clean
clean:
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBDIR)/libaafiid$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk
