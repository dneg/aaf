###############################################################################
#                                                                             #
# File: win32aafsdk.mak                                                       #
#                                                                             #
# Purpose: Make file that will build the directory structure and copy         #
#   Source files for the AAF SDK. This make file is designed to be            #
#   run with nmake.                                                           #
#                                                                             #
#   This make file should be run after the comapi but before the codecs       #
#   or any other client examples. This make file should be located in the     #
#   AAFWinSDK project directory in the AAF-toolkit. Otherwise the AAFSDK      #
#   and AAFTOOLKIT variables must be properly defined.                        #
#                                                                             #
#                                                                             #
# Supported variables:                                                        #
#   AAFSDK - variable that will contain the location of the aafsdk.           #
#     WARNING: This directory will be removed during the clean.               #
#                                                                             #
#   AAFTOOLKIT - the directory that contains the AAF-toolkit                  #
#                                                                             #
#   CFG=FULL (default)                                                        #
#      nmake /nologo AAFSDK=d:\aafsdk -f win32aafsdk.mak                      #
#     Create a full aafsdk for a drop in the given AAFSDK directory           #
#      The bin directory will contain the release dlls and the lib directory  #
#      will contain the release libraries. The bin\debug will contain the     #
#      debug dlls and libraries.                                              #
#                                                                             #
#   CFG=Release                                                               #
#      nmake /nologo CFG="Release" win32aafsdk.mak                            #
#     Create the sdk in the toolkit\AAFWinSDK\aafsdk directory for testing.   #
#      The bin directory will contain the release dlls and the lib directory  #
#      will contain the release libraries. There will be not bin\debug        #
#      directory if one exists it will be deleted.                            #
#                                                                             #
#   CFG=Debug                                                                 #
#      nmake /nologo CFG="Debug" win32aafsdk.mak                              #
#     Create the sdk in the toolkit\AAFWinSDK\aafsdk directory for testing.   #
#      The bin directory will contain the debug dlls and the lib directory    #
#      will contain the debug libraries. There will be not bin\debug directory#
#      if one exists it will be deleted.                                      #
#                                                                             #
#                                                                             #
# Copyright (c) 1998 Avid Technology, Inc.                                    #
#                                                                             #
###############################################################################
# History:                                                                    #
# 08-APR-1999 : created by transdel avid                                      #
# 13-APR-1999 : transdel Added support for different configurations.          #
# 14-APR-1999 : transdel The AAFSDK now contains the location of the aafsdk   #
#               directory. Added minimal rebuild for switching between Debug  #
#               and Release. Added a plugins directory. No longer copy the    #
#               AAFPGAPI.dll as part of the SDK.                              #
# 14-JUL-1999 : transdel Updated with new required SDK files.                 #
# 05-OCT-1999 : transdel Updated with new required SDK files. And removed     #
#               vestiges of registration code.                                #
# 06-OCT-1999 : transdel Use copy instead of xcopy so that we can rename the  #
#               files during the copy operation. Also, removed bin\debug      #
#               directory from sdk. Also added support for nmake /a option.   #
# 12-OCT-1999 : transdel Fixed problems with CONFIG_FILES_TO_REMOVE macro for #
#               switching between configurations. The debug versions of the   #
#               dlls were not always replaced with release versions for the   #
#               release and "FULL" builds of the sdk.                         #
# 06-APR-2000 : transdel adding new required header files.                    #
###############################################################################



all : targets


#
# Missing form common.mk
# The command and options for standard copy operations.
#
#CP = xcopy
#CP_OPTS = /r /i
CP = copy
CP_OPTS = /v /b


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
# The location to put the AAF ADK
#
!if "$(SDKDIR)"==""
SDKDIR  = .
!endif


#
# The top level directory of the AAF SDK.
#
!if "$(SDKDIR)"=="."
AAFSDK = .
!else
AAFSDK = $(SDKDIR)\aafsdk
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
AAFSDK_DEBUG   = $(AAFSDK_BIN)\debug
AAFSDK_HELP    = $(AAFSDK)\help
AAFSDK_INCLUDE = $(AAFSDK)\include
AAFSDK_LIB     = $(AAFSDK)\lib


