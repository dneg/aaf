PACKAGE = aafiid

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../include/ref-api \
	   -I../include


.PHONY : all
all : aafiid


.PHONY : aafiid
aafiid : $(LIBDIR)/libaafiid$(LIB)


$(LIBDIR)/libaafiid$(LIB) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)


.PHONY : clean
clean:
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(LIBDIR)/libaafiid$(LIB)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

