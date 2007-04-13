PACKAGE = ref-impl

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I.


CFILES = xmlparse.c \
	 xmltok.c \
	 xmlrole.c

# Flags needed to build expat source
ADD_CFLAGS += -fexceptions -DHAVE_EXPAT_CONFIG_H


.PHONY : all
all : expat


.PHONY : expat
expat : $(OBJDIR) $(LIBDIR) $(LIBDIR)/libexpat$(LIBEXT)


$(LIBDIR)/libexpat$(LIBEXT) :  $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(LIBDIR)/libexpat$(LIBEXT)


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

