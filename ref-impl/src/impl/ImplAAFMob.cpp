
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#ifndef __ImplAAFFile_h__
#include "ImplAAFFile.h"
#endif

#ifndef __ImplEnumAAFMobSlots_h__
#include "ImplEnumAAFMobSlots.h"
#endif

#ifndef __ImplEnumAAFMobComments_h__
#include "ImplEnumAAFMobComments.h"
#endif






#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#include <assert.h>
#include "AAFResult.h"

ImplAAFMob::ImplAAFMob ()
: _mobID(			PID_MOB_MOBID,			"MobID"),
  _name(			PID_MOB_NAME,			"Name"),
  _creationTime(    PID_MOB_CREATE_TIME,	"CreateTime"),
  _lastModified(    PID_MOB_MOD_TIME,		"ModTime"),
  _slots(			PID_MOB_SLOTS,			"Slots")
{}


ImplAAFMob::~ImplAAFMob ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::IsAPrimaryMob (aafBool *  /*retval*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobID (aafUID_t *pMobID)
{
	aafErr_t aafError = OM_ERR_NONE;

	XPROTECT()
	  {
		/* This will support both 1.x and 2.x */
		if (pMobID)
		  {
			*pMobID = _mobID;
		  }
		else
		  {
			RAISE(AAFRESULT_NULL_PARAM);
		  }
	  }

	XEXCEPT
	  {
	  }
	XEND;

	return(AAFRESULT_SUCCESS);
}


static void stringPropertyToAAFString(aafString_t *aafString, OMStringProperty& stringProperty)
{
  const char* string = stringProperty;
  aafString->length = stringProperty.length();
  aafString->value = new aafWChar[aafString->length + 1];
  mbstowcs(aafString->value, string, aafString->length);
  aafString->value[aafString->length] = L'\0';
  //!!!Leaking string?
}


