PACKAGE = ref-impl
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
	$(RM) $(OBJDIR)/*$(OBJ)
	$(RM) $(LIBDIR)/libom$(LIB)


include $(AAFBASE)/build/rules.mk

