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
all : $(BINDIR)/ComAAFInfo$(EXE) $(BINDIR)/ComClientAAF$(EXE)


CXXFILES = AAFInfo.cpp


$(BINDIR)/ComAAFInfo$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(LIBDIR) -laaflib -laafiid -o $@


CXXFILES2 = ComClientTestAAF.cpp


$(BINDIR)/ComClientAAF$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS2)
	$(LD) $(CXXOBJS2) -L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(BINDIR)/ComAAFInfo$(EXE)
	@$(RM) $(BINDIR)/ComClientAAF$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

