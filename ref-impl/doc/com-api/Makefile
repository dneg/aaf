###############################################################################
#
# $Id$ $Name$
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

# Makefile (assumes GnuMake under CYGWIN)
# builds AAF COM API documentation using doxygen
# and Microsoft HTMLHelp Compiler


# Various binaries which might be in non-conventional locations
DOXYGEN = doxygen
ZIP = zip
TAR = tar

# Directory for output.  Relative paths are relative to the directory
# containing this Makefile.
OUTPUT_DIR = output


#######################################################
# These variables are largely based on the above set.
# These should not need to be changed.

# Path from this directory to the COMAPI documents
COMAPI_PATH = ../../include/com-api

AAF_TAG_FILE = $(AAF_HTML_MAN_DIR)/aaf.tag

AAF_HTML_MAN_TITLE = "AAF COM API"
PLUGIN_HTML_MAN_TITLE = "AAF Plug-In API"

# The following set is just a collection of output file names
# and paths.  All are relative to OUTPUT_DIR
AAF_HTML_MAN_COMP = aafapiman
AAF_HTML_MAN_DIR = $(OUTPUT_DIR)/$(AAF_HTML_MAN_COMP)
AAF_HTML_MAN = $(AAF_HTML_MAN_DIR)/index.html

AAF_HTML_MAN_ZIP_FILE = aafapiman.zip
AAF_HTML_MAN_ZIP = $(OUTPUT_DIR)/$(AAF_HTML_MAN_ZIP_FILE)

AAF_HTML_MAN_TGZ_FILE = aafapiman.tgz
AAF_HTML_MAN_TGZ = $(OUTPUT_DIR)/$(AAF_HTML_MAN_TGZ_FILE)

AAF_HTML_HELP_DIR = $(OUTPUT_DIR)/aafapichm
AAF_HTML_HELP_PROJ = $(AAF_HTML_HELP_DIR)/index.hhp

AAF_HTML_HELP_MAN_FILE = aafapi.chm
AAF_HTML_HELP_MAN = $(OUTPUT_DIR)/$(AAF_HTML_HELP_MAN_FILE)


PLUGIN_HTML_MAN_COMP = pluginapiman
PLUGIN_HTML_MAN_DIR = $(OUTPUT_DIR)/$(PLUGIN_HTML_MAN_COMP)
PLUGIN_HTML_MAN = $(PLUGIN_HTML_MAN_DIR)/index.html

PLUGIN_HTML_MAN_ZIP_FILE = pluginapiman.zip
PLUGIN_HTML_MAN_ZIP = $(OUTPUT_DIR)/$(PLUGIN_HTML_MAN_ZIP_FILE)
PLUGIN_HTML_MAN_TGZ_FILE = pluginapiman.tgz
PLUGIN_HTML_MAN_TGZ = $(OUTPUT_DIR)/$(PLUGIN_HTML_MAN_TGZ_FILE)
PLUGIN_HTML_HELP_MAN = $(OUTPUT_DIR)/pluginapi.chm

PLUGIN_HTML_HELP_DIR = $(OUTPUT_DIR)/pluginapichm
PLUGIN_HTML_HELP_PROJ = $(PLUGIN_HTML_HELP_DIR)/index.hhp

PLUGIN_HTML_HELP_MAN_FILE = pluginapi.chm
PLUGIN_HTML_HELP_MAN = $(OUTPUT_DIR)/$(PLUGIN_HTML_HELP_MAN_FILE)




#####################################################################
## Top-level targets

.PHONY: doc doczip htmldoc htmlhelp windoc dirs clean usage

usage:
	@echo "Documentation Targets:"
	@echo "    doc		- Creates HTML docs."
	@echo "    doczip	- Creates HTML docs and zip/tgz's of such."
	@echo "    windoc	- Creates Microsoft HTML Help (.chm) files."
	@echo ""
	@echo "All targets create two sets of docs: AAF COM API and Plug-In API"

doc: dirs htmldoc
doczip: dirs htmldoczip
windoc: dirs htmlhelp
clean:
	rm -rf $(OUTPUT_DIR)



#####################################################################
## Top-level utility targets. Do not call directly


htmldoc: $(AAF_HTML_MAN) $(PLUGIN_HTML_MAN) \
         $(AAF_HTML_MAN_DIR)/aafDSlogo.gif \
         $(PLUGIN_HTML_MAN_DIR)/aafDSlogo.gif

