/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif






#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFConstValue_h__
#include "ImplAAFConstValue.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFConstValue::ImplAAFConstValue ()
{}


ImplAAFConstValue::~ImplAAFConstValue ()
{}


	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstValue::GetValue (
      aafUInt32  /*valueSize*/,
      aafDataBuffer_t  /*pValue*/,
      aafUInt32*  /*bytesRead*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstValue::GetValueBufLen (
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstValue::SetValue (
      aafUInt32  /*valueSize*/,
      aafDataBuffer_t  /*pValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFConstValue, AUID_AAFConstValue);


