// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#ifdef macintosh
	#include <console.h>
#endif 

#include "omPublic.h"
#include "omMedia.h"

#include "AAFException.h"
#include "OMFException.h"
#include "AutoRelease.h"

// OMF Includes


#include "AafOmf.h"

#include "AAFDomainUtils.h"
#include "OMFDomainUtils.h"
#if AVID_SPECIAL
#include "ConvertAvid.h"
#include "AAFDomainAvidUtils.h"
#include "OMFDomainAvidUtils.h"
#else
#include "AAFDomainExtensions.h"
#include "OMFDomainExtensionUtils.h"
#include "Extensions.h"
#endif
#include "Aaf2Omf.h"
#include "Omf2Aaf.h"
#include "AAFClassDefUIDs.h"
#include "EffectTranslate.h"

//#include "AAFUtils.h"
//AAFRESULT aafMobIDNew(aafMobID_t *mobID);
//AAFRESULT aafMobIDFromMajorMinor(
//        aafUInt32	major,
//		aafUInt32	minor,
//		aafMobID_t *mobID);     /* OUT - Newly created Mob ID */

// ============================================================================
// Global Variables and functions
// ============================================================================
AafOmfGlobals*	gpGlobals = NULL;
// ============================================================================
// Delete a file given its  name
// ============================================================================
int deleteFile(char* fileName)
{

	return(remove(fileName));

}


void AUIDtoString(aafUID_t *uid, char *buf)
{
	sprintf(buf, "%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			uid->Data1, uid->Data2, uid->Data3, (int)uid->Data4[0],
			(int)uid->Data4[1], (int)uid->Data4[2], (int)uid->Data4[3], (int)uid->Data4[4],
			(int)uid->Data4[5], (int)uid->Data4[6], (int)uid->Data4[7]);
}

void MobIDtoString(aafMobID_constref uid, char *buf)
{
    sprintf( buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-" \
		  "%02x-%02x-%02x-%02x-" \
		  "%08x%04x%04x" \
		  "%02x%02x%02x%02x%02x%02x%02x%02x",

	(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], 
	(int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3],
	(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], 
	(int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7],
	(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[9], 
	(int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11],

	(int)uid.length, (int)uid.instanceHigh, 
	(int)uid.instanceMid, (int)uid.instanceLow,

	uid.material.Data1, uid.material.Data2, uid.material.Data3,

	(int)uid.material.Data4[0], (int)uid.material.Data4[1], 
	(int)uid.material.Data4[2], (int)uid.material.Data4[3],
	(int)uid.material.Data4[4], (int)uid.material.Data4[5], 
	(int)uid.material.Data4[6], (int)uid.material.Data4[7] );
}


struct SMPTELabel
{
	aafUInt32	MobIDMajor;
	aafUInt16	MobIDMinorLow;
	aafUInt16	MobIDMinorHigh;
	aafUInt8	oid;
	aafUInt8	size;
	aafUInt8	ulcode;
	aafUInt8	SMPTE;
	aafUInt8	Registry;
	aafUInt8	unused;
	aafUInt8	MobIDPrefixLow;
	aafUInt8	MobIDPrefixHigh;
};



struct OMFMobID
{
    aafUInt8			SMPTELabel[12];		// 12-bytes of label prefix
	aafUInt8			length;
    aafUInt8			instanceHigh;
    aafUInt8			instanceMid;
    aafUInt8			instanceLow;
	struct SMPTELabel	material;
};

union MobIDOverlay
{
	aafMobID_t			mobID;
	struct OMFMobID		OMFMobID;
};

AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	prefix,
        aafUInt32	major,
		aafUInt32	minor,
		aafUInt8	UMIDType,
		aafMobID_t *mobID)     /* OUT - Newly created Mob ID */
{
	union MobIDOverlay		aLabel;
	
    aLabel.OMFMobID.SMPTELabel[0]	= 0x06;
    aLabel.OMFMobID.SMPTELabel[1]	= 0x0C;
    aLabel.OMFMobID.SMPTELabel[2]	= 0x2B;
    aLabel.OMFMobID.SMPTELabel[3]	= 0x34;
    aLabel.OMFMobID.SMPTELabel[4]	= 0x02;			// Still Open
    aLabel.OMFMobID.SMPTELabel[5]	= 0x05;			// Still Open
    aLabel.OMFMobID.SMPTELabel[6]	= 0x11;			// Still Open
    aLabel.OMFMobID.SMPTELabel[7]	= 0x01;			// Still Open
    aLabel.OMFMobID.SMPTELabel[8]	= 0x01;			// Still Open
    aLabel.OMFMobID.SMPTELabel[9]	= UMIDType;
    aLabel.OMFMobID.SMPTELabel[10]	= 0x10;			// Still Open
    aLabel.OMFMobID.SMPTELabel[11]	= 0x00;
	aLabel.OMFMobID.length			= 0x13;
    aLabel.OMFMobID.instanceHigh		= 0x00;
    aLabel.OMFMobID.instanceMid		= 0x00;
	aLabel.OMFMobID.instanceLow		= 0x00;
	aLabel.OMFMobID.material.oid				= 0x06;
	aLabel.OMFMobID.material.size				= 0x0E;
	aLabel.OMFMobID.material.ulcode			= 0x2B;
	aLabel.OMFMobID.material.SMPTE				= 0x34;
	aLabel.OMFMobID.material.Registry			= 0x7F;
	aLabel.OMFMobID.material.unused			= 0x7F;
	aLabel.OMFMobID.material.MobIDPrefixHigh	= (aafUInt8)((prefix >> 7L) | 0x80);
	aLabel.OMFMobID.material.MobIDPrefixLow	= (aafUInt8)(prefix & 0x7F);

	aLabel.OMFMobID.material.MobIDMajor		= major;
	aLabel.OMFMobID.material.MobIDMinorLow		= (aafUInt16)(minor & 0xFFFF);
	aLabel.OMFMobID.material.MobIDMinorHigh	=  (aafUInt16)((minor >> 16L) & 0xFFFF);

	*mobID = (aafMobID_t)aLabel.mobID;
	return(AAFRESULT_SUCCESS);
}


