Instructions for building an API Ref manual from the IDL with DocJet

Original - Joshua Goldman, Avid
Updated  - Simon Carter, AAF Assoc Dev Support (Sept 01)


Requirements:

Windows NT4, 2K
DocJet from Talltree Software http://www.talltree.com/docjet/  (currently 4.0build7)
DocJet 'team' licence (there are more than 1000 objects) $550
Hacked DocJet 'Summary Hook' parser - to spilt Description at first sentence (DLL and source in this dir)
HTMLHelp SDK/Compiler from Microsoft (might have to dig a bit, Seems to have disappeared off MSDN recently)
Winzip including command line support add-on (better compression than Cygwin/GNU zip)
Cygwin or equivalent for cvs, GnuMake, perl

Steps

1. do a CVS update on your workspace to make sure the dodo files and headers are up to date

2. From a cygwin shell (or equivalent) run Make in this directory (AAF/ref-impl/doc/com-api)
   
2.1 The first step will be to copy CVS based files from this  directory into the DocJet destination dirs.
   (Some of the paths might need changing - see the macros at the top of the makefile)
   This includes the Summary Hook extension which has been tweaked (See SummaryHook.cpp and diff with
   DocJet distribution) to allow the first sentence of a Comment to be isolated as the 'Summary'
   (quick one line description) and the remainder be passed as the 'Remarks' section.
   The original Summary Hook can only handle an initial sentence or paragraph one line long and
   it repeats the first sentence in the remarks.
   
2.2 There are two targets, regular HTML and Microsoft HtmlHelp which will be generated in
   Documemtation\New Standard HTML and Documemtation\New Standard HTMLHelp subdirectories.
   
2.3 Perl scripts idlcleanup.perl and idlblanlkcomfix.perl are used to preprocess and tidy up the IDL
   to run under Cygwin with CRLF line endings the CR's had to be stripped out with 'tr' first.
   With these scripts AAF/ref-impl/include/com-api/AAF.idl yields AAFnospace.idl

2.4 DocJet is run on this IDL file using the refDocFromIDL.djt and refHHDocFromIDL.djt configuration files.
   The output is redirected to the file errors.txt, it is worth examining this file,
   There will be loads of Warnings for missing sections but there should not be
   any major parsing 'Error D11042' could not decipher... or stuff gets left out of the HTML.
   
----

   generating the HTMLHelp .chm file..

   In DocJet seperate configuration file refHHDoc|FromIDL.djt select output as Standard/HTMLHelp
   Set the name to aafapi
   If it finds it DocJet will also run HTMLHelp complier, BUT we want a manual without the additional CHI file
   so the Makefile tweaks the HHP file to leave outthe CHI and re-runs the compiler.

To edit the DocJet settings, double click on:

   RefDocFRomIDL.djt

The aafapi.dof is derived from New Standard HTML format
Basic mods to generic settings:
Sources tab
I've specified the Sources files listed above.

General tab to generate
aafapi -- HTML     vanilla HTML (I modified New Standard HTML to produce this)
New Standard HTML   HTML for Windows MS Internet Explorer (don't really need this)
New Standard -- HTMLHelp  Compiled help on windows
Argument hook
Group hook
Blank line hook (which doesn't really seem to work)

IDL Cleanup Hook - check "Remove all attributes"

Variables tab
(settings may be local to output format)
Check what I set these. Mostly what I did was get rid of the Tall-tree logo and 
web link
<SMC> Knobble the 'Trailer Banner' and 'Trailer URL' Under "New Standard"

Automatics and Comment Sections tabs
didn't do anything here

11. Makefile should leave new aafapi.chm, aafapiman.zip and aafapiman.tgz in this directory

12. scp man.zip to aaf.sourceforge.net:/home/groups/a/aa/aaf/htdocs/docs/com-api

13. ssh to sourceforge and Unzip on into com-api-new directory using.

14. Remove old com-api directory and rename new.

TODO Fixes to source
    Get rid of incorrect stub only
    Fix cut-and-paste errors in comments
    Fix undocumented interfaces:
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(4817) : Warning D2000: Undocumented object, 'IAAFEdgecode'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(4874) : Warning D2000: Undocumented object, 'IAAFOperationDef'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(8954) : Warning D2000: Undocumented object, 'IAAFFindSourceInfo'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(14137) : Warning D2000: Undocumented object, 'IAAFParameter'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(14232) : Warning D2000: Undocumented object, 'IAAFParameterDef'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(14503) : Warning D2000: Undocumented object, 'IAAFPropertyDef'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(24615) : Warning D2000: Undocumented object, 'IEnumAAFCodecDefs'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(25027) : Warning D2000: Undocumented object, 'IEnumAAFContainerDefs'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(25175) : Warning D2000: Undocumented object, 'IEnumAAFControlPoints'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(25323) : Warning D2000: Undocumented object, 'IEnumAAFDataDefs'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(25631) : Warning D2000: Undocumented object, 'IEnumAAFInterpolationDefs'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(25779) : Warning D2000: Undocumented object, 'IEnumAAFOperationDefs'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(26024) : Warning D2000: Undocumented object, 'IEnumAAFKLVData'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(27246) : Warning D2000: Undocumented object, 'IEnumAAFPropertyDefs'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(27814) : Warning D2000: Undocumented object, 'IEnumAAFSegments'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(27962) : Warning D2000: Undocumented object, 'IEnumAAFTaggedValues'
D:\aafview\coresw\AAF-toolkit\AAFWinSDK\Ref doc from aaf idl\..\include\AAFnoSpace.idl(28110) : Warning D2000: Undocumented object, 'IEnumAAFTypeDefs'


-----------

