PACKAGE = OM
export PACKAGE

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


INCLUDES = -I. \
	   -I../../include/OM \
	   -I$(AAFBASE)/ss-impl/ref/h


.PHONY : all
all : om


.PHONY : om
om : $(LIBDIR)/libom$(LIB)


$(LIBDIR)/libom$(LIB) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(LIBDIR)/libom$(LIB)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

