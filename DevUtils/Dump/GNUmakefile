
PACKAGE = DevUtils

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../ref-impl/include \
	   -I../../ref-impl/include/ref-api \
	   -I../../ref-impl/include/com-api \
	   -I../../Utilities/Include \
	   -I../../ss-impl/ref/h

.PHONY : all
all : $(BINDIR)/dump$(EXE_SUFFIX)

$(BINDIR)/dump$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(CC) -n32 -Wl,-wall $(CXXOBJS) $(CC2LDOPT) \
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -lrefstg -lprops -o $@ -lC

.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(BINDIR)/dump$(EXE_SUFFIX)

include $(AAFBASE)/build/rules.mk