// ============================================================================
// InitGlobalVars
// 
//		Allocates memory and intializes the global variables
//
// ============================================================================
HRESULT InitGlobalVars( void )
{
	gpGlobals = new AafOmfGlobals;
	if (gpGlobals == NULL)
		return AAFRESULT_NOMEMORY;

	gpGlobals->bAAFFileOpen = kAAFFalse;
	gpGlobals->bConvertAllObjects = kAAFFalse;
	gpGlobals->bCreateTOCFile = kAAFFalse;
	gpGlobals->bDefFile = kAAFFalse;
	gpGlobals->bLogFile = kAAFFalse;
	gpGlobals->pLogger = NULL;
	gpGlobals->bOMFFileOpen = kAAFFalse;
	gpGlobals->bVerboseMode = kAAFFalse;
	gpGlobals->numIndents = 0;
	gpGlobals->pProgramName = NULL;
	gpGlobals->bDeleteOutput  = kAAFTrue;
	gpGlobals->bConvertAAFFile  = kAAFFalse;

	gpGlobals->nNumAAFMobs = 0;
	gpGlobals->nNumAAFObjects = 0;
	gpGlobals->nNumAAFProperties = 0;
	gpGlobals->nNumOMFMobs = 0;
	gpGlobals->nNumOMFObjects = 0;
	gpGlobals->nNumOMFProperties  = 0;
	gpGlobals->nNumUndefinedOMFObjects = 0;
	gpGlobals->nNumUndefinedOMFProperties = 0;
	gpGlobals->nNumUndefinedAAFObjects = 0;
	gpGlobals->nNumUndefinedAAFProperties = 0;

	memset(gpGlobals->sDefinitionFileName, 0,sizeof(gpGlobals->sDefinitionFileName));
	memset(gpGlobals->indentLeader, 0, sizeof(gpGlobals->indentLeader));
	memset(gpGlobals->sInFileName, 0, sizeof(gpGlobals->sInFileName));
	memset(gpGlobals->sLogFileName, 0, sizeof(gpGlobals->sLogFileName));
	memset(gpGlobals->sOutFileName, 0, sizeof(gpGlobals->sOutFileName));
	memset(gpGlobals->sTOCFileName, 0, sizeof(gpGlobals->sTOCFileName));

	return AAFRESULT_SUCCESS;
}

