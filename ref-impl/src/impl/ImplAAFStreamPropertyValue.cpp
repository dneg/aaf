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
  _streamProperty(NULL),
  _streamElementType(NULL),
  _propertyContainer(NULL),
  _externalElementSize(0),
  _internalElemeentSize(0)
{}


ImplAAFStreamPropertyValue::~ImplAAFStreamPropertyValue ()
{
  if (_propertyContainer)
  {
    _propertyContainer->ReleaseReference();
    _propertyContainer = NULL;
  }
}



// special accessor
aafUInt32 ImplAAFStreamPropertyValue::internalElementSize(void) const
{
  if (0 == _internalElemeentSize)
  {
    // The stream element type was not registered when this object was initialized.
    // Update the internal size and preserve conceptual const-ness.
    const_cast<ImplAAFStreamPropertyValue *>(this)->_internalElemeentSize = _streamElementType->NativeSize();
  }
  
  return _internalElemeentSize;
}
 





AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetElementCount (
      aafInt64 *  pElementCount)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementCount)
    return AAFRESULT_NULL_PARAM;

  // Compute the number of externalized elements in the stream.
  *pElementCount = (aafInt64)(_streamProperty->size() /  _externalElementSize); 
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetElementCount (
      aafInt64  newElementCount)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newElementCount) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;

  // Compute the new size of the stream
  OMUInt64 newSize = (OMUInt64)newElementCount * _externalElementSize;
  
  // Set the new size of the data stream.
  _streamProperty->setSize(newSize); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetElementIndex (
      aafInt64 *  pIndex)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pIndex)
    return AAFRESULT_NULL_PARAM;
    
  OMUInt64 offset = _streamProperty->position();
  *pIndex = (aafInt64)(offset / _externalElementSize);
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetElementIndex (
      aafInt64  newElementIndex)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newElementIndex) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;

  // Compute the new position of the stream
  OMUInt64 newPosition = (OMUInt64)newElementIndex * _externalElementSize;
  
  // Set the new size of the data stream.
  _streamProperty->setPosition(newPosition); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}





 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::ReadElements (
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!_streamElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 elementSize = internalElementSize(); 
  if (0 == elementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / elementSize;
  if (dataSize != (elementCount * elementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Read the elements from the data stream.
  OMUInt32 elementsRead;
  _streamProperty->readTypedElements(_streamElementType,
                                     _externalElementSize,
                                     pData,
                                     elementCount,
                                     elementsRead);
  *bytesRead = elementsRead * elementCount;
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  assert(elementCount == elementsRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::WriteElements (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!_streamElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 elementSize = internalElementSize(); 
  if (0 == elementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / elementSize;
  if (dataSize != (elementCount * elementSize))
    return AAFRESULT_INVALID_PARAM;
  
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(_streamElementType,
                                     _externalElementSize,
                                     pData,
                                     elementCount,
                                     elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  assert(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::ReadElement (
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!_streamElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 elementSize = internalElementSize(); 
  if (0 == elementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize must be equal to the stream element size.
  if (dataSize !=  elementSize)
    return AAFRESULT_INVALID_PARAM;
  
  // Write the elements to the data stream.
  OMUInt32 elementsRead;
  _streamProperty->readTypedElements(_streamElementType,
                                     _externalElementSize,
                                     pData,
                                     1,
                                     elementsRead);
  *bytesRead = elementsRead * 1;
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  assert (1 == elementsRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::WriteElement (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!_streamElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 elementSize = internalElementSize(); 
  if (0 == elementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize must be equal to the stream element size.
  if (dataSize != elementSize)
    return AAFRESULT_INVALID_PARAM;
  
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(_streamElementType,
                                      _externalElementSize,
                                      pData,
                                      1,
                                      elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE;
    
  assert (1 == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::AppendElements (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!_streamElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 elementSize = internalElementSize(); 
  if (0 == elementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / elementSize;
  if (dataSize != (elementCount * elementSize))
    return AAFRESULT_INVALID_PARAM;
    
  // Set the position to the size of the stream.
  _streamProperty->setPosition(_streamProperty->size());
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(_streamElementType,
                                      _externalElementSize,
                                      pData,
                                      elementCount,
                                      elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  assert(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::AppendElement (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!_streamElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 elementSize = internalElementSize(); 
  if (0 == elementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize must be equal to the stream element size.
  if (dataSize != elementSize)
    return AAFRESULT_INVALID_PARAM;
      
  // Set the position to the size of the stream.
  _streamProperty->setPosition(_streamProperty->size());
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(_streamElementType,
                                      _externalElementSize,
                                      pData,
                                      1,
                                      elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE;
    
  assert (1 == elementsWritten); 
  return AAFRESULT_SUCCESS;
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
  
  // Set the new size of the data stream.
  _streamProperty->setSize(newSize); // What happens if this call fails?
    
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
//  ImplAAFTypeDefStream *streamType = const_cast<ImplAAFTypeDefStream *>
//                          (dynamic_cast<const ImplAAFTypeDefStream *>(type));
//  assert (streamType);
//  if (NULL == streamType)
//    return AAFRESULT_INVALID_PARAM;
    
  ImplAAFTypeDefSP streamElementType;
  result = streamType->GetElementType(&streamElementType);
  if (AAFRESULT_FAILED(result))
    return result;
    
  aafUInt32 externalElementSize = streamElementType->PropValSize();
  assert (0 != externalElementSize);
  if (0 == externalElementSize)
     return AAFRESULT_INVALID_PARAM;
  
  // The given property must be an OM stream property.  
  OMDataStreamProperty *streamProperty = dynamic_cast<OMDataStreamProperty *>(property);
  assert (streamProperty);
  if (NULL == streamProperty)
    return AAFRESULT_INVALID_PARAM;

  // Get the storable container for this property. Since this is a "direct 
  // access" interface we need to hold onto a reference so tha the container
  // is not deleted.
  ImplAAFRoot * propertyContainer = dynamic_cast<ImplAAFRoot *>
                                      (property->propertySet()->container());
  assert (propertyContainer);
  if (NULL == propertyContainer)
    return AAFRESULT_INVALID_PARAM;
  
  // Save our initialized member data.
  SetType(const_cast<ImplAAFTypeDefStream *>(streamType));
  _streamProperty = streamProperty;
  _streamElementType = streamElementType;
  _propertyContainer = propertyContainer;
  _propertyContainer->AcquireReference();
 
  _externalElementSize = externalElementSize;
  if (_streamElementType->IsRegistered())
    _internalElemeentSize = _streamElementType->NativeSize();
  
 
  // This instance is now fully initialized.
  setInitialized();
  
  return AAFRESULT_SUCCESS;
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
