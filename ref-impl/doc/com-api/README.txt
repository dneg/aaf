Instructions for building an API Ref manual from the IDL with DocJet

Original - Joshua Goldman, Avid
Updated  - Simon Carter, AAF Assoc Dev Support


Requirements:

Windows NT4, 2K
DocJet from Talltree Software http://www.talltree.com/docjet/  (currently 4.0build7)
DocJet 'team' licence (there are more than 1000 objects) $550
Hacked DocJet 'Summary Hook' parser - to spilt Description at first sentence
HTMLHelp SDK/Compiler from Microsoft
Cygwin or equivalent for cvs, GnuMake, perl

Steps

1. do a CVS update on your workspace to make sure the dodo files and headers are up to date

2. From a cygwin shell (or equivalent) run Make in this directory (AAF/ref-impl/doc/com-api)
   The first step will be to copy CVS based files from this  directory into the DocJet destination dirs.
   This includes the Summary Hook extension which has been tweaked (See SummaryHook.cpp and diff with
   DocJet distribution) to allow the first sentence of a Comment to be isolated as the 'Summary'
   (quick one line description) and the remainder be passed as the 'Remarks' section.
   The original Summary Hook can only handle an initial sentence or paragraph one line long and
   it repeats the first sentence in the remarks.
   There are two targets, regular HTML and Microsoft HtmlHelp which will be generated in
   Documemtation\New Standard HTML  and Documemtation\New Standard HTMLHelp subdirectories
   Perl scripts idlcleanup.perl and idlblanlkcomfix.perl are used to preprocess and tidy up the IDL
   to run under Cygwin with CRLF line endings the CR's had to be stripped out with 'tr' first.
   With these scripts AAF/ref-impl/include/com-api/AAF.idl yields AAFnospace.idl
   DocJet is run on this IDL file using the refDocFromIDL.djt and refHHDocFromIDL.djt configuration files.
   The output is redirected to the file errors.txt, it is worth examining this file,
   There will be loads of Warnings for missing sections but there should not be
   any major parsing 'Error D11042' could not decipher... or stuff gets left out of the HTML.
   
**** FIXME Extra steps not yet being performed !

4. Fix AAFGlobals.h if these interfaces have changed.
Global functions not in AAF.idl. Global functions in AAF.h but
comments are not. Workaround: create AAFGlobals.h with .h file and
comments from .idl. Need to put the {filename in and remove comments from
function and parameter lines. A sample version is in this directory. I put this
file in the AAFWinSDK\include directory.

5. Run Visual Studio C++. Open AAF SDK workspace. Add following Project to Workspace:

     Ref doc from aaf idl/Ref doc from aaf idl.dsw 

[you might want to rename it to get rid of the spaces.]
This dsw project assumes that the input files are in the AAFWinSDK\include directory.
This could be cleaned up.

This project uses the following source files:

AAFnospace.idl
AAFTypes.h
AAFglobal.h

To edit the DocJet settings, double click on:

   RefDocFRomIDL.djt

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

11. Copy mhelp.chm to projects directory

12. scp man.zip to aaf.sourceforge.net:/home/groups/a/aa/aaf/htdocs/docs/

13. Unzip on Unix into com-api-new directory using unzip.

14. Remove old com-api directory and rename new.

15. Fixes to source
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

Ascii Art Text illustration in IAAFDigitalImageDescriptor is in CVS as ImgDescArea.gif and .bmp for HTMLHelp

-----------

<SMC>
how to generate HTMLHelp .chm file

In DocJet configuration select output as Standard/HTMLHelp and make the files
Should also run HTMLHelp complier
Select the mhelp.hhp file and double click it to launch HTML Help workshop and hit the compile button

This should give you a .chm and .chi file which need to be zipped together and uploaded.
