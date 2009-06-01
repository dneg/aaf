#ifndef _AAF_OMF_
#define _AAF_OMF_	1
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

/************************************************************************
 *
 * Omf2Aaf.h Describes structures and prototypes for the OMF to AAF 
 *          conversion utility.
 *
 ************************************************************************/

  
#include "AAF.h"
#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFParameterDefs.h"
#include "AAFInterpolatorDefs.h"
#include "AAFTypeDefUIDs.h"
#include "StreamLogger.h"

//#ifndef _INCLUDED_OMF2_
//#define _INCLUDED_OMF2_
//namespace OMF2
//{
//#include "omPublic.h"
//#include "omMedia.h"
//}
//#endif

const int MAX_INDENT = 8;
const unsigned	kLogError = 0; 	// Error level.
const unsigned	kLogWarn 	= 1;  // Warning level.
const unsigned	kLogInfo  = 2; 	// Informational level.

typedef struct _AafOmfGlobals
{
	aafInt16		numIndents;
	char			indentLeader[MAX_INDENT+1];	

	aafBool			bVerboseMode;
	aafBool			bCreateTOCFile;
	aafBool			bConvertAllObjects;
	aafBool			bOMFFileOpen;
	aafBool			bAAFFileOpen;
	aafBool			bLogFile;
	aafBool			bDefFile;
	aafBool			bDeleteOutput;
	aafBool			bConvertAAFFile;

	// For Statistical summary
	aafInt32		nNumOMFMobs;
	aafInt32		nNumAAFMobs;
	aafInt32		nNumOMFObjects;
	aafInt32		nNumAAFObjects;
	aafInt32		nNumOMFProperties;
	aafInt32		nNumAAFProperties;
	aafInt32		nNumUndefinedOMFObjects;
	aafInt32		nNumUndefinedOMFProperties;
	aafInt32		nNumUndefinedAAFObjects;
	aafInt32		nNumUndefinedAAFProperties;

	char			sInFileName[256];
	char			sTOCFileName[256];
	char			sDefinitionFileName[256];
	char			sOutFileName[256];
	char			sLogFileName[256];
	char*			pProgramName;
	StreamLogger*		pLogger;

	// MC Private Properties
	omfProperty_t		pvtEffectIDProp;
	omfProperty_t		pvtAppCode;
	omfProperty_t		pvtAttributes;
	omfProperty_t		pvtDataAttribute;

	// Codec Properties
	omfProperty_t		omCDCIComponentWidth;
	omfProperty_t		omCDCIHorizontalSubsampling;
	omfProperty_t		omCDCIColorSiting;
	omfProperty_t		omCDCIBlackReferenceLevel;
	omfProperty_t		omCDCIWhiteReferenceLevel;
	omfProperty_t		omCDCIColorRange;
	omfProperty_t		omCDCIPaddingBits;
	~_AafOmfGlobals( void )		{ delete pLogger; }
} AafOmfGlobals;


int deleteFile( char* fileName );
void IncIndentLevel( void );
void DecIndentLevel( void );
void AUIDtoString(aafUID_t *uid, char *buf);
void MobIDtoString(aafMobID_constref uid, char *buf);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	prefix,
        aafUInt32	major,
		aafUInt32	minor,
		aafUInt8	UMIDType,
		aafMobID_t *mobID);     /* OUT - Newly created Mob ID */
void RegisterCodecProperties(AafOmfGlobals *globals, omfSessionHdl_t OMFSession);
HRESULT InitGlobalVars( void );
HRESULT IsOMFFile (char * pFileName );

extern AafOmfGlobals*	gpGlobals;


#define COMMON_ERR_BASE		(AAFRESULT)0xE0000000
#define AAF2OMF_ERR_BASE	(AAFRESULT)0xE0001000
#define OMF2AAF_ERR_BASE	(AAFRESULT)0xE0006000
#define AAFRESULT_FILE_NOT_OMF (OMF2AAF_ERR_BASE + 1)
#endif
