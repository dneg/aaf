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

  OMDECLARE_STORABLE(ImplAAFMob);

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
        (aafString_t *  name);  //@parm [in,out] Mob Name

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
        (aafString_t *  name,   //@parm [in,out] Mob Name
		 aafTimeStamp_t *  lastModified,   //@parm [out] Modified Time
         aafTimeStamp_t *  creationTime);  //@parm [out] Creation Time


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
		 aafString_t *  name,   //@parm [in,ref] Mob Name (optional)
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
    SetIdentity
        (aafUID_t *  mobID);  //@parm [in, ref] New Mob ID


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafString_t *  name);  //@parm [in, ref] Mob Name


// skip virtual aafErr_t Verify(char *buf, validateData_t *result);
// What doe's this do?

  //****************
  // IsMobKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsMobKind
        (aafMobKind_t  mobKind,   //@parm [in] A valid mob kind.
		 aafBool *  result);  //@parm [out,retval] True is matches given mobKind



  //****************
  // AppendNewSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewSlot
        (aafRational_t  editRate,   //@parm [in] Edit rate property value
		 ImplAAFSegment * segment,   //@parm [in] Segment to append as track component
		 aafTrackID_t  trackID,   //@parm [in] The Track ID
         aafString_t *  trackName,   //@parm [in] Track Name (optional)
		 ImplAAFMobSlot ** newTrack);  //@parm [out] Newly created track


  //****************
  // GetAllMobSlots()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAllMobSlots
        (ImplEnumAAFMobSlots ** ppEnum);  //@parm [out,retval] Mob Slot Enumeration


  //****************
  // AppendComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendComment
        (aafString_t *  category,   //@parm [in,ref] Comment heading
		 aafString_t *  comment);  //@parm [in,ref] Comment value


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
    GetComments
        (ImplEnumAAFMobComments ** ppEnum);  //@parm [out,retval] Mob Comments



  //****************
  // OffsetToMobTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OffsetToMobTimecode
        (ImplAAFSegment * tcTrackID,   //@parm [in] Current track
		 aafPosition_t *  offset,   //@parm [in,ref] Offset into segment
         aafTimecode_t *  result);  //@parm [out] The resulting timecode


  //****************
  // FindTrackByTrackID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    FindTrackByTrackID
        (aafTrackID_t  trackID,   //@parm [in] The requested track id
		 ImplAAFMobSlot ** destTrack);  //@parm [out] The requested track


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
  //****************
  // OffsetToTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OffsetToTimecode
        (aafTrackID_t*  trackID,   //@parm [in,ref] Track ID of the track in the input mob
		 aafPosition_t *  offset,   //@parm [in] Offset into the given track
         aafTimecode_t *  result);  //@parm [out] The resulting timecode


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
  //****************
  // TimecodeToOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    TimecodeToOffset
        (aafTimecode_t  timecode,   //@parm [in] The timecode value
		 aafTrackID_t  trackID,   //@parm [in] Track ID of track in source mob
         aafFrameOffset_t *  result);  //@parm [out] Resulting offset in source track




  //****************
  // Copy()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Copy
        (aafString_t *  destMobName,   //@parm [in] Optional Input. The name to be assigned to the new copy of the Mob.
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

	protected:
	OMFixedSizeProperty<aafUID_t>		_mobID;
	OMStringProperty					_name;
//!!! Creation time and last modified should be OMStructuredProperty
	OMFixedSizeProperty<aafTimeStamp_t>	_creationTime;
	OMFixedSizeProperty<aafTimeStamp_t>	_lastModified;
	private:
    OMStrongReferenceVectorProperty<ImplAAFMobSlot> _slots;
};

#endif // ! __ImplAAFMob_h__



////////////////////////////////////////////////////////////////////////////////
