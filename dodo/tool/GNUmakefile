PACKAGE = dodoTool

AAFBASE ?= ../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I$(AAFBASE)/ref-impl/bld_cfg

.PHONY : all
all : dodo

.PHONY : dodo
#dodo : dodoLib dodoExe
dodo: dodoExe

#tbd: support building targets in test/ directory
dodoExe: $(BINDIR)/dodo

.PHONY : dodoLib
dodoLib : $(LIBDIR)/libdodo$(LIB_SUFFIX)	

#$(LIBDIR)/libdodo$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
#	$(LD_DYN_LIB) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) -L$(LIBDIR) \
#	$(CXXOBJS) -o $@

#CXXFILES2 = DodoMain.cpp

#$(BINDIR)/dodo : $(OBJDIR) $(BINDIR) $(CXXOBJS2)
#	$(CC) $(CFLAGS) -L$(LIBDIR) $(CXXOBJS2) -ldodo -o $@

$(BINDIR)/dodo: $(OBJDIR) $(BINDIR) $(CXXOBJS)
	$(CC) $(CFLAGS) $(CXXOBJS) -o $@

.PHONY : test
test : dodoLib
	cd test && $(MAKE)

.PHONY : check
check :
	cd test && $(MAKE) check

.PHONY : clean
clean :
	cd test && $(MAKE) clean
	-@rm -rf $(BINDIR)/dodo
	-@rm -rf $(OBJDIR)

include $(AAFBASE)/build/rules.mk







