PACKAGE = ref-impl
export PACKAGE

include $(AAFBASE)/build/defs.mk

INCLUDES = -I. \
	   -I../../include/OM \
	   -I$(AAFBASE)/ss-impl/ref/h

.PHONY : all
all : om

.PHONY : om
om : $(LIBDIR)/libom$(LIB_SUFFIX)

$(LIBDIR)/libom$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB)  $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) \
	-L$(LIBDIR) -L$(AAFSDK)/ss-impl/lib  $(CXXOBJS) -o $@ -lrefstg

.PHONY : clean
clean :
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBDIR)/libom$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk
