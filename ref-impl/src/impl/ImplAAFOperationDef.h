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
 *  prior written permission of Avid Technology, Inc.
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


class ImplAAFParameterDef;

class ImplAAFLocator;

class ImplEnumAAFLocators;

class ImplEnumAAFParameterDefs;

class ImplEnumAAFOperationDefs;

 
#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

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

protected:
  virtual ~ImplAAFOperationDef ();

public:

  //****************
  // GetDataDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDefinitionID
        // @parm [out,retval] pointer to the return value
        (aafUID_t *pDataDefID);

  //****************
  // SetDataDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDataDefinitionID
        // @parm [in] Pointer to the new data Definition Object
        (aafUID_t  *pDataDefID);

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
        // @parm [in] Degrade To Effect Definition Object
        (aafBool  IsTimeWarp);

  //****************
  // PrependDegradeToEffects()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependDegradeToOperations (
        // @parm [in] is timewarp value
      ImplAAFOperationDef  *pOperationDef);

  //****************
  // AppendDegradeToEffects()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendDegradeToOperations (
        // @parm [in] is timewarp value
      ImplAAFOperationDef  *pOperationDef);

  //****************
  // GetDegradeToEffects()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDegradeToOperations (
        // @parm [out] Operation Definition Enumeration
      ImplEnumAAFOperationDefs  **ppEnum);

  //****************
  // GetCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCategory (
        wchar_t		*pCategory,
		aafInt32	bufSize);

  //****************
  // GetCategoryBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	GetCategoryBufLen (
			aafInt32 *		pLen);

			//****************
  // SetCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCategory
        // @parm [in] category
        (wchar_t *category);

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
    AddParameterDefs
        // @parm [in] Parameter definition Object
        (ImplAAFParameterDef * pAAFParameterDef);

  //****************
  // GetParameterDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefinitions
        // @parm [out,retval] Parameter definition enumeration
        (ImplEnumAAFParameterDefs ** ppEnum);

private:
	OMFixedSizeProperty<aafUID_t>					_dataDef;
	OMFixedSizeProperty<aafBool>					_isTimeWarp;
	OperationDefWeakRefArrayProp_t						_degradeTo;
	OMWideStringProperty                            _category;
	OMFixedSizeProperty<aafInt32>					_numInputs;
	OMFixedSizeProperty<aafUInt32>					_bypass;
	parmDefWeakRefArrayProp_t						_paramDefined;
};

#endif // ! __ImplAAFOperationDef_h__


