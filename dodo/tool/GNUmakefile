PACKAGE = dodoTool

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


INCLUDES = -I$(AAFBASE)/ref-impl/bld_cfg


# DODO_COMMON_OBJS is the list of object files used both in 
# dodo executable and dodo test application.
#
DODO_COMMON_OBJS = $(OBJDIR)/SourceInfo$(OBJ) $(OBJDIR)/TextStream$(OBJ) \
	$(OBJDIR)/ArgDef$(OBJ) $(OBJDIR)/ArgSet$(OBJ) \
	$(OBJDIR)/MacroDef$(OBJ) $(OBJDIR)/MacroSet$(OBJ)

# DODO_APP_OBJS is the list of object files used in dodo tool.
#
DODO_APP_OBJS = $(DODO_COMMON_OBJS) $(OBJDIR)/DodoMain$(OBJ)

# DODO_TEST_OBJS is the list of object files used in dodo test application.
#
DODO_TEST_OBJS = $(DODO_COMMON_OBJS) $(OBJDIR)/TestMain$(OBJ) \
	$(OBJDIR)/SourceInfoTest$(OBJ) $(OBJDIR)/TextStreamTest$(OBJ) \
	$(OBJDIR)/ArgDefTest$(OBJ) $(OBJDIR)/ArgSetTest$(OBJ) \
	$(OBJDIR)/MacroDefTest$(OBJ) $(OBJDIR)/MacroSetTest$(OBJ)


.PHONY : all
all : dodo
#all : dodo test


.PHONY : dodo
dodo: $(BINDIR)/dodo$(EXE)


.PHONY : test
test: $(BINDIR)/dodo.test$(EXE)


# Note: Although all the object files are in dependencies list
# we don't link all of them to make the target. 
# 
$(BINDIR)/dodo$(EXE): $(OBJDIR) $(BINDIR) $(CXXOBJS)
	$(CC) $(CFLAGS) $(DODO_APP_OBJS) -o $@


# Note: Although all the object files are in dependencies list
# we don't link all of them to make the target. 
# 
$(BINDIR)/dodo.test$(EXE) : $(OBJDIR) $(BINDIR) $(CXXOBJS)
	$(CC) $(CFLAGS) $(DODO_TEST_OBJS) -o $@


.PHONY : clean
clean :
	@$(RM) $(DODO_APP_OBJS) $(DODO_TEST_OBJS)
	@$(RM) $(DEPS)
	@$(RM) $(BINDIR)/dodo$(EXE)
	@$(RM) $(BINDIR)/dodo.test$(EXE)


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

