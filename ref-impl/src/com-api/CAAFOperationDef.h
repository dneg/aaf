//@doc
//@class    AAFOperationDef | Implementation class for AAFOperationDef
#ifndef __CAAFOperationDef_h__
#define __CAAFOperationDef_h__

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








 


#ifndef __CAAFDefObject_h__
#include "CAAFDefObject.h"
#endif


class CAAFOperationDef
  : public IAAFOperationDef,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFOperationDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFOperationDef ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Init all fields of a definition object.
  //
  STDMETHOD (Initialize) (
    // AUID for new DeObject
    /*[in, ref]*/ aafUID_constref  id,

    // Name for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pName,

    // Description for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pDescription);


  //***********************************************************
  //
  // GetDataDef()
  //
  // Places the DataDefinition object attached to this
  // IAAFOperationDef into the *ppDataDef argument.  The data
  // definition will match one of the AUIDs in the file
  // AAFDataDefs.h, (which includes DDEF_PICTURE, and
  // DDEF_SOUND), and indicates what type of data the operation will
  // be performed upon.  For example, a video dissolve will have the
  // data def DEF_VIDEO.  If a data definition is used which is not
  // from AAFDataDefs.h, then the client is responsible for making
  // sure that a data definition object with that ID exists in the
  // dictionary.  The SDK will take care of creating the standard data
  // definitions.
  //
  // Succeeds if all of the following are true:
  // - the ppDataDefID pointer is valid.
  // - A valid DataDefinition exists.
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
  //   - ppDataDef is null.
  //
  // AAFRESULT_INVALID_DATA_DEF
  //   - There is no IAAFDataDefinition.  There has to be one of some
  //     kind for this to be a valid operation definition.
  //
  STDMETHOD (GetDataDef) (
    // Returned DataDefinition object 
    /*[out]*/ IAAFDataDef ** ppDataDef);


  //***********************************************************
  //
  // SetDataDef()
  //
  // Sets the IAAFDataDefinition of this IAAFOperationDef to be the
  // given one.  The
  // data definition will match one of the AUIDs in the file
  // AAFDataDefs.h, (which includes DDEF_PICTURE, and
  // DDEF_SOUND), and indicates what type of data the operation will
  // be performed upon.  For example, a video dissolve will have the
  // data def DEF_VIDEO.  If a data definition is used which is not
  // from AAFDataDefs.h, then the client is responsible for making
  // sure that a data definition object with that ID exists in the
  // dictionary.  The SDK will take care of creating the standard data
  // definitions.
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
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetDataDef) (
    // Essence Descriptor object 
    /*[in]*/ IAAFDataDef * pDataDef);


  //***********************************************************
  //
  // IsTimeWarp()
  //
  // Returns the value of isTimeWarp.  IsTimeWarp is true if the
  // length of an IAAFOperationGroup is different from the lengths of
  // the input segments.  For example, a slow motion effect.
  //
  // Succeeds if all of the following are true:
  // - the bIsTimeWarp pointer is valid.
  // 
  // If this method fails nothing will be written to *bIsTimeWarp.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - bIsTimeWarp arg is NULL.
  //
  STDMETHOD (IsTimeWarp) (
    // pointer to the return value 
    /*[out, retval]*/ aafBoolean_t *  bIsTimeWarp);


  //***********************************************************
  //
  // SetIsTimeWarp()
  //
  // Sets the IsTimeWarp boolean.  IsTimeWarp is true if the length of
  // an IAAFOperationGroup is different from the lengths of the input
  // segments.  For example, a slow motion effect.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetIsTimeWarp) (
    // is timewarp value 
    /*[in]*/ aafBoolean_t  IsTimeWarp);


  //***********************************************************
  //
  // PrependDegradeToOperation()
  //
  // Prepend another operation definition to the DegradeTo list of
  // definitions.  Use this function to add an operation definition to
  // be scanned first when searching for the a replacement (a more
  // desirable alternate operation).
  // 
  // Succeeds if all of the following are true:
  // - the pOperationDef pointer is valid.
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
  //   - pOperationDef is null.
  //
  STDMETHOD (PrependDegradeToOperation) (
    // Degrade To Operation Definition Object 
    /*[in]*/ IAAFOperationDef * pOperationDef);


  //***********************************************************
  //
  // AppendDegradeToOperation()
  //
  // Append another operation definition to the DegradeTo list of
  // definitions.  Use this function to add an operation definition to
  // be scanned last when searching for the a replacement (a less
  // desirable alternate operation).
  // 
  // Succeeds if all of the following are true:
  // - the pOperationDef pointer is valid.
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
  //   - pOperationDef is null.
  //
  STDMETHOD (AppendDegradeToOperation) (
    // Degrade To operation Definition Object 
    /*[in]*/ IAAFOperationDef * pOperationDef);


  //***********************************************************
  //
  // InsertDegradeToOperationAt()
  //
  // Insert an operation definition into the DegradeTo list of
  // definitions at the given index.  Operation definitions already
  // existing at the given and higher indices will be moved up to the
  // next higher index to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - the pOperationDef pointer is valid.
  // - index is less than or equal to the value returned by
  //   CountDegradeToOperations().
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
  //   - pOperationDef is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than the value returned by
  //     CountDegradeToOperations().
  //
  STDMETHOD (InsertDegradeToOperationAt) (
    // index where operation def is to be inserted
    /*[in]*/ aafUInt32  index,

    // Degrade To operation Definition Object
    /*[in]*/ IAAFOperationDef * pOperationDef);


  //***********************************************************
  //
  // RemoveDegradeToOperationAt()
  //
  // Removes the indexed operation definition from the DegradeTo list
  // of definitions.  Operation definitions already existing at
  // indices higher than the given index will be moved down to the
  // next lower index to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - index is less than the value returned by
  //   CountDegradeToOperations().
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
  // AAFRESULT_BADINDEX
  //   - index is greater or equal to than the value returned by
  //     CountDegradeToOperations().
  //
  STDMETHOD (RemoveDegradeToOperationAt) (
    // index where operation def is to be inserted 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // GetDegradeToOperations()
  //
  // Return an enumerator for aaf operation definitions, ordered from
  // the most desirable to the least desirable alternative.
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
  STDMETHOD (GetDegradeToOperations) (
    // Operation Definition Enumeration 
    /*[out, retval]*/ IEnumAAFOperationDefs ** ppEnum);


  //***********************************************************
  //
  // CountDegradeToOperations()
  //
  // Retrieves the number of Degrade To operations currently present
  // and places that number in *pResult.
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
  //   - pResult is null.
  //
  STDMETHOD (CountDegradeToOperations) (
    // resulting count of Degrade To operations 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetCategory()
  //
  // This method returns an AUID indicating the category of the given operation.
  //
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
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
  STDMETHOD (GetCategory) (
    // The enumeration describing the current category 
    /*[out]*/ aafUID_t*  pValue);


  //***********************************************************
  //
  // SetCategory()
  //
  // This method sets an AUID indicating the category of the given operation.
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
  STDMETHOD (SetCategory) (
    // The enumeration describing the current categor 
    /*[in, ref]*/ aafUID_constref  value);


  //***********************************************************
  //
  // GetNumberInputs()
  //
  // Gets the Number of input media segments.
  // 
  // Succeeds if all of the following are true:
  // - the pNumberInputs pointer is valid.
  // 
  // If this method fails nothing will be written to *pNumberInputs.
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
  //   - pNumberInputs arg is NULL.
  //
  STDMETHOD (GetNumberInputs) (
    // Pointer to Number of Inputs 
    /*[out, retval]*/ aafInt32 *  pNumberInputs);


  //***********************************************************
  //
  // SetNumberInputs()
  //
  // Sets the Number of input media segments.
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
  STDMETHOD (SetNumberInputs) (
    // Number of Inputs 
    /*[in]*/ aafInt32  NumberInputs);


  //***********************************************************
  //
  // GetBypass()
  //
  // Gets the Bypass media segment index, which is a value from 0
  // to one less than that returned by GetNumberInputs().  This
  // value allows the client application to pick one of the inputs
  // (foreground, background, etc.) to stand in for the
  // effect if it is not available, and none of the degrade to
  // effects are available.
  // 
  // Succeeds if all of the following are true:
  // - the pBypass pointer is valid.
  // 
  // If this method fails nothing will be written to *pBypass.
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
  //   - pBypass arg is NULL.
  //
  STDMETHOD (GetBypass) (
    // Pointer to a Bypass media segment index  
    /*[out,retval]*/ aafUInt32 *  pBypass);


  //***********************************************************
  //
  // SetBypass()
  //
  // Sets the media segment index, which is a value from 0 to one
  // less than that returned by GetNumberInputs().  This value
  // allows the client application to pick one of the inputs
  // (foreground, background, etc.) to stand in for the
  // effect if it is not available, and none of the degrade to
  // effects are available.
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
  STDMETHOD (SetBypass) (
    // Bypass media segment index 
    /*[in]*/ aafUInt32  bypass);


  //***********************************************************
  //
  // AddParameterDef()
  //
  // Add the Parameter Definition object to the unordered list of
  // parameter definitions. 
  // 
  // Succeeds if all of the following are true:
  // - the pAAFParameterDef pointer is valid. 
  // - the given Parameter Definition does not exists in this Operation
  //   Definition already
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
  //   - pAAFParameterDef is null.
  //
  // AAFRESULT_OBJECT_ALREADY_ATTACHED
  //	- pAAFParameterDef is already in this OperationDef
  //
  STDMETHOD (AddParameterDef) (
    // Parameter definition Object 
    /*[in]*/ IAAFParameterDef * pParameterDef);


  //***********************************************************
  //
  // GetParameterDefs()
  //
  // Return an enumerator for the unordered list of AAF Parameter
  // definitions.
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
  STDMETHOD (GetParameterDefs) (
    // Parameter definition enumeration 
    /*[out,retval]*/ IEnumAAFParameterDefs ** ppEnum);


  //***********************************************************
  //
  // CountParameterDefs()
  //
  // This method returns the number of parameter defs.
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
  STDMETHOD (CountParameterDefs) (
    // Number of parameter defs 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // LookupParameterDef()
  //
  // This method looks up the parameter def corresponding to the given
  // ID.
  //
  // Succeeds if all of the following are true:
  // - the given ID corresponds to a known parameter def
  // - the ppParameterDef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppParameterDef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pParameterDef arg is NULL.
  //
  // AAFRESULT_OBJECT_NOT_FOUND
  //   - given ID does not correspond to a known parameter def.
  //
  STDMETHOD (LookupParameterDef) (
    // ID of parameter def to look up
    /*[in]*/ aafUID_constref  parameterDefId,

    // resulting parameter def
    /*[out]*/ IAAFParameterDef ** ppParameterDef);

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

#endif // ! __CAAFOperationDef_h__


