PACKAGE = ComModAAF

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/com-api \
	   -I../../../ref-impl/src/com-api/com-dll \
           -I../../../ref-impl/src/com-api \
           -I../../../ref-impl/src/impl \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../Utilities/Include


# CXXFILES2 is declared just to get the list of object files
# from ModuleTests directory. These object files are created 
# when building target ModuleTests
CXXFILES2 = $(wildcard ModuleTests/*.$(CPP_EXTENSION))


.PHONY : all
all : $(BINDIR)/ComModAAF$(EXE)


#tbd: -n32 should not go here (need a link options flag)
#tbd: need -lstdc++ for gcc, but can't use it for CC
$(BINDIR)/ComModAAF$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS) ModuleTests
	$(LD) $(CXXOBJS) $(CXXOBJS2) \
	-L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : ModuleTests
ModuleTests :
	@echo "\nBuilding ModuleTests\n"
	cd ModuleTests && $(MAKE)


.PHONY : clean
clean :
	cd ModuleTests && $(MAKE) clean
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(BINDIR)/ComModAAF$(EXE)


.PHONY : check
check : $(BINDIR)/ComModAAF$(EXE)
	$(BINDIR)/ComModAAF$(EXE)


include $(AAFBASE)/build/rules.mk

