//@doc
//@class    AAFMob | Implementation class for AAFMob
#ifndef __ImplAAFMob_h__
#define __ImplAAFMob_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#ifndef __ImplAAFTimelineMobSlot_h__
#include "ImplAAFTimelineMobSlot.h"
#endif

const int PID_MOB_MOBID			= 0;
const int PID_MOB_NAME			= 1;
const int PID_MOB_CREATE_TIME	= 2;
const int PID_MOB_MOD_TIME		= 3;
const int PID_MOB_SLOTS			= 4;


class ImplAAFSegment;

class ImplAAFMobSlot;

class ImplAAFFile;

class ImplEnumAAFMobSlots;

class ImplEnumAAFMobComments;

class ImplAAFFindSourceInfo;

class ImplAAFScopeStack;

class ImplAAFEffectInvocation;





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

  OMDECLARE_STORABLE(ImplAAFMob)

  //****************
  // IsAPrimaryMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsAPrimaryMob
        (aafBool *  retval);  //@parm [retval][out] Set to true if this is a primary mob


  //****************
  // GetMobID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMobID
        (aafUID_t *  pMobID);  //@parm [out] The unique media object id


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (aafWChar *  name,  //@parm [in] Mob Name
		aafInt32 bufSize);	  //@parm [in] size of the buffer required to hold Mob Name + terminator
  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        (aafInt32 *  nameLen);  //@parm [in,out] Mob Name length

  //****************
  // AppendSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendSlot
        (ImplAAFMobSlot *  pSlot);  //@parm [in,out] Mob Name length
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
		 aafWChar *  name,   //@parm [in,out] Mob Name
		 aafInt32  strSize);

  //****************
  // GetNumSlots()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumSlots
        (aafNumSlots_t *  numSlots);  //@parm [out] Number of slots



  //****************
  // SetNewProps()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetNewProps
        (aafBool  isMasterMob,   //@parm [in] Whether or not this is a Master Mob
		 aafWChar *  name,   //@parm [in,ref] Mob Name (optional)
         aafBool  isPrimary);  //@parm [in] Whether or not this is a primary mob


  //****************
  // SetModTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetModTime
        (aafTimeStamp_t *  modTime);  //@parm [in, ref] New Modification Time


  //****************
  // SetPrimary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPrimary
        (aafBool  isPrimary);  //@parm [in] Whether or not the mob is a primary mob


  //****************
  // SetIdentity()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMobID
        (aafUID_t *  mobID);  //@parm [in, ref] New Mob ID


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafWChar *  name);  //@parm [in, ref] Mob Name


// skip virtual aafErr_t Verify(char *buf, validateData_t *result);
// What doe's this do?



  //****************
  // AppendNewSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewSlot
        (ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         aafWChar *  slotName,   //@parm [in] Slot Name (optional)
		 ImplAAFMobSlot ** newSlot);  //@parm [out] Newly created slot


  //****************
  // AppendNewSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewTimelineSlot
        (aafRational_t  editRate,   //@parm [in] Edit rate property value
		 ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         aafWChar *  slotName,   //@parm [in] Slot Name (optional)
		 aafPosition_t  origin,
		 ImplAAFTimelineMobSlot ** newSlot);  //@parm [out] Newly created slot


  //****************
  // GetAllMobSlots()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFAllMobSlots
        (ImplEnumAAFMobSlots ** ppEnum);  //@parm [out,retval] Mob Slot Enumeration


  //****************
  // AppendComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendComment
        (aafWChar *  category,   //@parm [in,ref] Comment heading
		 aafWChar *  comment);  //@parm [in,ref] Comment value

  //****************
  // RemoveComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveComment
        (aafMobComment_t *  comment);

  //****************
  // GetNumComments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumComments
        (aafUInt32 *  pEnum);  //@parm [out,retval] Number  of Mob Comments


  //****************
  // GetComments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFAllMobComments
        (ImplEnumAAFMobComments ** ppEnum);  //@parm [out,retval] Mob Comments



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
        (aafWChar *  destMobName,   //@parm [in] Optional Input. The name to be assigned to the new copy of the Mob.
  // The destMobName argument is optional. Specify a NULL value if no desti-
  // nation Mob name is desired.
		 ImplAAFMob ** destMob);  //@parm [out] Destination Mob


  //****************
  // ChangeRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ChangeRef
        (aafUID_t *  oldMobID,   //@parm [in,ref] Old Mob ID reference in source clip
		 aafUID_t *  newMobID);  //@parm [in,ref] New Mob ID reference in source clip



  //****************
  // CloneExternal()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CloneExternal
        (aafDepend_t  resolveDependencies,   //@parm [in] Whether to clone dependent mobs
		 aafIncMedia_t  includeMedia,   //@parm [in] Whether to include media data
         ImplAAFFile * destFile,   //@parm [in] Destination AAF File
		 ImplAAFMob ** destMob);  //@parm [out] Destination Mob


  // Override from AAFObject
  virtual AAFRESULT STDMETHODCALLTYPE
    Delete ();

  // @commDeletes the entire Mob structure \(the MOBJ object and all its contained objects\)
  // and deletes the entry from the Header.

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFMobTest.cpp.
  static AAFRESULT test();

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
         aafUID_t * pEssenceKind,
		aafSourceRef_t  ref,
        aafLength_t  srcRefLength);

	virtual AAFRESULT InternalSearchSource(	
    aafSlotID_t trackID,             /* IN */
	aafPosition_t offset,             /* IN */
	aafMobKind_t mobKind,             /* IN */
	aafMediaCriteria_t *pMediaCrit,    /* IN */
	aafEffectChoice_t *pEffectChoice,  /* IN */  /* NOTE: take this arg out? */
	ImplAAFFindSourceInfo **ppSourceInfo);  /* OUT */

	virtual AAFRESULT MobFindLeaf(ImplAAFMobSlot *track,
					 aafMediaCriteria_t *mediaCrit,
					 aafEffectChoice_t *effectChoice,
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
					 ImplAAFEffectInvocation **effeObject,
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
					   aafEffectChoice_t *effectChoice,
					   ImplAAFFindSourceInfo *sourceInfo,
					   aafBool *foundSource);

virtual AAFRESULT STDMETHODCALLTYPE
    GetMobKind (aafMobKind_t *pMobKind);

  // SDK Internal
  virtual AAFRESULT ReconcileMobLength(void);

	protected:
	OMFixedSizeProperty<aafUID_t>		_mobID;
	OMWideStringProperty				_name;
//!!! Creation time and last modified should be OMStructuredProperty
	OMFixedSizeProperty<aafTimeStamp_t>	_creationTime;
	OMFixedSizeProperty<aafTimeStamp_t>	_lastModified;
	private:
    OMStrongReferenceVectorProperty<ImplAAFMobSlot> _slots;
};

#endif // ! __ImplAAFMob_h__



////////////////////////////////////////////////////////////////////////////////
