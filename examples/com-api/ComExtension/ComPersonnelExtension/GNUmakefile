
PACKAGE = Examples

AAFBASE ?= ../../../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../../../ref-impl/include \
	   -I../../../../ref-impl/include/ref-api \
	   -I../../../../ref-impl/include/com-api \
	   -I../../../../ref-impl/src/com-api/com-dll \
	   -I../../../../Utilities/Include \
	   -I..

.PHONY : all
all : $(LIBDIR)/libAAFPersonnelExtension$(LIB_SUFFIX) \
      $(BINDIR)/ComPersonnelPluginWrite$(EXE_SUFFIX)

CXXFILES = AAFPersonnelExtension.cpp \
	   CAAFAdminMob.cpp \
	   CAAFPersonnelResource.cpp

SUPOBJS = $(OBJDIR)/../../com-dll/obj/CAAFClassFactory.o \
	  $(OBJDIR)/../../com-dll/obj/CAAFServer.o \
	  $(OBJDIR)/../../com-dll/obj/CAAFUnknown.o \
	  $(OBJDIR)/extensionUtils.o

$(LIBDIR)/libAAFPersonnelExtension$(LIB_SUFFIX) : $(LIBDIR) $(OBJDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(SUPOBJS) $(CXXOBJS) \
	$(CC2LDOPT) $(LD_DYN_LIB_PATH_FLAG) $(LIBDIR) \
	-L$(LIBDIR) -o $@ -laaflib -laafiid

CXXFILES2 = extensionReadPlugin.cpp \
	    extensionWritePlugin.cpp \
	    writePlugin.cpp

$(BINDIR)/ComPersonnelPluginWrite$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS2)
	$(CC) -n32 -Wl,-wall $(CXXOBJS2) $(OBJDIR)/extensionUtils.o $(CC2LDOPT)\
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -laaflib -laafiid -o $@ -lC -laafutils

.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(CXXOBJS2)
	@rm -f $(BINDIR)/ComPersonnelExtension$(EXE_SUFFIX)
	@rm -f $(BINDIR)/ComPersonnelPluginWrite$(EXE_SUFFIX)

include $(AAFBASE)/build/rules.mk
