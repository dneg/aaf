//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFAuxiliaryDescriptor_h__
#include "ImplAAFAuxiliaryDescriptor.h"
#endif

#include "AAFPropertyIDs.h"

#include <string.h>


ImplAAFAuxiliaryDescriptor::ImplAAFAuxiliaryDescriptor ()
  : _mimeType( PID_AuxiliaryDescriptor_MimeType, L"MimeType" ),
    _charSet( PID_AuxiliaryDescriptor_CharSet, L"CharSet" )
{
  _persistentProperties.put( _mimeType.address() );
  _persistentProperties.put( _charSet.address() );
}


ImplAAFAuxiliaryDescriptor::~ImplAAFAuxiliaryDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::SetMimeType (
      aafCharacter_constptr pMimeType )
{
  return SetString( pMimeType, _mimeType );
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::GetMimeType (
      aafCharacter *  pMimeType,
      aafUInt32  bufSize)
{
  return GetString( pMimeType, bufSize, false, _mimeType );
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::GetMimeTypeBufLen (
      aafUInt32 *  pBufSize)
{
  return GetStringBufLen( pBufSize, false, _mimeType );
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::SetCharSet (
      aafCharacter_constptr  pCharSet)
{
  return SetString( pCharSet, _charSet );
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::GetCharSet (
      aafCharacter *  pCharSet,
      aafUInt32  bufSize)
{
  return GetString( pCharSet, bufSize, true, _charSet );
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFAuxiliaryDescriptor::GetCharSetBufLen (
      aafUInt32 *  pBufSize)
{
  return GetStringBufLen( pBufSize, true, _charSet );
}


// FIXME These are cut/paste/modify version of the the
// ImplAAFMob::{Get,Set}Name.  The mob routines, and many others,
// could be factored out and a variation of these routines used.

AAFRESULT ImplAAFAuxiliaryDescriptor::SetString (aafCharacter_constptr  pString,
						 OMWideStringProperty& theStoredString )
{
  if(pString == NULL)
    return(AAFRESULT_NULL_PARAM);
  
  theStoredString = pString;
  
  return(AAFRESULT_SUCCESS); 
}

AAFRESULT ImplAAFAuxiliaryDescriptor::GetString( aafCharacter* pString, aafUInt32 bufSize,
						 bool isOptional,
						 OMWideStringProperty& theStoredString )
{
  bool stat;
  
  if(pString == NULL)
    return(AAFRESULT_NULL_PARAM);
  
  if( isOptional && !theStoredString.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;
  else
    stat = theStoredString.copyToBuffer(pString, bufSize);
  
  if (!stat) {
    return AAFRESULT_SMALLBUF;
  }
  
  return(AAFRESULT_SUCCESS); 
  
}


AAFRESULT ImplAAFAuxiliaryDescriptor::GetStringBufLen( aafUInt32 *  pBufSize,
						       bool isOptional,
						       OMWideStringProperty& theStoredString )
{
  if(pBufSize == NULL)
    return(AAFRESULT_NULL_PARAM);
  
  if( isOptional && !theStoredString.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;
  else {
    *pBufSize = theStoredString.size();
    return(AAFRESULT_SUCCESS); 
  }
}
