AAFBASE ?= ../../../..


# Common definitions
include $(AAFBASE)/build/common.mk

# Include directories
INCLUDES = -I../../../include/ref-api \
	   -I../../../include/OM \
	   -I../../../src/OM \
	   -I../../../include -Icom-dll \
	   -I../../../include/platform \
	   -I../test -I.. -I. -I../../impl


CFLAGS += -DDONT_DELETE_LAST_REFERENCE


.PHONY : all
all : com-dll


.PHONY : com-dll
com-dll : $(OBJDIR) $(LIBDIR) $(CXXOBJS)


.PHONY : clean
clean:
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

