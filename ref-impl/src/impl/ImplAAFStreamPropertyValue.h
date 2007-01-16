#ifndef __ImplAAFStreamPropertyValue_h__
#define __ImplAAFStreamPropertyValue_h__
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFRoot;
class ImplAAFTypeDefStream;
class OMDataStreamProperty;
class OMDataStreamPropertyFilter;

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif


class ImplAAFStreamPropertyValue : public ImplAAFPropertyValue
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFStreamPropertyValue ();



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
        (// @parm [out] count of bytes in the specified stream property value
         aafInt64 *  pSize);

  //****************
  // SetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSize
        (// @parm [in] new count of bytes in the specified stream property value
         aafInt64  newSize);

  //****************
  // GetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
        (// @parm [out] current byte position in the specified stream property value
         aafInt64 *  pPosition);

  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition
        (// @parm [in] the new position in the specified stream property value
         aafInt64  newPosition);


  // Sequential access 

  //****************
  // Read()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Read
        (// @parm [in] number of bytes to read
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
        (// @parm [in] number of bytes to write
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);


  // Extending the stream 

  //****************
  // Append()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Append
        (// @parm [in] number of bytes to write (must be equal to the element type length)
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
        (// @parm [out] kAAFTrue if this stream has a stored byte order
         aafBoolean_t *  pHasByteOrder);

  //****************
  // GetStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStoredByteOrder
        (// @parm [out] Pointer to variable where byte order is to be copied
         eAAFByteOrder_t *  pByteOrder);

  //****************
  // SetStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStoredByteOrder
        (// @parm [in] byte order is to be stored with the stream
         eAAFByteOrder_t  byteOrder);

  //****************
  // ClearStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ClearStoredByteOrder(void);



  // 
  // Access in typed chunks of Elements 
  // 

  //****************
  // ReadElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadElements
        (// @parm [in] the type definition of the elements to read
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
        (// @parm [in] the type definition of the elements to read
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
        (// @parm [in] the type definition of the elements to read
         ImplAAFTypeDef * pElementType,

         // @parm [in] number of bytes to write (must be evenly divisible by the element type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);


  // Filtered stream size and positioning 

  //****************
  // GetSizeFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSizeFiltered
        (// @parm [out] count of bytes in the specified stream property value
         aafInt64 *  pSize);

  //****************
  // SetSizeFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSizeFiltered
        (// @parm [in] new count of bytes in the specified stream property value
         aafInt64  newSize);

  //****************
  // GetPositionFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPositionFiltered
        (// @parm [out] current byte position in the specified stream property value
         aafInt64 *  pPosition);

  //****************
  // SetPositionFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPositionFiltered
        (// @parm [in] the new position in the specified stream property value
         aafInt64  newPosition);


  // Sequential access 

  //****************
  // ReadFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadFiltered
        (// @parm [in] number of bytes to read
         aafUInt32  dataSize,

         // @parm [out, size_is(dataSize), length_is(*bytesRead)] buffer into which one element from the stream should be written
         aafMemPtr_t  pData,

         // @parm [out,ref] number of bytes actually read (will be either dataSize or 0 if there 
         // is in error)
         aafUInt32 *  bytesRead);

  //****************
  // WriteFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteFiltered
        (// @parm [in] number of bytes to write
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);


  // Extending the filtered stream 

  //****************
  // AppendFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendFiltered
        (// @parm [in] number of bytes to write (must be equal to the element type length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);



  // 
  // Access in typed chunks of Elements in filtered stream
  // 

  //****************
  // ReadElementsFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadElementsFiltered
        (// @parm [in] the type definition of the elements to read
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
  // WriteElementsFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteElementsFiltered
        (// @parm [in] the type definition of the elements to read
         ImplAAFTypeDef * pElementType,

         // @parm [in] number of bytes to write (must be evenly divisible by the element type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);


  // Extend in chunks of typed Elements 

  //****************
  // AppendElementsFiltered()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElementsFiltered
        (// @parm [in] the type definition of the elements to read
         ImplAAFTypeDef * pElementType,

         // @parm [in] number of bytes to write (must be evenly divisible by the element type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);




  // non-published method to initialize this object.
  // NOTE: The given property's type must be a stream type.
  AAFRESULT Initialize (const ImplAAFTypeDefStream *streamType,
                        OMProperty *property);

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);

  void setStreamAccess(OMDataStreamAccess* access);

protected:
  virtual ~ImplAAFStreamPropertyValue ();
  
  // special accessor
  aafUInt32 internalElementSize(void) const;
  
private:
	OMDataStreamProperty * _streamProperty;
	OMDataStreamPropertyFilter * _streamPropertyFilter;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFStreamPropertyValue> ImplAAFStreamPropertyValueSP;

#endif // ! __ImplAAFStreamPropertyValue_h__
