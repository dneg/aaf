DocJet Procedures:
0. Checkin Tom's changes to do the stored class inheritance stuff

1. Copyout files and do a clearmake

2. Open the Visual C++ workspace and build the project to generate the AAF.idl and 
    AAFTypes.h files you'll need in the following steps. 

3. User perl scripts to convert AAF.idl and save as
   AAFWinSDK/include/AAFnoSpace.idl. Use idlcleanup.perl and
   idlblankcomfix.perl to produce it.

# perl idlcleanup.perl AAF.idl |perl idlblankcomfix.perl > AAFNoSpace.idl

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

   REF DOC FROM AAF IDL.DJT

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

6. Set project to "Ref doc from aaf idl" and build.

[Need to add doc about paragraph sign to put types, interfaces in lower-left box. 
Why are the types not defined in main box but are in lower-left?]

7,8,9 deleted

10. Update DocStatus.txt for release notes

11. Copy mhelp.chm to projects directory

12. Zip NewStandard HTML directory, copy to Eng:/web_site/AAF/specs/aafapinew

13. Unzip on Unix using unzip.

14. Remove old aafapi and rename new.

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

Fix text illustration in IAAFDigitalImageDescriptor

-----------

<SMC>
how to generate HTMLHelp .chm file

In DocJect configuration select output as Standard/HTMLHelp and make the files
Should also run HTMLHelp complier
Select the mhelp.hhp file and double click it to launch HTML Help workshop and hit the compile button

This should give you a 





