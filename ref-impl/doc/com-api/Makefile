# Makefile (assumes GnuMake under CYGWIN)
# builds AAF COM API documentation using DocJet and Microsoft HTMLHelp Compiler

DOCJET_PATH = /cygdrive/c/Program\ Files/TallTree/DocJet
DOCJET_BIN = $(DOCJET_PATH)/Program
GENERATOR = $(DOCJET_BIN)/generator.exe
SUMMARY_HOOK = $(DOCJET_PATH)/extensions/Summary\ Hook.dll
PERL	=	perl
COMAPI_PATH = ../../include/com-api
HTML_MAN_PATH = Documentation/New\ Standard\ HTML
HTML_HELP_MAN_PATH = Documentation/New\ Standard\ HTMLHelp
HTML_MAN = man.zip
HTML_HELP_MAN = $(HTML_HELP_MAN_PATH)/aafapi.chm

.PHONY:	docjet

all:	docjet $(HTML_MAN)

docjet: $(SUMMARY_HOOK)

$(SUMMARY_HOOK) : Summary\ Hook.dll
	cp Summary\ Hook.dll $(SUMMARY_HOOK)

$(HTML_MAN_PATH) :
	if [ ! -d $@ ]; then \
		mkdir -p $@; \
	fi

$(HTML_HELP_MAN_PATH) :
	if [ ! -d $@ ]; then \
		mkdir -p $@; \
	fi

$(HTML_MAN_PATH)/DigImgAreas.gif: DigImgAreas.gif
	cp DigImgAreas.gif $(HTML_MAN_PATH)
	
$(HTML_HELP_MAN_PATH)/DigImgAreas.bmp: DigImgAreas.bmp
	cp DigImgAreas.bmp $(HTML_HELP_MAN_PATH)
	
$(HTML_MAN_PATH)/aafDSlogo.gif: aafDSlogo.gif
	cp aafDSlogo.gif $(HTML_MAN_PATH)
	
$(HTML_MAN_PATH)/frontCover.html: frontCover.html
	cp frontCover.html $(HTML_MAN_PATH)
	
$(HTML_HELP_MAN_PATH)/frontCover.html: frontCover.html
	cp frontCover.html $(HTML_HELP_MAN_PATH)
	
AAFnospace.idl:	 $(COMAPI_PATH)/AAF.idl
#	Since idlcleanup reads paragraphs, had to strip CR's first with tr
	tr -d "\r" <  $(COMAPI_PATH)/AAF.idl | $(PERL) idlcleanup.perl | $(PERL) idlblankcomfix.perl > AAFnospace.idl

AAFglobals.h:	 $(COMAPI_PATH)/AAF.idl $(COMAPI_PATH)/AAF.h 

AAFTypes.h:	 $(COMAPI_PATH)/AAFTypes.idl
	cp  $(COMAPI_PATH)/AAFTypes.idl AAFTypes.idl

$(HTML_MAN):	$(HTML_MAN_PATH) (HTML_MAN_PATH)/frontCover.html $(HTML_MAN_PATH)/DigImgAreas.gif AAFTypes.h AAFnospace.idl 
	- $(GENERATOR) -v refDocFromIDL.djt 2> errors.txt
#   Just echo the Errors from the logfile
	grep -v Warning errors.txt
#   Had to use find to list files thru stdio because there are too many for bash to glob
	cd $(HTML_MAN_PATH); find . -print | zip -q ../../man -@

$(HTML_HELP_MAN):	$(HTML_HELP_MAN_PATH) (HTML_HELP_MAN_PATH)/frontCover.html $(HTML_MAN_PATH)/DigImgAreas.gif AAFTypes.h AAFnospace.idl 
	- $(GENERATOR) -v refHHDocFromIDL.djt 2> errors.txt
#   Just echo the Errors from the logfile
	grep -v Warning errors.txt
#   Had to use find to list files thru stdio because there are too many for bash to glob
	cd $(HTML_HELP_MAN_PATH);  zip -q ../../HHman apiman.chm apiman.chi
