###############################################################################
#
# $Id$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################

###############################################################################
#                                                                             #
# File: PostMakeSDK.mak                                                       #
#                                                                             #
# Purpose: This is make file updates the SDK header and idl files that are    #
#          shared by all configurations (release/debug) and                   #
#          platforms (Win32/x86_64). It is a simplified version of the old    #
#          win32aafsdk.mak file that is part of the vs9 build. It is executed #
#          as a post event when the MakeSDK project is built. It has no       #
#          dependency on the configuration or platform.
#                                                                             #
# Supported variables:                                                        #
#   AAFSDK - variable that will contain the location of the aafsdk.           #
#     WARNING: This directory will be removed during the clean.               #
#                                                                             #
#   AAFTOOLKIT - the directory that contains the AAF-toolkit                  #
#
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
# 27-JUL-2000 : transdel adding support for "aafext" plugin directory         #
# 20-OCT-2000 : transdel added new platform type include                      #
# 16-FEB-2001 : transdel added missing AAFExtEnum.h and AAFFileSignatures.h   #
# 07-MAR-2001 : tjb copy OMF toolkit DLL once only.                           #
# 19-MAR-2001 : akharkev Use update.cmd when copying over checked in files.   #
# 26-DEC-2001 : whoopy include AAFSDKBuild.h in files to be updated for SDK   #
# 16-DEC-2005 : ajakowpak include AAFOPDefs.h in files to be updated for SDK. #
# 22-MAY-2012:  jptrainor: Adapted the original win32aafsdk.mak for use with  #
#               VS10. Substantially simplified. Now only responsible for      # 
#               moving the header files that are shared by all configurations #
#               and platforms.                                                #
###############################################################################

all : targets


# The command and options for standard copy operations.
CP = copy
CP_OPTS = /v /b

#
# The command and options for standard remove operations.
#
RM = del
RM_OPTS = /f /q

UPDATE = update.cmd

#
# The path to the AAF-toolkit
#
!if "$(AAFTOOLKIT)"==""
AAFTOOLKIT = ..\..
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
# Directory structure for the AAF SDK
#
AAFSDK_INCLUDE = $(AAFSDK)\include

#
# Toolkit directories from the AAF Development tree
#
TOOLKIT_INCLUDE = $(AAFTOOLKIT)\ref-impl\include
TOOLKIT_INCLUDE_COMAPI = $(TOOLKIT_INCLUDE)\com-api
TOOLKIT_INCLUDE_REFAPI = $(TOOLKIT_INCLUDE)\ref-api

#
# Target directories
#
TARGET_DIRS = $(AAFSDK_INCLUDE)

#
# Target Header files that need to be copied
#
TARGET_H_FILES = \
	$(AAFSDK_INCLUDE)\AAFClassDefUIDs.h \
	$(AAFSDK_INCLUDE)\AAFCodecDefs.h \
	$(AAFSDK_INCLUDE)\AAFCOMPlatform.h \
	$(AAFSDK_INCLUDE)\AAFCOMPlatformTypes.h \
	$(AAFSDK_INCLUDE)\AAFCompressionDefs.h \
	$(AAFSDK_INCLUDE)\AAFContainerDefs.h \
	$(AAFSDK_INCLUDE)\AAFDataDefs.h \
	$(AAFSDK_INCLUDE)\AAFDefUIDs.h \
	$(AAFSDK_INCLUDE)\AAFEssenceFormats.h \
	$(AAFSDK_INCLUDE)\AAFExtEnum.h \
	$(AAFSDK_INCLUDE)\AAFFileKinds.h \
	$(AAFSDK_INCLUDE)\AAFFileMode.h \
	$(AAFSDK_INCLUDE)\AAFInterpolatorDefs.h \
	$(AAFSDK_INCLUDE)\AAFMetaDictionary.h \
	$(AAFSDK_INCLUDE)\AAFOPDefs.h \
	$(AAFSDK_INCLUDE)\AAFOperationCategories.h \
	$(AAFSDK_INCLUDE)\AAFOperationDefs.h \
	$(AAFSDK_INCLUDE)\AAFParameterDefs.h \
	$(AAFSDK_INCLUDE)\AAFPlatform.h \
	$(AAFSDK_INCLUDE)\AAFPluginDefs.h \
	$(AAFSDK_INCLUDE)\AAFPropertyDefs.h \
	$(AAFSDK_INCLUDE)\AAFPropertyIDs.h \
	$(AAFSDK_INCLUDE)\AAFResult.h \
	$(AAFSDK_INCLUDE)\AAFSmartPointer.h \
	$(AAFSDK_INCLUDE)\AAFSmartPointer2.h \
	$(AAFSDK_INCLUDE)\AAFSmartPointerBase.h \
	$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h \
	$(AAFSDK_INCLUDE)\AAFTypeDefUIDs.h \
	$(AAFSDK_INCLUDE)\AAFSDKBuild.h


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
# Target files that need to be cleanup up.
#
TARGET_FILES_TO_REMOVE = \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES) \
	$(TARGET_MIDL_FILES)


