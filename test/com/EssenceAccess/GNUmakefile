PACKAGE = Test

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../ref-impl/include/com-api \
	   -I../../../Utilities/Include


.PHONY : all
all : $(BINDIR)/EssenceAccess$(EXE)


$(BINDIR)/EssenceAccess$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : clean
clean :
	$(RM) $(CXXOBJS)
	$(RM) $(BINDIR)/EssenceAccess$(EXE)


include $(AAFBASE)/build/rules.mk