#
# Toolkit directories from the AAF Development tree
#
TOOLKIT_INCLUDE = $(AAFTOOLKIT)\ref-impl\include
TOOLKIT_INCLUDE_COMAPI = $(TOOLKIT_INCLUDE)\com-api
TOOLKIT_INCLUDE_REFAPI = $(TOOLKIT_INCLUDE)\ref-api
TOOLKIT_PLUGINS = $(AAFTOOLKIT)\ref-impl\plugins
TOOLKIT_COMIDL = $(AAFTOOLKIT)\AAFWinSDK\ref-impl\include\comidl
TOOLKIT_DEBUG_REFIMPL = $(AAFTOOLKIT)\AAFWinSDK\Debug\RefImpl
TOOLKIT_RELEASE_REFIMPL = $(AAFTOOLKIT)\AAFWinSDK\Release\RefImpl
OMF_LIBS = $(AAFTOOLKIT)\Omf


#
# The type of AAF SDK Configuration.
#
!if "$(CFG)"=="FULL"
TOOLKIT_TARGET_REFIMPL = $(TOOLKIT_RELEASE_REFIMPL)
OMF_DLL_NAME = "omfToolkit.dll"
!elseif "$(CFG)"=="Debug"
TOOLKIT_TARGET_REFIMPL = $(TOOLKIT_DEBUG_REFIMPL)
OMF_DLL_NAME = "omfToolkitd.dll"
!elseif "$(CFG)"=="Release"
TOOLKIT_TARGET_REFIMPL = $(TOOLKIT_RELEASE_REFIMPL)
OMF_DLL_NAME = "omfToolkit.dll"
!else
!ERROR Unknown configuration!
!endif


#
# Target directories
#
TARGET_DIRS = \
!if "$(AAFSDK)"!="."
	$(AAFSDK) \
!endif
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
	$(AAFSDK_INCLUDE)\AAFClassDefUIDs.h \
	$(AAFSDK_INCLUDE)\AAFCodecDefs.h \
	$(AAFSDK_INCLUDE)\AAFCOMPlatform.h \
	$(AAFSDK_INCLUDE)\AAFCOMPlatformTypes.h \
	$(AAFSDK_INCLUDE)\AAFContainerDefs.h \
	$(AAFSDK_INCLUDE)\AAFDataDefs.h \
	$(AAFSDK_INCLUDE)\AAFDefUIDs.h \
	$(AAFSDK_INCLUDE)\AAFEssenceFormats.h \
	$(AAFSDK_INCLUDE)\AAFFileKinds.h \
	$(AAFSDK_INCLUDE)\AAFFileMode.h \
	$(AAFSDK_INCLUDE)\AAFInterpolatorDefs.h \
	$(AAFSDK_INCLUDE)\AAFMetaDictionary.h \
	$(AAFSDK_INCLUDE)\AAFOperationCategories.h \
	$(AAFSDK_INCLUDE)\AAFOperationDefs.h \
	$(AAFSDK_INCLUDE)\AAFParameterDefs.h \
	$(AAFSDK_INCLUDE)\AAFPluginDefs.h \
	$(AAFSDK_INCLUDE)\AAFPropertyDefs.h \
	$(AAFSDK_INCLUDE)\AAFPropertyIDs.h \
	$(AAFSDK_INCLUDE)\AAFResult.h \
	$(AAFSDK_INCLUDE)\AAFSmartPointer.h \
	$(AAFSDK_INCLUDE)\AAFSmartPointerBase.h \
	$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h \
	$(AAFSDK_INCLUDE)\AAFTypeDefUIDs.h


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
	$(AAFSDK_INCLUDE)\AAF.h \
	$(AAFSDK_INCLUDE)\AAFPlugin.h \
	$(AAFSDK_INCLUDE)\AAFPluginTypes.h \
	$(AAFSDK_INCLUDE)\AAFPlugin_i.c \
	$(AAFSDK_INCLUDE)\AAFTypes.h \
	$(AAFSDK_INCLUDE)\AAF_i.c


#
# Release Target Library files that need to be copied
#
RELEASE_LIB_FILES = \
	$(AAFSDK_LIB)\aafiid.lib \
	$(AAFSDK_LIB)\aaf.lib