#
# Directories to delete
# Note: Order is important: must have child before parent directory.
#
TARGET_DIRS_TO_REMOVE = $(AAFSDK_INCLUDE)

#
# Main dependency order for all targets
#
SDK_TARGETS = \
	$(TARGET_DIRS) \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES)\
	$(TARGET_MIDL_FILES)

#
# Dependencies and build rules to create the AAF SDK directory structure.
#

targets: $(SDK_TARGETS)

$(AAFSDK_INCLUDE) :
	if not exist $(AAFSDK_INCLUDE) \
	    md $(AAFSDK_INCLUDE)

#
# Dependency and build rules for the Header targets.
#
$(AAFSDK_INCLUDE)\AAFClassDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFClassDefUIDs.h
    $(UPDATE) $(TOOLKIT_INCLUDE)\AAFClassDefUIDs.h $@

$(AAFSDK_INCLUDE)\AAFCodecDefs.h : $(TOOLKIT_INCLUDE)\AAFCodecDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFCodecDefs.h $@

$(AAFSDK_INCLUDE)\AAFCOMPlatform.h : $(TOOLKIT_INCLUDE)\AAFCOMPlatform.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFCOMPlatform.h $@

$(AAFSDK_INCLUDE)\AAFCOMPlatformTypes.h : $(TOOLKIT_INCLUDE)\AAFCOMPlatformTypes.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFCOMPlatformTypes.h $@

$(AAFSDK_INCLUDE)\AAFCompressionDefs.h : $(TOOLKIT_INCLUDE)\AAFCompressionDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFCompressionDefs.h $@

$(AAFSDK_INCLUDE)\AAFContainerDefs.h : $(TOOLKIT_INCLUDE)\AAFContainerDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFContainerDefs.h $@

$(AAFSDK_INCLUDE)\AAFDataDefs.h : $(TOOLKIT_INCLUDE)\AAFDataDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFDataDefs.h $@

$(AAFSDK_INCLUDE)\AAFDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFDefUIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFDefUIDs.h $@

$(AAFSDK_INCLUDE)\AAFEssenceFormats.h : $(TOOLKIT_INCLUDE)\AAFEssenceFormats.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFEssenceFormats.h $@

$(AAFSDK_INCLUDE)\AAFExtEnum.h : $(TOOLKIT_INCLUDE)\AAFExtEnum.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFExtEnum.h $@

$(AAFSDK_INCLUDE)\AAFFileKinds.h : $(TOOLKIT_INCLUDE)\AAFFileKinds.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFFileKinds.h $@

$(AAFSDK_INCLUDE)\AAFFileMode.h : $(TOOLKIT_INCLUDE)\AAFFileMode.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFFileMode.h $@

$(AAFSDK_INCLUDE)\AAFInterpolatorDefs.h : $(TOOLKIT_INCLUDE)\AAFInterpolatorDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFInterpolatorDefs.h $@

$(AAFSDK_INCLUDE)\AAFMetaDictionary.h : $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFMetaDictionary.h $@

$(AAFSDK_INCLUDE)\AAFOPDefs.h : $(TOOLKIT_INCLUDE)\AAFOPDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFOPDefs.h $@

$(AAFSDK_INCLUDE)\AAFOperationCategories.h : $(TOOLKIT_INCLUDE)\AAFOperationCategories.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFOperationCategories.h $@

