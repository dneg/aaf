
PACKAGE = Utilities

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../Include \
	   -I../../OMF/Include \
	   -I../../OMF/portinc \
	   -I../../ref-impl/include/ref-api \
	   -I../../ref-impl/include \
	   -I../../ref-impl/include/com-api

.PHONY : all
all : $(BINDIR)/AafOmf$(EXE_SUFFIX)

#tbd: AafOmf converter does not compile under Irix CC yet
$(BINDIR)/AafOmf$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
#	$(CC) -n32 -Wl,-wall $(CXXOBJS) $(CC2LDOPT) \
#	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
#	-L$(LIBDIR) $(CC2LDOPT) -laaflib -laafiid -o $@ -lC -laafutils

.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(BINDIR)/AafOmf$(EXE_SUFFIX)

CFLAGS += -DNO_CONTAINER_EXPORTS

include $(AAFBASE)/build/rules.mk