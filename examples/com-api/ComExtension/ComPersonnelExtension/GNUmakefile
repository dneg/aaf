PACKAGE = Examples

AAFBASE ?= ../../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../../../ref-impl/include \
	   -I../../../../ref-impl/include/ref-api \
	   -I../../../../ref-impl/include/com-api \
	   -I../../../../ref-impl/src/com-api/com-dll \
	   -I../../../../Utilities/Include \
	   -I..


.PHONY : all
all : $(LIBDIR)/libAAFPersonnelExtension$(DLL) \
      $(BINDIR)/ComPersonnelPluginWrite$(EXE)


CXXFILES = AAFPersonnelExtension.cpp \
	   CAAFAdminMob.cpp \
	   CAAFPersonnelResource.cpp


SUPOBJS = $(OBJDIR)/../../com-api/obj/com-dll/CAAFClassFactory$(OBJ) \
	  $(OBJDIR)/../../com-api/obj/com-dll/CAAFServer$(OBJ) \
	  $(OBJDIR)/../../com-api/obj/com-dll/CAAFUnknown$(OBJ) \
	  $(OBJDIR)/extensionUtils$(OBJ)


$(LIBDIR)/libAAFPersonnelExtension$(DLL) : $(LIBDIR) $(OBJDIR) $(CXXOBJS)
	$(LD_DYN_LIB) $(SUPOBJS) $(CXXOBJS) -L$(LIBDIR) -o $@ -laaflib -laafiid


CXXFILES2 = extensionReadPlugin.cpp \
	    extensionWritePlugin.cpp \
	    writePlugin.cpp


$(BINDIR)/ComPersonnelPluginWrite$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS2)
	$(LD) $(CXXOBJS2) $(OBJDIR)/extensionUtils$(OBJ) \
	-L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(BINDIR)/ComPersonnelExtension$(EXE)
	@$(RM) $(BINDIR)/ComPersonnelPluginWrite$(EXE)


include $(AAFBASE)/build/rules.mk

