//@doc
//@class    AAFAuxiliaryDescriptor | Implementation class for AAFAuxiliaryDescriptor
#ifndef __ImplAAFAuxiliaryDescriptor_h__
#define __ImplAAFAuxiliaryDescriptor_h__

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




#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif









#ifndef __ImplAAFPhysicalDescriptor_h__
#include "ImplAAFPhysicalDescriptor.h"
#endif


class ImplAAFAuxiliaryDescriptor : public ImplAAFPhysicalDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFAuxiliaryDescriptor ();

protected:
  virtual ~ImplAAFAuxiliaryDescriptor ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();


  //****************
  // SetMimeType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMimeType
        // @parm [in, string] buffer from which MimeType is to be read
        (aafCharacter_constptr  pMimeType);


  //****************
  // GetMimeType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMimeType
        (// @parm [out, string, size_is(bufSize)] buffer into which MimeType is to be written
         aafCharacter *  pMimeType,

         // @parm [in] size of *pMimeType buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetMimeTypeBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMimeTypeBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

  //****************
  // SetCharSet()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCharSet
        // @parm [in, string] buffer from which CharSet is to be read
        (aafCharacter_constptr  pCharSet);


  //****************
  // GetCharSet()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCharSet
        (// @parm [out, string, size_is(bufSize)] buffer into which CharSet is to be written
         aafCharacter *  pCharSet,

         // @parm [in] size of *pCharSet buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetCharSetBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCharSetBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

};

#endif // ! __ImplAAFAuxiliaryDescriptor_h__


