################################################################################
#                                                                              #
# File: win32aafsdk.mak                                                        #
#                                                                              #
# Purpose: Make file that will build the directory structure and copy          #
#   Source files for the AAF SDK. This make file is designed to be             #
#   run with nmake.                                                            #
#                                                                              #
#   This make file should be run after the comapi but before the codecs        #
#   or any other client examples.                                              #
#                                                                              #
# Copyright (c) 1998 Avid Technology, Inc.                                     #
#                                                                              #
################################################################################
# History:                                                                     #
# 08-APR-1999 : created by transdel avid                                       #
################################################################################



all : targets


# The path to the AAF-toolkit
!if "$(AAFTOOLKIT)"==""
AAFTOOLKIT = ..
!endif

# The root directory for the AAF ADK
!if "$(AAFSDK)"==""
AAFSDK  = .
!endif


#
# Missing form common.mk
# The command and options for standard copy operations.
CP = xcopy
CP_OPTS = /q /k /h

# The command and options for standard remove operations.
RM = del
RM_OPTS = /f /q


#
# Directory structure for the AAF SDK
#
AAFSDK_BIN     = $(AAFSDK)\bin
AAFSDK_DEBUG   = $(AAFSDK)\bin\debug
AAFSDK_HELP    = $(AAFSDK)\help
AAFSDK_INCLUDE = $(AAFSDK)\include
AAFSDK_LIB     = $(AAFSDK)\lib


#
# Toolkit directories from the AAF Development tree
#
TOOLKIT_INCLUDE = $(AAFTOOLKIT)\ref-impl\include
TOOLKIT_INCLUDE_COMAPI = $(TOOLKIT_INCLUDE)\com-api
TOOLKIT_PLUGINS = $(AAFTOOLKIT)\ref-impl\plugins
TOOLKIT_COMIDL = $(AAFTOOLKIT)\AAFWinSDK\ref-impl\include\comidl


#
# Target directories
#
TARGET_DIRS = \
	$(AAFSDK_BIN) \
	$(AAFSDK_DEBUG) \
	$(AAFSDK_HELP) \
	$(AAFSDK_INCLUDE) \
	$(AAFSDK_LIB)


#
# Target idl files that need to be copied.
#
TARGET_IDL_FILES = \
	$(AAFSDK_INCLUDE)\AAFTypes.idl \
	$(AAFSDK_INCLUDE)\AAF.idl \
	$(AAFSDK_INCLUDE)\AAFPlugin.idl \
	$(AAFSDK_INCLUDE)\AAFPluginTypes.idl


#
# Target midl files that need to be copied.
#
TARGET_MIDL_FILES = \
	$(AAFSDK_INCLUDE)\AAFTypes.h \
	$(AAFSDK_INCLUDE)\AAF.h \
	$(AAFSDK_INCLUDE)\AAFPlugin.h \
	$(AAFSDK_INCLUDE)\AAFPluginTypes.h \
	$(AAFSDK_INCLUDE)\AAF_i.c \
	$(AAFSDK_INCLUDE)\AAFPlugin_i.c


#
# Target Header files that need to be copied
#
TARGET_H_FILES = \
	$(AAFSDK_INCLUDE)\AAFDefUIDs.h \
	$(AAFSDK_INCLUDE)\AAFMetaDictionary.h \
	$(AAFSDK_INCLUDE)\AAFPropertyIDs.h \
	$(AAFSDK_INCLUDE)\AAFResult.h \
	$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h


#
# Target files that need to be cleanup up.
#
TARGET_FILES_TO_REMOVE = \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES) \
	$(TARGET_MIDL_FILES)


#
# Main dependency order for all targets
#
targets : $(TARGET_DIRS)
targets : $(TARGET_H_FILES)
targets : $(TARGET_IDL_FILES)
targets : $(TARGET_MIDL_FILES)


#
# Dependencies and build rules to create the AAF SDK directory structure.
#
$(AAFSDK) :
	if !exist("$(AAFSDK)") md $(AAFSDK)

$(AAFSDK_BIN) : $(AAFSDK)
!if !exist("$(AAFSDK_BIN)")
	md $(AAFSDK_BIN)
!endif

$(AAFSDK_DEBUG) : $(AAFSDK_BIN)
!if !exist("$(AAFSDK_DEBUG)")
	md $(AAFSDK_DEBUG)
!endif

$(AAFSDK_HELP) : $(AAFSDK)
!if !exist ("$(AAFSDK_HELP)")
	md $(AAFSDK_HELP)
!endif

$(AAFSDK_INCLUDE) : $(AAFSDK)
!if !exist("$(AAFSDK_INCLUDE)")
	md $(AAFSDK_INCLUDE)
!endif

$(AAFSDK_LIB) : $(AAFSDK)
!if !exist("$(AAFSDK_LIB)")
	md $(AAFSDK_LIB)
!endif


#
# Dependency and build rules for the Header targets.
#
$(AAFSDK_INCLUDE)\AAFDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFDefUIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFDefUIDs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFMetaDictionary.h : $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPropertyIDs.h : $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFResult.h : $(TOOLKIT_INCLUDE)\AAFResult.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFResult.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h : $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h $(AAFSDK_INCLUDE)\


#
# Dependency and build rules for the IDL targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAF.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPluginTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPlugin.idl : $(TOOLKIT_PLUGINS)\AAFPlugin.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_PLUGINS)\AAFPlugin.idl $(AAFSDK_INCLUDE)\


#
# Dependency and build rules for the MIDL targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.h : $(TOOLKIT_COMIDL)\AAFTypes.h
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAFTypes.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAF.h : $(TOOLKIT_COMIDL)\AAF.h
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAF.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAF_i.c : $(TOOLKIT_COMIDL)\AAF_i.c
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAF_i.c $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPluginTypes.h : $(TOOLKIT_COMIDL)\AAFPluginTypes.h
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAFPluginTypes.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPlugin.h : $(TOOLKIT_PLUGINS)\AAFPlugin.h
	$(CP) $(CP_OPTS) $(TOOLKIT_PLUGINS)\AAFPlugin.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPlugin_i.c : $(TOOLKIT_PLUGINS)\AAFPlugin_i.c
	$(CP) $(CP_OPTS) $(TOOLKIT_PLUGINS)\AAFPlugin_i.c $(AAFSDK_INCLUDE)\


#
# The cleanup rules for all targets created by this make file.
#
clean :
	@for %%f in ( $(TARGET_FILES_TO_REMOVE) ) do \
	    @$(RM) $(RM_OPTS) %%f
	@if exist $(AAFSDK_DEBUG) rmdir $(AAFSDK_DEBUG)
	@if exist $(AAFSDK_HELP) rmdir $(AAFSDK_HELP)
	@if exist $(AAFSDK_INCLUDE) rmdir $(AAFSDK_INCLUDE)
	@if exist $(AAFSDK_LIB) rmdir $(AAFSDK_LIB)
!if "$(AAFSDK)" != "."
	@if exist $(AAFSDK_BIN) rmdir $(AAFSDK_BIN)
	@if exist $(AAFSDK) rmdir $(AAFSDK)
!endif

