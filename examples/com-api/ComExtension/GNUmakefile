
PACKAGE = Examples

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../ref-impl/include/com-api \
	   -I../../../Utilities/Include

.PHONY : all
all : $(BINDIR)/ComExtensionRead$(EXE_SUFFIX) \
      $(BINDIR)/ComExtensionWrite$(EXE_SUFFIX) \
      ComPersonnelExtension

.PHONY : ComPersonnelExtension
ComPersonnelExtension :
	cd ComPersonnelExtension && $(MAKE)

CXXFILES = read.cpp \
	   extensionRead.cpp \
	   extensionUtils.cpp

$(BINDIR)/ComExtensionRead$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(CC) -n32 -Wl,-wall $(CXXOBJS) $(CC2LDOPT) \
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -laaflib -laafiid -o $@ -lC -laafutils

CXXFILES2 = write.cpp \
	    extensionRead.cpp \
	    extensionUtils.cpp \
	    extensionWrite.cpp

$(BINDIR)/ComExtensionWrite$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS2)
	$(CC) -n32 -Wl,-wall $(CXXOBJS2) $(CC2LDOPT) \
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -laaflib -laafiid -o $@ -lC -laafutils

.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(CXXOBJS2)
	@rm -f $(BINDIR)/ComExtensionRead$(EXE_SUFFIX)
	@rm -f $(BINDIR)/ComExtensionWrite$(EXE_SUFFIX)

include $(AAFBASE)/build/rules.mk
