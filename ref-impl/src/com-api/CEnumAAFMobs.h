//@doc
//@class    EnumAAFMobs | Implementation class for EnumAAFMobs
#ifndef __CEnumAAFMobs_h__
#define __CEnumAAFMobs_h__

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






#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CEnumAAFMobs
  : public IEnumAAFMobs,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CEnumAAFMobs (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CEnumAAFMobs ();

public:


  //***********************************************************
  //
  // NextOne()
  //
  // Enumerates to the next element in the enumerators list. The
  // caller is responsible for properly releasing the returned pointer
  // when it is no longer needed.
  // 
  // Succeeds if all of the following are true:
  // - the ppMob pointer is valid.
  // - there are Mob objects remaining to be returned.
  // 
  // If this method fails nothing is written to *ppMob.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppMob is null.
  //
  // AAFRESULT_NO_MORE_OBJECTS
  //   - no Mobs remaining to be returned.
  //
  STDMETHOD (NextOne) (
    // The Next Mob 
    /*[out, retval]*/ IAAFMob ** ppMob);


  //***********************************************************
  //
  // Next()
  //
  // Enumerates the next count elements (AAFMob pointers) in the
  // enumerator's list, returning them in the given array along with
  // the actual number of enumerated elements in pNumFetched. The caller
  // is responsible for properly releasing the returned pointers.
  // 
  // Succeeds if all of the following are true:
  // - the ppMobs pointer is valid.
  // - the pNumFetched pointer is valid.
  // - there are Mob objects remaining to be returned.
  // 
  // If this method fails nothing is written to *ppMobs or
  // pNumFetched.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either ppMob or pNumFetched is null.
  //
  STDMETHOD (Next) (
    // number of mobs requested
    /*[in]*/ aafUInt32  count,

    // array to receive mobs
    /*[out, size_is(count), length_is(*pNumFetched)]*/ IAAFMob ** ppMobs,

    // number of actual Mobs fetched into ppMobs array
    /*[out]*/ aafUInt32 *  pNumFetched);


  //***********************************************************
  //
  // Skip()
  //
  // Instructs the enumerator to skip the next count elements in the
  // enumeration so that the next call to EnumAAFMobs::Next will not
  // return those elements.
  // 
  // Succeeds if all of the following are true:
  // - count is less than or equal to the number of remaining objects.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NO_MORE_OBJECTS
  //   - count exceeded number of remaining objects.
  //
  STDMETHOD (Skip) (
    // Number of elements to skip 
    /*[in]*/ aafUInt32  count);


  //***********************************************************
  //
  // Reset()
  //
  // Instructs the enumerator to position itself at the beginning of
  // the list of elements.
  // 
  // Always succeeds.
  // 
  // This method will return the following code:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (Reset)
     ();



  //***********************************************************
  //
  // Clone()
  //
  // Creates another mob enumerator with the same state as the current
  // enumerator to iterate over the same list. This method makes it
  // possible to record a point in the enumeration sequence in order
  // to return to that point at a later time.
  //
  // Note: The caller must release this new enumerator separately from
  // the first enumerator.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
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
  STDMETHOD (Clone) (
    // new enumeration 
    /*[out, retval]*/ IEnumAAFMobs ** ppEnum);

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

#endif // ! __CEnumAAFMobs_h__


