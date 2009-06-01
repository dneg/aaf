#ifndef __ImplAAFTypeDefStream_h__
#define __ImplAAFTypeDefStream_h__
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=






#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMStreamType.h"


class ImplAAFPlainStreamData;
class ImplAAFPropertyValue;
class ImplAAFStreamPropertyValue;
struct IAAFIOCompletion;
struct IAAFStreamAccess;

class ImplAAFTypeDefStream : public ImplAAFTypeDef,
                             public OMStreamType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefStream ();

protected:
  virtual ~ImplAAFTypeDefStream ();

public:
  //  
  // All reads/and writes advance the current position 
  // 


  // 
  // Raw byte stream access 
  // 


  // Stream size and positioning 

  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [out] count of bytes in the specified stream property value
         aafInt64 *  pSize);

  //****************
  // SetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSize
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] new count of bytes in the specified stream property value
         aafInt64  newSize);

  //****************
  // GetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [out] current byte position in the specified stream property value
         aafInt64 *  pPosition);

  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] the new position in the specified stream property value
         aafInt64  newPosition);


  // Sequential access 

  //****************
  // Read()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Read
        (// @parm [in] stream property value to read
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] number of bytes to read
         aafUInt32  dataSize,

         // @parm [out, size_is(dataSize), length_is(*bytesRead)] buffer into which one element from the stream should be written
         aafMemPtr_t  pData,

         // @parm [out,ref] number of bytes actually read (will be either dataSize or 0 if there 
         // is in error)
         aafUInt32 *  bytesRead);

  //****************
  // Write()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Write
        (// @parm [in] stream property value to modify
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] number of bytes to write
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);


  // Extending the stream 

  //****************
  // Append()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Append
        (// @parm [in] stream property value to modify
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] number of bytes to write (must be equal to the element type length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);



  // 
  // Access byte order of the stream 
  // 


  //****************
  // HasStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    HasStoredByteOrder
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [out] kAAFTrue if this stream has a stored byte order
         aafBoolean_t *  pHasByteOrder);

  //****************
  // GetStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStoredByteOrder
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [out] Pointer to variable where byte order is to be copied
         eAAFByteOrder_t *  pByteOrder);

  //****************
  // SetStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStoredByteOrder
        (// @parm [in] stream property value
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] byte order is to be stored with the stream
         eAAFByteOrder_t  byteOrder);

  //****************
  // ClearStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ClearStoredByteOrder
        // @parm [in] stream property value
        (ImplAAFPropertyValue * pStreamPropertyValue);



  // 
  // Access in typed chunks of Elements 
  // 

  //****************
  // ReadElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadElements
        (// @parm [in] stream property value to read
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] the type definition of the elements to read
         ImplAAFTypeDef * pElementType,

         // @parm [in] number of bytes to read (must be evenly divisible by the element 
         // type length)
         aafUInt32  dataSize,

         // @parm [out, size_is(dataSize), length_is(*pBytesRead)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData,

         // @parm [out,ref] number of bytes actually read (will be either dataSize or 0 if 
         // there is in error)
         aafUInt32 *  pBytesRead);

  //****************
  // WriteElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteElements
        (// @parm [in] stream property value to modify
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] the type definition of the elements to read
         ImplAAFTypeDef * pElementType,

         // @parm [in] number of bytes to write (must be evenly divisible by the element type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);


  // Extend in chunks of typed Elements 

  //****************
  // AppendElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElements
        (// @parm [in] stream property value to modify
         ImplAAFPropertyValue * pStreamPropertyValue,

         // @parm [in] the type definition of the elements to read
         ImplAAFTypeDef * pElementType,

         // @parm [in] number of bytes to write (must be evenly divisible by the element type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);


  //****************
  // GetMXFEssenceStream()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPlainStreamData
        (// @parm [in] reserved (type of the stream data filter)
         aafUInt32 reserved,

         // @parm [in] file encoding-independent stream data
         ImplAAFPlainStreamData ** pPlainStreamData);


  //****************
  // GetTypeCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory
        // @parm [out] Returned type category
        (eAAFTypeCategory_t *  pTid);


  //****************
  // RawAccessType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        // @parm [out] the raw access type definition
        (ImplAAFTypeDef ** ppRawTypeDef);


protected:


  //****************
  // GetStreamPropertyValue()
  //
  // Validate/conversion utility.
  //
  AAFRESULT STDMETHODCALLTYPE
    GetStreamPropertyValue
        (// @parm [in] opaque property value
         ImplAAFPropertyValue * pPropertyValue,
         
         // @parm [out] stream property value
         ImplAAFStreamPropertyValue *& pStreamPropertyValue);
 
public:

  //
  // Methods used internally by the SDK.
  //

  //****************
  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;

  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


  virtual AAFRESULT STDMETHODCALLTYPE
	SetCallback
		(
		ImplAAFPropertyValue * pPropertyValue,
        IAAFStreamAccess*  pCallbackIF,
        aafMemPtr_t  pUserData);
public:

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

  //
  // ImplAAFTypeDef methods
  //
  virtual aafBool IsFixedSize (void) const;
  virtual OMUInt32 PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual OMUInt32 NativeSize (void) const;

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

private:
};

#endif // ! __ImplAAFTypeDefStream_h__
