
PACKAGE = Utilities

AAFBASE ?= ../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../Include \
	   -I../../OMF/Include \
	   -I../../OMF/portinc \
	   -I../../ref-impl/include/ref-api \
	   -I../../ref-impl/include \
	   -I../../ref-impl/include/com-api

LIBTARGET = $(LIBDIR)/libAAFConverterLib$(LIB_SUFFIX)
BINTARGET = $(BINDIR)/AafOmf$(EXE_SUFFIX)

.PHONY : all
all : $(LIBTARGET) $(BINTARGET)

$(LIBTARGET) : $(LIBDIR) $(OBJDIR) $(CXXOBJS)
	$(LD_DYN_LIB) -DCOMPILE_AS_DLL $(CXXOBJS) \
	$(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) \
	$(CC2LDOPT) -L$(LIBDIR) -o $@

$(BINTARGET) : $(BINDIR) $(OBJDIR) $(CXXOBJS)
	$(CC) -n32 -Wl,-wall $(CXXOBJS) $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) \
	$(LIBDIR) $(CC2LDOPT) -L$(AAFBASE)/OMF/IrixLibs -lAJPG -lOMFI -lbento \
	-ljpeg $(CC2LDOPT) -L$(LIBDIR) -laaflib -laafiid -o $@ \
	-lC -lm -laafutils 


.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(OBJDIR)/*.o
	@rm -f $(LIBTARGET)
	@rm -f $(BINTARGET)

CFLAGS += -DNO_CONTAINER_EXPORTS

include $(AAFBASE)/build/rules.mk
