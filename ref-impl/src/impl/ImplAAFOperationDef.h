//@doc
//@class    AAFEffectDef | Implementation class for AAFEffectDef
#ifndef __ImplAAFEffectDef_h__
#define __ImplAAFEffectDef_h__


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


class ImplAAFParameterDef;

class ImplAAFLocator;

class ImplEnumAAFLocators;

class ImplEnumAAFParameterDefs;

class ImplEnumAAFEffectDefs;

 
#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "ImplAAFParameterDef.h"
#include "ImplEnumAAFEffectDefs.h"
#include "ImplEnumAAFParameterDefs.h"


class ImplAAFEffectDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEffectDef ();

protected:
  virtual ~ImplAAFEffectDef ();

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
    PrependDegradeToEffects (
        // @parm [in] is timewarp value
      ImplAAFEffectDef  *pEffectDef);

  //****************
  // AppendDegradeToEffects()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendDegradeToEffects (
        // @parm [in] is timewarp value
      ImplAAFEffectDef  *pEffectDef);

  //****************
  // GetDegradeToEffects()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDegradeToEffects (
        // @parm [out] Effect Definition Enumeration
      ImplEnumAAFEffectDefs  **ppEnum);

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

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEffectDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFEffectDefTest.cpp.
  static AAFRESULT test();
private:
	OMFixedSizeProperty<aafUID_t>					_dataDef;
	OMFixedSizeProperty<aafBool>					_isTimeWarp;
//!!!	effectDefWeakRefArrayProp_t						_degradeTo;
	OMWideStringProperty                            _category;
	OMFixedSizeProperty<aafInt32>					_numInputs;
	OMFixedSizeProperty<aafUInt32>					_bypass;
	parmDefWeakRefArrayProp_t						_paramDefined;
};

#endif // ! __ImplAAFEffectDef_h__


