PACKAGE = aaflib

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../include/ref-api \
	   -I../include

CXXFILES = aaflib.cpp \
	   aaflibsgi.cpp

.PHONY : all
all : aaflib

.PHONY : aaflib
aaflib : $(LIBDIR)/libaaflib$(LIB_SUFFIX)

$(LIBDIR)/libaaflib$(LIB_SUFFIX) : $(OBJDIR) $(LIBDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) \
	$(LIBDIR) $(CC2LDOPT) -shared -L$(LIBDIR) $(CXXOBJS) -o \
	$@ $(PROPS_LIB)

.PHONY : clean
clean:
	-@rm -rf $(OBJDIR)/*.o
	-@rm -rf $(LIBDIR)/libaaflib$(LIB_SUFFIX)

include $(AAFBASE)/build/rules.mk


