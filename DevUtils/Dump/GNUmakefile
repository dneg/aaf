PACKAGE = DevUtils

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../ref-impl/include \
	   -I../../ref-impl/include/ref-api \
	   -I../../ref-impl/include/com-api \
	   -I../../Utilities/Include \
	   -I../../ss-impl/ref/h


.PHONY : all
all : $(BINDIR)/dump$(EXE)


$(BINDIR)/dump$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(LD) $(CXXOBJS) $(RPATH_OPT) -L$(LIBDIR) -lrefstg -o $@


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(BINDIR)/dump$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

