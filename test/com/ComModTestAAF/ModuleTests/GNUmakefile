PACKAGE = ComModAAF

AAFBASE ?= ../../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I.. -I. \
	   -I../../../../ref-impl/include \
	   -I../../../../ref-impl/include/ref-api \
	   -I../../../../ref-impl/include/com-api \
	   -I../../../../ref-impl/src/com-api/com-dll \
           -I../../../../ref-impl/src/com-api \
           -I../../../../ref-impl/src/impl \
	   -I../../../../Utilities/Include


.PHONY : all
all : $(OBJDIR) $(CXXOBJS)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

