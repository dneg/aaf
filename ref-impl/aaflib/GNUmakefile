PACKAGE = aaflib

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../include/ref-api \
	   -I../include


.PHONY : all
all : aaflib


.PHONY : aaflib
aaflib : $(LIBDIR)/libaaflib$(LIB)


$(LIBDIR)/libaaflib$(LIB) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)


.PHONY : clean
clean:
	$(RM) $(OBJDIR)/*$(OBJ)
	$(RM) $(LIBDIR)/libaaflib$(LIB)


include $(AAFBASE)/build/rules.mk

