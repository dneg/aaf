// @doc INTERNAL
// @com This file handles bdirectional translation of effect metadata between OMF and AAF

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

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

// OMF Includes

#include "AafOmf.h"
#include "EffectTranslate.h"
#include "AAFDomainUtils.h"
#include "OMFDomainUtils.h"
#include "AAFClassDefUIDs.h"
#include "AAFException.h"

#define CHECKAAF
#define CompareUUID(a, b) (memcmp((char *)&a, (char *)&b, sizeof(aafUID_t)) == 0)


static bool LookupAAFIDInTable(aafUID_t					&AAFEffectID,
							aafUInt32				numEntries,
							effectXlate_t			*table,
							OMF2::omfUniqueNamePtr_t effectID,
							OMF2::omfUniqueNamePtr_t ExtendedEffectID);
static void AddDirectionalPart(IAAFOperationGroup *effect,
									  OMF2::omfUniqueNamePtr_t ExtendedEffectID);
static void BuildWipeEffectID(IAAFOperationGroup *effect,
									  OMF2::omfUniqueNamePtr_t ExtendedEffectID);

EffectTranslate::EffectTranslate()
{
}

// ============================================================================
// Destructor
// ============================================================================
EffectTranslate::~EffectTranslate()
{
}
// ============================================================================
// GetEffectIDsFromAUID
//
//			This function converts an AAF effectID into an OMF unique name. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//			
// NOTE :	the buffer passed to this routine should be big enough to accomodate
//			the whole OMF name.!!
// ============================================================================


	effectXlate_t stdXlateTable[] = {
	{ &kAAFEffectVideoDissolve, "SimpleVideoDissolve", "BLEND_DISSOLVE" },
	{ &kAAFEffectVideoFadeToBlack, "VideoFadeToBlack", "BLEND_FADE_DOWN" },
	{ &kAAFEffectVideoSpeedControl, "VideoSpeedControl", "EFF_TIMEWARP.MOTION_CTL" },
	{ &kAAFEffectVideoRepeat, "VideoRepeat","EFF_TIMEWARP.REPEAT" },
	{ &kAAFEffectVideoFrameToMask, "VideoFrameMask","EFF_TIMEWARP.CAPTURE_MASK" },
	{ &kAAFEffectMonoAudioDissolve, "SimpleMonoAudioDissolve","BLEND_AUDIO_DISSOLVE" },
	{ &kAAFEffectMonoAudioPan, "MonoAudioPan","Audio Pan Volume" },
	{ &kAAFEffectMonoAudioGain, "MonoAudioGain","Audio Pan Volume" },
	{ &kAAFEffectStereoAudioDissolve, "StereoAudioDissolve" },
	{ &kAAFEffectSMPTEVideoWipe, "SMPTEVideoWipe", NULL },
	{ &kAAFEffectStereoAudioGain, "StereoAudioGain" } };
	long numStdEntries = sizeof(stdXlateTable)/sizeof(effectXlate_t);



	HRESULT EffectTranslate::GetEffectIDs(IAAFOperationGroup *effect,
									   OMF2::omfUniqueNamePtr_t effectID,
									   OMF2::omfUniqueNamePtr_t ExtendedEffectID)
{
	IAAFOperationDef		*pOpDef = NULL;
	IAAFDefObject			*pDef = NULL;
	long					numStdEntries = sizeof(stdXlateTable)/sizeof(effectXlate_t);
	bool					found = false;
	aafUID_t				AAFEffectID;

	try
	{
		AAFCheck rc;
		effectID[0] = '\0';
		ExtendedEffectID[0] = '\0';
		
		rc = effect->GetOperationDefinition(&pOpDef);
		rc = pOpDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef);
		rc = pDef->GetAUID(&AAFEffectID);


		found = LookupAAFIDInTable(AAFEffectID, numStdEntries,
							stdXlateTable, effectID, ExtendedEffectID);

		if(!found)
		{
			//		AUIDtoString(&Datadef, szAUID);
			//		printf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
			//		fprintf(stderr,"%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
			rc = AAFRESULT_INVALID_DATADEF;
		}
	}	// end try
	catch( ExceptionBase &e )
	{
		return e.Code();
	}

	return AAFRESULT_SUCCESS;
}

static bool LookupAAFIDInTable(aafUID_t					&AAFEffectID,
							aafUInt32				numEntries,
							effectXlate_t			*xlateTable,
							OMF2::omfUniqueNamePtr_t effectID,
							OMF2::omfUniqueNamePtr_t ExtendedEffectID)
{
	bool		found = false;
	aafUInt32	n;

	for(n = 0; (n < numEntries) && !found; n++)
	{
		if ( CompareUUID(AAFEffectID, *(xlateTable[n].aafID)))
		{
			if(xlateTable[n].omfEffectID != NULL)
			{
				strcpy(effectID, "omfi:effect:");
				strcat(effectID, xlateTable[n].omfEffectID);
			}
			else if(xlateTable[n].ExtendedEffectID != NULL)
				strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
				
			if(xlateTable[n].ExtendedEffectID != NULL)
				strcpy(ExtendedEffectID, xlateTable[n].ExtendedEffectID);
			else
				ExtendedEffectID[0] = '\0';
			found = true;
		}
	}

	return(found);
}


bool EffectTranslate::isPrivateEffect(aafUID_t& uid)
{
	long	n, numStdEntries = sizeof(stdXlateTable)/sizeof(effectXlate_t);
	bool	found = false;
	bool	result = true;

	for(n = 0; (n < numStdEntries) && !found; n++)
	{
		if ( CompareUUID(uid, *(stdXlateTable[n].aafID)))
		{
			result = (stdXlateTable[n].omfEffectID == NULL);
			found = true;
		}
	}

	return result;
}

HRESULT EffectTranslate::GetAAFEffectID(	OMF2::omfUniqueNamePtr_t OMFEffectIDPtr,
						OMF2::omfUniqueNamePtr_t ExtendedEffectIDPtr,
						aafUID_t	*aafUID)
{
	IAAFOperationDef	*pOpDef = NULL;
	IAAFDefObject		*pDef = NULL;
	IAAFParameter		*pParameter = NULL;
	IAAFConstantValue	*pCVal = NULL;
	HRESULT				rc = AAFRESULT_SUCCESS;
	long				n, numStdEntries = sizeof(stdXlateTable)/sizeof(effectXlate_t);
	bool				found = false;
	OMF2::omfUniqueName_t	OMFEffectID;
	OMF2::omfUniqueName_t	ExtendedEffectID;
	char				*init = (char *)aafUID;

	for(n = 0; n < sizeof(aafUID_t); n++)
		init[n] = 0;
	
	for(n = 0; (n < numStdEntries) && !found; n++)
	{
		if(stdXlateTable[n].omfEffectID != NULL)
			found = (strcmp(OMFEffectID, stdXlateTable[n].omfEffectID) == 0);
		else
			found = (strcmp(ExtendedEffectID, stdXlateTable[n].ExtendedEffectID) == 0);
		if (found)
		{
			*aafUID = *(stdXlateTable[n].aafID);			
		}
	}
	if(!found)
	{
		//		AUIDtoString(&Datadef, szAUID);
		//		printf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		//		fprintf(stderr,"%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		rc = AAFRESULT_INVALID_DATADEF;
	}

	return rc;
}

