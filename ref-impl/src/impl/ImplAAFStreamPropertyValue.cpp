/***********************************************************************
 *
 *              Copyright (c) 2000 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include "ImplAAFStreamPropertyValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#include "OMProperty.h"
#include "OMPropertyDefinition.h"
#include "OMDataStreamProperty.h"

#include <assert.h>
#include <string.h>


ImplAAFStreamPropertyValue::ImplAAFStreamPropertyValue () :
  _streamProperty(NULL)
{}


ImplAAFStreamPropertyValue::~ImplAAFStreamPropertyValue ()
{
}
 









AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetSize (
      aafInt64 *  pSize)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;    
  
  *pSize = (aafInt64)(_streamProperty->size());
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetSize (
      aafInt64  newSize)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newSize) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;
  
  // *** Structured Storage PATCH! *** transdel:2000-JUN-20
  // Save the old position so that we can detect whether
  // or not the stream is being truncated.
  OMUInt64 position = _streamProperty->position();


  // Set the new size of the data stream.
  _streamProperty->setSize((OMUInt64)newSize); // What happens if this call fails?


  // *** Structured Storage PATCH! *** transdel:2000-JUN-20
  // If the file is truncated then force the position
  // to be the same as the new stream size (eof).
  // Without this PATCH Structrured Storage may leave
  // "stale bytes" in the stream after the next write
  // operation.
  if (position > (OMUInt64)newSize)
    _streamProperty->setPosition((OMUInt64)newSize); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetPosition (
      aafInt64 *  pPosition)
{
 if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pPosition)
    return AAFRESULT_NULL_PARAM;
    
  *pPosition = (aafInt64)_streamProperty->position();
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetPosition (
      aafInt64  newPosition)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newPosition) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;
  
  // Set the new size of the data stream.
  _streamProperty->setPosition((OMUInt64)newPosition); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}





 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::Read (
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
    
  // Read the bytes from the data stream.
  _streamProperty->read(pData, dataSize, *bytesRead);
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  assert(dataSize == *bytesRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::Write (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;
    
  // Write the bytes from the data stream.
  OMUInt32 bytesWritten = 0;
  if (0 < dataSize)
  {
    _streamProperty->write(pData, dataSize, bytesWritten);
  }
  
  if (0 < dataSize && 0 == bytesWritten)
    return AAFRESULT_END_OF_DATA;
  
  assert(dataSize == bytesWritten);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::Append (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;
      
  // Set the position to the size of the stream.
  _streamProperty->setPosition(_streamProperty->size());
       
  // Write the bytes from the data stream.
  OMUInt32 bytesWritten = 0;
  if (0 < dataSize)
  {
    _streamProperty->write(pData, dataSize, bytesWritten);
  }
  
  if (0 < dataSize && 0 == bytesWritten)
    return AAFRESULT_END_OF_DATA;
  
  assert(dataSize == bytesWritten);  
  return AAFRESULT_SUCCESS;
}






AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::HasStoredByteOrder (
      aafBoolean_t *  pHasByteOrder)
{
  if (NULL == pHasByteOrder)
    return AAFRESULT_NULL_PARAM;

  if (_streamProperty->hasByteOrder())
    *pHasByteOrder = kAAFTrue;
  else
    *pHasByteOrder = kAAFFalse;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetStoredByteOrder (
      eAAFByteOrder_t *  pByteOrder)
{
  if (NULL == pByteOrder)
    return AAFRESULT_NULL_PARAM;

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;

  OMByteOrder byteOrder = _streamProperty->byteOrder();
  if (byteOrder == littleEndian)
    *pByteOrder = kAAFByteOrderLittle;
  else
    *pByteOrder = kAAFByteOrderBig;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetStoredByteOrder (
      eAAFByteOrder_t  byteOrder)
{
  // Cannot set the byte order if there is an existing byte order.
  if (_streamProperty->hasByteOrder())
    return AAFRESULT_INVALID_BYTEORDER;
  
  // Cannot set the byte order if the stream is non-empty.
  if (0 < _streamProperty->size())
    return AAFRESULT_INVALID_BYTEORDER;
  
  if (byteOrder == kAAFByteOrderLittle)
    _streamProperty->setByteOrder(littleEndian);
  else
    _streamProperty->setByteOrder(bigEndian);

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::ClearStoredByteOrder (void)
{
  // Cannot clear the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;

  _streamProperty->clearByteOrder();
  return AAFRESULT_SUCCESS;
}



 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::ReadElements (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;    

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize();
  assert (0 < internalElementSize);
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Get the external size of an element from the type.  
  OMUInt32 externalElementSize = pElementType->PropValSize();
  assert (0 < externalElementSize);
  if (0 == externalElementSize)
     return AAFRESULT_INVALID_PARAM; 
  
  // Read the elements from the data stream.
  OMUInt32 elementsRead;
  _streamProperty->readTypedElements(pElementType,
                                     externalElementSize,
                                     pData,
                                     elementCount,
                                     elementsRead);
  *bytesRead = elementsRead * internalElementSize;
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  assert(elementCount == elementsRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::WriteElements (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData)
    return AAFRESULT_NULL_PARAM;    

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize(); 
  assert (0 < internalElementSize);
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Get the external size of an element from the type.  
  OMUInt32 externalElementSize = pElementType->PropValSize();
  assert (0 < externalElementSize);
  if (0 == externalElementSize)
     return AAFRESULT_INVALID_PARAM; 
  
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(pElementType,
                                     externalElementSize,
                                     pData,
                                     elementCount,
                                     elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  assert(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::AppendElements (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize(); 
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Get the external size of an element from the type.  
  OMUInt32 externalElementSize = pElementType->PropValSize();
  assert (0 < externalElementSize);
  if (0 == externalElementSize)
     return AAFRESULT_INVALID_PARAM; 
  
  // Set the position to the size of the stream.
  _streamProperty->setPosition(_streamProperty->size());
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(pElementType,
                                      externalElementSize,
                                      pData,
                                      elementCount,
                                      elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  assert(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





AAFRESULT ImplAAFStreamPropertyValue::Initialize (
  const ImplAAFTypeDefStream *streamType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  assert (property);
  if (NULL == streamType || NULL == property)
    return AAFRESULT_NULL_PARAM;

  // Get the type definition. This must be a stream type.
  assert (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  assert (type);
  
  // The given property must be an OM stream property.  
  OMDataStreamProperty *streamProperty = dynamic_cast<OMDataStreamProperty *>(property);
  assert (streamProperty);
  if (NULL == streamProperty)
    return AAFRESULT_INVALID_PARAM;

  result = ImplAAFPropertyValue::Initialize(streamType, streamProperty);
  if (AAFRESULT_SUCCEEDED(result))
  {
    _streamProperty = streamProperty;
   
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFStreamPropertyValue::WriteTo(
  OMProperty* pOmProp)
{
  // Make sure that the given property is the same one that was used to 
  // initialize this property value. NOTE: Copying stream data to a 
  // different OMProperty should be handled through another interface.
  if (pOmProp != _streamProperty)
    return AAFRESULT_INVALID_PARAM;
    
  return AAFRESULT_SUCCESS;
}
