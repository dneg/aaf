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
all : $(BINDIR)/ComExtensionRead$(EXE) \
      $(BINDIR)/ComExtensionWrite$(EXE) \
      ComPersonnelExtension


.PHONY : ComPersonnelExtension
ComPersonnelExtension :
	cd ComPersonnelExtension && $(MAKE)


CXXFILES = read.cpp \
	   extensionRead.cpp \
	   extensionUtils.cpp


$(BINDIR)/ComExtensionRead$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(LIBDIR) -laaflib -laafiid -o $@


CXXFILES2 = write.cpp \
	    extensionRead.cpp \
	    extensionUtils.cpp \
	    extensionWrite.cpp


$(BINDIR)/ComExtensionWrite$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS2)
	$(LD) $(CXXOBJS2) -L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(BINDIR)/ComExtensionRead$(EXE)
	@$(RM) $(BINDIR)/ComExtensionWrite$(EXE)


include $(AAFBASE)/build/rules.mk