htmldoczip: htmldoc $(AAF_HTML_MAN_ZIP) $(AAF_HTML_MAN_TGZ) \
            $(PLUGIN_HTML_MAN_ZIP) $(PLUGIN_HTML_MAN_TGZ)

htmlhelp: $(AAF_HTML_HELP_DIR)/aafDSlogo.gif \
	$(PLUGIN_HTML_HELP_DIR)/aafDSlogo.gif \
	$(AAF_HTML_HELP_MAN) $(PLUGIN_HTML_HELP_MAN)



##############################################################
#### Output directory creation targets

dirs: $(OUTPUT_DIR) $(AAF_HTML_MAN_DIR) $(PLUGIN_HTML_MAN_DIR) \
	$(AAF_HTML_HELP_DIR) $(PLUGIN_HTML_HELP_DIR)

$(AAF_HTML_MAN_DIR) $(PLUGIN_HTML_MAN_DIR) \
$(AAF_HTML_HELP_DIR) $(PLUGIN_HTML_HELP_DIR) \
$(OUTPUT_DIR):
	if [ ! -d $@ ]; then \
		mkdir -p $@; \
	fi



##############################################################
####  HTML doc generating targets


$(OUTPUT_DIR)/AAF.idl: $(COMAPI_PATH)/AAF.idl
	sed -e s/ROLE/Application/g -e s/APIIDL/AAF/g -e s/TYPESIDL/AAFTypes/g -e s/DOCNAME/aafapi/g frontCover.html \
		> $(OUTPUT_DIR)/aafapiFrontCover.html
	echo "/**" > $(OUTPUT_DIR)/AAF.idl
	echo " * @mainpage notitle" >> $(OUTPUT_DIR)/AAF.idl
	echo " * @htmlinclude $(OUTPUT_DIR)/aafapiFrontCover.html" >> $(OUTPUT_DIR)/AAF.idl
	echo " */" >> $(OUTPUT_DIR)/AAF.idl
	cat $(COMAPI_PATH)/AAF.idl >> $(OUTPUT_DIR)/AAF.idl

$(AAF_TAG_FILE) \
$(AAF_HTML_MAN): $(OUTPUT_DIR)/AAF.idl \
                 $(COMAPI_PATH)/AAFTypes.idl \
		frontCover.html doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(OUTPUT_DIR)/AAF.idl $(COMAPI_PATH)/AAFTypes.idl ; \
	  echo OUTPUT_DIRECTORY=$(AAF_HTML_MAN_DIR) ; \
	  echo GENERATE_TAGFILE=$(AAF_TAG_FILE) ; \
	  echo PROJECT_NAME=\"$(AAF_HTML_MAN_TITLE)\" \
	) | $(DOXYGEN) -


$(OUTPUT_DIR)/AAFPlugin.idl: $(COMAPI_PATH)/AAFPlugin.idl 
	sed -e s/ROLE/Plugin/g -e s/APIIDL/AAFPlugin/g -e s/TYPESIDL/AAFPluginTypes/g -e s/DOCNAME/pluginapi/g frontCover.html \
		> $(OUTPUT_DIR)/pluginFrontCover.html
	echo "/**" > $(OUTPUT_DIR)/AAFPlugin.idl
	echo " * @mainpage notitle" >> $(OUTPUT_DIR)/AAFPlugin.idl
	echo " * @htmlinclude $(OUTPUT_DIR)/pluginFrontCover.html" >> $(OUTPUT_DIR)/AAFPlugin.idl
	echo " */" >> $(OUTPUT_DIR)/AAFPlugin.idl
	cat $(COMAPI_PATH)/AAFPlugin.idl >> $(OUTPUT_DIR)/AAFPlugin.idl

$(PLUGIN_HTML_MAN): $(OUTPUT_DIR)/AAFPlugin.idl \
                    $(COMAPI_PATH)/AAFPluginTypes.idl \
                    $(AAF_TAG_FILE) \
                    frontCover.html doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(OUTPUT_DIR)/AAFPlugin.idl $(COMAPI_PATH)/AAFPluginTypes.idl ; \
	  echo OUTPUT_DIRECTORY=$(PLUGIN_HTML_MAN_DIR) ; \
	  echo TAGFILES=\"$(AAF_TAG_FILE)=../$(AAF_HTML_MAN_COMP)\" ; \
	  echo PROJECT_NAME=\"$(PLUGIN_MAN_TITLE)\" \
	) | $(DOXYGEN) -

##############################################################
####  Windows HTML Help doc generating targets
####  These are largely the same as the HTML equivalents except it does
####  not do cross-linking of the doc sets (HTML Help doesn't handle this nicely)

