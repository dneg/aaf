/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "ImplAAFSourceClip.h"
#include "ImplAAFSourceMob.h"
#include "ImplAAFComponent.h"
#include "ImplAAFDataDef.h"
#include "ImplAAFFindSourceInfo.h"
#include "ImplAAFMasterMob.h"
#include "ImplAAFHeader.h"
#include "ImplAAFContentStorage.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafCvt.h"

extern "C" const aafClassID_t CLSID_AAFSourceClip;

ImplAAFMasterMob::ImplAAFMasterMob ()
{}


ImplAAFMasterMob::~ImplAAFMasterMob ()
{}

//***********************************************************
//
// AddMasterSlot()
//
// This function adds a slot to the specified Master Mob that
// references the specified a slot in the specified Source Mob. The
// new slot in the Master Mob contains a Source Clip that specifies
// the Source Mob in its source reference properties.
//
// The pDataDef parameter requires a data kind valid for a media
// stream. Valid data kinds are:
//
// - DDEF_Video
// - DDEF_Audio
//
// Note: If pSlotName is passed in with zero length, then the
// slot is not assigned a name.  Slot names are not used by the
// SDK, and exist only so the user can name slots.
// 
// Succeeds if all of the following are true:
// (more conditions here)
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - One or more of the following parameters are NULL pSourceMob,
//     pDataDef, and pSlotName.
//
// AAFRESULT_INVALID_DATAKIND
//   - The data kind of the source MOB slot to be added to the Master
//     Mob does not match what is specfied in pDataDef.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Source Mob slot was not found.
//
// AAFRESULT_SLOT_EXISTS
//   - The specified Master slot ID already exists.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::AddMasterSlot (aafUID_t*			pDataDef,
									 aafSlotID_t		sourceSlotID,
									 ImplAAFSourceMob*	pSourceMob,
									 aafSlotID_t		masterSlotID,
									 aafWChar*			pSlotName)
{
	aafLength_t	slotLength;
	aafUID_t	sourceMobID;
	HRESULT		hr;

	if (!pDataDef || !pSourceMob || !pSlotName)
		return AAFRESULT_NULL_PARAM;

	// Get the slot length and mob id.  Verify that data kind
	// of the slot is the same as pDataDef
	hr = pSourceMob->GetMobID(&sourceMobID);
	if (SUCCEEDED(hr))
	{
		ImplAAFMobSlot*	pMobSlot;

		hr = pSourceMob->FindSlotBySlotID(sourceSlotID, &pMobSlot);
		if (SUCCEEDED(hr))
		{
			ImplAAFSegment*	pSegment;

			hr = pMobSlot->GetSegment(&pSegment);
			if (SUCCEEDED(hr))
			{
				aafUID_t	DataDef;

				pSegment->GetLength(&slotLength);
				pSegment->GetDataDef(&DataDef);
				pSegment->ReleaseReference();
				pSegment = NULL;

				// Make sure the slot contains the expected media type.
				if (memcmp(&DataDef, pDataDef, sizeof(aafUID_t)) != 0)
					hr = AAFRESULT_INVALID_DATAKIND;
			}
			pMobSlot->ReleaseReference();
			pMobSlot = NULL;
		}
	}

	// Add the master slot
	if (SUCCEEDED(hr))
	{
		ImplAAFSourceClip*	pSrcClip = NULL;
		aafSourceRef_t		ref;
		aafPosition_t		zeroPos;

		CvtInt32toPosition(0, zeroPos);
		ref.sourceID = sourceMobID;
		ref.sourceSlotID = sourceSlotID;
		ref.startTime = zeroPos;

		pSrcClip = (ImplAAFSourceClip *)CreateImpl(CLSID_AAFSourceClip);
		if (pSrcClip)
		{
			ImplAAFMobSlot	*pNewSlot = NULL;

			hr = pSrcClip->InitializeSourceClip(pDataDef, &slotLength, ref);
			if (SUCCEEDED(hr))
			{
				hr = AppendNewSlot(pSrcClip, masterSlotID, pSlotName, &pNewSlot);
				if (SUCCEEDED(hr))
				{
					pNewSlot->ReleaseReference();
					pNewSlot = NULL;
				}
			}

			pSrcClip->ReleaseReference();
			pSrcClip = NULL;
		}
	}

	return hr;
}

//***********************************************************
//
// GetTapeName()
//
// Finds the videotape Source Mob associated with a Master Mob slot
// and writes the name of the videotape, which is stored in the
// Mobs Name property, into the pTapeName buffer.  The buffer is
// allocated by the caller.  The size of the buffer is given by
// bufSize.  If the property name has not yet been set, a
// zero-length string will be written (that is, only the trailing
// null character).
// 
// Caller may call GetTapeNameBufLen() to determine the required
// buffer size.
// 
// Succeeds if all of the following are true:
// - the pTapeName pointer is valid.
// - bufSize indicates the buffer is large enough to hold the name.
// 
// If this method fails nothing will be written to *pTapeName.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pTapeName arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// AAFRESULT_NOT_TAPEMOB
//   - The specified Master Slot does not contain a Tape MOB.
//
// AAFRESULT_SMALLBUF
//   - bufSize indicates the buffer is too small to hold the string.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeName (aafInt32		masterSlotID,
								   aafWChar*	pTapeName,
								   aafInt32		bufSize)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}


