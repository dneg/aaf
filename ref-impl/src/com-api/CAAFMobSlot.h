//@doc
//@class    AAFMobSlot | Implementation class for AAFMobSlot
#ifndef __CAAFMobSlot_h__
#define __CAAFMobSlot_h__

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


class CAAFMobSlot
  : public IAAFMobSlot,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFMobSlot (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFMobSlot ();

public:


  //***********************************************************
  //
  // GetSegment()
  //
  // This method will get the segment for this mob slot and place an
  // interface for it into the **ppResult argument.  If a segment
  // exists, the result will be AddRef()ed.  If not, the result will
  // be NULL.
  //
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
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
  //   - pResult arg is NULL.
  //
  STDMETHOD (GetSegment) (
    // Segment property value 
    /*[out,retval]*/ IAAFSegment ** ppResult);


  //***********************************************************
  //
  // SetName()
  //
  // Sets the Mob Slot Name string property.
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
  // Gets the Mob Slot Name string property.
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
  // GetPhysicalNum()
  //
  // Returns information about the physical output channel associated
  // with the  Slot.
  //
  // This function returns the physical slot number of the specified
  // slot. The physical slot number identifies the physical slot
  // associated with the media.  For File Source Mobs that describe
  // stereo audio media, the left channel should have a PhysicalSlot
  // of 1 and the right channel should have a Physical-Slot of 2.
  //
  // The function returns an error if the object specified in the slot
  // parameter is not a slot.
  //
  // Succeeds if all of the following are true:
  // - the pDatadef pointer is valid.
  // - the object in the slot parameter is a slot.
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
  //   - pDatadef arg is NULL.
  //
  // AAFRESULT_SLOT_NOT_FOUND
  //   - object specified is not a slot.
  //
  STDMETHOD (GetPhysicalNum) (
    // The physical slot number property value 
    /*[out,retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetDataDef()
  //
  // This method will return the Data Definition object
  // associated with the segment in this Mob Slot.  Common
  // DataDefinitions are DDEF_Picture, DDEF_Sound, DDEF_Timecode,
  // and DDEF_Edgecode.
  //
  // Succeeds if all of the following are true:
  // - the ppResult pointer is valid.
  // 
  // If this method fails nothing will be written to *ppResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppResult arg is NULL.
  //
  STDMETHOD (GetDataDef) (
    // Data Definition AUID 
    /*[out,retval]*/ IAAFDataDef ** ppResult);


  //***********************************************************
  //
  // GetSlotID()
  //
  // This method will return the slot id of this mob slot.
  //
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
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
  //   - pResult arg is NULL.
  //
  STDMETHOD (GetSlotID) (
    // Slot id of the Mob Slot 
    /*[out,retval]*/ aafSlotID_t *  pResult);


  //***********************************************************
  //
  // SetSegment()
  //
  // This method will set the segment for this mob slot.  If a segment
  // already exists for this mob slot, it will be discarded.
  // 
  // Always succeeds.
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
  STDMETHOD (SetSegment) (
    // Segment property value 
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // SetPhysicalNum()
  //
  // // This function sets the physical slot number of the specified
  // slot.  The physical slot number identifies the physical slot
  // associated with the media.  For File Source Mobs that describe
  // stereo audio media, the left channel should have a PhysicalSlot
  // of 1 and the right channel should have a Physical-Slot of 2.
  //
  // The function returns an error if the Mob Slot passed in is not a
  // slot.
  // 
  // Succeeds if all of the following are true:
  // - the Mob Slot passed in is a slot.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.
  //
  // AAFRESULT_SLOT_NOT_FOUND
  //   - object specified is not a slot.
  //
  STDMETHOD (SetPhysicalNum) (
    // The physical slot number property value 
    /*[in]*/ aafUInt32  number);


  //***********************************************************
  //
  // SetSlotID()
  //
  // This method will set the slot id of this mob slot.
  // 
  // Always succeeds.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetSlotID) (
    // Slot id of the Mob Slot 
    /*[in]*/ aafSlotID_t  value);

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

#endif // ! __CAAFMobSlot_h__


