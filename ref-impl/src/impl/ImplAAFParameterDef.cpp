/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFReferenceValue_h__
#include "ImplAAFReferenceValue.h"
#endif

#ifndef __ImplEnumAAFReferenceValues_h__
#include "ImplEnumAAFReferenceValues.h"
#endif


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

 


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFParameterDef::ImplAAFParameterDef ()
{}


ImplAAFParameterDef::~ImplAAFParameterDef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetParameterDataDefID (
      aafUID_t *  /*pParameterDataDefID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetParameterDataDefID (
      aafUID_t  /*ParameterDataDefID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnits (
      wchar_t *  /*pDisplayUnits*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnitsBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetDisplayUnits (
      wchar_t *  /*pDisplayUnits*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::AddReferenceValue (
      ImplAAFReferenceValue * /*pReferenceValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetRefValues (
      ImplEnumAAFReferenceValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFParameterDef, AUID_AAFParameterDef);


