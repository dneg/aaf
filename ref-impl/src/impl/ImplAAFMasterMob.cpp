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
// references the specified File Source Mob. The new slot in the
// Master Mob contains a Source Clip that specifies the File Source
// Mob in its source reference properties. Typically this is done
// automatically by passing the Master Mob handle to
// omfmMedia-Create, but this function allows you to add it later.
//
// Note: The mediaKind  parameter requires a data kind valid for a
// media stream. Valid data kinds are:
// - Picture
// - Sound
//
// Note: If pSlotName is passed in with zero length, then the
// slot is not assigned a name.  Slot names are not used by the
// Toolkit, and exist only so the user can name slots.
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
// (more results here)
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::AddMasterSlot (aafUID_t*			pDataDef,
									 aafSlotID_t		slotID,
									 aafSlotID_t		fileSlotID,
									 aafWChar*			pSlotName,
									 ImplAAFSourceMob*	pFileMob)
{
	ImplAAFSegment			*pSegment = NULL;
	ImplAAFSourceClip		*pSrcClip = NULL;
	ImplAAFMobSlot			*pMobSlot = NULL;
	aafLength_t				slotLength;
	aafPosition_t			zeroPos;
	aafSourceRef_t			ref;
	aafUID_t				fileMobID;
	aafUID_t				DataDef;

	if (!pDataDef || !pFileMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pFileMob->GetMobID(&fileMobID));
		CHECK(pFileMob->FindSlotBySlotID(fileSlotID, &pMobSlot));
		CHECK(pMobSlot->GetSegment(&pSegment));
		CHECK(pSegment->GetLength(&slotLength));
		CHECK(pSegment->GetDataDef(&DataDef));

		pSegment->ReleaseRef();
		pMobSlot->ReleaseRef();
		
		// Make sure the slot contains the expected media type.
		if (memcmp(&DataDef, pDataDef, sizeof(aafUID_t)) != 0)
			return AAFRESULT_INVALID_DATAKIND;

		CvtInt32toPosition(0, zeroPos);

		ref.sourceID = fileMobID;
		ref.sourceSlotID = fileSlotID;
		ref.startTime = zeroPos;

		pSrcClip = (ImplAAFSourceClip *)CreateImpl(CLSID_AAFSourceClip);
		if (pSrcClip)
		{
			ImplAAFMobSlot	*pNewSlot = NULL;

			CHECK(pSrcClip->InitializeSourceClip(pDataDef, &slotLength, ref));
			CHECK(AppendNewSlot(pSrcClip, slotID, pSlotName, &pNewSlot));

			pNewSlot->ReleaseRef();
			pSrcClip->ReleaseRef();
		}

	} /* XPROTECT */
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
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
// AAFRESULT_SMALLBUF
//   - bufSize indicates the buffer is too small to hold the string.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeName (aafInt32		masterSlotID,
								   aafWChar*	pTapeName,
								   aafInt32		bufSize)
{
#if FULLTOOLKIT
//	ImplAAFFindSourceInfo*	pSourceInfo;
	ImplAAFSourceClip*	pSrcClip = NULL;
	aafSourceRef_t		ref;
//	aafPosition_t			zeroPos;
	
	XPROTECT()
	{
		CHECK(GetRepresentationSourceClip (masterSlotID, 0, &pSrcClip));
		CHECK(pSrcClip->GetRef(&ref));


//		CvtInt32toPosition(0, zeroPos);

//		CHECK(SearchSource(masterSlotID, zeroPos, kTapeMob, NULL, NULL, NULL, &pSourceInfo));

		// TODO: Need to implement ImplAAFFindSourceInfo, there are currently no methods.
		//CHECK(pSourceInfo->mob->GetName(pTapeName, bufSize));

		// TODO: Release reference to source info
		//if (pSourceInfo)
		//	pSourceInfo->ReleaseObject();

	}
	XEXCEPT
	XEND
	
	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
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
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeNameBufLen (aafInt32	masterSlotID,
										 aafInt32*  pLen)
{
#if FULLTOOLKIT
	aafSlotID_t				slotID = 0;
 	aafPosition_t			zeroPos;
	ImplAAFFindSourceInfo*	pSourceInfo;
	
	XPROTECT()
	{
		CvtInt32toPosition(0, zeroPos);

		CHECK(GetNameLen(pLen));

		// TODO: need to implement ImplAAFFindSourceInfo
		CHECK(SearchSource(slotID, zeroPos, kTapeMob, NULL, NULL, NULL, &pSourceInfo));
		//CHECK(pSourceInfo->mob->GetNameLen(pLen));

		// TODO: Release reference to source info
		//if (pSourceInfo)
		//	pSourceInfo->ReleaseObject();
	}
	XEXCEPT
	XEND
	
	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



//***********************************************************
//
// GetNumRepresentations()
//
// This function returns the number of media representations
// available for the specified SlotID on a specified Master
// Mob. This function is meant to work with
// GetRepresentationSourceClip, so that you can iterate through
// all of the choices yourself.  In most cases, you can use
// GetCriteriaSourceClip to handle multiple
// representations. this function and
// GetRepresentationSourceClip are lower-level functions.
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
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetNumRepresentations (aafSlotID_t	slotID,
											 aafNumSlots_t*	pNumReps)
{
	ImplAAFMobSlot	*pSlot = NULL;
   	ImplAAFSegment	*pSegment = NULL;
	
	if (!pNumReps)
		return AAFRESULT_NULL_PARAM;

	*pNumReps = 0;
	
	XPROTECT()
	{
		CHECK(FindSlotBySlotID(slotID, &pSlot));
		CHECK(pSlot->GetSegment(&pSegment));
		CHECK(pSegment->NumRepresentations(pNumReps));
		pSegment->ReleaseRef();
		pSlot->ReleaseRef();
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
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
// and returns the indexed File Source Mob. You can make calls to
// functions such as omfmGetVideoInfo and omfmIsMediaContiguous to
// determine which media is the best fit.
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
// AAFRESULT_MISSING_MEDIA_REP
//   - add this to idl
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetRepresentationSourceClip (aafSlotID_t			slotID,
												   aafInt32				index,
												   ImplAAFSourceClip**	ppSourceClip)
{
#if FULLTOOLKIT
	ImplAAFMobSlot	*pSlot = NULL;
  	ImplAAFSegment	*pSegment = NULL;
	implCompType_t	compType;
	HRESULT			hr = AAFRESULT_SUCCESS;
	
	if (!ppSourceClip)
		return AAFRESULT_NULL_PARAM;

	*ppSourceClip = NULL;
		
	XPROTECT()
	{
		CHECK(FindSlotBySlotID(slotID, &pSlot));
		CHECK(pSlot->GetSegment(&pSegment));
		pSegment->GetCompType(&compType);
//		if(compType == kEssenceGroup)
//		{
//			aafNumSlots_t	numReps;
//
//			numReps = pSegment->GetObjRefArrayLength(OMMGRPChoices);
//			if(index >= 1 && index <= numReps)
//			{
//				ImplAAFObject	*pTmp;
//
//				CHECK(pSegment->ReadNthObjRefArray(OMMGRPChoices, &pTmp, index));
//				*sourceClip = (AAFSourceClip *)pTmp;
//			}
//			else
//				status = AAFRESULT_MISSING_MEDIA_REP;
//		}
//		else
		{
			*ppSourceClip = static_cast<ImplAAFSourceClip*> (pSegment);	//!!CASTING
		}

		// TODO: Release reference to segment if error occurred
		//if (pSegment && status != AAFRESULT_SUCCESS)
		//	pSegment->ReleaseObject();

		pSlot->ReleaseRef();
	}
	XEXCEPT
	XEND
	
	return hr;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



//***********************************************************
//
// GetCriteriaSourceClip()
//
// Returns the Source Clip on the specified slot of a Master Mob
// that references the File Source Mob that best meets the specified
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
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetCriteriaSourceClip (aafSlotID_t			slotID,
											 aafMediaCriteria_t*	pCriteria,
											 ImplAAFSourceClip**	ppSourceClip)
{
#if FULLTOOLKIT
	aafInt32				n, numReps, score, highestScore;
	aafBool				isAAF;
	ImplAAFFile *				dataFile;
	ImplAAFObject			*mdes;
	ImplAAFMob				*fileMob;
	ImplAAFSourceClip		*highestScoreSourceClip, *sourceClip;
	aafCodecID_t		codecID;
	aafCodecSelectInfo_t selectInfo;
	
	if ( !pCriteria || !ppSourceClip)
		return AAFRESULT_NULL_PARAM;

	highestScore = 0;
	highestScoreSourceClip = NULL;
	*ppSourceClip = NULL;
		
	XPROTECT()
	{
		CHECK(GetNumRepresentations(slotID, &numReps));
		for(n = 1; n <= numReps; n++)
		{
			CHECK(GetRepresentationSourceClip(slotID, n, &sourceClip));
			if(numReps == 1)
			{
				highestScoreSourceClip = sourceClip;
				break;
			}
			CHECK(sourceClip->ResolveRef(&fileMob));
				
			CHECK(((AAFFileMob *)fileMob)->ReadObjRef(OMSMOBMediaDescription, &mdes));
			codecID = NULL;
	
#if 0	//!!!
			CHECK(mdes->GetClassID(mdesTag));
			CHECK(TableFirstEntryMatching(_file->_session->_codecMDES, &iter, 
														 mdesTag, &more));
			while(more && (codecID == NULL))
			{
				codecPtr = (codecTable_t *)iter.valuePtr;
				CHECK(codecGetSelectInfo(file, codecPtr, mdes, &selectInfo));
				if(selectInfo.willHandleMDES)
				{
					CHECK(codecGetMetaInfo(file->session, codecPtr, NULL, 0,
													&meta));
					codecID = meta.codecID;
				}
			
				CHECK(TableNextEntry(&iter, &more));
			}
			if(codecID == NULL)
				continue;
#endif
				
			/* Check for locator file existance & continue if not present
			 * A file which is supposed to be an AAF file must be opened
			 * to check for the existance of the data object, so we must
			 * open the file here.
			 */
			CHECK(((AAFFileMob *)fileMob)->LocateMediaFile(&dataFile, &isAAF));
			if(dataFile == NULL)
				continue;
			if(dataFile != _file)
				dataFile->Close();

			score = 0;
			switch(criteria->type)
			{
			case kAAFAnyRepresentation:
				break;
				
			case kAAFFastestRepresentation:
				if(selectInfo.hwAssisted)
					score += 10;
				if(selectInfo.isNative)
					score += 10;
				break;
				
			case kAAFBestFidelityRepresentation:
				score = (100 - selectInfo.relativeLoss);
				break;
				
			case kAAFSmallestRepresentation:
				score = -1 * selectInfo.avgBitsPerSec;
				break;
				
			case kAAFUseRepresentationProc:
				score = (*criteria->proc)(_file, mdes, codecID);
				break;
			}
	
			if((score > highestScore) || (highestScoreSourceClip == NULL))
			{
				highestScore = score;
				highestScoreSourceClip = sourceClip;
			}
		}
	}
	XEXCEPT
	XEND
		
	*retSrcClip = highestScoreSourceClip;
	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::SearchSource (aafSlotID_t  /*slotID*/,
                           aafPosition_t  /*offset*/,
                           aafMobKind_t  /*mobKind*/,
                           aafMediaCriteria_t *  /*pMediaCrit*/,
                           aafEffectChoice_t *  /*pEffectChoice*/,
                           ImplAAFComponent ** /*ppThisCpnt*/,
                           ImplAAFFindSourceInfo ** /*ppSourceInfo*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



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