$(AAF_HTML_HELP_PROJ): $(OUTPUT_DIR)/AAF.idl \
                       $(COMAPI_PATH)/AAFTypes.idl \
                       frontCover.html doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(OUTPUT_DIR)/AAF.idl $(COMAPI_PATH)/AAFTypes.idl ; \
	  echo OUTPUT_DIRECTORY=$(AAF_HTML_HELP_DIR) ; \
	  echo PROJECT_NAME=\"$(AAF_HTML_MAN_TITLE)AAF COM API\" ; \
	  echo GENERATE_HTMLHELP=YES ; \
	  echo DISABLE_INDEX=YES ; \
	  echo CHM_FILE=$(AAF_HTML_HELP_MAN_FILE) \
	) | $(DOXYGEN) -

$(PLUGIN_HTML_HELP_PROJ): $(OUTPUT_DIR)/AAFPlugin.idl \
                          $(COMAPI_PATH)/AAFPluginTypes.idl \
                          $(AAF_TAG_FILE) \
                          doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(OUTPUT_DIR)/AAFPlugin.idl $(COMAPI_PATH)/AAFPluginTypes.idl ; \
	  echo OUTPUT_DIRECTORY=$(PLUGIN_HTML_HELP_DIR) ; \
	  echo PROJECT_NAME=\"$(PLUGIN_MAN_TITLE)\" ; \
	  echo GENERATE_HTMLHELP=YES ; \
	  echo DISABLE_INDEX=YES ; \
	  echo CHM_FILE=$(PLUGIN_HTML_HELP_MAN_FILE) \
	) | $(DOXYGEN) -

# hhc.exe is required to build the MS Compiled Help File .chm files,
# and must be in the PATH. Normally it installed in the following
# location: C:/Program\ Files/HTML\ Help\ Workshop/hhc.exe.  Note that
# hhc.exe command returns a non-zero exit status even when it
# generates the chm file successfully. This is the reason from the
# leading "-" character.

$(AAF_HTML_HELP_MAN): $(AAF_HTML_HELP_PROJ)
	-hhc $(AAF_HTML_HELP_PROJ)
	mv $(AAF_HTML_HELP_DIR)/$(AAF_HTML_HELP_MAN_FILE) $(AAF_HTML_HELP_MAN)

$(PLUGIN_HTML_HELP_MAN): $(PLUGIN_HTML_HELP_PROJ)
	-hhc $(PLUGIN_HTML_HELP_PROJ)
	mv $(PLUGIN_HTML_HELP_DIR)/$(PLUGIN_HTML_HELP_MAN_FILE) $(PLUGIN_HTML_HELP_MAN)



##############################################################
####  Document archive generation

$(AAF_HTML_MAN_ZIP): $(AAF_HTML_MAN)
	rm -f $(AAF_HTML_MAN_ZIP)
	cd $(OUTPUT_DIR) && \
		$(ZIP) -9rq $(AAF_HTML_MAN_ZIP_FILE) $(AAF_HTML_MAN_COMP)

$(AAF_HTML_MAN_TGZ): $(AAF_HTML_MAN)
	rm -f $(AAF_HTML_MAN_TGZ)
	cd $(OUTPUT_DIR) && \
		$(TAR) cfz $(AAF_HTML_MAN_TGZ_FILE) $(AAF_HTML_MAN_COMP)

$(PLUGIN_HTML_MAN_ZIP): $(PLUGIN_HTML_MAN)
	rm -f $(PLUGIN_HTML_MAN_ZIP)
	cd $(OUTPUT_DIR) && \
		$(ZIP) -9rq $(PLUGIN_HTML_MAN_ZIP_FILE) $(PLUGIN_HTML_MAN_COMP)

$(PLUGIN_HTML_MAN_TGZ): $(PLUGIN_HTML_MAN)
	rm -f $(PLUGIN_HTML_MAN_TGZ)
	cd $(OUTPUT_DIR) && \
		$(TAR) cfz $(PLUGIN_HTML_MAN_TGZ_FILE) $(PLUGIN_HTML_MAN_COMP)


##############################################################
#### Misc. targets

$(AAF_HTML_MAN_DIR)/aafDSlogo.gif \
$(PLUGIN_HTML_MAN_DIR)/aafDSlogo.gif \
$(AAF_HTML_HELP_DIR)/aafDSlogo.gif \
$(PLUGIN_HTML_HELP_DIR)/aafDSlogo.gif: aafDSlogo.gif
	cp aafDSlogo.gif $@