static void AAFStringToStringProperty(OMStringProperty& stringProperty, aafString_t *aafString)
{
	char* string;
	string = new char[aafString->length + 1];	//!!!S/b more than we need
	wcstombs(string, aafString->value, aafString->length);
	stringProperty = string;
  //!!!Leaking string?
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetName (aafString_t *name)
{
	aafAssert(name != NULL, NULL, AAFRESULT_NULL_PARAM);
//!!!	if(_name != NULL)
//	{
		stringPropertyToAAFString(name, _name);
//	}
//	else if(strSize > 0)
//		*name = '\0';

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetEditRate (aafRational_t *  /*editRate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetModTime (aafTimeStamp_t *lastModified)
{
	aafAssert(lastModified != NULL, NULL, OM_ERR_NULL_PARAM);
	*lastModified = _lastModified;
	return(OM_ERR_NONE); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetCreateTime (aafTimeStamp_t *creationTime)
{
	aafAssert(creationTime != NULL, NULL, OM_ERR_NULL_PARAM);

	*creationTime = _creationTime;

	return(OM_ERR_NONE); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobInfo (aafString_t *  /*name*/,
                           aafTimeStamp_t *  /*lastModified*/,
                           aafTimeStamp_t *  /*creationTime*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetNumTracks (aafNumTracks_t *  /*numTracks*/)
{
#if FULL_TOOLKIT
   aafInt32 tmpNumTracks = 0, numSlots, loop;
    AAFMobSlot * slot = NULL;
    AAFObject * tmp = NULL;
	aafErr_t aafError = OM_ERR_NONE;

	*numTracks = 0;
	aafAssertValidFHdl(_file);

	XPROTECT(_file)
	  {
		/* kAAFRev2x */
		CHECK(GetNumSlots(&numSlots));
	
		for (loop = 1; loop <= numSlots; loop++)
		  {
			CHECK(ReadNthObjRefArray(OMMOBJSlots, &tmp, loop));
			slot = (AAFMobSlot *)tmp;	//!!CASTING
			if (slot->MobSlotIsTrack(&aafError))
			  {
				if (aafError == OM_ERR_NONE)
				  tmpNumTracks++;
				else
				  {
					RAISE(aafError);
				  }
			  }

			/* Release Bento reference, so the useCount is decremented */
			slot->ReleaseObject();
	       } /* for */
	  } /* XPROTECT */

	XEXCEPT
	  {
		*numTracks = 0;
		return(XCODE());
	  }
	XEND;

	*numTracks = tmpNumTracks;
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetNumSlots (aafNumSlots_t *  /*numSlots*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetNewProps (aafBool  /*isMasterMob*/,
                           aafString_t *  /*name*/,
                           aafBool  /*isPrimary*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetModTime (aafTimeStamp_t *modTime)
{
	_lastModified = *modTime;

	return(OM_ERR_NONE);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetPrimary (aafBool  /*isPrimary*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetIdentity (aafUID_t *  /*mobID*/)
{
#if FULL_TOOLKIT
    aafUID_t oldMobID;
    aafBool hasMobID = AAFFalse;
	AAFObject * head = NULL;
	AAFMob	*foundMob = NULL;
	aafInt32 index = 0;
	aafProperty_t prop = OMNoProperty;
	aafErr_t aafError = OM_ERR_NONE;
	AAFMob *mobPtr;
	
	aafAssertValidFHdl(_file);

	XPROTECT(_file)
	  {
		/* Remember the old mob ID so it can be removed from the hash table */
		hasMobID = IsPropertyPresent(OMMOBJMobID, OMUID);
		if(hasMobID)
		{
			CHECK(GetMobID(&oldMobID));
		}

		/* Does a mob with the new ID already exist?  If so, return error */
		aafError = _file->LookupMob(newMobID, &mobPtr) ;
		if(aafError == OM_ERR_NONE)
		  {
			if(mobPtr == NULL)
				{
				_mobID = newMobID;
				CHECK(_file->FillOutStubMob(newMobID, this));
				}
			else
			{
				RAISE(OM_ERR_DUPLICATE_MOBID);
			}
		  }
		else
		{
			_mobID = newMobID;

			/* Remove the hash table entry for the old mobID, and add new one */
		}
		
		/* Remove it last, so the old hash entry is still there on error */

		/* If there was a previous MobID, delete from hash table.  Also,
		 * delete from the mob index if it is a 1.x file.
		 */
		if (hasMobID)
		  {
			CHECK(_file->RemoveMobID(oldMobID));
		  }
	  } /* XPROTECT */

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;

	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetName (aafString_t *name)
{	
	AAFStringToStringProperty(_name, name);

	return(OM_ERR_NONE);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetEditRate (aafRational_t  /*editRate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


// skip virtual aafErr_t Verify(char *buf, validateData_t *result);
// What doe's this do?

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::IsMobKind (aafMobKind_t  /*mobKind*/,
                           aafBool *  /*result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendNewSlot (aafRational_t  /*editRate*/,
                           ImplAAFSegment * /*segment*/,
                           ImplAAFMobSlot ** /*newSlot*/)
{
#if FULL_TOOLKIT
	AAFMobSlot * tmpSlot = NULL;
	aafLength_t length = CvtInt32toLength(0, length);
	aafLength_t	mobLength = CvtInt32toLength(0, mobLength);
	aafErr_t aafError = OM_ERR_NONE;

	*newSlot = NULL;
	aafAssertValidFHdl(_file);
	aafAssert((segment != NULL), _file, OM_ERR_NULLOBJECT);

	XPROTECT(_file)
	  {
		tmpSlot = new AAFMobSlot(_file);
		  {
			CHECK(tmpSlot->WriteRational(OMMSLTEditRate, editRate));
			CHECK(tmpSlot->WriteObjRef(OMMSLTSegment, segment));
		
			/* Append new slot to mob */
			CHECK(AppendObjRefArray(OMMOBJSlots, tmpSlot));
		  }
	  } /* XPROTECT */

	XEXCEPT
	  {
	    tmpSlot->Delete();
		return(XCODE());
	  }
	XEND;

	*newSlot = tmpSlot;
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::AppendNewTrack (aafRational_t  /*editRate*/,
                           ImplAAFSegment * /*segment*/,
                           aafPosition_t  /*origin*/,
                           aafTrackID_t  /*trackID*/,
                           aafString_t *  /*trackName*/,
                           ImplAAFMobSlot ** /*newTrack*/)
{
#if FULL_TOOLKIT
	aafTDescObj_t trackDesc = NULL;
	AAFObject * tmpTrack = NULL;
	aafLength_t length = CvtInt32toLength(0, length);
	aafLength_t mobLength = CvtInt32toLength(0, mobLength);
	aafBool hasDesc = AAFFalse;
	AAFDataKind * datakind = NULL;
	aafErr_t aafError = OM_ERR_NONE;

	*newTrack = NULL;
	aafAssertValidFHdl(_file);
	aafAssert((segment != NULL), _file, OM_ERR_NULLOBJECT);

	XPROTECT(_file)
	  {
		tmpTrack = new AAFMobSlot(_file);
		/* NOTE: write unique labels, return error if not unique */
		  {
			CHECK(tmpTrack->WriteRational(OMMSLTEditRate, editRate));
			
			CHECK(tmpTrack->WriteObjRef(OMMSLTSegment, segment));
			
			CHECK(((AAFMobSlot *)tmpTrack)->SetTrackDesc(origin, trackID, trackName, &trackDesc));
			hasDesc = AAFTrue;
			
			/* Append new slot to mob */
			CHECK(AppendObjRefArray(OMMOBJSlots, tmpTrack));
		  }
	  } /* XPROTECT */

	XEXCEPT
	  {
		if (hasDesc)
		  trackDesc->Delete();
	    tmpTrack->Delete();
		return(XCODE());
	  }
	XEND;

	*newTrack = (AAFMobSlot *)tmpTrack;
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetAllMobSlots (ImplEnumAAFMobSlots ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendComment (aafString_t *  /*category*/,
                           aafString_t *  /*comment*/)
{
#if FULL_TOOLKIT
	aafProperty_t		prop;
	aafInt32			n, cnt;
	AAFObject			*attb, *head;
    char				oldCommentName[64];
    aafBool				commentFound;
    aafAttributeKind_t	kind;
	
	aafAssertValidFHdl(_file);
	aafAssert((category != NULL), _file, OM_ERR_NULLOBJECT);
	aafAssert((comment != NULL), _file, OM_ERR_NULLOBJECT);

	XPROTECT(_file)
	{
			head = this;
		
		{
			prop = OMMOBJUserAttributes;
		}

		commentFound = AAFFalse;
		cnt = head->GetNumAttributes(prop);
		for(n = 1; n <= cnt; n++)
		{
			CHECK(head->ReadNthAttribute(prop, &attb, n));
			CHECK(attb->ReadAttrKind(OMATTBKind, &kind));
			CHECK(attb->ReadString(OMATTBName, oldCommentName, sizeof(oldCommentName)));
			if((kind == kAAFObjectAttribute) &&
			   (strcmp(oldCommentName, comment) == 0))
			{
				commentFound = AAFTrue;
				CHECK(attb->WriteString(OMATTBName, comment));
				break;
			}
		}
			
		if(!commentFound)
		{
			attb = new AAFAttribute(_file);
			CHECK(attb->WriteAttrKind(OMATTBKind, kAAFStringAttribute));
			CHECK(attb->WriteString(OMATTBName, category));
			CHECK(attb->WriteString(OMATTBStringAttribute, comment));
			CHECK(head->AppendAttribute(prop, attb));
		}

	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND;
	
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetNumComments (aafUInt32 *  /*pEnum*/)
{
#if FULL_TOOLKIT
	aafAssert((numComments != NULL), _file, OM_ERR_NULLOBJECT);
	*numComments = 0;

	XPROTECT(_file)
	  {
	  	{
	  		*numComments = GetNumAttributes(OMMOBJUserAttributes);
	  	}
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND;

	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetComments (ImplEnumAAFMobComments ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToMobTimecode (ImplAAFSegment * /*tcTrackID*/,
                           aafPosition_t *  /*offset*/,
                           aafTimecode_t *  /*result*/)
{
#if FULL_TOOLKIT
  AAFMobSlot * slot = NULL;
  AAFPulldown *pdwn = NULL;
  AAFSegment *pdwnInput = NULL;
  AAFIterate *iterHdl = NULL;
  aafInt32 numSlots, loop;
  aafTimecode_t timecode;
  aafBool tcFound = AAFFalse, reverse = AAFFalse;
  aafUInt32	frameOffset;
  AAFDataKind * datakind = NULL;
  aafPosition_t newStart;
  aafInt32 start32;
  aafErr_t aafError = OM_ERR_NONE;
  
	memset(result, 0, sizeof(aafTimecode_t));
	memset(&timecode, 0, sizeof(aafTimecode_t));
	result->startFrame = 0;

	aafAssertValidFHdl(_file);

	XPROTECT(_file)
	{
		if(tcSeg == NULL)
		{
			AAFSegment *seg = NULL;
			
			/* Find timecode track in mob */
			iterHdl = new AAFIterate(_file);
			CHECK(GetNumSlots(&numSlots));
			for (loop = 1; loop <= numSlots; loop++)
			{
				CHECK(iterHdl->MobGetNextSlot(this, NULL, &slot));
				CHECK(slot->GetSegment(&seg));

				/* Verify that it's a timecode track by looking at the
				 * datakind of the track segment. 
				 */
				CHECK(seg->GetDatakind(&datakind));

				if (datakind->IsTimecodeKind(kExactMatch, &aafError))
					tcSeg = seg;
				else
					seg->ReleaseObject();	
			}
		}
		if (tcSeg == NULL)
		{
			RAISE(OM_ERR_TIMECODE_NOT_FOUND);
		}
			
		  CHECK(tcSeg->SegmentOffsetToTC(offset, &timecode, &tcFound))

		  /* Release Bento reference, so the useCount is decremented */
		  if (tcSeg)
			{
			  tcSeg->ReleaseObject();	
			  tcSeg = NULL;
			}
		  if (slot)
			{
			  slot->ReleaseObject();	
			  slot = NULL;
			}

	  /* Assume found at this point, so finish generating result */
	  /* If this is a Film Composer file that has a mask or pulldown object
	   * in the timecode track, pass the position through the mask/pulldown
	   * before adding it to the start timecode.
	   */
	  if (pdwn)
		{
		  reverse = AAFFalse;
		  CHECK(pdwn->MapOffset(offset, reverse, &newStart, NULL));
		  CHECK(TruncInt64toInt32(newStart, &start32));
		  timecode.startFrame += start32;
		}
	  else
	  {
	  	CHECK(TruncInt64toUInt32(offset, &frameOffset));
		timecode.startFrame += frameOffset;
	  }

	  *result = timecode;
	  if (iterHdl)
		{
			delete iterHdl;
		  iterHdl = NULL;
		}
	} /* XPROTECT */

  XEXCEPT
	{
	  if (iterHdl)
			delete iterHdl;
	  return(XCODE());
	}
  XEND;

  return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::FindTrackByTrackID (aafTrackID_t  /*trackID*/,
                           ImplAAFMobSlot ** /*destTrack*/)
{
#if FULL_TOOLKIT
  AAFIterate *mobIter = NULL;
  aafInt32 numTracks, loop;
  AAFMobSlot *tmpTrack = NULL;
  aafTrackID_t tmpTrackID;
  aafBool foundTrack = AAFFalse;

  XPROTECT(_file)
	{
	  *destTrack = NULL;

	  mobIter = new AAFIterate(_file);
	  CHECK(GetNumTracks(&numTracks));
	  for (loop = 1; loop <= numTracks; loop++)
		{
		  CHECK(mobIter->MobGetNextTrack(this, NULL, &tmpTrack));
		  CHECK(tmpTrack->GetTrackID(&tmpTrackID));
		  if (tmpTrackID == trackID)
			{
			  foundTrack = AAFTrue;
			  break;
			}
		}
	  delete mobIter;
	mobIter = NULL;
	  if (!foundTrack)
		{
		  RAISE(OM_ERR_TRACK_NOT_FOUND);
		}
	  *destTrack = tmpTrack;
	}
  XEXCEPT
	{
		if (mobIter)
		  delete mobIter;
	}
  XEND;

  return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToTimecode (aafTrackID_t*  /*trackID*/,
                           aafPosition_t *  /*offset*/,
                           aafTimecode_t *  /*result*/)
{
#if FULL_TOOLKIT
  AAFMobSlot			*slot = NULL;
  AAFObject			*subSegment = NULL;
  AAFSegment		*seg = NULL;
  AAFIterate 		*iterHdl = NULL;
  aafInt32 			numSlots, loop;
  aafTimecode_t 	timecode;
  aafMediaCriteria_t mediaCrit;
  AAFDataKind 		*datakind = NULL;
  aafErr_t 			aafError = OM_ERR_NONE;
  aafFindSourceInfo_t	sourceInfo;
  aafRational_t		editRate;
  aafPosition_t		frameOffset64;
  AAFIterate		*sequIter = NULL;
  aafLength_t		zeroLen = CvtInt32toLength(0, zeroLen);
  
  sourceInfo.mob = NULL;
  memset(result, 0, sizeof(aafTimecode_t));
  memset(&timecode, 0, sizeof(aafTimecode_t));
  result->startFrame = 0;
  aafAssertValidFHdl(_file);

  XPROTECT(_file)
	{
	  mediaCrit.type = kAAFAnyRepresentation;
	  CHECK(InternalMobSearchSource(trackID, offset, kTapeMob,
							  &mediaCrit, NULL, NULL, &sourceInfo));

	  iterHdl = new AAFIterate(_file);
	  CHECK(sourceInfo.mob->GetNumSlots(&numSlots));
	  for (loop = 1; loop <= numSlots; loop++)
		{
		  CHECK(iterHdl->MobGetNextSlot(sourceInfo.mob, NULL, &slot));
		  CHECK(((AAFMobSlot *)slot)->GetEditRate(&editRate));
		  CHECK(((AAFMobSlot *)slot)->GetSegment(&seg));

		  /* Verify that it's a timecode track by looking at the
			* datakind of the track segment.
			*/
		  CHECK(seg->GetDatakind(&datakind));
		  if (datakind->IsTimecodeKind(kExactMatch, &aafError))
			 {
				/* Assume found at this point, so finish generating result */
		  		CHECK(AAFConvertEditRate(sourceInfo.editrate, sourceInfo.position,
		  									editRate, kRoundCeiling, &frameOffset64));

				CHECK(sourceInfo.mob->OffsetToMobTimecode(seg, frameOffset64, &timecode));
				break;
			}
		} /* for */

	  /* Release Bento reference, so the useCount is decremented */
	  if (sourceInfo.mob)
		 sourceInfo.mob->ReleaseObject();

	  *result = timecode;
	  if (iterHdl)
		{
		  delete iterHdl;
		  iterHdl = NULL;
		}
	} /* XPROTECT */

  XEXCEPT
	{
	  if (iterHdl)
		delete iterHdl;
	  return(XCODE());
	}
  XEND;

  return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::TimecodeToOffset (aafTimecode_t  /*timecode*/,
                           aafTrackID_t  /*trackID*/,
                           aafFrameOffset_t *  /*result*/)
{
#if FULL_TOOLKIT
  AAFIterate *iterHdl = NULL, *sequIter = NULL;
  AAFMobSlot *slot = NULL;
  AAFObject *tmpSlot = NULL;
  AAFPulldown *pdwn = NULL;
  AAFSegment *seg;
  AAFSegment *pdwnInput = NULL;
  aafInt32 numSlots, loop;
  aafPosition_t zero;
  aafErr_t	aafError = OM_ERR_NONE;
  aafBool found = AAFFalse;
  aafRational_t	editRate;
  aafLength_t	zeroLen;
  aafFindSourceInfo_t	sourceInfo;

  aafAssertValidFHdl(_file);
  
  CvtInt32toPosition(0, zero);
  CvtInt32toLength(0, zeroLen);
  sourceInfo.mob = NULL;
  
  XPROTECT(_file)
	{
	  CHECK(InternalMobSearchSource(trackID, zero, kTapeMob,
							  NULL /* mediaCrit */, NULL, NULL, &sourceInfo));

	  iterHdl = new AAFIterate(_file);
	  CHECK(sourceInfo.mob->GetNumSlots(&numSlots));
   for (loop = 1; (loop <= numSlots) & !found; loop++)
		{
			CHECK(iterHdl->MobGetNextSlot(sourceInfo.mob, NULL, &slot));
			CHECK(((AAFMobSlot *)slot)->GetEditRate(&editRate));
			CHECK(((AAFMobSlot *)slot)->GetSegment(&seg));

			CHECK(seg->SegmentTCToOffset(timecode, editRate, result, &found));			  

			/* Release Bento reference, so the useCount is decremented */
			if (seg)
			  {
				 seg->ReleaseObject();	
				 seg = NULL;
			  }
			if (slot)
			  {
				 slot->ReleaseObject();	
				 slot = NULL;
			  }
		 } /* for */

	 if(!found)
	  {
		 RAISE(OM_ERR_TIMECODE_NOT_FOUND);
	  }

	  /* Release Bento reference, so the useCount is decremented */
	  if (sourceInfo.mob)
		 OMLReleaseObject((OMLObject)sourceInfo.mob);	
		
	  if (iterHdl)
		{
		  delete iterHdl;
		  iterHdl = NULL;
		}
	}

  XEXCEPT
	{
	  if(iterHdl)
	  	delete iterHdl;
	}
  XEND;

  return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::Copy (aafString_t *  /*destMobName*/,
                           ImplAAFMob ** /*destMob*/)
{
#if FULL_TOOLKIT
	AAFMob * newMob;
	aafBool isPrimary, isMaster;
	AAFHeader * head;
	aafErr_t aafError = OM_ERR_NONE;
	
	*destMob = NULL;
	aafAssert((_file->GetOpenType() != kOmOpenRead), _file, 
					OM_ERR_WRONG_OPENMODE);

	XPROTECT(_file)
	  {
		CHECK(CopyTree((AAFObject **)&newMob));

		/* MobSetNewProps will create a new mob ID, set name, and add
		 * to the Primary mobs index if needed
		 */

		isPrimary = IsAPrimaryMob(&aafError);
		isMaster = IsTypeOf("MMOB", &aafError);
		if (aafError == OM_ERR_NONE)
		  {
			CHECK(newMob->SetPrimary(isPrimary));
			CHECK(newMob->SetName(destMobName));
		  }
		else
		  {
			RAISE(aafError);
		  }

		/* Store the new mob in the HEAD Mobs */
		CHECK(_file->GetHeadObject(&head));
		CHECK(head->AddToMobList(newMob));
	  } /* XPROTECT */

	XEXCEPT
	  {
		/* NOTE: This function needs more cleanup (delete mob, out of index) */
		return(XCODE());
	  }
	XEND;

	*destMob = newMob;
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


static aafBool IsThisSCLP(
						  AAFFile *file,    /* IN - File Handle */
						  AAFObject *obj,  /* IN - Object to match */
						  void *data)       /* IN/OUT - Match Data */
{
#if FULL_TOOLKIT
  aafSourceRef_t *matchRef = (aafSourceRef_t *)data;
  aafSourceRef_t sourceRef;
  aafErr_t aafError = OM_ERR_NONE;

  if (obj->IsTypeOf("SCLP", &aafError))
    {
      aafError = ((AAFSourceClip *)obj)->GetRef(&sourceRef);
      if (equalUIDs(sourceRef.sourceID, (*matchRef).sourceID))
		return(AAFTrue);
      else 
		return(AAFFalse);
    }
  else
    return(AAFFalse);
#else
  return AAFFalse;
#endif
}

static aafErr_t LocalChangeRef(
						  AAFFile *file,    /* IN - File Handle */
						  AAFObject *obj,  /* IN - Object to execute */
						  aafInt32 level,   /* IN - Depth level */
						  void *data)       /* IN/OUT - Execute data */
{
#if FULL_TOOLKIT
  aafUID_t *newMobID = (aafUID_t *)data;
  aafSourceRef_t sourceRef;
  aafErr_t aafError = OM_ERR_NONE;

  XPROTECT(file)
	{
	  if(obj->IsTypeOf("SCLP", &aafError))
		{
		  if (aafError == OM_ERR_NONE)
			{
			  CHECK(((AAFSourceClip *)obj)->GetRef(&sourceRef));
			  copyUIDs(sourceRef.sourceID, (*newMobID));
			  CHECK(((AAFSourceClip *)obj)->SetRef(sourceRef));
			}
		  else
			{
			  RAISE(aafError);
			}
		}
    } /* XPROTECT */

  XEXCEPT
	{
	  return(XCODE());
	}
  XEND;

  return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::ChangeRef (aafUID_t *  /*oldMobID*/,
                           aafUID_t *  /*newMobID*/)
{
#if FULL_TOOLKIT
    aafInt32 matches;

	XPROTECT(_file)
	  {
		CHECK(MobMatchAndExecute(0, /* level */
		      IsThisSCLP, &oldMobID,
		      LocalChangeRef, &newMobID, 
		      &matches));
	  }

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;

	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



 /*************************************************************************
 * Function: CloneExternal()
 *
 *      This function clones the given mob in the source file into
 *      a destination mob in the destination file with the same Mob ID.
 *      If resolveDependencies is AAFTrue, it will also clone all mobs 
 *      referenced by the given source mob. 
 *
 *      If includeMedia is AAFTrue, it will also copy the media data 
 *      associated with the source mob.  The destination mob is
 *      returned. All private data is also cloned.
 *
 *      If the media data is not in the file, it will not attempt to
 *      try and find it in another file and clone it.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::CloneExternal (aafDepend_t  /*resolveDependencies*/,
                           aafIncMedia_t  /*includeMedia*/,
                           ImplAAFFile * /*destFile*/,
                           ImplAAFMob ** /*destMob*/)
{
#if FULL_TOOLKIT
    aafUID_t saveMobID, newMobID;
	aafString destMobName = NULL;
    AAFMob * tmpDestMob = NULL;
	AAFObject *tmpDestMedia = NULL;
	AAFHeader *destHead = NULL;
	aafTimeStamp_t saveCreationTime, saveModTime;
	aafFileRev_t srcRev, destRev;
	aafErr_t aafError = OM_ERR_NONE;

	*destMob = NULL;

	XPROTECT(_file)
	  {
		CHECK(_file->GetRevision(&srcRev));
		CHECK(destFile->GetRevision(&destRev));
		if (srcRev != destRev)
		  {
			RAISE(OM_ERR_FILEREV_DIFF);
		  }

		/* Copy the Mob, then change the MobID back to its original */
		/* Remember the creation and modification times, to restore later */
		CHECK(GetMobID(&saveMobID));
		CHECK(GetModTime(&saveModTime));
		CHECK(GetCreateTime(&saveCreationTime));

		destMobName = (aafString)_file->omOptMalloc(OMMOBNAME_SIZE);
		strncpy(destMobName, "\0", 1);
		  {
			aafError = ReadString(OMMOBJName, 
									  (char *)destMobName, OMMOBNAME_SIZE);
		  }

		CHECK(CopyExternal(resolveDependencies,
								  includeMedia, destMobName, destFile,
								  &tmpDestMedia, &tmpDestMob));
		CHECK(tmpDestMob->GetMobID(&newMobID));

		 /* omfiMobSetIdentity will take out the existing hash table
		 * entry and add an entry for the original cloned mobID.
		 */
		CHECK(tmpDestMob->SetIdentity(saveMobID));

		/* Restore creation and modification times */
		  {
			CHECK(tmpDestMob->WriteTimeStamp(OMMOBJCreationTime,
									 saveCreationTime));
		  }
		CHECK(tmpDestMob->WriteTimeStamp(OMMOBJLastModified,
								 saveModTime)); 

		/* If it is a primary mob, put it in the primary mob index */
		if (IsAPrimaryMob(&aafError))
		  {
			if (aafError == OM_ERR_NONE)
			  {
				CHECK(tmpDestMob->SetPrimary(AAFTrue));
			  }
			else
			  {
				RAISE(aafError);
			  }
		  }
		if (destMobName)
		  _file->omOptFree(destMobName);
		destMobName = NULL;

		if (includeMedia)
		  {
			/* The media data object has already been copied.  Now, we have
			 * to change the MobID back to the original, since we're cloning 
			 */
			if (tmpDestMedia)
			  {
				/* Change the mob ID to be the original mob ID */
				  {
					CHECK(tmpDestMedia->WriteUID(OMMDATMobID,saveMobID));
				  }

//!!!Need to put this back				CHECK(TableRemoveUID(destHead->_dataObjs, newMobID));
				/* Add to the MediaData hash table */
				CHECK(destHead->AppendDataObject(saveMobID, tmpDestMedia));
			  }

			/* If the media data is not in this file, don't report an error */
		  }

	  }
	XEXCEPT
	  {
		if (destMobName)
		  _file->omOptFree(destMobName);
		/* NOTE: This function needs more cleanup (delete mob, out of index) */
		if (XCODE() == OM_ERR_TABLE_DUP_KEY)
		  return(OM_ERR_DUPLICATE_MOBID);
		return(XCODE());
	  }
	XEND;

	*destMob = tmpDestMob;
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


  // Override from AAFObject
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::Delete ()
  {
#if FULL_TOOLKIT
	aafInt32 index;
	aafBool isComp = AAFFalse;
	mobTableEntry_t *entry;
	AAFMob	*test;
	aafErr_t aafError = OM_ERR_NONE;

	XPROTECT(_file)
	{
		CHECK(_head->FindMobInIndex(_mobID, OMHEADMobs, &index, &test));
		//!!Later, assert that test == this

		/* Take out of Primary Mob index, if necessary */
		if (IsAPrimaryMob(&aafError))
		  {
			if (aafError == OM_ERR_NONE)
			  {
				CHECK(SetPrimary(AAFFalse));
			  }
			else
			  {
				RAISE(aafError);
			  }
		  }

		/* Delete out of mob cache */
		entry = (mobTableEntry_t *)TableUIDLookupPtr(_head->_mobs, _mobID);
		/* Tell Bento that cache's object reference has been deleted */
		if (entry)
		  _container->OMLReleaseObject(entry->mob->_obj);
		CHECK(TableRemoveUID(_head->_mobs, _mobID));

		/* Remove from Mobs index */
			CHECK(_head->RemoveNthObjRefArray(OMHEADMobs, index));

		/* Delete Object */
		CHECK(DeleteTree());
	}

	XEXCEPT
	  {
		/* NOTE: This function needs more cleanup (delete mob, out of index) */
		return(XCODE());
	  }
	XEND;

	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
  }

  // @commDeletes the entire Mob structure \(the MOBJ object and all its contained objects\)
  // and deletes the entry from the Header.




////////////////////////////////////////////////////////////////////////////////
