/***********************************************************************
*
* The contents of this file are subject to the AAF SDK Public
* Source License Agreement (the "License"); You may not use this file
* except in compliance with the License.  The License is available in
* AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
* Association or its successor.
*
* Software distributed under the License is distributed on an "AS IS"
* basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
* the License for the specific language governing rights and limitations
* under the License.
*
* The Original Code of this file is Copyright 1998-2001, Licensor of the
* AAF Association.
*
* The Initial Developer of the Original Code of this file and the
* Licensor of the AAF Association is Avid Technology.
* All rights reserved.
*
************************************************************************/

/*
 *	AAFHeaders.c
 *
 *	Script to generate the 'AAFHeaders<xxx>' precompiled header for Metrowerks C/C++.
 */

/*
 *	Required for c-style toolbox glue function: c2pstr and p2cstr
 *	the inverse operation (pointers_in_A0) is performed at the end ...
 */

#if __MC68K__ && !__CFM68K__
 #pragma d0_pointers on
#endif

/*
 *	To allow the use of ToolBox calls which have now become obsolete on PowerPC, but
 *	which are still needed for System 6 applications, we need to #define OBSOLETE.  If
 *	your application will never use these calls then you can comment out this #define.
 *	NB: This is only for 68K ...
 */

#ifndef 	OLDROUTINENAMES
#define		OLDROUTINENAMES 	0
#endif

#ifndef		OLDROUTINELOCATIONS
#define 	OLDROUTINELOCATIONS 0
#endif

//#if !defined(powerc) && !defined(OBSOLETE)
// #define OBSOLETE	1
//#endif

/*
 *	Metrowerks-specific definitions
 *
 *	These definitions are commonly used but not in Apple's headers. We define
 *	them in our precompiled header so we can use the Apple headers without modification.
 */

#ifndef PtoCstr
#define PtoCstr		p2cstr
#endif
#ifndef CtoPstr
#define CtoPstr		c2pstr
#endif
#ifndef PtoCString
#define PtoCString	p2cstr
#endif
#ifndef CtoPString
#define CtoPString	c2pstr
#endif

#ifndef topLeft
#define topLeft(r)	(((Point *) &(r))[0])
#endif
#ifndef botRight
#define botRight(r)	(((Point *) &(r))[1])
#endif

#ifndef TRUE
#define TRUE		true
#endif
#ifndef FALSE
#define FALSE		false
#endif

#ifndef powerc
 #include <MixedMode.h>
 long GetCurrentA5(void)
  ONEWORDINLINE(0x200D);
#endif

/*
 *	Apple #include files
 *
 *	Uncomment any additional #includes you want to add to AAFHeaders.
 */

//	#include <ADSP.h>
//	#include <AEObjects.h>
//	#include <AEPackObject.h>
//	#include <AERegistry.h>
//	#include <AEUserTermTypes.h>
//	#include <AIFF.h>
//	#include <Aliases.h>
//	#include <AppleEvents.h>
//	#include <AppleGuide.h>
//	#include <AppleScript.h>
//	#include <AppleTalk.h>
//	#include <ASDebugging.h>
//	#include <ASRegistry.h>
//	#include <Balloons.h>
//	#include <CMApplication.h>
//	#include <CMComponent.h>
	#include <CodeFragments.h>
//	#include <ColorPicker.h>
//	#include <CommResources.h>
//	#include <Components.h>
	#include <ConditionalMacros.h>
//	#include <Connections.h>
//	#include <ConnectionTools.h>
//	#include <Controls.h>
//	#include <ControlStrip.h>
//	#include <CRMSerialDevices.h>
//	#include <CTBUtilities.h>
//	#include <CursorCtl.h>
//	#include <CursorDevices.h>
//	#include <DatabaseAccess.h>
//	#include <DeskBus.h>
//	#include <Devices.h>
//	#include <Dialogs.h>
//	#include <Dictionary.h>
//	#include <DisAsmLookup.h>
//	#include <Disassembler.h>
//	#include <DiskInit.h>
//	#include <Disks.h>
//	#include <Displays.h>
//	#include <Drag.h>
//	#include <Editions.h>
//	#include <ENET.h>
//	#include <EPPC.h>
//	#include <ErrMgr.h>
	#include <Errors.h>