//***********************************************************
//
// GetTapeNameBufLen()
//
// Returns the length of buffer required for the GetTapeName()
// method.  The value is placed into the location specified by pLen.
// The value will include space required for the trailing null
// character.
//
// Succeeds if all of the following are true:
// - the pLen pointer is valid.
//
// If this method fails nothing will be written to *pLen.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pLen arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// AAFRESULT_NOT_TAPEMOB
//   - The specified Master Slot does not contain a Tape MOB.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeNameBufLen (aafInt32	masterSlotID,
										 aafInt32*  pLen)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// GetNumRepresentations()
//
// This function returns the number of media representations
// available for the specified SlotID on a specified Master
// Mob.
//
// Succeeds if all of the following are true:
// - the pNumReps pointer is valid.
//
// If this method fails nothing will be written to *pLen.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pNumReps arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetNumRepresentations (aafSlotID_t	slotID,
											 aafNumSlots_t*	pNumReps)
{
	ImplAAFMobSlot	*pSlot = NULL;
  ImplAAFSegment	*pSegment = NULL;
	HRESULT			hr;
	
	if (!pNumReps)
		return AAFRESULT_NULL_PARAM;

	*pNumReps = 0;
	
	hr = FindSlotBySlotID(slotID, &pSlot);
	if (SUCCEEDED(hr))
	{
		hr = pSlot->GetSegment(&pSegment);
		if (SUCCEEDED(hr))
		{
			hr = pSegment->NumRepresentations(pNumReps);
			pSegment->ReleaseReference();
			pSegment = NULL;
		}
		pSlot->ReleaseReference();
		pSlot = NULL;
	}

	return hr;
}

//***********************************************************
//
// GetRepresentationSourceClip()
//
// This method returns the indexed media representation for the specified
// Master Mob, SlotID, and index.  This call is meant to work
// with GetNumRepresentations, so that you can iterate through all
// of the choices yourself.  This method uses an integer index, not
// an iterator.  The function GetRepresentationSourceClip takes an
// index between 1 and the number of representations [inclusive],
// and returns the indexed Source Mob.
//
// Succeeds if all of the following are true:
// - the ppSourceClip pointer is valid.
//
// If this method fails nothing will be written to *ppSourceClip.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// AAFRESULT_BADINDEX
//   - No Source Mob at specified index.
//
// AAFRESULT_MISSING_MEDIA_REP
//   - TODO: add this to idl
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetRepresentationSourceClip (aafSlotID_t			slotID,
												   aafInt32				index,
												   ImplAAFSourceClip**	ppSourceClip)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// GetCriteriaSourceClip()
//
// Returns the Source Clip on the specified slot of a Master Mob
// that references the Source Mob that best meets the specified
// criteria.  This function will work whether multiple media
// representations exist or not.
//
// Note:
// The aafMediaCriteria_t is defined as the following structure:
// typedef struct
// {
//	aafCriteriaType_t type;
//	aafCriteriaProc_t proc;
// } aafMediaCriteria_t;
// 
// Note:
// The type field can have one of the following values: |
// typedef enum
// {
//	kAAFAnyRepresentation = 0,
//	kAAFFastestRepresentation,
//	kAAFBestFidelityRepresentation,
//	kAAFSmallestRepresentation,
//	kAAFUseRepresentationProc
// } aafCriteriaType_t;
//
// Succeeds if all of the following are true:
// - the ppSourceClip pointer is valid.
//
// If this method fails nothing will be written to *ppSourceClip.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetCriteriaSourceClip (aafSlotID_t			slotID,
											 aafMediaCriteria_t*	pCriteria,
											 ImplAAFSourceClip**	ppSourceClip)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}



//***********************************************************
//
// SearchSource()
//
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::SearchSource (aafSlotID_t				slotID,
									aafPosition_t			offset,
									aafMobKind_t			mobKind,
									aafMediaCriteria_t*		pMediaCrit,
									aafEffectChoice_t*		pEffectChoice,
									ImplAAFComponent**		ppThisCpnt,
									ImplAAFFindSourceInfo**	ppSourceInfo)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// GetMobKind()
//
// Returns kMasterMob in *pMobKind.
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kMasterMob;

	return AAFRESULT_SUCCESS;
}


extern "C" const aafClassID_t CLSID_AAFMasterMob;

OMDEFINE_STORABLE(ImplAAFMasterMob, CLSID_AAFMasterMob);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFMasterMob::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFMasterMob, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}

