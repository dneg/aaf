//@doc
//@class    AAFMob | Implementation class for AAFMob
#ifndef __ImplAAFMob_h__
#define __ImplAAFMob_h__

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
 * prior written permission of Avid Technology, Inc.
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

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#ifndef __ImplAAFTimelineMobSlot_h__
#include "ImplAAFTimelineMobSlot.h"
#endif

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#ifndef __ImplEnumAAFTaggedValues_h__
#include "ImplEnumAAFTaggedValues.h"
#endif

#include "OMDataTypes.h"

class ImplAAFSegment;

class ImplAAFMobSlot;

class ImplAAFFile;

class ImplEnumAAFMobSlots;

class ImplEnumAAFTaggedValues;

class ImplAAFFindSourceInfo;

class ImplAAFScopeStack;

class ImplAAFOperationGroup;




#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFMob : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMob ();
  ~ImplAAFMob ();


  //****************
  // GetMobID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMobID
        (aafMobID_t *  pMobID);  //@parm [out] The unique media object id


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (aafCharacter *  name,  //@parm [in] Mob Name
		aafUInt32 bufSize);	  //@parm [in] size of the buffer required to hold Mob Name + terminator
  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        (aafUInt32 *  nameLen);  //@parm [in,out] Mob Name length

  //****************
  // AppendSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendSlot
        (ImplAAFMobSlot *  pSlot);  //@parm [in,out] slot to append

  //****************
  // PrependSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependSlot
        (ImplAAFMobSlot *  pSlot);  //@parm [in,out] slot to prepend

  //****************
  // InsertSlotAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertSlotAt
        (aafUInt32 index,			//@parm [in] index to insert
		 ImplAAFMobSlot *  pSlot);  //@parm [in] slot to insert

  //****************
  // RemoveSlotAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSlotAt
        (aafUInt32 index);  //@parm [in] index of slot to remove

  //****************
  // GetSlotAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSlotAt
        (aafUInt32 index,		     //@parm [in] index to of slot get
		 ImplAAFMobSlot ** ppSlot);  //@parm [out] returned slot

  //****************
  // LookupSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupSlot
        (aafSlotID_t slotId,	     //@parm [in] ID of slot to get
		 ImplAAFMobSlot ** ppSlot);  //@parm [out] returned slot

  //****************
  // RemoveSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSlot
        (ImplAAFMobSlot *  pSlot);  //@parm [in,out] Mob Name length
   
  //****************
  // GetModTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetModTime
        (aafTimeStamp_t *  lastModified);  //@parm [out] Modified Time


  //****************
  // GetCreateTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCreateTime
        (aafTimeStamp_t *  creationTime);  //@parm [out] Creation Time


  //****************
  // GetMobInfo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMobInfo
        (aafTimeStamp_t *  lastModified,   //@parm [out] Modified Time
         aafTimeStamp_t *  creationTime,  //@parm [out] Creation Time
		 aafCharacter *  name,   //@parm [in,out] Mob Name
		 aafUInt32  strSize);

  //****************
  // CountSlots()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountSlots
        (aafNumSlots_t *  numSlots);  //@parm [out] Number of slots

  //****************
  // SetModTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetModTime
        (aafTimeStamp_constref modTime);  //@parm [in, ref] New Modification Time

  //****************
  // SetCreateTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCreateTime
        (aafTimeStamp_constref createTime);  //@parm [in, ref] New Creation Time

  //****************
  // SetIdentity()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMobID
        (aafMobID_constref  mobID);  //@parm [in, ref] New Mob ID


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (const aafCharacter *  name);  //@parm [in, ref] Mob Name


