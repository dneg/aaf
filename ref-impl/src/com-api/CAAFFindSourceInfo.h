//@doc
//@class    AAFFindSourceInfo | Implementation class for AAFFindSourceInfo
#ifndef __CAAFFindSourceInfo_h__
#define __CAAFFindSourceInfo_h__

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


 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif





#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFFindSourceInfo
  : public IAAFFindSourceInfo,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFFindSourceInfo (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFFindSourceInfo ();

public:



  //***********************************************************
  //
  // GetMob()
  //
  // // This function returns the mob found by the function
  // which generated this IAAFFindSourceInfo as a result.
  //
  // Succeeds if all of the following are true:
  // - the ppMob pointer is valid.
  // 
  // If this method fails nothing will be written to *ppMob.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULLOBJECT
  //   - There is no valid mob referenced
  //
  // AAFRESULT_NULL_PARAM
  //   - ppMob arg is NULL.
  //
  STDMETHOD (GetMob) (
    // The final media object referenced 
    /*[out]*/ IAAFMob ** ppMob);

  //***********************************************************
  //
  // GetSourceReference()
  //
  // // This function returns the source reference found by the function
  // which generated this IAAFFindSourceInfo as a result.
  //
  // Note: the 3 properties that make up the "source
  // reference" are sourceID, sourceTrackID, and startTime.
  //
  // Succeeds if all of the following are true:
  // - This object has already been Initialize()d.
  // - the pSourceRef pointer is valid.
  // 
  // If this method fails nothing will be written to *pSourceRef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSourceRef arg is NULL.
  //
  STDMETHOD (GetSourceReference) (
    // Source Reference 
    /*[out]*/ aafSourceRef_t *  pSourceRef);

  //***********************************************************
  //
  // GetLength()
  //
  // Gets the  of this component.
  // This function returns the duration in edit units of the result.
  //	
  // Succeeds if all of the following are true:
  // - the pLength pointer is valid.
  // - the optional length property is present for this object.
  //
  // This method deals with an optional property, which will only be
  // present for time-varying media.
  // 
  // If this method fails nothing will be written to *pLength.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLength arg is NULL.
  //
  // AAFRESULT_BAD_PROP
  //   - the optional length property is not present for this object.
  //
  STDMETHOD (GetLength) (
    // Length of this component 
    /*[retval][out]*/ aafLength_t *  pLength);

  //***********************************************************
  //
  // GetEditRate()
  //
  // This method will get the edit rate for this result.
  // 
  // Succeeds if all of the following are true:
  // - the pEditRate pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pEditRate arg is NULL.
  //
  STDMETHOD (GetEditRate) (
    // Edit rate property value 
    /*[out,retval]*/ aafRational_t *  pEditRate);


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

#endif // ! __CAAFFindSourceInfo_h__


