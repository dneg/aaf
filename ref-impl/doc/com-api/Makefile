# Makefile for GnuMake under CYGWIN

DOCJETBIN = /cygdrive/c/Program\ Files/TallTree/DocJet/Program/
GENERATOR = $(DOCJETBIN)/generator.exe
PERL	=	perl
COMAPIPATH = ../../../../AAFWinSDK/include


all:	AAFnospace.idl $(COMAPIPATH)/AAFTypes.h
	$(GENERATOR) -v refDocFromIDL.djt
	
Documentation/New\ Standard\ HTMLHelp/aafapi.chm:	AAFnospace.idl $(COMAPIPATH)/AAFTypes.h
	$(GENERATOR) -v refHHDocFromIDL.djt
	
#AAFnospace.idl:	 $(COMAPIPATH)/AAF.idl
#	$(PERL) idlcleanup.perl $(COMAPIPATH)/AAF.idl | perl idlblankcomfix.perl > AAFNoSpace.idl
AAFnospace.idl:	 $(COMAPIPATH)/AAF.idl idlcleanup.perl
	$(PERL) idlcleanup.perl $(COMAPIPATH)/AAF.idl > AAFNoSpace.idl
AAFglobal.h:	 $(COMAPIPATH)/AAF.idl $(COMAPIPATH)/AAF.h 

