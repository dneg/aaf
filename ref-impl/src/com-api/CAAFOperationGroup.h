//@doc
//@class    AAFOperationGroup | Implementation class for AAFOperationGroup
#ifndef __CAAFOperationGroup_h__
#define __CAAFOperationGroup_h__

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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif












#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFOperationGroup
  : public IAAFOperationGroup,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFOperationGroup (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFOperationGroup ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes an operation group object with the given property
  // values.
  //
  // This function takes an already created operation definition
  // object as an argument.  To add slots to the operation group, call
  // AddNewSlot.  To add renderings, call SetRender.
  //
  STDMETHOD (Initialize) (
    // Data Definition Object
    /*[in]*/ IAAFDataDef * pDataDef,

    // Length property value
    /*[in]*/ aafLength_t  length,

    // Operation Definition object
    /*[in]*/ IAAFOperationDef * operationDef);


  //***********************************************************
  //
  // GetOperationDefinition()
  //
  // Returns the operation definition for this invocation.
  // 
  // Succeeds if all of the following are true:
  // - the ppOperationDef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppOperationDef.
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
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppOperationDef arg is NULL.
  //
  STDMETHOD (GetOperationDefinition) (
    // Operation definition object 
    /*[out]*/ IAAFOperationDef ** ppOperationDef);

  //***********************************************************
  //
  // SetOperationDefinition()
  //
  // This function sets the operation definition for the given operation
  // group.
  // 
  // Succeeds if all of the following are true:
  // - the pOperationDef pointer is valid.
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
  //   - pOperationDef arg is NULL.
  //
  STDMETHOD (SetOperationDefinition) (
    // Operation definition object 
    /*[in]*/ IAAFOperationDef * pOperationDef);

  //***********************************************************
  //
  // GetRender()
  //
  // Returns the segment that represents the optional rendering on an
  // operation group object.  Working and final renderings are handled
  // by using an IAAFEssenceGroup as the segment. 
  // 
  // Succeeds if all of the following are true:
  // - the ppSourceRef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppSourceRef.
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
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppSourceRef arg is NULL.
  //
  STDMETHOD (GetRender) (
    // Final rendering segment 
    /*[out]*/ IAAFSourceReference ** ppSourceRef);


  //***********************************************************
  //
  // IsATimeWarp()
  //
  // This boolean function returns whether or not an operation group
  // is a timewarp effect.
  // 
  // Succeeds if all of the following are true:
  // - the pIsTimeWarp pointer is valid.
  // 
  // If this method fails nothing will be written to *pIsTimeWarp.
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
  //   - pIsTimeWarp arg is NULL.
  //
  STDMETHOD (IsATimeWarp) (
    // Returned boolean value 
    /*[out, retval]*/ aafBoolean_t *  pIsTimeWarp);


  //***********************************************************
  //
  // GetBypassOverride()
  //
  // Returns the optional bypass override propertyvalue from the input
  // operation def object.
  // 
  // Succeeds if all of the following are true:
  // - the pBypassOverride pointer is valid.
  // 
  // If this method fails nothing will be written to *pBypassOverride.
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
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - pBypassOverride arg is NULL.
  //
  STDMETHOD (GetBypassOverride) (
    // Bypass override property value 
    /*[out]*/ aafUInt32 *  pBypassOverride);


  //***********************************************************
  //
  // CountSourceSegments()
  //
  // Returns the number of media sources to the operation group.
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
  STDMETHOD (CountSourceSegments) (
    // Number of source media segments in the operation group 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // IsValidTranOperation()
  //
  // Verifies that the input operation group object can be used in a
  // transition.
  // 
  // Succeeds if all of the following are true:
  // - the pValidTransition pointer is valid.
  // 
  // If this method fails nothing will be written to *pValidTransition.
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
  //   - pValidTransition arg is NULL.
  //
  STDMETHOD (IsValidTranOperation) (
    // TRUE if the operation group is valid in a transition 
    /*[out]*/ aafBoolean_t *  pValidTransition);


  //***********************************************************
  //
  // CountParameters()
  //
  // Returns the number of parameters in the operation group.
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
  STDMETHOD (CountParameters) (
    // Number of parameter slots in the operation group 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // AddParameter()
  //
  // Adds a new parameter object.
  //
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_DUPLICATE_PARAMETER
  //   - The given argID is already present.
  //
  // AAFRESULT_NULL_PARAM
  //   - pValue arg is NULL.
  //
  STDMETHOD (AddParameter) (
    // Parameter to place in operation group slot 
    /*[in]*/ IAAFParameter * pParameter);


  //***********************************************************
  //
  // AppendInputSegment()
  //
  // Appends another input segment to an operation group.
  // 
  // Succeeds if all of the following are true:
  // - the pSegment pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_DUPLICATE_INDEX
  //   - The given index value is already present.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSegment arg is NULL.
  //
  STDMETHOD (AppendInputSegment) (
    // Segment to place in operation group 
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // PrependInputSegment()
  //
  // Prepends another input segment to an operation group.
  // 
  // Succeeds if all of the following are true:
  // - the pSegment pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_DUPLICATE_INDEX
  //   - The given index value is already present.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSegment arg is NULL.
  //
  STDMETHOD (PrependInputSegment) (
    // Segment to place in operation group 
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // InsertInputSegmentAt()
  //
  // Inserts an input segment to an operation group at the given
  // index.  The existing segments at the given and higher indices are
  // moved up by one to make room.
  // 
  // Succeeds if all of the following are true:
  // - the pSegment pointer is valid.
  // - the index is not greater than the value returned by
  //   CountInputSegments().
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_DUPLICATE_INDEX
  //   - The given index value is already present.
  //
  // AAFRESULT_BAD_INDEX
  //   - index is greater than the value returned by
  //     CountInputSegments().
  //
  // AAFRESULT_NULL_PARAM
  //   - pSegment arg is NULL.
  //
  STDMETHOD (InsertInputSegmentAt) (
    // index to place this segment
    /*[in]*/ aafUInt32  index,

    // Segment to place in operation group
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // SetRender()
  //
  // This function sets the final rendering for the given operation
  // group to the input source clip.  Multiple renderings may exist if
  // the source clip refers to a master mob that contains a Essence
  // group.
  //
  STDMETHOD (SetRender) (
    // A segment containing a representation of the rendering 
    /*[in]*/ IAAFSourceReference * ppSourceRef);
  // 
  // Succeeds if all of the following are true:
  // - the ppSourceRef pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  \(This is the only code indicating success.\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\(\) called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppSourceRef arg is NULL.)


  //***********************************************************
  //
  // SetBypassOverride()
  //
  // This function sets the optional bypass override property on the
  // given operation group object.
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
  STDMETHOD (SetBypassOverride) (
    // Bypass override 
    /*[in]*/ aafUInt32  bypassOverride);


  //***********************************************************
  //
  // GetParameters()
  //
  // returns an enumerator across all parameters.
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
  //   - ppEnum arg is NULL.
  //
  STDMETHOD (GetParameters) (
    // Parameter enumerator 
    /*[out]*/ IEnumAAFParameters ** ppEnum);


  //***********************************************************
  //
  // LookupParameter()
  //
  // Given an argID, returns the corresponding parameter slot and
  // parameter slot value.
  // 
  // Succeeds if all of the following are true:
  // - the ppParameter pointer is valid.
  // 
  // If this method fails nothing will be written to *ppParameter.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_MISSING_PARAMETER
  //   - The given argID is not present.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppParameter arg is NULL.
  //
  STDMETHOD (LookupParameter) (
    // Arg ID
    /*[in]*/ aafArgIDType_constref  argID,

    // Parameter object
    /*[out]*/ IAAFParameter ** ppParameter);


  //***********************************************************
  //
  // GetInputSegmentAt()
  //
  // Given an index, returns the corresponding input segment.
  // Working and final renderings are handled by using an
  // IAAFEssenceGroup as the segment.
  // 
  // Succeeds if all of the following are true:
  // - the ppInputSegment pointer is valid.
  // 
  // If this method fails nothing will be written to *ppInputSegment.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_MISSING_INDEX
  //   - The given index value is not present.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppInputSegment arg is NULL.
  //
  STDMETHOD (GetInputSegmentAt) (
    // 0-based index into the effet inputs
    /*[in]*/ aafUInt32  index,

    // Input segment
    /*[out]*/ IAAFSegment ** ppInputSegment);


  //***********************************************************
  //
  // RemoveInputSegmentAt()
  //
  // Removes the segment at the given index.  Existing segments at
  // higher indices are moved down by one to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - index is less than the result of CountInputSegments().
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
  // AAFRESULT_MISSING_INDEX
  //   - The given index value is not present.
  //
  STDMETHOD (RemoveInputSegmentAt) (
    // 0-based index into the effect inputs 
    /*[in]*/ aafUInt32  index);

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

#endif // ! __CAAFOperationGroup_h__