//	#include <Events.h>
//	#include <fenv.h>
//	#include <Files.h>
//	#include <FileTransfers.h>
//	#include <FileTransferTools.h>
//	#include <FileTypesAndCreators.h>
//	#include <Finder.h>
//	#include <FixMath.h>
//	#include <Folders.h>
//	#include <Fonts.h>
//	#include <fp.h>
//	#include <FragLoad.h>
//	#include <FSM.h>
	#include <Gestalt.h>
//	#include <HyperXCmd.h>
//	#include <Icons.h>
//	#include <ImageCodec.h>
//	#include <ImageCompression.h>
//	#include <IntlResources.h>
//	#include <Language.h>
//	#include <Lists.h>
	#include <LowMem.h>
//	#include <MachineExceptions.h>
//	#include <MacTCP.h>
//	#include <MediaHandlers.h>
	#include <Memory.h>
//	#include <Menus.h>
//	#include <MIDI.h>
	#include <MixedMode.h>
//	#include <Movies.h>
//	#include <MoviesFormat.h>
//	#include <Notification.h>
//	#include <OSA.h>
//	#include <OSAComp.h>
//	#include <OSAGeneric.h>
//	#include <OSUtils.h>
//	#include <Packages.h>
//	#include <Palettes.h>
//	#include <Picker.h>
//	#include <PictUtil.h>
//	#include <PictUtils.h>
//	#include <PLStringFuncs.h>
//	#include <Power.h>
//	#include <PPCToolbox.h>
//	#include <Printing.h>
//	#include <Processes.h>
//	#include <QDOffscreen.h>
//	#include <Quickdraw.h>
//	#include <QuickdrawText.h>
//	#include <QuickTimeComponents.h>
//	#include <Resources.h>
//	#include <Retrace.h>
//	#include <ROMDefs.h>
#ifndef powerc
//	#include <SANE.h>
#endif
//	#include <Scrap.h>
//	#include <Script.h>
//	#include <SCSI.h>
//	#include <SegLoad.h>
//	#include <Serial.h>
//	#include <ShutDown.h>
//	#include <Slots.h>
//	#include <Sound.h>
//	#include <SoundComponents.h>
//	#include <SoundInput.h>
//	#include <Speech.h>
//	#include <StandardFile.h>
//	#include <Start.h>
//	#include <Strings.h>
//	#include <Terminals.h>
//	#include <TerminalTools.h>
//	#include <TextEdit.h>
//	#include <TextServices.h>
//	#include <TextUtils.h>
//	#include <Threads.h>
//	#include <Timer.h>
//	#include <ToolUtils.h>
//	#include <Translation.h>
//	#include <TranslationExtensions.h>
//	#include <Traps.h>
//	#include <TSMTE.h>
	#include <Types.h>
//	#include <Unmangler.h>
//	#include <Video.h>
//	#include <Windows.h>
//	#include <WorldScript.h>


#define __dest_os	__mac_os

//#define CHECKMEMORYLEAKS // Uncomment this line to turn leak checking on

// Symbols needed for OLE/ActiveX headers.
#define _MAC
#define NONAMELESSUNION


#if !defined(CDECL) && defined(_MSC_VER)
#define CDECL	_cdecl
#endif // CDECL

#if !defined(FAR)
#define FAR
#endif

#include "ole2.h"
#include "dispatch.h"
#include "wintypes.h"
#include "olectl.h"

// Mac OLE/ActiveX headers do not define IID, CLSID, GUID in multiple header
// files so they do not need to have any special include guards to keep
// the symbols from being multiply defined. For compatibility with other WIN32
// OLE/COM we need to define the following symbolic include guards to keep
// the compiler from redefining IID, CLSID and GUID.
#ifndef __IID_DEFINED__
#define __IID_DEFINED__
#endif // __IID_DEFINED__
#ifndef CLSID_DEFINED
#define CLSID_DEFINED
#endif // CLSID_DEFINED
#ifndef GUID_DEFINED
#define GUID_DEFINED
#endif // GUID_DEFINED


// Include the main AAF header files
#include "AAF.h"


//#include <iostream.h>
//#include <stdio.h>
//#include <string.h>


/*
 *	required for c-style toolbox glue function: c2pstr and p2cstr
 *	(match the inverse operation at the start of the file ...
 */

#if __MC68K__ && !__CFM68K__
 #pragma d0_pointers reset
#endif
