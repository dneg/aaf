//@doc
//@class    AAFParameterDef | Implementation class for AAFParameterDef
#ifndef __ImplAAFParameterDef_h__
#define __ImplAAFParameterDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




class ImplEnumAAFReferenceValues;
#include "ImplAAFReferenceValue.h"


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

 



#ifndef __ImplAAFPluggableDef_h__
#include "ImplAAFPluggableDef.h"
#endif

class ImplAAFTypeDef;

class ImplAAFParameterDef : public ImplAAFPluggableDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFParameterDef ();

protected:
  virtual ~ImplAAFParameterDef ();

public:


  //****************
  // GetParameterDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDef
        // @parm [retval][out] Pointer to an AUID reference
        (ImplAAFTypeDef **  pParameterDataDefID);

  //****************
  // SetParameterDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTypeDef
        // @parm [in] an AUID reference
        (ImplAAFTypeDef * ParameterDataDefID);

  //****************
  // GetDisplayUnits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayUnits
        (// @parm [in,string] DisplayUnits
         wchar_t *  pDisplayUnits,

         // @parm [in] length of the buffer to hold DisplayUnits
         aafInt32  bufSize);

  //****************
  // GetDisplayUnitsBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayUnitsBufLen
        // @parm [out] DisplayUnits
        (aafInt32 *  pLen);



  //****************
  // SetDisplayUnits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayUnits
        // @parm [in, string] DisplayUnits
        (wchar_t *  pDisplayUnits);

  //****************
  // AddReferenceValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddReferenceValue
        // @parm [in] Ref value to be added
        (ImplAAFReferenceValue * pReferenceValue);

  //****************
  // GetRefValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRefValues
        // @parm [out,retval] Reference value Enumeration
        (ImplEnumAAFReferenceValues ** ppEnum);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFParameterDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFParameterDefTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafUID_t>							_typeDef;
    OMStrongReferenceVectorProperty<ImplAAFReferenceValue>	_refVal;
	OMWideStringProperty									_displayUnits;
};

#endif // ! __ImplAAFParameterDef_h__

