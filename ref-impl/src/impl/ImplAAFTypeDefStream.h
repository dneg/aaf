//@doc
//@class    AAFTypeDefStream | Implementation class for AAFTypeDefStream
#ifndef __ImplAAFTypeDefStream_h__
#define __ImplAAFTypeDefStream_h__


/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
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





#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefStream : public ImplAAFTypeDef
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

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t *  pID,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         wchar_t *  pTypeName);


  //****************
  // GetType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef);


  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pPropVal,

         // @parm [out] count of elements in the specified stream property value
         aafInt64 *  pCount);


  //****************
  // AppendElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElements
        (// @parm [in] property value corresponding to stream to which elements are to
    // be appended
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in, size_is(numElements)] values to be appended to this stream
         ImplAAFPropertyValue ** ppMemberPropVals,

         // @parm [in] number of values to be appended
         aafUInt32  numElements);


  //****************
  // CreateEmpty()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateEmpty
        // @parm [out] newly-created property value
        (ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetElementValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementValues
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] zero-based index into elements in this array type
         aafInt64  startElement,

         // @parm [out, size_is(numElements)] array of values that are read
         ImplAAFPropertyValue ** pOutPropVals,

         // @parm [in] number of elements to get
         aafUInt32  numElements);


  //****************
  // GetElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElements
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropVal,

         // @parm [in] zero-based index into elements in this array type
         aafInt64  startElement,

         // @parm [out, size_is(numElements)] buffer into which C array data should be written
         aafMemPtr_t  pData,

         // @parm [in] number of elements to get
         aafUInt32  numElements);


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  /*
  virtual void reorder(OMByte* bytes,
                       size_t bytesSize) const;

  virtual size_t externalSize(void) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(void) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;
						   */

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefStream)
};

#endif // ! __ImplAAFTypeDefStream_h__