// skip virtual aafErr_t Verify(char *buf, validateData_t *result);
// What doe's this do?



  //****************
  // AppendNewSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewSlot
        (ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         const aafCharacter *  slotName,   //@parm [in] Slot Name (optional)
		 ImplAAFMobSlot ** newSlot);  //@parm [out] Newly created slot


  //****************
  // AppendNewSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewTimelineSlot
        (const aafRational_t &editRate,   //@parm [in] Edit rate property value
		 ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         const aafCharacter *  slotName,   //@parm [in] Slot Name (optional)
		 aafPosition_t  origin,
		 ImplAAFTimelineMobSlot ** newSlot);  //@parm [out] Newly created slot


  //****************
  // GetAllMobSlots()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSlots
        (ImplEnumAAFMobSlots ** ppEnum);  //@parm [out,retval] Mob Slot Enumeration


  //****************
  // AppendComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendComment
        (const aafCharacter *  category,   //@parm [in,ref] Comment heading
		 const aafCharacter *  comment);  //@parm [in,ref] Comment value

  //****************
  // RemoveComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveComment
        (ImplAAFTaggedValue * comment);

  //****************
  // CountComments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountComments
        (aafUInt32 *  pNumComments);  //@parm [out,retval] Number  of Mob Comments


  //****************
  // GetComments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComments
        (ImplEnumAAFTaggedValues ** ppEnum);  //@parm [out,retval] Mob Comments



  //****************
  // OffsetToMobTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OffsetToMobTimecode
        (ImplAAFSegment * tcSlotID,   //@parm [in] Current slot
		 aafPosition_t *  offset,   //@parm [in,ref] Offset into segment
         aafTimecode_t *  result);  //@parm [out] The resulting timecode


  //****************
  // FindSlotBySlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    FindSlotBySlotID
        (aafSlotID_t  slotID,   //@parm [in] The requested slot id
		 ImplAAFMobSlot ** destSlot);  //@parm [out] The requested slot


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
  //****************
  // OffsetToTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OffsetToTimecode
        (aafSlotID_t*  slotID,   //@parm [in,ref] Slot ID of the slot in the input mob
		 aafPosition_t *  offset,   //@parm [in] Offset into the given slot
         aafTimecode_t *  result);  //@parm [out] The resulting timecode


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
  //****************
  // TimecodeToOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    TimecodeToOffset
        (aafTimecode_t  timecode,   //@parm [in] The timecode value
		 aafSlotID_t  slotID,   //@parm [in] Slot ID of slot in source mob
         aafFrameOffset_t *  result);  //@parm [out] Resulting offset in source slot




  //****************
  // Copy()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Copy
        (const aafCharacter *  destMobName,   //@parm [in] Optional Input. The name to be assigned to the new copy of the Mob.
  // The destMobName argument is optional. Specify a NULL value if no desti-
  // nation Mob name is desired.
		 ImplAAFMob ** destMob);  //@parm [out] Destination Mob


  //****************
  // ChangeRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ChangeRef
        (aafMobID_constref oldMobID,   //@parm [in,ref] Old Mob ID reference in source clip
		 aafMobID_constref newMobID);  //@parm [in,ref] New Mob ID reference in source clip



  //****************
  // CloneExternal()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CloneExternal
        (aafDepend_t  resolveDependencies,   //@parm [in] Whether to clone dependent mobs
		 aafIncMedia_t  includeMedia,   //@parm [in] Whether to include media data
         ImplAAFFile * destFile,   //@parm [in] Destination AAF File
		 ImplAAFMob ** destMob);  //@parm [out] Destination Mob

  // @commDeletes the entire Mob structure \(the MOBJ object and all its contained objects\)
  // and deletes the entry from the Header.

public:
	// Interfaces visible inside the toolkit, but not exposed through the API
AAFRESULT
    GetNthMobSlot (aafInt32 index /* 0-based*/, ImplAAFMobSlot **ppMobSlot);
  //****************
  // AddPhysSourceRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddPhysSourceRef
        (aafAppendOption_t  addType,
		 aafRational_t  editrate,
		 aafSlotID_t  aMobSlot,
         const aafUID_t & essenceKind,
		 aafSourceRef_t  ref,
		 aafLength_t  srcRefLength);

	virtual AAFRESULT InternalSearchSource(	
    aafSlotID_t trackID,             /* IN */
	aafPosition_t offset,             /* IN */
	aafMobKind_t mobKind,             /* IN */
	aafMediaCriteria_t *pMediaCrit,    /* IN */
	aafOperationChoice_t *pOperationChoice,  /* IN */  /* NOTE: take this arg out? */
	ImplAAFFindSourceInfo **ppSourceInfo);  /* OUT */

	virtual AAFRESULT MobFindLeaf(ImplAAFMobSlot *track,
					 aafMediaCriteria_t *mediaCrit,
					 aafOperationChoice_t *operationChoice,
					 ImplAAFComponent *rootObj,
					 aafPosition_t rootPos,
					 aafLength_t rootLen,
					 ImplAAFComponent	*prevObject,
					 ImplAAFComponent *nextObject,
					 ImplAAFScopeStack *scopeStack,
					 aafPosition_t	currentObjPos,
					 ImplAAFComponent **foundObj,
					 aafLength_t *minLength,
					 aafBool *foundTransition,
					 ImplAAFOperationGroup **groupObject,
					 aafInt32	*nestDepth,
					 aafPosition_t *diffPos);

	virtual AAFRESULT FindNextMob(ImplAAFMobSlot *track, 
					 ImplAAFSegment *segment,
					 aafLength_t length,
					 aafPosition_t diffPos,
					 ImplAAFMob **retMob,
					 aafSlotID_t *retTrackID,
					 aafPosition_t *retPos,
					 ImplAAFPulldown **pulldownObj,
					 aafInt32 *pulldownPhase,
					 aafLength_t *retLen);

virtual AAFRESULT MobFindSource(
					   aafSlotID_t trackID,
					   aafPosition_t offset, /* offset in referenced units */
					   aafLength_t length,   /* expected length of clip */
					   aafMobKind_t mobKind,
					   aafMediaCriteria_t *mediaCrit,
					   aafOperationChoice_t *operationChoice,
					   ImplAAFFindSourceInfo *sourceInfo,
					   aafBool *foundSource);

virtual AAFRESULT STDMETHODCALLTYPE
    GetMobKind (aafMobKind_t *pMobKind);

  // SDK Internal
  virtual AAFRESULT ReconcileMobLength(void);
  virtual const OMMaterialIdentification& identification(void) const;

	protected:
	OMFixedSizeProperty<aafMobID_t>		_mobID;
	OMWideStringProperty				_name;
//!!! Creation time and last modified should be OMStructuredProperty
	OMFixedSizeProperty<aafTimeStamp_t>	_creationTime;
	OMFixedSizeProperty<aafTimeStamp_t>	_lastModified;
	private:
    OMStrongReferenceVectorProperty<ImplAAFMobSlot> _slots;
    OMStrongReferenceVectorProperty<ImplAAFTaggedValue> _userComments;
};

#endif // ! __ImplAAFMob_h__



////////////////////////////////////////////////////////////////////////////////
