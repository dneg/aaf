//@doc
//@class    AAFOperationDef | Implementation class for AAFOperationDef
#ifndef __ImplAAFOperationDef_h__
#define __ImplAAFOperationDef_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


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