#
# Debug  Library files that need to be copied
#
DEBUG_LIB_FILES = \
	$(AAFSDK_LIB)\aafiidd.lib \
	$(AAFSDK_LIB)\aafd.lib


#
# Target Library files that need to be copied
#
TARGET_LIB_FILES = \
!if "$(CFG)"=="FULL"
	$(RELEASE_LIB_FILES) \
	$(DEBUG_LIB_FILES)
!elseif "$(CFG)"=="Release"
	$(RELEASE_LIB_FILES)
!else
	$(DEBUG_LIB_FILES)
!endif


#
# Release dynamic link libraries.
#
RELEASE_DLL_FILES = \
	$(AAFSDK_BIN)\aafcoapi.dll \
	$(AAFSDK_BIN)\aafintp.dll \
	$(AAFSDK_BIN)\aafpgapi.dll \
	$(AAFSDK_BIN)\omfToolkit.dll

#
# Release dynamic link libraries.
#
DEBUG_DLL_FILES = \
	$(AAFSDK_DEBUG)\aafcoapi.dll \
	$(AAFSDK_DEBUG)\aafintp.dll \
	$(AAFSDK_DEBUG)\aafpgapi.dll \
	$(AAFSDK_DEBUG)\omfToolkitd.dll


#
# Target Dynamic Link Libraries files that need to be copied
#
TARGET_DLL_FILES = \
!if "$(CFG)"=="FULL"
	$(RELEASE_DLL_FILES) \
	$(DEBUG_DLL_FILES)
!else
	$(RELEASE_DLL_FILES)
!endif


#
# Configuration files that need to be cleanup up from LASTCFG.
#
CONFIG_FILES_TO_REMOVE = \
!if "$(CFG)"=="Release"
!if "$(LASTCFG)"=="Debug"
	$(RELEASE_DLL_FILES)\
!endif
	$(DEBUG_DLL_FILES)
!elseif "$(CFG)"=="Debug"
!if "$(LASTCFG)"=="Release" || "$(LASTCFG)"=="FULL"
	$(RELEASE_DLL_FILES) \
!endif
	$(DEBUG_DLL_FILES)
!elseif "$(LASTCFG)"=="Debug"
	$(RELEASE_DLL_FILES)
!endif

#
# Configuration directories that need to be cleanup up from LASTCFG.
#
CONFIG_DIRS_TO_REMOVE = \
!if "$(CFG)"=="Release" || "$(CFG)"=="Debug"
	$(AAFSDK_DEBUG)
!endif

#
# Target files that need to be cleanup up.
#
TARGET_FILES_TO_REMOVE = \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES) \
	$(TARGET_MIDL_FILES) \
	$(RELEASE_LIB_FILES) \
	$(DEBUG_LIB_FILES) \
	$(RELEASE_DLL_FILES) \
	$(DEBUG_DLL_FILES) \
	$(AAFSDK_CFG)


#
# Directories to delete
# Note: Order is important: must have child before parent directory.
#
TARGET_DIRS_TO_REMOVE = \
	$(AAFSDK_DEBUG) \
	$(AAFSDK_BIN) \
	$(AAFSDK_HELP) \
	$(AAFSDK_INCLUDE) \
!if "$(AAFSDK)"=="."
	$(AAFSDK_LIB)
!else
	$(AAFSDK_LIB) \
	$(AAFSDK)
!endif

#
# Main dependency order for all targets
#
SDK_TARGETS = \
	$(TARGET_DIRS) \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES)\
	$(TARGET_MIDL_FILES)\
	$(TARGET_LIB_FILES)\
	$(TARGET_DLL_FILES)


#
# Dependency that builds the sdk_targets and then the current configuration
# file. Special case switching between Release and Debug builds so that
# the entire SDK does not have to be rebuilt.
#
!if "$(LASTCFG)"=="Debug" && "$(CFG)"=="Release" 
targets : cleanconfigfiles
!elseif "$(LASTCFG)"=="Release" && "$(CFG)"=="Debug" 
targets : cleanconfigfiles
!elseif "$(LASTCFG)"!="$(CFG)" && "$(LASTCFG)"!=""
targets : cleanconfigfiles
!endif
targets : $(SDK_TARGETS)
targets : $(AAFSDK_CFG)


