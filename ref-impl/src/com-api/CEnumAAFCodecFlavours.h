//@doc
//@class    EnumAAFCodecFlavours | Implementation class for EnumAAFCodecFlavours
#ifndef __CEnumAAFCodecFlavours_h__
#define __CEnumAAFCodecFlavours_h__

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


class CEnumAAFCodecFlavours
  : public IEnumAAFCodecFlavours,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CEnumAAFCodecFlavours (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CEnumAAFCodecFlavours ();

public:


  //***********************************************************
  //
  // NextOne()
  //
  // Enumerates to the next element in the enumerators list. The
  // caller is responsible for passing in a pointer to an aafUID_t,
  // and retains control over the pointer.
  //
  // This is a just simplified version of the Next method. 
  //
  STDMETHOD (NextOne) (
    // The Next flavour code 
    /*[out,retval]*/ aafUID_t *  pAAFCodecFlavour);


  //***********************************************************
  //
  // Next()
  //
  // Enumerates the next count elements (codec flavour IDs) in
  // the enumerator's list, returning them in the given array along
  // with the actual number of enumerated elements in pFetched. The
  // caller is responsible for passing in a pointer to an array of
  // aafUID_t, and retains control over the pointer.
  //
  STDMETHOD (Next) (
    // number of AAFPluggableDefs requested
    /*[in]*/ aafUInt32  count,

    // array to receive flavour codes
    /*[out, size_is(count), length_is(*pFetched)]*/ aafUID_t *  pAAFCodecFlavours,

    // number of actual flavour IDs fetched into pAAFCodecFlavours array
    /*[out,ref]*/ aafUInt32 *  pFetched);


  //***********************************************************
  //
  // Skip()
  //
  // Instructs the enumerator to skip the next count elements in the
  // enumeration so that the next call to EnumAAFCodecFlavours::Next
  // will not return those elements.
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
  // There is no guarantee that the same set of elements will be
  // enumerated on each pass through the list, nor will the elements
  // necessarily be enumerated in the same order. The exact behavior
  // depends on the collection being enumerated.
  //
  STDMETHOD (Reset)
     ();



  //***********************************************************
  //
  // Clone()
  //
  // Creates another EnumAAFCodecFlavours enumerator with the same
  // state as the current enumerator to iterate over the same
  // list.  This method makes it possible to record a point in the
  // enumeration sequence in order to return to that point at a later
  // time.
  // 
  // The caller must release this new enumerator separately from the
  // first enumerator.
  //
  STDMETHOD (Clone) (
    // new enumeration 
    /*[out,retval]*/ IEnumAAFCodecFlavours ** ppEnum);

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

#endif // ! __CEnumAAFCodecFlavours_h__


