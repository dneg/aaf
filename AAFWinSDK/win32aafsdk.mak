################################################################################
#                                                                              #
# File: win32aafsdk.mak                                                        #
#                                                                              #
# Purpose: Make file that will build the directory structure and copy          #
#   Source files for the AAF SDK. This make file is designed to be             #
#   run with nmake.                                                            #
#                                                                              #
#   This make file should be run after the comapi but before the codecs        #
#   or any other client examples. This make file should be located in the      #
#   AAFWinSDK project directory in the AAF-toolkit. Otherwise the AAFSDK       #
#   and AAFTOOLKIT variables must be properly defined.                         #
#                                                                              #
#                                                                              #
# Supported variables:                                                         #
#   AAFSDK - variable that will contain the contents of the sdk.               #
#     WARNING: This directory will be removed during the clean.                #
#                                                                              #
#   AAFTOOLKIT - the directory that contains the AAF-toolkit                   #
#                                                                              #
#   CFG=FULL (default)                                                         #
#      nmake /nologo AAFSDK=d:\\aafsdk -f win32aafsdk.mak                      #
#     Create a full aafsdk for a drop in the given AAFSDK directory            #
#      The bin directory will contain the release dlls and the lib directory   #
#      will contain the release libraries. The bin\debug will contain the      #
#      debug dlls and libraries.                                               #
#                                                                              #
#   CFG=Release                                                                #
#      nmake /nologo CFG="Release" win32aafsdk.mak                             #
#     Create the sdk in the toolkit\AAFWinSDK directory for testing.           #
#      The bin directory will contain the release dlls and the lib directory   #
#      will contain the release libraries. There will be not bin\debug         #
#      directory if one exists it will be deleted.                             #
#                                                                              #
#   CFG=Debug                                                                  #
#      nmake /nologo CFG="Debug" win32aafsdk.mak                               #
#     Create the sdk in the toolkit\AAFWinSDK directory for testing.           #
#      The bin directory will contain the debug dlls and the lib directory     #
#      will contain the debug libraries. There will be not bin\debug directory #
#      if one exists it will be deleted.                                       #
#                                                                              #
#                                                                              #
# Copyright (c) 1998 Avid Technology, Inc.                                     #
#                                                                              #
################################################################################
# History:                                                                     #
# 08-APR-1999 : created by transdel avid                                       #
# 13-APR-1999 : transdel Added support for different configurations.           #
################################################################################



all : targets


#
# Missing form common.mk
# The command and options for standard copy operations.
#
CP = xcopy
CP_OPTS = /k /r /i


#
# The command and options for standard remove operations.
#
RM = del
RM_OPTS = /f /q


#
# The path to the AAF-toolkit
#
!if "$(AAFTOOLKIT)"==""
AAFTOOLKIT = ..
!endif


#
# The root directory for the AAF ADK
#
!if "$(AAFSDK)"==""
AAFSDK  = .
!endif


#
# The type of AAF SDK Configuration.
#
!if "$(CFG)"==""
CFG=FULL
!elseif "$(CFG)"=="FULL" || "$(CFG)"=="Full" || "$(CFG)"=="full"
CFG=FULL
!elseif "$(CFG)"=="Release" || "$(CFG)"=="Debug"
CFG=$CFG
!else
!ERROR Unknown configuration!
!endif


#
# Load previous configuration if one exists.
# If the configuration has changed then signal a rebuild be deleting
# the old config file.
#
AAFSDK_CFG=$(AAFSDK)\config.mk
#
!if exist("$(AAFSDK_CFG)")
!include $(AAFSDK_CFG)
!endif


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
TOOLKIT_DEBUG_REFIMPL = $(AAFTOOLKIT)\AAFWinSDK\Debug\RefImpl
TOOLKIT_RELEASE_REFIMPL = $(AAFTOOLKIT)\AAFWinSDK\Release\RefImpl


#
# The type of AAF SDK Configuration.
#
!if "$(CFG)"=="FULL"
TOOLKIT_TARGET_REFIMPL = $(TOOLKIT_RELEASE_REFIMPL)
!elseif "$(CFG)"=="Debug"
TOOLKIT_TARGET_REFIMPL = $(TOOLKIT_DEBUG_REFIMPL)
!elseif "$(CFG)"=="Release"
TOOLKIT_TARGET_REFIMPL = $(TOOLKIT_RELEASE_REFIMPL)
!else
!ERROR Unknown configuration!
!endif


