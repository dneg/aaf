//@doc
//@class    AAFTypeDefCharacter | Implementation class for AAFTypeDefCharacter
#ifndef __ImplAAFTypeDefCharacter_h__
#define __ImplAAFTypeDefCharacter_h__


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



class ImplAAFPropertyValue;






#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefCharacter : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefCharacter ();

protected:
  virtual ~ImplAAFTypeDefCharacter ();

public:


  //****************
  // Initialize()
  //
  AAFRESULT pvtInitialize
        (// @parm [in, ref] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in] friendly name of this type definition
         aafCharacter_constptr  pTypeName);

  //****************
  // CreateValueFromCharacter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromCharacter
        (// @parm [in] character data to write the the new value
         aafCharacter  character,

         // @parm [out] character property value with the given character
         ImplAAFPropertyValue ** ppCharacterValue);

  //****************
  // GetCharacter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCharacter
        (// @parm [in] character property value to read
         ImplAAFPropertyValue * pCharacterValue,

         // @parm [out] character that is read
         aafCharacter *  pCharacter);

  //****************
  // GetCharacter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCharacter
        (// @parm [in] character property value to modify
         ImplAAFPropertyValue * pCharacterValue,

         // @parm [in] character that is to be written
         aafCharacter character);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);

public:
  //
  // OMType methods
  //
  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(const OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(const OMByte* externalBytes,
                              size_t externalSize) const;

  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;

  
  
  //
  // ImplAAFTypeDef methods
  //
  virtual aafBool IsFixedSize (void) const;
  virtual size_t PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

};

#endif // ! __ImplAAFTypeDefCharacter_h__


