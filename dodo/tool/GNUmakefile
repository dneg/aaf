
PACKAGE = dodoTool

AAFBASE ?= ../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I$(AAFBASE)/ref-impl/bld_cfg

.PHONY : all
all : dodo

.PHONY : dodo
dodo: $(BINDIR)/dodo

$(BINDIR)/dodo: $(OBJDIR) $(BINDIR) $(CXXOBJS)
	$(CC) $(CFLAGS) $(CXXOBJS) -o $@

.PHONY : clean
clean :
	-@rm -rf $(BINDIR)/dodo
	-@rm -rf $(OBJDIR)

include $(AAFBASE)/build/rules.mk







