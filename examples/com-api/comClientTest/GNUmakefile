
PACKAGE = Examples

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../ref-impl/include/com-api \
	   -I../../../Utilities/Include

.PHONY : all
all : $(BINDIR)/ComAAFInfo$(EXE_SUFFIX) $(BINDIR)/ComClientAAF$(EXE_SUFFIX)

CXXFILES = AAFInfo.cpp

$(BINDIR)/ComAAFInfo$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(CC) -n32 -Wl,-wall $(CXXOBJS) $(CC2LDOPT) \
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -laaflib -laafiid -o $@ -lC -laafutils

CXXFILES2 = ComClientTestAAF.cpp

$(BINDIR)/ComClientAAF$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS2)
	$(CC) -n32 -Wl,-wall $(CXXOBJS2) $(CC2LDOPT) \
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -laaflib -laafiid -o $@ -lC -laafutils

.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(CXXOBJS2)
	@rm -f $(BINDIR)/ComAAFInfo$(EXE_SUFFIX)
	@rm -f $(BINDIR)/ComClientAAF$(EXE_SUFFIX)

include $(AAFBASE)/build/rules.mk
