PACKAGE = ComModAAF

AAFBASE ?= ../../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I.. -I. \
	   -I../../../../ref-impl/include \
	   -I../../../../ref-impl/include/com-api \
	   -I../../../../ref-impl/src/com-api/com-dll \
           -I../../../../ref-impl/src/com-api \
           -I../../../../ref-impl/src/impl \
	   -I../../../../ref-impl/include/ref-api \
	   -I../../../../Utilities/Include


OBJDIR = $(PACKDIR)/obj/ModuleTests


.PHONY : all
all : $(OBJDIR) $(CXXOBJS)


.PHONY : clean
clean :
	@$(RMDIR) $(OBJDIR)/*


include $(AAFBASE)/build/rules.mk

