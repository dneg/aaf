//@doc
//@class    AAFSourceClip | Implementation class for AAFSourceClip
#ifndef __CAAFSourceClip_h__
#define __CAAFSourceClip_h__

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







#ifndef __CAAFSourceReference_h__
#include "CAAFSourceReference.h"
#endif


class CAAFSourceClip
  : public IAAFSourceClip,
    public CAAFSourceReference
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSourceClip (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSourceClip ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // This method initializes a source clip object with
  // the given properties.  Only required properties are set.
  // Optional properties are added with separate functions.
  //
  // Succeds if:
  // - This object has not already been Initialize()d.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pDataDef argument is NULL.
  //
  STDMETHOD (Initialize) (
    // Data Definition object
    /*[in]*/ IAAFDataDef * pDataDef,

    // Length property value
    /*[in]*/ aafLength_constref  length,

    // Source Reference
    /*[in]*/ aafSourceRef_t  sourceRef);


  //***********************************************************
  //
  // GetFade()
  //
  // This function returns the optional fade information from a
  // source clip.  This function only applies to audio source clips.
  // Length units are specified by the containing mob slot's edit
  // rate.
  //
  // Succeeds if all of the following are true:
  // - This object has already been Initialize()d.
  // - the all argument pointers are valid.
  // 
  // If this method fails nothing will be written to any of the
  // locations specified by the arguments.
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
  //   - any argument is NULL.
  //
  STDMETHOD (GetFade) (
    // Fade In Length
    /*[out]*/ aafLength_t *  pFadeInLen,

    // Fade In Type
    /*[out]*/ aafFadeType_t *  pFadeInType,

    // Fade In Type
    /*[out]*/ aafBoolean_t *  pFadeInPresent,

    // Fade Out Length
    /*[out]*/ aafLength_t *  pFadeOutLen,

    // Fade Out Type
    /*[out]*/ aafFadeType_t *  pFadeOutType,

    // Fade In Type
    /*[out]*/ aafBoolean_t *  pFadeOutPresent);


  //***********************************************************
  //
  // ResolveRef()
  //
  // Given a source clip object, this function returns a pointer
  // to the mob that it references.
  //
  // The returned mob is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - This object has already been Initialize()d.
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
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppMob arg is NULL.
  //
  // AAFRESULT_MOB_NOT_FOUND
  //   - this mob does not exist.
  //
  STDMETHOD (ResolveRef) (
    // Referenced mob 
    /*[out]*/ IAAFMob ** ppMob);


  //***********************************************************
  //
  // GetSourceReference()
  //
  // // This function returns the source reference of this source clip.
  //
  // Note: the 3 properties of a source Clip that make up the "source
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
  // SetFade()
  //
  // This function sets the optional fade properties on this source
  // clip object.  The fade properties only apply to a source clip of
  // data definition (or convertible to a data definition) of type
  // Sound.  All arguments should be specified.  Length units are
  // specified by the containing mob slot's edit rate.
  // 
  // Succeeds if all of the following are true:
  // - This object has already been Initialize()d.
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
  STDMETHOD (SetFade) (
    // Fade In Length
    /*[in]*/ aafInt32  fadeInLen,

    // Fade In Type
    /*[in]*/ aafFadeType_t  fadeInType,

    // Fade Out Length
    /*[in]*/ aafInt32  fadeOutLen,

    // Fade Out Type
    /*[in]*/ aafFadeType_t  fadeOutType);


  //***********************************************************
  //
  // SetSourceReference()
  //
  // This function sets the source reference of this source clip.
  //
  // Note: the  3 properties of a source Clip that make up the "source
  // reference" are sourceID, sourceTrackID, and startTime.
  // 
  // Succeeds if all of the following are true:
  // - This object has already been Initialize()d.
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
  STDMETHOD (SetSourceReference) (
    // Source Reference 
    /*[in]*/ aafSourceRef_t  sourceRef);

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

#endif // ! __CAAFSourceClip_h__


