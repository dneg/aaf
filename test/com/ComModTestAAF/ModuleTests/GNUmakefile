PACKAGE = ComModAAF
include $(AAFBASE)/build/defs.mk

INCLUDES = -I.. -I. \
	   -I../../../../ref-impl/include \
	   -I../../../../ref-impl/include/com-api \
	   -I../../../../ref-impl/src/com-api/com-dll \
           -I../../../../ref-impl/src/com-api \
           -I../../../../ref-impl/src/impl \
	   -I../../../../ref-impl/include/ref-api \
	   -I../../../../Utilities/Include

OBJDIR = $(PACKDIR)/obj/ModTests

.PHONY : all
all : $(LIBDIR)/libModuleTests$(LIB_SUFFIX)

$(LIBDIR)/libModuleTests$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) \
	$(LIBDIR) -L$(LIBDIR) $(CXXOBJS) -o $@ -lC

.PHONY : clean
clean :
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBDIR)/libModuleTests$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk

