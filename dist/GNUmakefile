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

# The top level make target does not currently work under Win2K.
# Instead, run the following from this directory:
#
#    make AAFPLATFORM=Win AAFSDK=../AAFWinSDK release-dist
#    make AAFPLATFORM=Win AAFSDK=../AAFWinSDK sdk-dist
#
# It was tested using Cygwin.

AAFBASE ?= ..

#
# Common definitions
#
include $(AAFBASE)/build/common.mk

ifeq ($(AAFPLATFORM),Win)
RELEASE_FILES_SRC = ./release-files.win
SDK_FILES_SRC = ./sdk-files.win
else
RELEASE_FILES_SRC = ./release-files.unix
SDK_FILES_SRC = ./sdk-files.unix
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
	./CreateDistribution.bash release Dist/sdk-files Dist/aaf_sdk $(AAFSDK) $(AAFPLATFORM) $(AAFSDKBUILD_H)

.PHONY : clean
clean : realclean

.PHONY : realclean
realclean :
	$(RMDIR) $(DST_DIR)

