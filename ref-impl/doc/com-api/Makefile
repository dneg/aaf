###############################################################################
#
# $Id$ $Name$
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
# The Original Code of this file is Copyright 1998-2005, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
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

# Only used for building the 'chm' (Windows Help file)
HTMLHELP_WORKSHOP = C:/Program\ Files/HTML\ Help\ Workshop

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
	echo "/**" > $(OUTPUT_DIR)/AAF.idl
	echo " * @mainpage notitle" >> $(OUTPUT_DIR)/AAF.idl
	echo " * @htmlinclude frontCover.html" >> $(OUTPUT_DIR)/AAF.idl
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


$(PLUGIN_HTML_MAN): $(COMAPI_PATH)/AAFPlugin.idl \
                    $(COMAPI_PATH)/AAFPluginTypes.idl \
                    $(AAF_TAG_FILE) \
                    doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(COMAPI_PATH)/AAFPlugin.idl $(COMAPI_PATH)/AAFPluginTypes.idl ; \
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

$(PLUGIN_HTML_HELP_PROJ): $(COMAPI_PATH)/AAFPlugin.idl \
                          $(COMAPI_PATH)/AAFPluginTypes.idl \
                          $(AAF_TAG_FILE) \
                          doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(COMAPI_PATH)/AAFPlugin.idl $(COMAPI_PATH)/AAFPluginTypes.idl ; \
	  echo OUTPUT_DIRECTORY=$(PLUGIN_HTML_HELP_DIR) ; \
	  echo PROJECT_NAME=\"$(PLUGIN_MAN_TITLE)\" ; \
	  echo GENERATE_HTMLHELP=YES ; \
	  echo DISABLE_INDEX=YES ; \
	  echo CHM_FILE=$(PLUGIN_HTML_HELP_MAN_FILE) \
	) | $(DOXYGEN) -

$(AAF_HTML_HELP_MAN): $(HTMLHELP_WORKSHOP)/hhc \
                      $(AAF_HTML_HELP_PROJ)
	# 'hhc' exits non-zero, so we have to ignore the exit status...
	-$(HTMLHELP_WORKSHOP)/hhc $(AAF_HTML_HELP_PROJ)
	mv $(AAF_HTML_HELP_DIR)/$(AAF_HTML_HELP_MAN_FILE) $(AAF_HTML_HELP_MAN)

$(PLUGIN_HTML_HELP_MAN): $(HTMLHELP_WORKSHOP)/hhc \
                         $(PLUGIN_HTML_HELP_PROJ)
	# 'hhc' exits non-zero, so we have to ignore the exit status...
	-$(HTMLHELP_WORKSHOP)/hhc $(PLUGIN_HTML_HELP_PROJ)
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
	



