PACKAGE = Examples

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../ref-impl/include/com-api \
	   -I../../../Utilities/Include


.PHONY : all
all : $(BINDIR)/$(EXAMPLE)$(EXE)


$(BINDIR)/$(EXAMPLE)$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(LD) $(CXXOBJS) $(RPATH_OPT) \
	-L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(BINDIR)/$(EXAMPLE)$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

