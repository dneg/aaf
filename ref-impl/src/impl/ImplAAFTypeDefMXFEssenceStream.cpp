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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFTypeDefMXFEssenceStream.h"
#include "ImplAAFStreamPropertyValue.h"


//
// Macro to encapsulate the convertions and validation of a ImplAAFPropertyValue
// into an ImplAAFStreamPropertyValue.
//
#define PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pv, spv)\
  AAFRESULT result = AAFRESULT_SUCCESS;\
  ImplAAFStreamPropertyValue * spv = NULL;\
  result = GetStreamPropertyValue(pv, spv);\
  if (AAFRESULT_FAILED(result))\
    return result



ImplAAFTypeDefMXFEssenceStream::ImplAAFTypeDefMXFEssenceStream ()
: ImplAAFTypeDefStream()
{
}


ImplAAFTypeDefMXFEssenceStream::~ImplAAFTypeDefMXFEssenceStream ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::GetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetSizeFiltered(pSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::SetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetSizeFiltered(newSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::GetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetPositionFiltered(pPosition);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::SetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetPositionFiltered(newPosition);
}



AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefMXFEssenceStream::Read (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ReadFiltered(dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::Write (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->WriteFiltered(dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::Append (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->AppendFiltered(dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefMXFEssenceStream::ReadElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ReadElementsFiltered(pElementType, dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::WriteElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->WriteElementsFiltered(pElementType, dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefMXFEssenceStream::AppendElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->AppendElementsFiltered(pElementType, dataSize, pData);
}



// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
/*
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefMXFEssenceStream::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
 
  result = ImplAAFTypeDefStream::CreatePropertyValue (property, ppPropertyValue);
  return result;
}
*/


