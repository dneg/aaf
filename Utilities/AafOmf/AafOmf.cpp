// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

#ifdef macintosh
	#include <console.h>
#endif 
namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

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
#include "aafclassdefuids.h"
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

void MobIDtoString(aafMobID_t *uid, char *buf)
{
	sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x--%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
		(int)uid->SMPTELabel[0], (int)uid->SMPTELabel[1], (int)uid->SMPTELabel[2], (int)uid->SMPTELabel[3], 
		(int)uid->SMPTELabel[4], (int)uid->SMPTELabel[5], (int)uid->SMPTELabel[6], (int)uid->SMPTELabel[7], 
		(int)uid->SMPTELabel[8], (int)uid->SMPTELabel[8], (int)uid->SMPTELabel[10], (int)uid->SMPTELabel[11], 
		(int)uid->length, (int)uid->instanceHigh, (int)uid->instanceMid, (int)uid->instanceLow, 
		uid->material.Data1, uid->material.Data2, uid->material.Data3, (int)uid->material.Data4[0],
		(int)uid->material.Data4[1], (int)uid->material.Data4[2], (int)uid->material.Data4[3],
		(int)uid->material.Data4[4],
		(int)uid->material.Data4[5], (int)uid->material.Data4[6], (int)uid->material.Data4[7]);
}



struct SMPTELabel		// Change to match GUID to ensure correct byte swapping
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

union label
{
	aafMobID_t			mobID;
	struct SMPTELabel	smpte;
};

AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	major,
		aafUInt32	minor,
		aafMobID_t *mobID)     /* OUT - Newly created Mob ID */
{
	union label		aLabel;
	
	aLabel.smpte.oid = 0x06;
	aLabel.smpte.size = 0x0E;
	aLabel.smpte.ulcode = 0x2B;
	aLabel.smpte.SMPTE = 0x34;
	aLabel.smpte.Registry = 0x02;
	aLabel.smpte.unused = 0;
	aLabel.smpte.MobIDPrefixLow = 42;		// Means its an OMF Uid
	aLabel.smpte.MobIDPrefixHigh = 0;		// Means its an OMF Uid

	aLabel.smpte.MobIDMajor = major;
	aLabel.smpte.MobIDMinorLow = (aafUInt16)(minor & 0xFFFF);
	aLabel.smpte.MobIDMinorHigh =  (aafUInt16)((minor >> 16L) & 0xFFFF);

	*mobID = aLabel.mobID;
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

void RegisterCodecProperties(AafOmfGlobals *globals, OMF2::omfSessionHdl_t OMFSession)
{
	OMFCheck	OMFError;

	// To get the CDCI codec related properties we first reister them in OMF
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "ComponentWidth", OMClassCDCI, 
									   OMF2::OMVersionType, OMF2::kPropRequired, 
									   &(globals->omCDCIComponentWidth));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "HorizontalSubsampling", OMClassCDCI, 
									   OMF2::OMBoolean, OMF2::kPropRequired, 
									   &(globals->omCDCIHorizontalSubsampling));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "ColorSiting", OMClassCDCI, 
									   OMF2::OMBoolean, OMF2::kPropRequired, 
									   &(globals->omCDCIColorSiting));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "BlackReferenceLevel", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIBlackReferenceLevel));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "WhiteReferenceLevel", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIWhiteReferenceLevel));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "ColorRange", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIColorRange));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "PaddingBits", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIPaddingBits));
}
