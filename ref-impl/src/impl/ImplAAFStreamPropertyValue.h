//@doc
//@class    AAFStreamPropertyValue | Implementation class for AAFPropertyValue
#ifndef __ImplAAFStreamPropertyValue_h__
#define __ImplAAFStreamPropertyValue_h__


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


class ImplAAFRoot;
class ImplAAFTypeDefStream;
class OMDataStreamProperty;

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
  // All reads/and writes advance the current element 
  // 

  // Stream element count and positioning. 

  //****************
  // GetElementCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementCount
        (// @parm [out] count of elements in the specified stream property value
         aafInt64 *  pElementCount);

  //****************
  // SetElementCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetElementCount
        (// @parm [in] new count of elements in the specified stream property value
         aafInt64  newElementCount);

  //****************
  // GetElementIndex()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementIndex
        (// @parm [out] current index in the specified stream property value
         aafInt64 *  pIndex);

  //****************
  // SetElementIndex()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetElementIndex
        (// @parm [in] the current element index in the specified stream property value
         aafInt64  newElementIndex);


  // Sequential access in chunks of Elements 

  //****************
  // ReadElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadElements
        (// @parm [in] number of bytes to read (must be evenly divisible by the elemenet 
         // type length)
         aafUInt32  dataSize,

         // @parm [out, size_is(dataSize), length_is(*bytesRead)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData,

         // @parm [out,ref] number of bytes actually read (will be either dataSize or 0 if 
         // there is in error)
         aafUInt32 *  bytesRead);

  //****************
  // WriteElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteElements
        (// @parm [in] number of bytes to write (must be evenly divisible by the elemenet type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);


  // Sequential access one Element at a time. 

  //****************
  // ReadElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadElement
        (// @parm [in] number of bytes to read (must be equal to the elemenet type length)
         aafUInt32  dataSize,

         // @parm [out, size_is(dataSize), length_is(*bytesRead)] buffer into which one element from the stream should be written
         aafMemPtr_t  pData,

         // @parm [out,ref] number of bytes actually read (will be either dataSize or 0 if there 
         // is in error)
         aafUInt32 *  bytesRead);

  //****************
  // WriteElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteElement
        (// @parm [in] number of bytes to write (must be equal to the elemenet type length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);


  // Extend in chunks of Elements 

  //****************
  // AppendElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElements
        (// @parm [in] number of bytes to write (must be evenly divisible by the elemenet type 
         // length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which elements from the stream should be written
         aafMemPtr_t  pData);


  // Extend one Element at a time 

  //****************
  // AppendElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElement
        (// @parm [in] number of bytes to write (must be equal to the elemenet type length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);


  // 
  // Raw un-typed stream access 
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
        (// @parm [in] number of bytes to write (must be equal to the elemenet type length)
         aafUInt32  dataSize,

         // @parm [in, ref, size_is(dataSize)] buffer into which should contain one element to be written to the stream
         aafMemPtr_t  pData);




  // non-published method to initialize this object.
  // NOTE: The given property's type must be a stream type.
  AAFRESULT Initialize (const ImplAAFTypeDefStream *streamType,
                        OMProperty *property);

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);

protected:
  virtual ~ImplAAFStreamPropertyValue ();
  
  // special accessor
  aafUInt32 internalElementSize(void) const;
  
private:
	OMDataStreamProperty * _streamProperty;
	ImplAAFTypeDef * _streamElementType;
	ImplAAFRoot * _propertyContainer; // save reference to owning container.
	aafUInt32 _externalElementSize;
	aafUInt32 _internalElemeentSize;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFStreamPropertyValue> ImplAAFStreamPropertyValueSP;

#endif // ! __ImplAAFStreamPropertyValue_h__
