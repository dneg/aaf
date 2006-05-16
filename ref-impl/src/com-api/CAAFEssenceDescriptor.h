//@doc
//@class    AAFEssenceDescriptor | Implementation class for AAFEssenceDescriptor
#ifndef __CAAFEssenceDescriptor_h__
#define __CAAFEssenceDescriptor_h__

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







#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFEssenceDescriptor
  : public IAAFEssenceDescriptor,
    public IAAFEssenceDescriptor2,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEssenceDescriptor ();

public:

  //***********************************************************
  //
  // CountLocators()
  //
  // Return the number of locators attached to this essence
  /// descriptor.  The number of locators may be zero if the essence is
  /// in the current file.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid.
  /// 
  /// If this method fails nothing is written to *pCount.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  //
  STDMETHOD (CountLocators) (
    // Returns the number of locators 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // AppendLocator()
  //
  // Append another locator to this essence descriptor.  Use this
  /// function to add a locator to be scanned last when searching for
  /// the essence (a secondary location for the essence).
  /// 
  /// Succeeds if all of the following are true:
  /// - the pLocator pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLocator is null.
  //
  STDMETHOD (AppendLocator) (
    // Locator to append 
    /*[in]*/ IAAFLocator * pLocator);


  //***********************************************************
  //
  // PrependLocator()
  //
  // Prepend another locator to this essence descriptor.  Use this
  /// function to add a locator to be scanned first when searching for
  /// the essence (a new primary location for the essence).
  /// 
  /// Succeeds if all of the following are true:
  /// - the pLocator pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLocator is null.
  //
  STDMETHOD (PrependLocator) (
    // Locator to append 
    /*[in]*/ IAAFLocator * pLocator);


  //***********************************************************
  //
  // InsertLocatorAt()
  //
  // Inserts the given locator at the given index.  Locators already
  /// existing at the given and higher indices will be moved to the
  /// next higher index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pLocator pointer is valid.
  /// - index is less than or equal to the value returned by
  ///   CountLocators().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLocator is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than the value returned by
  ///     CountLocators().
  //
  STDMETHOD (InsertLocatorAt) (
    // index at which locator is to be inserted
    /*[in]*/ aafUInt32  index,

    // Locator to append
    /*[in]*/ IAAFLocator * pLocator);


  //***********************************************************
  //
  // GetLocatorAt()
  //
  // Retrieves the locator at the given index.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pLocator pointer is valid.
  /// - index is less than the value returned by CountLocators().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLocator is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountLocators().
  //
  STDMETHOD (GetLocatorAt) (
    // index of locator to retrieve
    /*[in]*/ aafUInt32  index,

    // returned locator
    /*[out, retval]*/ IAAFLocator ** ppLocator);


  //***********************************************************
  //
  // RemoveLocatorAt()
  //
  // Removes the locator at the given index.  Locators already
  /// existing at indices higher than the given index will be moved to
  /// the next lower index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - index is less than the value returned by CountLocators().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountLocators().
  //
  STDMETHOD (RemoveLocatorAt) (
    // index of locator to remove 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // GetLocators()
  //
  // Returns an enumerator to the locators.  The number of locators
  /// may be zero if the essence is in the current file.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (GetLocators) (
    // An enumerator to the locators on this essence descriptor 
    /*[out]*/ IEnumAAFLocators ** ppEnum);


  //***********************************************************
  // METHOD NAME: CountSubDescriptors()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | CountSubDescriptors |
  // Return the number of subdescriptors attached to this essence
  /// descriptor.  The number of subdescriptors may be zero if the essence is
  /// in the current file.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid.
  /// 
  /// If this method fails nothing is written to *pCount.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  // @end
  // 
  STDMETHOD (CountSubDescriptors)
   (
    // @parm [out] aafUInt32 * | pResult | Returns the number of subdescriptors
    aafUInt32 *  pResult
  );

  //***********************************************************
  // METHOD NAME: AppendSubDescriptor()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | AppendSubDescriptor |
  // Append another subdescriptor to this essence descriptor.  Use this
  /// function to add a subdescriptor to be scanned last when searching for
  /// the essence (a secondary location for the essence).
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSubDescriptor pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSubDescriptor is null.
  // @end
  // 
  STDMETHOD (AppendSubDescriptor)
   (
    // @parm [in] AAFSubDescriptor | pSubDescriptors | SubDescriptor to append
    IAAFSubDescriptor * pSubDescriptors
  );

  //***********************************************************
  // METHOD NAME: PrependSubDescriptor()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | PrependSubDescriptor |
  // Prepend another subdescriptor to this essence descriptor.  Use this
  /// function to add a subdescriptor to be scanned first when searching for
  /// the essence (a new primary location for the essence).
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSubDescriptor pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSubDescriptor is null.
  // @end
  // 
  STDMETHOD (PrependSubDescriptor)
   (
    // @parm [in] AAFSubDescriptor | pSubDescriptor | SubDescriptor to append
    IAAFSubDescriptor * pSubDescriptor
  );

  //***********************************************************
  // METHOD NAME: InsertSubDescriptorAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | InsertSubDescriptorAt |
  // Inserts the given subdescriptor at the given index.  SubDescriptors already
  /// existing at the given and higher indices will be moved to the
  /// next higher index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSubDescriptor pointer is valid.
  /// - index is less than or equal to the value returned by
  ///   CountSubDescriptors().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSubDescriptor is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than the value returned by
  ///     CountSubDescriptors().
  // @end
  // 
  STDMETHOD (InsertSubDescriptorAt)
   (
    // @parm [in] aafUInt32 | index | index at which subdescriptor is to be inserted
    aafUInt32  index,

    // @parm [in] AAFSubDescriptor | pSubDescriptor | SubDescriptor to append
    IAAFSubDescriptor * pSubDescriptor
  );

  //***********************************************************
  // METHOD NAME: GetSubDescriptorAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | GetSubDescriptorAt |
  // Retrieves the subdescriptor at the given index.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSubDescriptor pointer is valid.
  /// - index is less than the value returned by CountSubDescriptors().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSubDescriptor is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountSubDescriptors().
  // @end
  // 
  STDMETHOD (GetSubDescriptorAt)
   (
    // @parm [in] aafUInt32 | index | index of subdescriptor to retrieve
    aafUInt32  index,

    // @parm [out, retval] AAFSubDescriptor | ppSubDescriptor | returned subdescriptor
    IAAFSubDescriptor ** ppSubDescriptor
  );

  //***********************************************************
  // METHOD NAME: RemoveSubDescriptorAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | RemoveSubDescriptorAt |
  // Removes the subdescriptor at the given index.  SubDescriptors already
  /// existing at indices higher than the given index will be moved to
  /// the next lower index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - index is less than the value returned by CountSubDescriptors().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountSubDescriptors().
  // @end
  // 
  STDMETHOD (RemoveSubDescriptorAt)
   (
    // @parm [in] aafUInt32 | index | index of subdescriptor to remove
    aafUInt32  index
  );

  //***********************************************************
  // METHOD NAME: GetSubDescriptors()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceDescriptor2 | GetSubDescriptors |
  // Returns an enumerator to the subdescriptors.  The number of subdescriptors
  /// may be zero if the essence is in the current file.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  // @end
  // 
  STDMETHOD (GetSubDescriptors)
   (
    // @parm [out] EnumAAFSubDescriptors | ppEnum | An enumerator to the subdescriptors on this essence descriptor
    IEnumAAFSubDescriptors ** ppEnum
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

#endif // ! __CAAFEssenceDescriptor_h__

