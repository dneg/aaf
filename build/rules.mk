#------------------------------------------------------------------------------
# This file contains makefile rules necessary to build the AAF toolkit.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Rules to compile the source files.
#------------------------------------------------------------------------------

$(OBJDIR)/%$(OBJ): %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(OBJDIR)/%$(OBJ): %.$(CPP_EXTENSION)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@


#------------------------------------------------------------------------------
# Rules to generate the header file dependencies makefiles from the source
# files.
#------------------------------------------------------------------------------

$(OBJDIR)/%.d : %.c 
	$(SHELL) -ec 'if [ ! -d $(OBJDIR) ]; then $(MKDIR) $(OBJDIR); fi; \
	$(CC) -M $(CFLAGS) $(INCLUDES) $< \
	| sed '\''s,\($(*F)\)\.o[ :]*,$(@D)/\1.o $@ : ,g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

$(OBJDIR)/%.d : %.$(CPP_EXTENSION)
	$(SHELL) -ec 'if [ ! -d $(OBJDIR) ]; then $(MKDIR) $(OBJDIR); fi; \
	$(CC) -M $(CFLAGS) $(INCLUDES) $< \
	| sed '\''s,\($(*F)\)\.o[ :]*,$(@D)/\1.o $@ : ,g'\'' > $@; \
	[ -s $@ ] || rm -f $@'


#------------------------------------------------------------------------------
# Rules to make the different directories.
#------------------------------------------------------------------------------

$(OBJDIR):
	$(MKDIR) $@

$(LIBDIR):
	$(MKDIR) $@

$(BINDIR):
	$(MKDIR) $@


#------------------------------------------------------------------------------
# Now includes the header file dependencies makefiles.
#------------------------------------------------------------------------------

ifneq ($(GENDEPS), 0)
-include $(DEPS)
endif

# Disable header dependencies for now, because they make the build process 
# approximately 10^64 times slower.

#ifneq (,$(CXXFILES))
#include $(CXXFILES:%.$(CPP_EXTENSION)=$(OBJDIR)/%.d)
#endif

#ifneq (,$(CXXFILES2))
#include $(CXXFILES2:%.$(CPP_EXTENSION)=$(OBJDIR)/%.d)
#endif

#ifneq (,$(CFILES))
#include $(CFILES:%.c=$(OBJDIR)/%.d)
#endif

