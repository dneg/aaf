//@doc
//@class    AAFMob | Implementation class for AAFMob
#ifndef __CAAFMob_h__
#define __CAAFMob_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the 
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif














#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFMob
  : public IAAFMob,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFMob (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFMob ();

public:


  //***********************************************************
  //
  // GetMobID()
  //
  // This method returns the unique Mob ID associated with this mob.
  //
  // Succeeds if all of the following are true:
  // - the pMobID pointer is valid.
  // 
  // If this method fails nothing will be written to *pMobID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMobID arg is NULL.
  //
  STDMETHOD (GetMobID) (
    // The unique media object id 
    /*[out]*/ aafMobID_t *  pMobID);


  //***********************************************************
  //
  // SetMobID()
  //
  // // When a mob is initially created, the Reference Implementation
  // internally creates a mobID for the new mob.  This method should
  // be used to change the mob's identity to an explicit mobID.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetMobID) (
    // New Mob ID 
    /*[in, ref]*/ aafMobID_constref  mobID);


  //***********************************************************
  //
  // SetName()
  //
  // Sets the Mob Name string property.
  //
  // Set the Name property to the value specified in
  // pName.  A copy is made of the data so the caller
  // retains ownership of the *pName buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pName pointer is valid.
  // 
  // If this method fails the Name property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pName arg is NULL.
  //
  STDMETHOD (SetName) (
    // buffer from which Name is to be read 
    /*[in, string]*/ aafCharacter_constptr  pName);


  //***********************************************************
  //
  // GetName()
  //
  // Gets the Mob Name string property.
  // 
  // Writes the Name property, with a trailing null
  // character, into the pName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Name property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pName.
  // 
  // Succeeds if:
  // - The pName pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Name.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pName arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Name.
  //
  STDMETHOD (GetName) (
    // buffer into which Name is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pName,

    // size of *pName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetName().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetNameBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // CountSlots()
  //
  // This method returns the number of slots contained by this mob.
  //
  // Succeeds if all of the following are true:
  // - the pNumSlots pointer is valid.
  // 
  // If this method fails nothing will be written to *pNumSlots.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumSlots arg is NULL.
  //
  STDMETHOD (CountSlots) (
    // Number of slots 
    /*[out]*/ aafNumSlots_t *  pNumSlots);


  //***********************************************************
  //
  // AppendSlot()
  //
  // Appends the given mob slot to the mob.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pSlot pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pSlot is null.
  //
  STDMETHOD (AppendSlot) (
    // slot to append 
    /*[in]*/ IAAFMobSlot * pSlot);


  //***********************************************************
  //
  // PrependSlot()
  //
  // Prepends the given mob slot to the mob.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pSlot pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pSlot is null.
  //
  STDMETHOD (PrependSlot) (
    // slot to prepend 
    /*[in]*/ IAAFMobSlot * pSlot);


  //***********************************************************
  //
  // InsertSlotAt()
  //
  // Inserts the given slot into this mob at the given index.  All
  // existing slots at the given and higher index will be moved up one
  // index to accommodate.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pSlot pointer is valid.
  // - index is less than or equal to the result obtained by
  //   CountSlots().
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pSlot is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than the result obtained from CountSlots().
  //
  STDMETHOD (InsertSlotAt) (
    // index where slot is to be inserted
    /*[in]*/ aafUInt32  index,

    // slot to insert
    /*[in]*/ IAAFMobSlot * pSlot);


  //***********************************************************
  //
  // RemoveSlotAt()
  //
  // Removes the slot at the given index.  All existing slots at
  // indices higher than the given index will be moved down one index
  // to accommodate.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - index is less than the result obtained by CountSlots().
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_BADINDEX
  //   - index is not less than the result obtained from
  //     CountSlots().
  //
  STDMETHOD (RemoveSlotAt) (
    // index of slot to be removed 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // GetSlotAt()
  //
  // Returns the indexed slot in *ppSlot.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - ppSlot is a valid pointer.
  // - index is less than the result obtained by CountSlots().
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppSlot is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is not less than the result obtained from
  //     CountSlots().
  //
  STDMETHOD (GetSlotAt) (
    // index of slot to be obtained
    /*[in]*/ aafUInt32  index,

    // the returned slot
    /*[out, retval]*/ IAAFMobSlot ** ppSlot);


  //***********************************************************
  //
  // GetSlots()
  //
  // return an enumeration for all mob slots.  The returned
  // enumerator is AddRef()ed before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  STDMETHOD (GetSlots) (
    // Mob Slot Enumeration 
    /*[out]*/ IEnumAAFMobSlots ** ppEnum);


  //***********************************************************
  //
  // GetModTime()
  //
  // This method will return the modification time for this mob.
  //
  // Succeeds if all of the following are true:
  // - the pLastModified pointer is valid.
  // 
  // If this method fails nothing will be written to *pLastModified.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLastModified arg is NULL.
  //
  STDMETHOD (GetModTime) (
    // Modified Time 
    /*[out]*/ aafTimeStamp_t *  pLastModified);


  //***********************************************************
  //
  // SetModTime()
  //
  // This method sets the modification time on a mob.  The
  // modification time is initially set to the time that the mob
  // was created.  The Reference Implementation does not maintain the
  // modification time every time that a mob has been updated.
  // Therefore, this method should be called explicitly to change the
  // modification time.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetModTime) (
    // New Modification Time 
    /*[in, ref]*/ aafTimeStamp_constref  modTime);


  //***********************************************************
  //
  // GetCreateTime()
  //
  // This method will return the creation time for this mob.
  //
  // Succeeds if all of the following are true:
  // - the pCreationTime pointer is valid.
  // 
  // If this method fails nothing will be written to *pCreationTime.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCreationTime arg is NULL.
  //
  STDMETHOD (GetCreateTime) (
    // Creation Time 
    /*[out]*/ aafTimeStamp_t *  pCreationTime);


  //***********************************************************
  //
  // SetCreateTime()
  //
  // This method sets the creation time on a mob.  The
  // creation time is initially set to the time that the mob
  // was created.
  // Therefore, this method should be called explicitly to change the
  // creation time.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetCreateTime) (
    // New Creation Time 
    /*[in, ref]*/ aafTimeStamp_constref  createTime);


  //***********************************************************
  //
  // AppendComment()
  //
  // Creates a user-defined comment and appends it to the specified
  // Mob.  A Mob comment is implemented as a AAFTaggedValue object of type 
  // WCharString.
  // 
  // Succeeds if all of the following are true:
  // - the pCategory pointer is valid.
  // - the pComment pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pCategory or pComment args is NULL.
  //
  STDMETHOD (AppendComment) (
    // Comment heading
    /*[in,string]*/ aafCharacter *  pCategory,

    // Comment value
    /*[in, string]*/ aafCharacter_constptr  pComment);


  //***********************************************************
  //
  // CountComments()
  //
  // return total number of comments attached to this mob.
  //
  // Succeeds if all of the following are true:
  // - the pNumComments pointer is valid.
  // 
  // If this method fails nothing will be written to *pNumComments.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumComments arg is NULL.
  //
  STDMETHOD (CountComments) (
    // Number  of Mob Comments 
    /*[out]*/ aafUInt32 *  pNumComments);


  //***********************************************************
  //
  // GetComments()
  //
  // return the enumeration for all mob comments.  The returned
  // enumerator is AddRef()ed before it is returned.  Mob comments are 
  // implemented as AAFTaggedValue of type WCharString.   The enumerator
  // is implemented as a EnumAAAFTaggedValues.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  STDMETHOD (GetComments) (
    // Mob Comments 
    /*[out]*/ IEnumAAFTaggedValues ** ppEnum);


  //***********************************************************
  //
  // RemoveComment()
  //
  // // Removes the given comment from this mob.
  // 
  // Succeeds if all of the following are true:
  // - the pComment pointer is valid.
  // - the given comment is present in the mob.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pComment is null.
  //
  // AAFRESULT_OBJECT_NOT_FOUND
  //   - the given comment is not in this mob.
  //
  STDMETHOD (RemoveComment) (
    // Comment to remove 
    /*[in]*/ IAAFTaggedValue * pComment);


  //***********************************************************
  //
  // AppendNewTimelineSlot()
  //
  // This method creates a new timeline mob slot with the given
  // property values and appends it to the input mob.
  // 
  // The returned mob slot is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the pSegment pointer is valid.
  // - the pSlotName pointer is valid.
  // - the ppNewSlot pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - any of pSegment, pSlotName, or ppNewSlot arguments is null.
  //
  STDMETHOD (AppendNewTimelineSlot) (
    // Edit rate property value
    /*[in]*/ aafRational_t  editRate,

    // Segment to append as slot component
    /*[in]*/ IAAFSegment * pSegment,

    // new slot ID
    /*[in]*/ aafSlotID_t  slotID,

    // new slot name
    /*[in, string]*/ aafCharacter_constptr  pSlotName,

    // The slot origin
    /*[in]*/ aafPosition_t  origin,

    // Newly created slot
    /*[out]*/ IAAFTimelineMobSlot ** ppNewSlot);


  //***********************************************************
  //
  // GetMobInfo()
  //
  // This method will get all mob property information is a single call.
  //
  // Caller may call GetNameBufLen() to determine the required pName
  // buffer size.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pLastModified pointer is valid.
  // - the pCreationTime pointer is valid.
  // - the pName pointer is valid.
  // 
  // If this method fails nothing will be written to *pLastModified,
  // *pCreationTime, or *pName.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - any of pLastModified, pCreationTime, or pName arguments is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  STDMETHOD (GetMobInfo) (
    // Modified Time
    /*[out]*/ aafTimeStamp_t *  pLastModified,

    // Creation Time
    /*[out]*/ aafTimeStamp_t *  pCreationTime,

    // Mob Name
    /*[out, size_is(bufSize), string]*/ aafCharacter *  pName,

    // size of the supplied buffer.
    /*[in]*/ aafInt32  bufSize);


  //***********************************************************
  //
  // OffsetToMobTimecode()
  //
  // This method will determine the timecode at the given offset into
  // the given timecode segment, and will return it in *pResult.  If
  // pTcSeg is NULL, will search for the slot containing a timecode
  // segment and will use that instead.
  //
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pTcSeg pointer is valid.
  // - the pOffset pointer is valid.
  // - the pResult pointer is valid.
  // - Timecode track exists.
  // 
  // If this method fails nothing will be written to *pResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pOffset or pResult argument is NULL.
  //
  // AAFRESULT_TIMECODE_NOT_FOUND
  //   - timecode track wasn't found.
  //
  STDMETHOD (OffsetToMobTimecode) (
    // Timecode Segment
    /*[in]*/ IAAFSegment * pTcSeg,

    // Offset into segment in edit units for that segment's mob slot
    /*[in]*/ aafPosition_t *  pOffset,

    // The resulting timecode
    /*[out]*/ aafTimecode_t *  pResult);


  //***********************************************************
  //
  // LookupSlot()
  //
  // The method will find the mob slot for the given slot id.
  //
  // The returned mob slot is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppDestSlot pointer is valid.
  // - the given slot ID is found.
  // 
  // If this method fails nothing will be written to *ppDestSlot.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDestSlot arg is NULL.
  //
  STDMETHOD (LookupSlot) (
    // The requested slot id
    /*[in]*/ aafSlotID_t  slotId,

    // The requested slot
    /*[out]*/ IAAFMobSlot ** ppDestSlot);


  //***********************************************************
  //
  // ChangeRef()
  //
  // Finds all Source Clips in the specified Mob that refer to the
  // specified old Mob, and changes the references to point to the
  // new Mob.
  //
  // This function traverses through the entire structure of the input
  // Mob looking for Source Clips, and changes the sourceID property
  // on all Source Clips with oldMobID to newMobID.
  //
  // NOTE! Stub only.   Implementation not yet added.
  //
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (ChangeRef) (
    // Old Mob ID reference in source clip
    /*[in, ref]*/ aafMobID_constref  oldMobID,

    // New Mob ID reference in source clip
    /*[in, ref]*/ aafMobID_constref  newMobID);


  //***********************************************************
  //
  // CloneExternal()
  //
  // Clones the specified Source Mob, and optionally all dependent
  // Mobs, to an external file, keeping the same MobID.  A pointer
  // to the newly created destination mob is returned in *ppDestMob.
  // 
  // This function clones the specified Source Mob in the source file
  // into a destination Mob, with the same MobID, in the destination
  // file.  If resolveDependencies is kFollowDepend, the function
  // also clones all Mobs referenced by the specified Source Mob.  If
  // includeMedia is kIncludeMedia, the function also copies the
  // media data associated with the Source Mob, returns the
  // destination Mob, and clones all private data.
  //
  // If the media data is not in the file, the function does not
  // attempt to find it in another file and clone it.  Both AAF files
  // must be open before you call this function and both must have the
  // same AAF Version number.
  //
  // The returned mob is AddRef()ed before it is returned.
  //
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pDestFile pointer is valid.
  // - the ppDestMob pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pDestFile or ppDestMob arguments is NULL.
  //
  STDMETHOD (CloneExternal) (
    // Whether to clone dependent mobs
    /*[in]*/ aafDepend_t  resolveDependencies,

    // Whether to include media data
    /*[in]*/ aafIncMedia_t  includeMedia,

    // Destination AAF File
    /*[in]*/ IAAFFile * pDestFile,

    // Destination Mob
    /*[out]*/ IAAFMob ** ppDestMob);


  //***********************************************************
  //
  // Copy()
  //
  // This function copies the specified Mob into a destination Mob in
  // the same AAF file. The new Mob is returned through the destMob
  // parameter. The function gives the destination Mob a new MobID and
  // the name specified in the destMobName parameter. The function
  // also copies all private data.
  // 
  // The returned mob is AddRef()ed before it is returned.
  //
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pDestMobName pointer is valid.
  // - the ppDestMob pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pDestMobName or pDestMob arguments is NULL.
  //
  STDMETHOD (Copy) (
    // Optional Input. The name to be assigned to the new copy of the
	// Mob.  The destMobName argument is optional. Specify a NULL
	// value if no destination Mob name is desired.
    /*[in, string]*/ aafCharacter_constptr  pDestMobName,

    // Destination Mob
    /*[out]*/ IAAFMob ** ppDestMob);

  //***********************************************************
  //
  // AppendKLVData()
  //
  // Appends a pre-existing KLV Data object to the specified
  // Mob.
  // 
  // Succeeds if all of the following are true:
  // - the pKLV pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - the pData arg is NULL.
  //
  STDMETHOD (AppendKLVData) (
    // KLV object 
    /*[in]*/ IAAFKLVData * pData);


  //***********************************************************
  //
  // CountKLVData()
  //
  // return total number of KLV data objects attached to this mob.
  //
  // Succeeds if all of the following are true:
  // - the pNumData pointer is valid.
  // 
  // If this method fails nothing will be written to *pNumComments.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumData arg is NULL.
  //
  STDMETHOD (CountKLVData) (
    // Number  of KLV data objects 
    /*[out]*/ aafUInt32 *  pNumData);


  //***********************************************************
  //
  // GetKLVData()
  //
  // return the enumeration for all KLV data objects on this mob.  The returned
  // enumerator is AddRef()ed before it is returned.  The enumerator
  // is implemented as a EnumAAFKLVData.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  STDMETHOD (GetKLVData) (
    // KLV data objects 
    /*[out]*/ IEnumAAFKLVData ** ppEnum);


  //***********************************************************
  //
  // RemoveKLVData()
  //
  // // Removes the given KLV data object from this mob.
  // 
  // Succeeds if all of the following are true:
  // - the pData pointer is valid.
  // - the given KLV data object is present in the mob.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pData is null.
  //
  // AAFRESULT_OBJECT_NOT_FOUND
  //   - the given KLV data object is not in this mob.
  //
  STDMETHOD (RemoveKLVData) (
    // KLV data object to remove 
    /*[in]*/ IAAFKLVData * pData);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFMob_h__


