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

# XXX - Not yet used
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

# The following set is just a collection of output file names
# and paths.  All are relative to OUTPUT_DIR
AAF_HTML_MAN_DIR_COMP = aafapiman
AAF_HTML_MAN_DIR = $(OUTPUT_DIR)/$(AAF_HTML_MAN_DIR_COMP)
AAF_HTML_MAN = $(AAF_HTML_MAN_DIR)/index.html

AAF_HTML_MAN_ZIP_FILE = aafapiman.zip
AAF_HTML_MAN_ZIP = $(OUTPUT_DIR)/$(AAF_HTML_MAN_ZIP_FILE)

AAF_HTML_MAN_TGZ_FILE = aafapiman.tgz
AAF_HTML_MAN_TGZ = $(OUTPUT_DIR)/$(AAF_HTML_MAN_TGZ_FILE)

AAF_HTML_HELP_MAN = $(OUTPUT_DIR)/aafapi.chm

PLUGIN_HTML_MAN_COMP = pluginapiman
PLUGIN_HTML_MAN_DIR = $(OUTPUT_DIR)/$(PLUGIN_HTML_MAN_COMP)
PLUGIN_HTML_MAN = $(PLUGIN_HTML_MAN_DIR)/index.html

PLUGIN_HTML_MAN_ZIP_FILE = pluginapiman.zip
PLUGIN_HTML_MAN_ZIP = $(OUTPUT_DIR)/$(PLUGIN_HTML_MAN_ZIP_FILE)
PLUGIN_HTML_MAN_TGZ_FILE = pluginapiman.tgz
PLUGIN_HTML_MAN_TGZ = $(OUTPUT_DIR)/$(PLUGIN_HTML_MAN_TGZ_FILE)
PLUGIN_HTML_HELP_MAN = $(OUTPUT_DIR)/pluginapi.chm



#####################################################################
## Top-level targets

.PHONY: doc doczip htmldoc dirs clean

doc: dirs htmldoc
doczip: dirs htmldoczip

htmldoc: $(AAF_HTML_MAN) $(PLUGIN_HTML_MAN) \
         $(AAF_HTML_MAN_DIR)/aafDSlogo.gif \
         $(PLUGIN_HTML_MAN_DIR)/aafDSlogo.gif

htmldoczip: htmldoc $(AAF_HTML_MAN_ZIP) $(AAF_HTML_MAN_TGZ) \
            $(PLUGIN_HTML_MAN_ZIP) $(PLUGIN_HTML_MAN_TGZ)

clean:
	rm -rf $(OUTPUT_DIR)

##############################################################
#### Output directory creation targets

dirs: $(OUTPUT_DIR) $(AAF_HTML_MAN_DIR) $(PLUGIN_HTML_MAN_DIR)

$(AAF_HTML_MAN_DIR) $(PLUGIN_HTML_MAN_DIR):
	if [ ! -d $@ ]; then \
		mkdir -p $@; \
	fi

$(OUTPUT_DIR) :
	if [ ! -d $@ ] ; then mkdir -p $@ ; fi




##############################################################
####  Main doc generating targets


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
	  echo PROJECT_NAME=\"AAF COM API\" \
	) | $(DOXYGEN) -


$(PLUGIN_HTML_MAN): $(COMAPI_PATH)/AAFPlugin.idl \
                    $(COMAPI_PATH)/AAFPluginTypes.idl \
                    $(AAF_TAG_FILE) \
                    doxygen.cfg
	( \
	  cat doxygen.cfg ; \
	  echo INPUT=$(COMAPI_PATH)/AAFPlugin.idl $(COMAPI_PATH)/AAFPluginTypes.idl ; \
	  echo OUTPUT_DIRECTORY=$(PLUGIN_HTML_MAN_DIR) ; \
	  echo TAGFILES=\"$(AAF_TAG_FILE)=../$(AAF_HTML_MAN_DIR_COMP)\" ; \
	  echo PROJECT_NAME=\"AAF Plug-In API\" \
	) | $(DOXYGEN) -



### XXX - Not yet working!

$(HTML_HELP_MAN): $(HTMLHELP_WORKSHOP)/hhc $(HTML_HELP_MAN_PATH) $(HTML_HELP_MAN_PATH)/frontCover.html $(HTML_MAN_PATH)/DigImgAreas.gif AAFTypes.h AAFnospace.idl 
	# XXX Try generating a file for HTML Help workshop on the fly and run it.
	- $(GENERATOR) -v refHHDocFromIDL.djt 2> errorsHH.txt
#   Just echo the Errors from the logfile
	grep -v Warning errorsHH.txt
#   Now compile again without CHI file
	cp $(HTML_HELP_MAN_PATH)/aafapi.hhp $(HTML_HELP_MAN_PATH)/aafapi.tmp
	sed 's/Create CHI file=yes/Create CHI file=No/' $(HTML_HELP_MAN_PATH)/aafapi.tmp > $(HTML_HELP_MAN_PATH)/aafapi.hhp
	- $(HTMLHELP_WORKSHOP)/hhc $(HTML_HELP_MAN_PATH)/$(HTML_HELP_MAN)
	cp $(HTML_HELP_MAN_PATH)/$(HTML_HELP_MAN) $(HTML_HELP_MAN)



##############################################################
####  Document archive generation

$(AAF_HTML_MAN_ZIP): $(AAF_HTML_MAN)
	rm -f $(AAF_HTML_MAN_ZIP)
	cd $(OUTPUT_DIR) && \
		$(ZIP) -9rq $(AAF_HTML_MAN_ZIP_FILE) $(AAF_HTML_MAN_DIR_COMP)

$(AAF_HTML_MAN_TGZ): $(AAF_HTML_MAN)
	rm -f $(AAF_HTML_MAN_TGZ)
	cd $(OUTPUT_DIR) && \
		$(TAR) cfz $(AAF_HTML_MAN_TGZ_FILE) $(AAF_HTML_MAN_DIR_COMP)

$(PLUGIN_HTML_MAN_ZIP): $(PLUGIN_HTML_MAN)
	rm -f $(PLUGIN_HTML_MAN_ZIP)
	cd $(OUTPUT_DIR) && \
		$(ZIP) -9rq $(PLUGIN_HTML_MAN_ZIP_FILE) $(PLUGIN_HTML_MAN_DIR_COMP)

$(PLUGIN_HTML_MAN_TGZ): $(PLUGIN_HTML_MAN)
	rm -f $(PLUGIN_HTML_MAN_TGZ)
	cd $(OUTPUT_DIR) && \
		$(TAR) cfz $(PLUGIN_HTML_MAN_TGZ_FILE) $(PLUGIN_HTML_MAN_DIR_COMP)


##############################################################
#### Misc. targets

$(AAF_HTML_MAN_DIR)/aafDSlogo.gif \
$(PLUGIN_HTML_MAN_DIR)/aafDSlogo.gif: aafDSlogo.gif
	cp aafDSlogo.gif $@
	



