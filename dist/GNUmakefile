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
###############################################################################

# This make file is normally called from the top level makefile
# (../GNUmakefile) using the "dist" target.

# Windows Notes:
#
# The top level make target does not currently work under Win2K.
# Instead, run the following from this directory:
#
#    make AAFPLATFORM=Win AAFSDK=../AAFWinSDK release-dist
#    make AAFPLATFORM=Win AAFSDK=../AAFWinSDK sdk-dist
#
# It was tested using Cygwin.

# Mac Notes:
#
# The top level make target does not currently work under Max (OS 9, OS X).
# Instead, run the following from this directory:
#
#   mkdir -p ../AAFMacSDK/Dist
#   cp release-files.mac ../AAFMacSDK/Dist/release-files
#  ./CreateDistribution.bash release Dist/release-files Dist/aaf_release ../AAFMacSDK Mac ../ref-impl/include/AAFSDKBuild.h
#
#   mkdir -p ../AAFMacSDK/Dist
#   cp sdk-files.mac ../AAFMacSDK/Dist/sdk-files
#  ./CreateDistribution.bash sdk Dist/sdk-files Dist/aaf_sdk ../AAFMacSDK Mac ../ref-impl/include/AAFSDKBuild.h
# 
#   The folowing should work, when/if a pdefs-Mac.mk file appears in the build directory.
#
#    make AAFPLATFORM=Mac AAFSDK=../AAFMacSDK release-dist
#    make AAFPLATFORM=Mac AAFSDK=../AAFMacSDK sdk-dist
# 
# Pending some form of top-level-build resolution, the above commands
# can be executed using "make mac"
#
# Note, as of V101 the SDK only supports OS9.  The OS X developers
# toolkit (available from Apple) includes GNU make, and other GNU
# tools.  Bash must be compiled It was tested using Cygwin.  The
# CreateDistribution.bash script is quite simple, hence, sh should
# work in lieu of bash.  The script can be executed from OS X, even if
# the rest of the build is done with OS 9.

AAFBASE ?= ..

#
# Common definitions
#
include $(AAFBASE)/build/common.mk

ifeq ($(AAFPLATFORM),Win)
RELEASE_FILES_SRC = ./release-files.win
SDK_FILES_SRC = ./sdk-files.win
else
ifeq ($(AAFPLATFORM),Mac)
RELEASE_FILES_SRC = ./release-files.Mac
SDK_FILES_SRC = ./sdk-files.mac
else
RELEASE_FILES_SRC = ./release-files.unix
SDK_FILES_SRC = ./sdk-files.unix
endif
endif

DST_DIR = $(AAFSDK)/Dist
RELEASE_FILES_DST = $(DST_DIR)/release-files
SDK_FILES_DST = $(DST_DIR)/sdk-files

AAFSDKBUILD_H = $(AAFBASE)/ref-impl/include/AAFSDKBuild.h

.PHONY : release-files
release-files :
	$(MKDIR) $(AAFSDK)/Dist
	$(CP) $(RELEASE_FILES_SRC) $(RELEASE_FILES_DST)

.PHONY : sdk-files
sdk-files :
	$(MKDIR) $(AAFSDK)/Dist
	$(CP) $(SDK_FILES_SRC) $(SDK_FILES_DST)

.PHONY : release-dist
release-dist : release-files
	./CreateDistribution.bash release Dist/release-files Dist/aaf_release $(AAFSDK) $(AAFPLATFORM) $(AAFSDKBUILD_H)

.PHONY : sdk-dist
sdk-dist : sdk-files
	./CreateDistribution.bash sdk Dist/sdk-files Dist/aaf_sdk $(AAFSDK) $(AAFPLATFORM) $(AAFSDKBUILD_H)


.PHONY : mac
mac :
	@echo ============== Creating release distribution ==============
	mkdir -p ../AAFMacSDK/Dist
	cp release-files.mac ../AAFMacSDK/Dist/release-files
	./CreateDistribution.bash release Dist/release-files Dist/aaf_release ../AAFMacSDK Mac ../ref-impl/include/AAFSDKBuild.h
	@echo ============== Creating sdk distribution ==============
	mkdir -p ../AAFMacSDK/Dist
	cp sdk-files.mac ../AAFMacSDK/Dist/sdk-files
	./CreateDistribution.bash sdk Dist/sdk-files Dist/aaf_sdk ../AAFMacSDK Mac ../ref-impl/include/AAFSDKBuild.h


.PHONY : clean
clean : realclean

.PHONY : realclean
realclean :
	$(RMDIR) $(DST_DIR)