#
# Target directories
#
TARGET_DIRS = \
	$(AAFSDK_BIN) \
!if "$(CFG)"=="FULL"
	$(AAFSDK_DEBUG) \
!endif
	$(AAFSDK_HELP) \
	$(AAFSDK_INCLUDE) \
	$(AAFSDK_LIB)


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
# Target Library files that need to be copied
#
TARGET_LIB_FILES = \
!if "$(CFG)"=="FULL"
	$(AAFSDK_DEBUG)\aafiid.lib \
	$(AAFSDK_DEBUG)\aafcoapi.lib \
!endif
	$(AAFSDK_LIB)\aafiid.lib \
	$(AAFSDK_LIB)\aafcoapi.lib


#
# Target Dynamic Link Libraries files that need to be registered and unregistered
#
TARGET_DLLS_TO_REGISTER = \
	$(AAFSDK_BIN)\aafcoapi.dll \
	$(AAFSDK_BIN)\aafpgapi.dll


#
# Target Dynamic Link Libraries files that need to be copied
#
TARGET_DLL_FILES = \
!if "$(CFG)"=="FULL"
	$(AAFSDK_DEBUG)\aafcoapi.dll \
	$(AAFSDK_DEBUG)\aafpgapi.dll \
!endif
	$(TARGET_DLLS_TO_REGISTER)


#
# Target files that need to be cleanup up.
#
TARGET_FILES_TO_REMOVE = \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES) \
	$(TARGET_MIDL_FILES) \
	$(TARGET_LIB_FILES) \
	$(TARGET_DLL_FILES) \
	$(AAFSDK_CFG)


#
# Directories to delete
# Note: Order is important: must have child before parent directory.
#
TARGET_DIRS_TO_REMOVE = \
	$(AAFSDK_DEBUG) \
!if "$(AAFSDK)" == "."
	$(AAFSDK_HELP) \
	$(AAFSDK_INCLUDE) \
	$(AAFSDK_LIB)
!else
	$(AAFSDK_BIN) \
	$(AAFSDK_HELP) \
	$(AAFSDK_INCLUDE) \
	$(AAFSDK_LIB) \
	$(AAFSDK)
!endif



#
# Main dependency order for all targets
#
sdk_targets : $(TARGET_DIRS)
sdk_targets : $(TARGET_H_FILES)
sdk_targets : $(TARGET_IDL_FILES)
sdk_targets : $(TARGET_MIDL_FILES)
sdk_targets : $(TARGET_LIB_FILES)
sdk_targets : $(TARGET_DLL_FILES)


#
# Dependency that builds the sdk_targets and then the current configuration file.
#
!if "$(LASTCFG)"!="$(CFG)"
targets : clean
!endif
targets : sdk_targets
targets : $(AAFSDK_CFG)


#
# Dependencies for config file.
#
$(AAFSDK_CFG) : sdk_targets
	@echo # > $(AAFSDK_CFG)
	@echo # generated configuration file...DO NOT EDIT >> $(AAFSDK_CFG)
	@echo # >> $(AAFSDK_CFG)
	@echo LASTCFG=$(CFG)  >> $(AAFSDK_CFG)


#
# Dependencies and build rules to create the AAF SDK directory structure.
#
$(AAFSDK) :
	md $(AAFSDK)

$(AAFSDK_BIN) : $(AAFSDK)
	md $(AAFSDK_BIN)

$(AAFSDK_DEBUG) : $(AAFSDK_BIN)
	md $(AAFSDK_DEBUG)

$(AAFSDK_HELP) : $(AAFSDK)
	md $(AAFSDK_HELP)

$(AAFSDK_INCLUDE) : $(AAFSDK)
	md $(AAFSDK_INCLUDE)

$(AAFSDK_LIB) : $(AAFSDK)
	md $(AAFSDK_LIB)


#
# Dependency and build rules for the Header targets.
#
$(AAFSDK_INCLUDE)\AAFDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFDefUIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFDefUIDs.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFMetaDictionary.h : $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFPropertyIDs.h : $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFResult.h : $(TOOLKIT_INCLUDE)\AAFResult.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFResult.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h : $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h "$(AAFSDK_INCLUDE)\"


