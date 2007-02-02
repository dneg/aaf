//@doc
//@class    AAFPlainEssenceData | Implementation class for AAFPlainEssenceData
#ifndef __CAAFPlainEssenceData_h__
#define __CAAFPlainEssenceData_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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





#ifndef __CAAFEssenceData_h__
#include "CAAFEssenceData.h"
#endif


class CAAFPlainEssenceData
  : public IAAFPlainEssenceData,
    public IAAFKLVEssenceDataParameters,
    public CAAFEssenceData
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPlainEssenceData (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPlainEssenceData ();

public:

  //***********************************************************
  // METHOD NAME: GetEssenceElementKey()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFKLVEssenceDataParameters | GetEssenceElementKey |
  // This method returns essence element key associated
  /// with the essence stream.
  ///
  /// Succeeds if all of the following are true:
  /// - pEssenceElementKey is a valid pointer.
  /// - the essence stream supports essence element keys.
  ///
  /// If this method fails nothing will be written to *pEssenceElementKey.
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
  //   - pEssenceElementKey arg is NULL.
  ///
  /// AAFRESULT_OPERATION_NOT_PERMITTED
  ///   - the essence stream does not support essence element keys.
  // @end
  // 
  STDMETHOD (GetEssenceElementKey)
   (
    // @parm [out] aafUID_t * | pEssenceElementKey | Essence element key.
    aafUID_t *  pEssenceElementKey
  );

  //***********************************************************
  // METHOD NAME: SetEssenceElementKey()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFKLVEssenceDataParameters | SetEssenceElementKey |
  // Set the essence element key on the essence stream.
  /// 
  /// Succeeds if all of the following are true:
  /// - the essence stream supports essence element keys.
  /// 
  /// If this method fails, the property will not be changed.
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
  ///
  /// AAFRESULT_OPERATION_NOT_PERMITTED
  ///   - the essence stream does not support essence element keys.
  // @end
  // 
  STDMETHOD (SetEssenceElementKey)
   (
    // @parm [in, ref] aafUID_constref | key | essence element key
    aafUID_constref  key
  );


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

#endif // ! __CAAFPlainEssenceData_h__

