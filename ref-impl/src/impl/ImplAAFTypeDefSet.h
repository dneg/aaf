//@doc
//@class    AAFTypeDefSet | Implementation class for AAFTypeDefSet
#ifndef __ImplAAFTypeDefSet_h__
#define __ImplAAFTypeDefSet_h__


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


class ImplAAFPropertyValue;

class ImplEnumAAFPropertyValues;





#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefSet : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefSet ();

protected:
  virtual ~ImplAAFTypeDefSet ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this set
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);


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

         // @parm [out] count of elements in the specified set property value
         aafUInt32 *  pCount);


  //****************
  // AddElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddElement
        (// @parm [in] property value corresponding to set to which element is added
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] value to be added to this set
         ImplAAFPropertyValue * pMemberPropVal);


  //****************
  // CreateValueFromValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromValues
        (// @parm [in, size_is(numElements)] array of property values for elements of set value which
    // is to be created.
         ImplAAFPropertyValue ** pElementValues,

         // @parm [in] size of pElementValues array.
         aafUInt32  numElements,

         // @parm [out] newly-created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // CreateValueFromCArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromCArray
        (// @parm [in, size_is(initDataSize)] pointer to compile-time C array containing data to use
         aafMemPtr_t  pInitData,

         // @parm [in] size of data in pInitData, in bytes
         aafUInt32  initDataSize,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElements
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] enumerator across property values
         ImplEnumAAFPropertyValues ** ppEnum);



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

  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void) const;
  virtual size_t PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual size_t NativeSize (void) const;

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;
};

#endif // ! __ImplAAFTypeDefSet_h__
