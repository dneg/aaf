# Microsoft Developer Studio Project File - Name="test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "test - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "test - Win32 Release"
# Name "test - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\testimg.bmp"

!IF  "$(CFG)" == "test - Win32 Release"

# Begin Custom Build - Test cjpeg, wrjpgcom, rdjpgcom: bmp
OutDir=.\Release
InputPath=..\..\..\ref-impl\libjpeg\testimg.bmp

BuildCmds= \
	$(OutDir)\cjpeg -dct int -outfile $(OutDir)\testoutb.jpg $(InputPath) \
	$(OutDir)\rdjpgcom -verbose $(OutDir)\testoutb.jpg \
	$(OutDir)\wrjpgcom -replace -comment "AVID AAF" $(OutDir)\testoutb.jpg $(OutDir)\testoutbc.jpg \
	$(OutDir)\rdjpgcom -verbose $(OutDir)\testoutbc.jpg \
	

"$(OutDir)\testoutb.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testoutbc.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# Begin Custom Build - Test cjpeg, wrjpgcom, rdjpgcom: bmp
OutDir=.\Debug
InputPath=..\..\..\ref-impl\libjpeg\testimg.bmp

BuildCmds= \
	$(OutDir)\cjpeg -dct int -outfile $(OutDir)\testoutb.jpg $(InputPath) \
	$(OutDir)\rdjpgcom -verbose $(OutDir)\testoutb.jpg \
	$(OutDir)\wrjpgcom -replace -comment "AVID AAF" $(OutDir)\testoutb.jpg $(OutDir)\testoutbc.jpg \
	$(OutDir)\rdjpgcom -verbose $(OutDir)\testoutbc.jpg \
	

"$(OutDir)\testoutb.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testoutbc.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\testimg.ppm"

!IF  "$(CFG)" == "test - Win32 Release"

# Begin Custom Build - Test cjpeg ppm...
InputDir=\coresw\AAF-toolkit\ref-impl\libjpeg
OutDir=.\Release
InputPath="..\..\..\ref-impl\libjpeg\testimg.ppm"

BuildCmds= \
	$(OutDir)\cjpeg -dct int -outfile $(OutDir)\testout.jpg $(InputPath) \
	$(OutDir)\cjpeg -dct int -progressive -opt -outfile $(OutDir)\testoutp.jpg $(InputPath) \
	fc /b $(InputDir)\testimg.jpg $(OutDir)\testout.jpg \
	fc /b $(InputDir)\testimgp.jpg $(OutDir)\testoutp.jpg \
	

"$(OutDir)\testout.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testoutp.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# Begin Custom Build - Test cjpeg ppm...
InputDir=\coresw\AAF-toolkit\ref-impl\libjpeg
OutDir=.\Debug
InputPath="..\..\..\ref-impl\libjpeg\testimg.ppm"

BuildCmds= \
	$(OutDir)\cjpeg -dct int -outfile $(OutDir)\testout.jpg $(InputPath) \
	$(OutDir)\cjpeg -dct int -progressive -opt -outfile $(OutDir)\testoutp.jpg $(InputPath) \
	fc /b $(InputDir)\testimg.jpg $(OutDir)\testout.jpg \
	fc /b $(InputDir)\testimgp.jpg $(OutDir)\testoutp.jpg \
	

"$(OutDir)\testout.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testoutp.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\testorig.jpg"

!IF  "$(CFG)" == "test - Win32 Release"

# Begin Custom Build - Test djpeg ppm, bmp...
InputDir=\coresw\AAF-toolkit\ref-impl\libjpeg
OutDir=.\Release
InputPath="..\..\..\ref-impl\libjpeg\testorig.jpg"

BuildCmds= \
	$(OutDir)\djpeg -dct int -ppm -outfile $(OutDir)\testout.ppm $(InputPath) \
	$(OutDir)\djpeg -dct int -bmp -colors 256 -outfile $(OutDir)\testout.bmp $(InputPath) \
	fc /b $(InputDir)\testimg.ppm $(OutDir)\testout.ppm \
	fc /b $(InputDir)\testimg.bmp $(OutDir)\testout.bmp \
	

"$(OutDir)\testout.bmp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testout.ppm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# Begin Custom Build - Test djpeg ppm, bmp...
InputDir=\coresw\AAF-toolkit\ref-impl\libjpeg
OutDir=.\Debug
InputPath="..\..\..\ref-impl\libjpeg\testorig.jpg"

BuildCmds= \
	$(OutDir)\djpeg -dct int -ppm -outfile $(OutDir)\testout.ppm $(InputPath) \
	$(OutDir)\djpeg -dct int -bmp -colors 256 -outfile $(OutDir)\testout.bmp $(InputPath) \
	fc /b $(InputDir)\testimg.ppm $(OutDir)\testout.ppm \
	fc /b $(InputDir)\testimg.bmp $(OutDir)\testout.bmp \
	

"$(OutDir)\testout.bmp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testout.ppm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\testprog.jpg"

!IF  "$(CFG)" == "test - Win32 Release"

# Begin Custom Build - Test djpeg progressing ppm...
InputDir=\coresw\AAF-toolkit\ref-impl\libjpeg
OutDir=.\Release
InputPath="..\..\..\ref-impl\libjpeg\testprog.jpg"

BuildCmds= \
	$(OutDir)\djpeg -dct int -ppm -outfile $(OutDir)\testoutp.ppm $(InputPath) \
	$(OutDir)\jpegtran -outfile $(OutDir)\testoutt.jpg $(InputPath) \
	fc /b $(InputDir)\testimg.ppm $(OutDir)\testoutp.ppm \
	fc /b $(InputDir)\testorig.jpg $(OutDir)\testoutt.jpg \
	

"$(OutDir)\testoutp.ppm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testoutt.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# Begin Custom Build - Test djpeg progressing ppm...
InputDir=\coresw\AAF-toolkit\ref-impl\libjpeg
OutDir=.\Debug
InputPath="..\..\..\ref-impl\libjpeg\testprog.jpg"

BuildCmds= \
	$(OutDir)\djpeg -dct int -ppm -outfile $(OutDir)\testoutp.ppm $(InputPath) \
	$(OutDir)\jpegtran -outfile $(OutDir)\testoutt.jpg $(InputPath) \
	fc /b $(InputDir)\testimg.ppm $(OutDir)\testoutp.ppm \
	fc /b $(InputDir)\testorig.jpg $(OutDir)\testoutt.jpg \
	

"$(OutDir)\testoutp.ppm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\testoutt.jpg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