#
# Dependency and build rules for the IDL targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAF.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFPluginTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFPlugin.idl : $(TOOLKIT_PLUGINS)\AAFPlugin.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_PLUGINS)\AAFPlugin.idl "$(AAFSDK_INCLUDE)\"


#
# Dependency and build rules for the MIDL generated targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.h : $(TOOLKIT_COMIDL)\AAFTypes.h
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAFTypes.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAF.h : $(TOOLKIT_COMIDL)\AAF.h
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAF.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAF_i.c : $(TOOLKIT_COMIDL)\AAF_i.c
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAF_i.c "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFPluginTypes.h : $(TOOLKIT_COMIDL)\AAFPluginTypes.h
	$(CP) $(CP_OPTS) $(TOOLKIT_COMIDL)\AAFPluginTypes.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFPlugin.h : $(TOOLKIT_PLUGINS)\AAFPlugin.h
	$(CP) $(CP_OPTS) $(TOOLKIT_PLUGINS)\AAFPlugin.h "$(AAFSDK_INCLUDE)\"

$(AAFSDK_INCLUDE)\AAFPlugin_i.c : $(TOOLKIT_PLUGINS)\AAFPlugin_i.c
	$(CP) $(CP_OPTS) "$(TOOLKIT_PLUGINS)\AAFPlugin_i.c" "$(AAFSDK_INCLUDE)\"


#
# Dependency and build rules for the library targets.
#
$(AAFSDK_LIB)\aafiid.lib : $(TOOLKIT_TARGET_REFIMPL)\aafiid.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafiid.lib "$(AAFSDK_LIB)\"

$(AAFSDK_LIB)\aafcoapi.lib : $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafcoapi.lib "$(AAFSDK_LIB)\"


#
# Dependency and build rules for the DLL targets.
#
$(AAFSDK_BIN)\aafcoapi.dll : $(TOOLKIT_TARGET_REFIMPL)\aafcoapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafcoapi.dll "$(AAFSDK_BIN)\"
	@regsvr32 /s /c $(AAFSDK_BIN)\aafcoapi.dll

$(AAFSDK_BIN)\aafpgapi.dll : $(TOOLKIT_TARGET_REFIMPL)\aafpgapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafpgapi.dll "$(AAFSDK_BIN)\"
	@regsvr32 /s /c $(AAFSDK_BIN)\aafpgapi.dll

#
# Dependency and build rules for the debug library targets.
#
$(AAFSDK_DEBUG)\aafiid.lib : $(TOOLKIT_DEBUG_REFIMPL)\aafiid.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafiid.lib "$(AAFSDK_DEBUG)\"

$(AAFSDK_DEBUG)\aafcoapi.lib : $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.lib "$(AAFSDK_DEBUG)\"


#
# Dependency and build rules for the debug DLL targets.
#
$(AAFSDK_DEBUG)\aafcoapi.dll : $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.dll "$(AAFSDK_DEBUG)\"

$(AAFSDK_DEBUG)\aafpgapi.dll : $(TOOLKIT_DEBUG_REFIMPL)\aafpgapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafpgapi.dll "$(AAFSDK_DEBUG)\"


#
# Unregister the dlls
#
unregisterdlls:
	@for %%f in ( $(TARGET_DLLS_TO_REGISTER) ) do \
	    @if exist %%f \
		    @echo regsvr32 /u /s/ /c %%f & \
	        regsvr32 /u /s/ /c %%f
	

#
# The cleanup rules for all targets created by this make file.
#
cleanfiles : unregisterdlls
	@for %%f in ( $(TARGET_FILES_TO_REMOVE) ) do \
	    @if exist %%f \
		    @echo $(RM) $(RM_OPTS) %%f & \
	        $(RM) $(RM_OPTS) %%f


#
# The cleanup rules for all targets created by this make file.
#
cleandirs :
	@for %%d in ( $(TARGET_DIRS_TO_REMOVE) ) do \
	    @if exist %%d \
		    @echo rmdir %%d & \
	        rmdir %%d

#
# Cleanup all target files and directories
#
clean : cleanfiles cleandirs


rebuildmsg :
	@echo Rebuilding the entire AAF SDK...

rebuildall : rebuildmsg clean targets
	@echo Done rebuilding.