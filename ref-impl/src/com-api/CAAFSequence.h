//@doc
//@class    AAFSequence | Implementation class for AAFSequence
#ifndef __CAAFSequence_h__
#define __CAAFSequence_h__

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










#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFSequence
  : public IAAFSequence,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSequence (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSequence ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // This function sets the properties on a newly created sequence
  // object with the given property values. The length of the sequence
  // is initially set to 0.  When components are appended to the
  // sequence with the AppendComponent() call, the length of the
  // appended component is added to the length of the sequence.  The
  // given DataDef specifies the kind of data which all components to
  // be contained in this sequence must share.
  // 
  // Succeeds if all of the following are true:
  // - this object has not yet been initialized.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDataDef is null.
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize) (
    // DataDef of this object 
    /*[in]*/ IAAFDataDef * pDataDef);


  //***********************************************************
  //
  // AppendComponent()
  //
  // This function appends the input component to the given sequence,
  // enforcing bottom up creation of mobs.  The length of the sequence
  // is incremented by the size of the component, unless the
  // component is a transition.  If the component is a transition, it
  // verifies that it is not the first object in a transition, and
  // that it is not neighboring another transition.  It also verifies
  // that there is enough source material on either side of the
  // transition.  The function also verifies that the datadefs are
  // compatible.
  //
  // This method will AddRef() the component if it succeeds.
  //
  // If the component is successfully appended to the sequence, the
  // reference count of the component is incremented.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pComponent pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pComponent is null.
  //
  // AAFRESULT_INVALID_DATADEF
  //   - The data kind of the component is not compatible with the 
  //     data def of the sequence.
  //
  // AAFRESULT_LEADING_TRAN
  //   - Attempted to append a transition as the first component of
  //     a sequence.  A sequence can not start with a transition. 
  //
  // AAFRESULT_ADJACENT_TRAN
  //   - Attempted to append a transition next to a transition.  A
  //     sequence can not contain back to back transitions.
  //
  // AAFRESULT_INSUFF_TRAN_MATERIAL
  //   - There is not enough source material to add this component.
  //
  STDMETHOD (AppendComponent) (
    // Component to append to the sequence 
    /*[in]*/ IAAFComponent * pComponent);


  //***********************************************************
  //
  // PrependComponent()
  //
  // This function prepends the input component to the given
  // sequence, enforcing bottom up creation of mobs.  The length of
  // the sequence is incremented by the size of the component, unless
  // the component is a transition.  If the component is a
  // transition, it verifies that it is not the first object in a
  // transition, and that it is not neighboring another transition.
  // It also verifies that there is enough source material on either
  // side of the transition.  The function also verifies that the
  // datadefs are compatible.
  //
  // This method will AddRef() the component if it succeeds.
  //
  // If the component is successfully appended to the sequence, the
  // reference count of the component is incremented.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pComponent pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pComponent is null.
  //
  // AAFRESULT_INVALID_DATADEF
  //   - The data kind of the component is not compatible with the 
  //     data def of the sequence.
  //
  // AAFRESULT_LEADING_TRAN
  //   - Attempted to append a transition as the first component of
  //     a sequence.  A sequence can not start with a transition. 
  //
  // AAFRESULT_ADJACENT_TRAN
  //   - Attempted to append a transition next to a transition.  A
  //     sequence can not contain back to back transitions.
  //
  // AAFRESULT_INSUFF_TRAN_MATERIAL
  //   - There is not enough source material to add this component.
  //
  STDMETHOD (PrependComponent) (
    // Component to prepend to the sequence 
    /*[in]*/ IAAFComponent * pComponent);


  //***********************************************************
  //
  // InsertComponentAt()
  //
  // This function inserts the input component into the given sequence
  // at the given index, enforcing bottom up creation of mobs.  The
  // length of the sequence is incremented by the size of the
  // component, unless the component is a transition.  If the
  // component is a transition, it verifies that it is not the first
  // object in a transition, and that it is not neighboring another
  // transition.  It also verifies that there is enough source
  // material on either side of the transition.  The function also
  // verifies that the datadefs are compatible.
  //
  // This method will AddRef() the component if it succeeds.
  //
  // If the component is successfully appended to the sequence, the
  // reference count of the component is incremented.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pComponent pointer is valid.
  // - index is less than or equal to the value returned by
  //   CountComponents().
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pComponent is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than the value returned by
  //     CountComponents().
  //
  // AAFRESULT_INVALID_DATADEF
  //   - The data kind of the component is not compatible with the 
  //     data def of the sequence.
  //
  // AAFRESULT_LEADING_TRAN
  //   - Attempted to append a transition as the first component of
  //     a sequence.  A sequence can not start with a transition. 
  //
  // AAFRESULT_ADJACENT_TRAN
  //   - Attempted to append a transition next to a transition.  A
  //     sequence can not contain back to back transitions.
  //
  // AAFRESULT_INSUFF_TRAN_MATERIAL
  //   - There is not enough source material to add this component.
  //
  STDMETHOD (InsertComponentAt) (
    // index to insert component
    /*[in]*/ aafUInt32  index,

    // Component to insert into the sequence
    /*[in]*/ IAAFComponent * pComponent);


  //***********************************************************
  //
  // GetComponentAt()
  //
  // This function retrieves the input component at the given index in
  // the given sequence.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the ppComponent pointer is valid.
  // - index is less than the value returned by
  //   CountComponents().
  // 
  // If this method fails nothing will be written to *ppComponent.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pComponent is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to the value returned by
  //     CountComponents().
  //
  STDMETHOD (GetComponentAt) (
    // index of component to retrieve
    /*[in]*/ aafUInt32  index,

    // Retrieved component
    /*[out, retval]*/ IAAFComponent ** ppComponent);


  //***********************************************************
  //
  // RemoveComponentAt()
  //
  // This function removes the input component at the given index in
  // the given sequence.  Components already existing at indices
  // higher than the given index will be moved to the next lower index
  // to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - index is less than the value returned by
  //   CountComponents().
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
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to the value returned by
  //     CountComponents().
  //
  STDMETHOD (RemoveComponentAt) (
    // index of component to remove 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // CountComponents()
  //
  // This function returns the number of components in the sequence.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pResult pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  //
  STDMETHOD (CountComponents) (
    // Number of components 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetComponents()
  //
  // Places an IEnumAAFComponents enumerator for the components
  // contained in the sequence into the *ppEnum argument.
  // 
  // The returned enumerator is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
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
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  // E_FAIL
  //   - Failed to create the enumerator.
  //
  STDMETHOD (GetComponents) (
    // Component Enumeration 
    /*[out, retval]*/ IEnumAAFComponents ** ppEnum);







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

#endif // ! __CAAFSequence_h__


