// @doc INTERNAL
// @com This file handles bdirectional translation of effect metadata between OMF and AAF

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

#include "omPublic.h"
#include "omMedia.h"

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
							omfUniqueNamePtr_t effectID,
							omfUniqueNamePtr_t ExtendedEffectID);
static void AddDirectionalPart(IAAFOperationGroup *effect,
									  omfUniqueNamePtr_t ExtendedEffectID);
static void BuildWipeEffectID(IAAFOperationGroup *effect,
									  omfUniqueNamePtr_t ExtendedEffectID);

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
									   omfUniqueNamePtr_t effectID,
									   omfUniqueNamePtr_t ExtendedEffectID)
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
							omfUniqueNamePtr_t effectID,
							omfUniqueNamePtr_t ExtendedEffectID)
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

HRESULT EffectTranslate::GetAAFEffectID(	omfUniqueNamePtr_t OMFEffectIDPtr,
						omfUniqueNamePtr_t ExtendedEffectIDPtr,
						aafUID_t	*aafUID)
{
	HRESULT				rc = AAFRESULT_SUCCESS;
	long				n, numStdEntries = sizeof(stdXlateTable)/sizeof(effectXlate_t);
	bool				found = false;
	omfUniqueName_t	OMFEffectID;
	omfUniqueName_t	ExtendedEffectID;
	char				*init = (char *)aafUID;
	char				*effectPrefix = "omfi:effect:";
	long				prefixLen = strlen(effectPrefix);

	for(n = 0; n < sizeof(aafUID_t); n++)
		init[n] = 0;

	if(strncmp(OMFEffectIDPtr, effectPrefix, prefixLen) == 0)
		strcpy(OMFEffectID, OMFEffectIDPtr+prefixLen);
	else
		strcpy(OMFEffectID, OMFEffectIDPtr);
	if(ExtendedEffectIDPtr != NULL)
		strcpy(ExtendedEffectID, ExtendedEffectIDPtr);
	else
		ExtendedEffectID[0] = '\0';
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
		*aafUID = kAAFOperationUnknown;
	}

	return rc;
}

bool EffectTranslate::RequiresNestedScope(aafUID_t &effectDefAUID)
{
	return isPrivateEffect(effectDefAUID);
}