/******************** IncIndentLevel *******************
	Increases the incrementation used for readability

	Inputs:		none
				  
	Outputs:	none

	Returns:	none

	Errors:		none
*********************************************************/
void IncIndentLevel( void )
{
	gpGlobals->numIndents++;
	if (gpGlobals->numIndents <= MAX_INDENT)
		gpGlobals->indentLeader[gpGlobals->numIndents-1] = '\t';
}

/******************** DecIndentLevel *******************
	Lessens the incrementation used for readability

	Inputs:		none
			  
	Outputs:	none

	Returns:	none

	Errors:		none
*********************************************************/
void DecIndentLevel(void)
{
	gpGlobals->numIndents--;
	if (gpGlobals->numIndents < 0)
		gpGlobals->numIndents = 0;
	if (gpGlobals->numIndents >= 0 && gpGlobals->numIndents < MAX_INDENT)
		gpGlobals->indentLeader[gpGlobals->numIndents] = 0;

}

// ============================================================================
// IsOMFFile
//  This function returns AAFRESULT_SUCCESS if the given file is an OMF file.
//  and the error code otherwise.
//
// ============================================================================
HRESULT IsOMFFile (char * pFileName )
{
	// verify if file exists
	gpGlobals->pLogger->Log( kLogInfo, "Verifying that \"%s\" is an OMF file\n", 
		pFileName );
	if ( strlen(pFileName) == 0)
	{
		return  AAFRESULT_NULL_PARAM;
	}

	FILE *pStream = fopen(pFileName, "r");
	if( pStream == NULL )
	{
		return AAFRESULT_BADOPEN;
	}

	HRESULT rc = AAFRESULT_FILE_NOT_OMF;
	int result = fseek(pStream, -24, SEEK_END);
	if( result == 0 )
	{	
		char		ReadBuffer[8];
		char		CompBuffer[8];
		CompBuffer[0] = (char)0xA4;
		CompBuffer[1] = 'C';
		CompBuffer[2] = 'M';
		CompBuffer[3] = (char)0xA5;
		CompBuffer[4] = 'H';
		CompBuffer[5] = 'd';
		CompBuffer[6] = 'r';
		CompBuffer[7] = 0x00;
		result = fread(ReadBuffer, sizeof( char ), sizeof(ReadBuffer), pStream);
		if (result > 0 && memcmp(CompBuffer, ReadBuffer, strlen(CompBuffer)) == 0)
		{
				rc = AAFRESULT_SUCCESS;
		}
	}

	fclose(pStream);
	return rc;
}

void RegisterCodecProperties(AafOmfGlobals *globals, omfSessionHdl_t OMFSession)
{
	OMFCheck	OMFError;

	// To get the CDCI codec related properties we first reister them in OMF
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "ComponentWidth", OMClassCDCI, 
									   OMVersionType, kPropRequired, 
									   &(globals->omCDCIComponentWidth));
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "HorizontalSubsampling", OMClassCDCI, 
									   OMBoolean, kPropRequired, 
									   &(globals->omCDCIHorizontalSubsampling));
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "ColorSiting", OMClassCDCI, 
									   OMBoolean, kPropRequired, 
									   &(globals->omCDCIColorSiting));
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "BlackReferenceLevel", OMClassCDCI, 
									   OMInt32, kPropRequired, 
									   &(globals->omCDCIBlackReferenceLevel));
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "WhiteReferenceLevel", OMClassCDCI, 
									   OMInt32, kPropRequired, 
									   &(globals->omCDCIWhiteReferenceLevel));
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "ColorRange", OMClassCDCI, 
									   OMInt32, kPropRequired, 
									   &(globals->omCDCIColorRange));
	OMFError = omfsRegisterDynamicProp(OMFSession, kOmfTstRevEither, 
									   "PaddingBits", OMClassCDCI, 
									   OMInt32, kPropRequired, 
									   &(globals->omCDCIPaddingBits));
}