#
# Dependencies for config file.
#
$(AAFSDK_CFG) : $(SDK_TARGETS)
	@echo # > $(AAFSDK_CFG)
	@echo # generated configuration file...DO NOT EDIT >> $(AAFSDK_CFG)
	@echo # >> $(AAFSDK_CFG)
	@echo LASTCFG=$(CFG)  >> $(AAFSDK_CFG)


#
# Dependencies and build rules to create the AAF SDK directory structure.
#
$(AAFSDK) :
	if not exist $(AAFSDK) \
	    md $(AAFSDK)

$(AAFSDK_BIN) :
	if not exist $(AAFSDK_BIN) \
	    md $(AAFSDK_BIN)

$(AAFSDK_DEBUG) : $(AAFSDK_BIN)
	if not exist $(AAFSDK_DEBUG) \
	    md $(AAFSDK_DEBUG)

$(AAFSDK_HELP) :
	if not exist $(AAFSDK_HELP) \
	    md $(AAFSDK_HELP)

$(AAFSDK_INCLUDE) :
	if not exist $(AAFSDK_INCLUDE) \
	    md $(AAFSDK_INCLUDE)

$(AAFSDK_LIB) :
	if not exist $(AAFSDK_LIB) \
	    md $(AAFSDK_LIB)


#
# Dependency and build rules for the Header targets.
#
$(AAFSDK_INCLUDE)\AAFClassDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFClassDefUIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFClassDefUIDs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFCodecDefs.h : $(TOOLKIT_INCLUDE)\AAFCodecDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFCodecDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFCOMPlatform.h : $(TOOLKIT_INCLUDE_REFAPI)\AAFCOMPlatform.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_REFAPI)\AAFCOMPlatform.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFCOMPlatformTypes.h : $(TOOLKIT_INCLUDE_REFAPI)\AAFCOMPlatformTypes.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_REFAPI)\AAFCOMPlatformTypes.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFContainerDefs.h : $(TOOLKIT_INCLUDE)\AAFContainerDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFContainerDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFDataDefs.h : $(TOOLKIT_INCLUDE)\AAFDataDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFDataDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFDefUIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFDefUIDs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFEssenceFormats.h : $(TOOLKIT_INCLUDE)\AAFEssenceFormats.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFEssenceFormats.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFFileKinds.h : $(TOOLKIT_INCLUDE)\AAFFileKinds.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFFileKinds.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFFileMode.h : $(TOOLKIT_INCLUDE)\AAFFileMode.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFFileMode.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFInterpolatorDefs.h : $(TOOLKIT_INCLUDE)\AAFInterpolatorDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFInterpolatorDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFMetaDictionary.h : $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFOperationCategories.h : $(TOOLKIT_INCLUDE)\AAFOperationCategories.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFOperationCategories.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFOperationDefs.h : $(TOOLKIT_INCLUDE)\AAFOperationDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFOperationDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFParameterDefs.h : $(TOOLKIT_INCLUDE)\AAFParameterDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFParameterDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPluginDefs.h : $(TOOLKIT_INCLUDE)\AAFPluginDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFPluginDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPropertyDefs.h : $(TOOLKIT_INCLUDE)\AAFPropertyDefs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFPropertyDefs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPropertyIDs.h : $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFResult.h : $(TOOLKIT_INCLUDE)\AAFResult.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFResult.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFSmartPointerBase.h : $(TOOLKIT_INCLUDE)\AAFSmartPointerBase.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFSmartPointerBase.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFSmartPointer.h : $(TOOLKIT_INCLUDE)\com-api\AAFSmartPointer.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\com-api\AAFSmartPointer.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h : $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFTypeDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFTypeDefUIDs.h
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE)\AAFTypeDefUIDs.h $(AAFSDK_INCLUDE)\


#
# Dependency and build rules for the IDL targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAF.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFModuleTest.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFModuleTest.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFModuleTest.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPluginTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl $(AAFSDK_INCLUDE)\

