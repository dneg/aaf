PACKAGE = dodoTool

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


INCLUDES = -I$(AAFBASE)/ref-impl/bld_cfg


.PHONY : all
all : dodo


.PHONY : dodo
dodo: $(BINDIR)/dodo


$(BINDIR)/dodo: $(OBJDIR) $(BINDIR) $(CXXOBJS)
	$(CC) $(CFLAGS) $(CXXOBJS) -o $@


.PHONY : clean
clean :
	@$(RMDIR) $(BINDIR)/dodo
	@$(RMDIR) $(OBJDIR)


include $(AAFBASE)/build/rules.mk

