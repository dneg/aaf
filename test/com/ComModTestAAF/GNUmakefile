PACKAGE = ComModAAF

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../ref-impl/include/com-api \
	   -I../../../ref-impl/src/com-api/com-dll \
           -I../../../ref-impl/src/com-api \
           -I../../../ref-impl/src/impl \
	   -I../../../Utilities/Include


# CXXFILES2 is declared just to get the list of object files
# from ModuleTests directory. These object files are created 
# when building target ModuleTests
CXXFILES_TMP = $(wildcard ModuleTests/*.$(CPP_EXTENSION)) $(wildcard ModuleTests/*.c)
CXXFILES2 = $(CXXFILES_TMP:ModuleTests/%=%)


.PHONY : all
all : $(BINDIR)/ComModAAF$(EXE)


$(BINDIR)/ComModAAF$(EXE) : $(BINDIR) $(OBJDIR) $(CXXOBJS) ModuleTests
	$(LD) $(CXXOBJS) $(CXXOBJS2) \
	-L$(LIBDIR) -laaflib -laafiid -o $@


.PHONY : ModuleTests
ModuleTests :
	@echo "\nBuilding ModuleTests\n"
	cd ModuleTests && $(MAKE)


.PHONY : clean
clean :
	cd ModuleTests && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(BINDIR)/ComModAAF$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd ModuleTests && $(MAKE) GENDEPS=0 $@


.PHONY : check
check : $(BINDIR)/ComModAAF$(EXE)
	$(BINDIR)/ComModAAF$(EXE)


include $(AAFBASE)/build/rules.mk

