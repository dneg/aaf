//@doc
//@class    AAFVaryingValue | Implementation class for AAFVaryingValue
#ifndef __CAAFVaryingValue_h__
#define __CAAFVaryingValue_h__

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










#ifndef __CAAFParameter_h__
#include "CAAFParameter.h"
#endif


class CAAFVaryingValue
  : public IAAFVaryingValue,
    public CAAFParameter
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFVaryingValue (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFVaryingValue ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a new varying value object to be identified with the
  // given the given parameter definition and interpolation definition.
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pParameterDef and pInterpolation are a valid pointers.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - This object has already had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pParameterDef or pInterpolation arg is NULL.
  // 
  //
  STDMETHOD (Initialize) (
    // Parameter definition for this object (this determines the type of the varying value)
    /*[in]*/ IAAFParameterDef * pParameterDef,

    // InterpolationDefinition object
    /*[in]*/ IAAFInterpolationDef * pInterpolation);

  //***********************************************************
  //
  // AddControlPoint()
  //
  // Adds a control point to the AAFVaryingValue object.
  // The point will be sorted by time order, not the order in which
  // the points were added.
  // 
  // Succeeds if all of the following are true:
  // - the pControlPoint pointer is valid.
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
  //   - pControlPoint is null.
  //
  STDMETHOD (AddControlPoint) (
    // pointer to IAAFControlPoint object 
    /*[in]*/ IAAFControlPoint * pControlPoint);


  //***********************************************************
  //
  // GetControlPoints()
  //
  // Return an enumerator for the list of IAAFControlPoints.  The list
  // will be returned in time order, not the order in which the
  // points were added.
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
  STDMETHOD (GetControlPoints) (
    // Parameter definition enumeration 
    /*[out,retval]*/ IEnumAAFControlPoints ** ppEnum);


  //***********************************************************
  //
  // CountControlPoints()
  //
  // This function returns the number of control points in the sequence.
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
  STDMETHOD (CountControlPoints) (
    // Number of control points 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetControlPointAt()
  //
  // This function retrieves the input control point at the given index in
  // the given sequence.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the ppControlPoint pointer is valid.
  // - index is less than the value returned by
  //   CountControlPoints().
  // 
  // If this method fails nothing will be written to *ppControlPoint.
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
  //   - pControlPoint is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to the value returned by
  //     CountControlPoints().
  //
  STDMETHOD (GetControlPointAt) (
    // index of control point to retrieve
    /*[in]*/ aafUInt32  index,

    // Retrieved control point
    /*[out, retval]*/ IAAFControlPoint ** ppControlPoint);


  //***********************************************************
  //
  // RemoveControlPointAt()
  //
  // This function removes the input control point at the given index in
  // the given sequence.  Control points already existing at indices
  // higher than the given index will be moved to the next lower index
  // to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - index is less than the value returned by
  //   CountControlPoints().
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
  //     CountControlPoints().
  //
  STDMETHOD (RemoveControlPointAt) (
    // index of control point to remove 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // GetInterpolationDefinition()
  //
  // Places the InterpolationDefinition object attached to this
  // VaryingValue into the *ppInterpolation argument.  If none exists
  // yet, NULL is placed into the *ppInterpolation argument.
  //
  // The returned InterpolationDefinition object, if it exists, is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppInterpolation pointer is valid.
  // - A valid InterpolationDefinition exists.
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
  //   - ppInterpolation is null.
  //
  STDMETHOD (GetInterpolationDefinition) (
    // Returned InterpolationDefinition object 
    /*[out]*/ IAAFInterpolationDef ** ppInterpolation);

  //***********************************************************
  //
  // GetValueBufLen()
  //
  // Returns the length of buffer required for the
  // GetInterpolatedValue() method.  The value is placed into the
  // location specified by pLen.
  //
  // Succeeds if all of the following are true:
  // - the pLen pointer is valid.
  //
  // If this method fails nothing will be written to *pLen.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLen arg is NULL.
  //
  STDMETHOD (GetValueBufLen) (
    // Mob Name 
    /*[out]*/ aafInt32 *  pLen);


  //***********************************************************
  //
  // GetInterpolatedValue()
  //
  // Writes the interpolated value of the IAAFVaryingValue at a given
  // position into the pValue buffer.  The buffer is allocated by the
  // caller, and the size of the buffer is given by valueSize.
  // 
  // Caller may call GetValueBufLen() to determine the
  // required buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // - valueSize indicates the buffer is large enough to hold the name.
  // 
  // If this method fails nothing will be written to *pValue.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pValue arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - valueSize indicates the buffer is too small to hold the value.
  //
  STDMETHOD (GetInterpolatedValue) (
    // Position along the operation group
    /*[in]*/ aafRational_t  inputValue,

    // Size of preallocated buffer
    /*[in]*/ aafInt32  valueSize,

    // Preallocated buffer to hold value
    /*[out,
   size_is(valueSize),length_is(*bytesRead)]*/ aafDataBuffer_t  pValue,

    // Number of actual bytes read
    /*[out]*/ aafInt32 *  bytesRead);

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

#endif // ! __CAAFVaryingValue_h__


