
PACKAGE = ComModAAF

AAFBASE ?= ../../..

include $(AAFBASE)/build/defs.mk

INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/com-api \
	   -I../../../ref-impl/src/com-api/com-dll \
           -I../../../ref-impl/src/com-api \
           -I../../../ref-impl/src/impl \
	   -I../../../ref-impl/include/ref-api

CXXFILES = CAAFModuleTest.cpp \
	   ComModTestAAF.cpp

.PHONY : all
all : $(BINDIR)/ComModAAF$(EXE_SUFFIX)

#tbd: -n32 should not go here (need a link options flag)
#tbd: need -lstdc++ for gcc, but can't use it for CC
$(BINDIR)/ComModAAF$(EXE_SUFFIX) : $(BINDIR) $(OBJDIR) $(CXXOBJS) \
	ModuleTests
	$(CC) -n32 -Wl,-wall $(CXXOBJS) $(CC2LDOPT) \
	$(LD_DYN_LIB_PATH_FLAG) $(CC2LDOPT) $(LIBDIR) \
	-L$(LIBDIR) $(CC2LDOPT) -lModuleTests  \
	-laaflib -laafiid -o $@ -lC -laafutils

.PHONY : ModuleTests
ModuleTests :
	@echo "\nBuilding ModuleTests\n"
	cd ModuleTests && $(MAKE)

.PHONY : clean
clean :
	@rm -f $(CXXOBJS)
	@rm -f $(BINDIR)/ComModAAF$(EXE_SUFFIX)
	cd ModuleTests && $(MAKE) clean

.PHONY : check
check : $(BINDIR)/ComModAAF$(EXE_SUFFIX)
	$(BINDIR)/ComModAAF$(EXE_SUFFIX)

include $(AAFBASE)/build/rules.mk





