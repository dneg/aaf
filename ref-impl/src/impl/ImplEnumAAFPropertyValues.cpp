//=---------------------------------------------------------------------=
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



#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif




#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#include <assert.h>
#include <string.h>


ImplEnumAAFPropertyValues::ImplEnumAAFPropertyValues ()
{}


ImplEnumAAFPropertyValues::~ImplEnumAAFPropertyValues ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::NextOne (
      ImplAAFPropertyValue ** /*ppPropertyValue*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Next (
      aafUInt32  /*count*/,
      ImplAAFPropertyValue ** /*ppPropertyValues*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Reset ()
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Clone (
      ImplEnumAAFPropertyValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