$(AAFSDK_INCLUDE)\AAFPlugin.idl : $(TOOLKIT_PLUGINS)\AAFPlugin.idl
	$(CP) $(CP_OPTS) $(TOOLKIT_PLUGINS)\AAFPlugin.idl $(AAFSDK_INCLUDE)\


#
# Dependency and build rules for the MIDL generated targets.
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
# Dependency and build rules for the release library targets.
#
$(AAFSDK_LIB)\aafiid.lib : $(TOOLKIT_RELEASE_REFIMPL)\aafiid.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_RELEASE_REFIMPL)\aafiid.lib $(AAFSDK_LIB)\

$(AAFSDK_LIB)\aaf.lib : $(TOOLKIT_RELEASE_REFIMPL)\aaf.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_RELEASE_REFIMPL)\aaf.lib $(AAFSDK_LIB)\


#
# Dependency and build rules for the debug library targets.
#
$(AAFSDK_LIB)\aafiidd.lib : $(TOOLKIT_DEBUG_REFIMPL)\aafiidd.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafiidd.lib $(AAFSDK_LIB)\

$(AAFSDK_LIB)\aafd.lib : $(TOOLKIT_DEBUG_REFIMPL)\aafd.lib
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafd.lib $(AAFSDK_LIB)\


#
# Dependency and build rules for the Release DLL targets.
#
$(AAFSDK_BIN)\aafcoapi.dll : $(TOOLKIT_TARGET_REFIMPL)\aafcoapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafcoapi.dll $(AAFSDK_BIN)\

$(AAFSDK_BIN)\aafintp.dll : $(TOOLKIT_TARGET_REFIMPL)\aafintp.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafintp.dll $(AAFSDK_BIN)\

$(AAFSDK_BIN)\aafpgapi.dll : $(TOOLKIT_TARGET_REFIMPL)\aafpgapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_TARGET_REFIMPL)\aafpgapi.dll $(AAFSDK_BIN)\

$(AAFSDK_BIN)\omfToolkit.dll : $(OMF_LIBS)\$(OMF_DLL_NAME)
	$(CP) $(CP_OPTS) $(OMF_LIBS)\$(OMF_DLL_NAME) $(AAFSDK_BIN)\
	

#
# Dependency and build rules for the Debug DLL targets.
#
$(AAFSDK_DEBUG)\aafcoapi.dll : $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafcoapi.dll $(AAFSDK_DEBUG)\

$(AAFSDK_DEBUG)\aafintp.dll : $(TOOLKIT_DEBUG_REFIMPL)\aafintp.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafintp.dll $(AAFSDK_DEBUG)\

$(AAFSDK_DEBUG)\aafpgapi.dll : $(TOOLKIT_DEBUG_REFIMPL)\aafpgapi.dll
	$(CP) $(CP_OPTS) $(TOOLKIT_DEBUG_REFIMPL)\aafpgapi.dll $(AAFSDK_DEBUG)\

$(AAFSDK_DEBUG)\omfToolkitd.dll : $(OMF_LIBS)\omfToolkitd.dll
	$(CP) $(CP_OPTS) $(OMF_LIBS)\omfToolkitd.dll $(AAFSDK_DEBUG)\

#
# Clean out all files that are specific to a particular configuration.
#
cleanconfigfiles:
!if "$(CONFIG_FILES_TO_REMOVE)" != ""
	@for %%f in ( $(CONFIG_FILES_TO_REMOVE) ) do \
	    @if exist %%f \
	        @echo $(RM) $(RM_OPTS) %%f & \
	        $(RM) $(RM_OPTS) %%f
!endif
!if "$(CONFIG_DIRS_TO_REMOVE)" != ""
	@for %%d in ( $(CONFIG_DIRS_TO_REMOVE) ) do \
	    @if exist %%d \
	        @echo rd %%d & \
	        rd %%d
!endif



#
# The cleanup rules for all targets created by this make file.
#
cleanfiles : cleanconfigfiles
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
	        @echo rd %%d & \
	        rd %%d

#
# Cleanup all target files and directories
#
clean : cleanfiles cleandirs


rebuildmsg :
	@echo Rebuilding the entire AAF SDK...

rebuildall : rebuildmsg clean targets
	@echo Done rebuilding.