$(AAFSDK_INCLUDE)\AAFOperationDefs.h : $(TOOLKIT_INCLUDE)\AAFOperationDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFOperationDefs.h $@

$(AAFSDK_INCLUDE)\AAFParameterDefs.h : $(TOOLKIT_INCLUDE)\AAFParameterDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFParameterDefs.h $@

$(AAFSDK_INCLUDE)\AAFPlatform.h : $(TOOLKIT_INCLUDE)\AAFPlatform.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFPlatform.h $@

$(AAFSDK_INCLUDE)\AAFPluginDefs.h : $(TOOLKIT_INCLUDE)\AAFPluginDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFPluginDefs.h $@

$(AAFSDK_INCLUDE)\AAFPropertyDefs.h : $(TOOLKIT_INCLUDE)\AAFPropertyDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFPropertyDefs.h $@

$(AAFSDK_INCLUDE)\AAFPropertyIDs.h : $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFPropertyIDs.h $@

$(AAFSDK_INCLUDE)\AAFResult.h : $(TOOLKIT_INCLUDE)\AAFResult.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFResult.h $@

$(AAFSDK_INCLUDE)\AAFSmartPointerBase.h : $(TOOLKIT_INCLUDE)\AAFSmartPointerBase.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFSmartPointerBase.h $@

$(AAFSDK_INCLUDE)\AAFSmartPointer.h : $(TOOLKIT_INCLUDE)\AAFSmartPointer.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFSmartPointer.h $@
	
$(AAFSDK_INCLUDE)\AAFSmartPointer2.h : $(TOOLKIT_INCLUDE)\AAFSmartPointer2.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFSmartPointer2.h $@

$(AAFSDK_INCLUDE)\AAFStoredObjectIDs.h : $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFStoredObjectIDs.h $@

$(AAFSDK_INCLUDE)\AAFTypeDefUIDs.h : $(TOOLKIT_INCLUDE)\AAFTypeDefUIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFTypeDefUIDs.h $@

$(AAFSDK_INCLUDE)\AAFSDKBuild.h : $(TOOLKIT_INCLUDE)\AAFSDKBuild.h
	$(UPDATE) $(TOOLKIT_INCLUDE)\AAFSDKBuild.h $@

#
# Dependency and build rules for the IDL targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.idl $@

$(AAFSDK_INCLUDE)\AAF.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAF.idl $@

$(AAFSDK_INCLUDE)\AAFModuleTest.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFModuleTest.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFModuleTest.idl $@

$(AAFSDK_INCLUDE)\AAFPluginTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.idl $@

$(AAFSDK_INCLUDE)\AAFPlugin.idl : $(TOOLKIT_INCLUDE_COMAPI)\AAFPlugin.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFPlugin.idl $@


#
# Dependency and build rules for the MIDL generated targets.
#
$(AAFSDK_INCLUDE)\AAFTypes.h : $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.h
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFTypes.h $@

$(AAFSDK_INCLUDE)\AAF.h : $(TOOLKIT_INCLUDE_COMAPI)\AAF.h
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAF.h $@

$(AAFSDK_INCLUDE)\AAF_i.c : $(TOOLKIT_INCLUDE_COMAPI)\AAF_i.c
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAF_i.c $@

$(AAFSDK_INCLUDE)\AAFPluginTypes.h : $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.h
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFPluginTypes.h $@

$(AAFSDK_INCLUDE)\AAFPlugin.h : $(TOOLKIT_INCLUDE_COMAPI)\AAFPlugin.h
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFPlugin.h $@

$(AAFSDK_INCLUDE)\AAFPlugin_i.c : $(TOOLKIT_INCLUDE_COMAPI)\AAFPlugin_i.c
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)\AAFPlugin_i.c $@


#
# The cleanup rules for all targets created by this make file.
#
cleanfiles :
	@for %%f in ( $(TARGET_FILES_TO_REMOVE) ) do \
	    @if exist %%f \
	        @echo $(RM) $(RM_OPTS) %%f & \
	        $(RM) $(RM_OPTS) %%f

#
# Cleanup all target files and directories
#
clean : cleanfiles

rebuildmsg :
	@echo Rebuilding the entire AAF SDK...

rebuildall : rebuildmsg clean targets
	@echo Done rebuilding.
