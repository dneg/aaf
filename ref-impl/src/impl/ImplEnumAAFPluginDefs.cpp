/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __ImplAAFPluginDescriptor_h__
#include "ImplAAFPluginDescriptor.h"
#endif





#ifndef __ImplEnumAAFPluginDescriptors_h__
#include "ImplEnumAAFPluginDesc.h"
#endif

#include <assert.h>
#include <string.h>


ImplEnumAAFPluginDescriptors::ImplEnumAAFPluginDescriptors ()
{}


ImplEnumAAFPluginDescriptors::~ImplEnumAAFPluginDescriptors ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::NextOne (
      ImplAAFPluginDescriptor ** /*ppAAFPluginDescriptor*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Next (
      aafUInt32  /*count*/,
      ImplAAFPluginDescriptor ** /*ppAAFPluginDescriptors*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Reset ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Clone (
      ImplEnumAAFPluginDescriptors ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}




