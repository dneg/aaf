# Makefile (assumes GnuMake under CYGWIN)
# builds AAF COM API documentation using DocJet and Microsoft HTMLHelp Compiler

DOCJET_PATH = C:/Program\ Files/TallTree/DocJet
HTMLHELP_WORKSHOP = C:/Program\ Files/HTML\ Help\ Workshop
WINZIP = C:/Program\ Files/winzip/wzzip
DOCJET_BIN = $(DOCJET_PATH)/Program
GENERATOR = $(DOCJET_BIN)/generator.exe
SUMMARY_HOOK = $(DOCJET_PATH)/extensions/Summary\ Hook.dll
AAFAPI_FORMAT = $(DOCJET_PATH)/Output\ Formats/AAFapi.dof
PERL	=	perl
COMAPI_PATH = ../../include/com-api
HTML_MAN_PATH = Documentation/New\ Standard\ HTML
HTML_HELP_MAN_PATH = Documentation/New\ Standard\ HTMLHelp
HTML_MAN = aafapiman.zip
HTML_HELP_MAN = aafapi.chm

.PHONY:	docjet

all:	docjet $(HTML_MAN) $(HTML_HELP_MAN)

docjet: $(SUMMARY_HOOK) $(AAFAPI_FORMAT)

$(SUMMARY_HOOK) : Summary\ Hook.dll
	cp Summary\ Hook.dll $(SUMMARY_HOOK)

$(AAFAPI_FORMAT) : AAFapi.dof
	cp AAFapi.dof $(AAFAPI_FORMAT)

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
	
AAFnospace.idl:	 idlcleanup.perl $(COMAPI_PATH)/AAF.idl
#	Since idlcleanup reads paragraphs, had to strip CR's first with tr
	tr -d "\r" <  $(COMAPI_PATH)/AAF.idl | $(PERL) idlcleanup.perl | $(PERL) idlblankcomfix.perl > AAFnospace.idl

AAFglobals.h:	 $(COMAPI_PATH)/AAF.idl $(COMAPI_PATH)/AAF.h 

AAFTypes.h:	 $(COMAPI_PATH)/AAFTypes.idl
	cp  $(COMAPI_PATH)/AAFTypes.idl AAFTypes.idl

$(HTML_MAN):	$(HTML_MAN_PATH) $(HTML_MAN_PATH)/frontCover.html $(HTML_MAN_PATH)/DigImgAreas.gif AAFTypes.h AAFnospace.idl 
	- $(GENERATOR) -v refDocFromIDL.djt 2> errors.txt
#   Just echo the Errors from the logfile
	grep -v Warning errors.txt
#   Had to use find to list files thru stdio because there are too many for bash to glob
	cd $(HTML_MAN_PATH); find . -print > ../../fileList.txt; $(WINZIP) ../../aafapiman.zip @../../fileList.txt
	cd $(HTML_MAN_PATH); tar -T ../../fileList.txt -cvf ../../aafapiman.tgz

$(HTML_HELP_MAN):	$(HTMLHELP_WORKSHOP)/hhc $(HTML_HELP_MAN_PATH) $(HTML_HELP_MAN_PATH)/frontCover.html $(HTML_MAN_PATH)/DigImgAreas.gif AAFTypes.h AAFnospace.idl 
	- $(GENERATOR) -v refHHDocFromIDL.djt 2> errorsHH.txt
#   Just echo the Errors from the logfile
	grep -v Warning errorsHH.txt
#   Now compile again without CHI file
	cp $(HTML_HELP_MAN_PATH)/aafapi.hhp $(HTML_HELP_MAN_PATH)/aafapi.tmp
	sed 's/Create CHI file=yes/Create CHI file=No/' $(HTML_HELP_MAN_PATH)/aafapi.tmp > $(HTML_HELP_MAN_PATH)/aafapi.hhp
	- $(HTMLHELP_WORKSHOP)/hhc $(HTML_HELP_MAN_PATH)/$(HTML_HELP_MAN)
	cp $(HTML_HELP_MAN_PATH)/$(HTML_HELP_MAN) $(HTML_HELP_MAN)

test:	test.idl
	- $(GENERATOR) -v test.djt
