//@doc
//@class    AAFTypeDefOpaque | Implementation class for AAFTypeDefOpaque
#ifndef __ImplAAFTypeDefOpaque_h__
#define __ImplAAFTypeDefOpaque_h__


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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTypeDefIndirect_h__
#include "ImplAAFTypeDefIndirect.h"
#endif

#include "OMOpaqueType.h"


class ImplAAFPropertyValue;
class OMTypeVisitor;

class ImplAAFTypeDefOpaque : public ImplAAFTypeDefIndirect,
                             public OMOpaqueType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefOpaque ();

protected:
  virtual ~ImplAAFTypeDefOpaque ();

public:

  //****************
  // GetActualTypeID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualTypeID
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pOpaquePropertyValue,

         // @parm [out] the type id of the actual data
         aafUID_t *  pActualTypeID);

  virtual AAFRESULT STDMETHODCALLTYPE
   GetHandle (
    // value to get data from.
    ImplAAFPropertyValue * pPropVal,

    // Size of preallocated buffer
    aafUInt32  handleSize,

    // Preallocated buffer to hold handle
    aafDataBuffer_t  pHandle,

    // Number of actual bytes read
    aafUInt32*  bytesRead);

  virtual AAFRESULT STDMETHODCALLTYPE
   GetHandleBufLen (
    // value to set.
   ImplAAFPropertyValue * pPropVal,

    // Pointer to an variable used to return the length
   aafUInt32 *  pLen);

  virtual AAFRESULT STDMETHODCALLTYPE
	SetHandle (
    // value to write data to.
    ImplAAFPropertyValue * pPropVal,

    // Size of preallocated buffer
	aafUInt32  handleSize,

    // buffer containing handle
	aafDataBuffer_t  pHandle);

  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromHandle (
    // pointer to buffer containing handle to use
     aafMemPtr_t  pInitData,

    // size of handle in pInitData
    aafUInt32  initDataSize,

    // newly created property value
    ImplAAFPropertyValue ** ppOpaquePropertyValue);
  
  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);

public:

  //****************
  // Initialize() 
  //   Called when we initialize as one of the "builtin" types.
  //
  AAFRESULT
    Initialize
        (// @parm [in, ref] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in, ref, string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);


  //****************
  // pvtInitialize() 
  //   Called when we initialize as one of the "builtin" types.
  //
  AAFRESULT
    pvtInitialize
        (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName,
       
       // @parm [in] the type definition for kAAFTypeID_AUID.
       ImplAAFTypeDef *pTypeDefAUID,

       // @parm [in] the dictionary for this instance
       ImplAAFDictionary *pDictionary);


  // Methods to return OMType. Helps to maintain client code once
  // ImplAAFTypeDef inheritance from OMType is removed.
  virtual const OMType* type() const;
  virtual OMType*       type();


  //*************************************************************
  //
  // Overrides from OMDefinition
  //
  //*************************************************************

  virtual const OMUniqueObjectIdentification& identification(void) const;

  virtual const wchar_t* name(void) const;

  virtual bool hasDescription(void) const;

  virtual const wchar_t* description(void) const;

  virtual bool isPredefined(void) const;

  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************
  virtual bool isFixedSize(void) const;

  virtual void reorder(OMByte* externalBytes,
                       OMUInt32 externalBytesSize) const;

  virtual OMUInt32 externalSize(const OMByte* internalBytes,
                                OMUInt32 internalBytesSize) const;

  virtual OMUInt32 externalSize(void) const;

  virtual void externalize(const OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual OMUInt32 internalSize(const OMByte* externalBytes,
                                OMUInt32 externalSize) const;

  virtual OMUInt32 internalSize(void) const;

  virtual void internalize(const OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual void accept(OMTypeVisitor& visitor) const;

  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

protected:
  // Find the actual type definition from the dictionary.
	// This override of TypeDefIndirect will also look in the
	// dictionary for opaque types.
  virtual AAFRESULT LookupActualType (aafUID_constref typeID, 
                                      ImplAAFTypeDef ** ppActualType) const;


private:
  //
  // Utility to make sure that the given property value is for an opaque type.
  //
  AAFRESULT ValidateOpaquePropertyValue(ImplAAFPropertyValue * pOpaquePropertyValue);

  //
  // Utility to make sure that the given property value is for an opaque type.
  //
  AAFRESULT GetOpaqueHandleInfo(aafUInt32 handleSize, aafDataBuffer_t pHandle,
                                aafUInt32& opaqueDataSize, aafDataBuffer_t& opaqueDataBits);

};

#endif // ! __ImplAAFTypeDefOpaque_h__


