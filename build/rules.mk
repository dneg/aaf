###############################################################################
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# 
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

#------------------------------------------------------------------------------
#
# rules.mk
#
# This file contains makefile rules necessary to build the AAF toolkit.
#
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
	@echo Generating dependencies for $<; \
	$(SHELL) -ec 'if [ ! -d $(OBJDIR) ]; then $(MKDIR) $(OBJDIR); fi; \
	$(CC) -M $(CFLAGS) $(INCLUDES) $< \
	| sed '\''s,\($(*F)\)\.o[ :]*,$(@D)/\1.o $@ : ,g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

$(OBJDIR)/%.d : %.$(CPP_EXTENSION)
	@echo Generating dependencies for $<; \
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


