//@doc
//@class    AAFOperationDef | Implementation class for AAFOperationDef
#ifndef __ImplAAFOperationDef_h__
#define __ImplAAFOperationDef_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFLocator;
class ImplAAFParameterDef;
class ImplAAFOperationDef;
template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFLocator> ImplEnumAAFLocators;
typedef ImplAAFEnumerator<ImplAAFOperationDef> ImplEnumAAFOperationDefs;
typedef ImplAAFEnumerator<ImplAAFParameterDef> ImplEnumAAFParameterDefs;

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "OMWeakRefProperty.h"
#include "OMWeakRefVectorProperty.h"
#include "OMWeakRefSetProperty.h"

#include "ImplAAFDataDef.h"
#include "ImplAAFParameterDef.h"
#include "ImplEnumAAFOperationDefs.h"
#include "ImplEnumAAFParameterDefs.h"


class ImplAAFOperationDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFOperationDef ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const aafCharacter *name,
		 const aafCharacter *description);


protected:
  virtual ~ImplAAFOperationDef ();

public:

  //****************
  // GetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDef
        // @parm [out,retval] pointer to the return value
        (ImplAAFDataDef ** ppDataDef);

  //****************
  // SetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDataDef
        // @parm [in] Pointer to the new data Definition Object
        (ImplAAFDataDef * pDataDef);

  //****************
  // IsTimeWarp()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsTimeWarp
        // @parm [out,retval] pointer to the return value
        (aafBool *  bIsTimeWarp);

  //****************
  // SetIsTimeWarp()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsTimeWarp
        // @parm [in] Degrade To Operation Definition Object
        (aafBool  IsTimeWarp);

  //****************
  // PrependDegradeToOperation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependDegradeToOperation (
        // @parm [in] is timewarp value
      ImplAAFOperationDef  *pOperationDef);

  //****************
  // AppendDegradeToOperation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendDegradeToOperation (
        // @parm [in] is timewarp value
      ImplAAFOperationDef  *pOperationDef);

  //****************
  // InsertDegradeToOperationAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertDegradeToOperationAt (
        // @parm [in] index to insert
      aafUInt32 index,
        // @parm [in] operation def to insert
      ImplAAFOperationDef  *pOperationDef);

  //****************
  // RemoveDegradeToOperationAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveDegradeToOperationAt (
        // @parm [in] index of operation def to remove
      aafUInt32 index);

  //****************
  // GetDegradeToOperations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDegradeToOperations (
        // @parm [out] Operation Definition Enumeration
      ImplEnumAAFOperationDefs  **ppEnum);

  //****************
  // CountDegradeToOperations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountDegradeToOperations (
        // @parm [out, retval] result
      aafUInt32 * pResult);

  //****************
  // GetCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCategory (
        aafUID_t		*pCategory);

			//****************
  // SetCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCategory
        // @parm [in] category
        (const aafUID_t category);

  //****************
  // GetNumberInputs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumberInputs
        // @parm [out,retval] Pointer to Number of Inputs
        (aafInt32 *  pNumberInputs);

  //****************
  // SetNumberInputs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetNumberInputs
        // @parm [in] Number of Inputs
        (aafInt32  NumberInputs);

  //****************
  // GetBypass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBypass
        // @parm [out,retval] Pointer to a Bypass media segment index 
        (aafUInt32 *  pBypass);

  //****************
  // SetBypass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBypass
        // @parm [in] Bypass media segment index
        (aafUInt32  baypass);

  //****************
  // AppendParameterDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddParameterDef
        // @parm [in] Parameter definition Object
        (ImplAAFParameterDef * pAAFParameterDef);

  //****************
  // GetParameterDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefs
        // @parm [out,retval] Parameter definition enumeration
        (ImplEnumAAFParameterDefs ** ppEnum);

  //****************
  // CountParameterDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountParameterDefs
        // @parm [out,retval] count of Parameter definitions
        (aafUInt32 * pResult);

  //****************
  // LookupParameterDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupParameterDef
        // @parm [in] id to look up
        (const aafUID_t & parameterDefId,
        // @parm [out, retval] returned parameter def
		 ImplAAFParameterDef ** ppParameterDef);

private:
	OMWeakReferenceProperty<ImplAAFDataDef>			_dataDef;
	OMFixedSizeProperty<aafBool>					_isTimeWarp;
	OMWeakReferenceVectorProperty<ImplAAFOperationDef>	_degradeTo;
	OMFixedSizeProperty<aafUID_t>                   _category;
	OMFixedSizeProperty<aafInt32>					_numInputs;
	OMFixedSizeProperty<aafUInt32>					_bypass;
	OMWeakReferenceSetProperty<ImplAAFParameterDef>	_paramDefined;
};

#endif // ! __ImplAAFOperationDef_h__


