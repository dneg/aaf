//@doc
//@class    AAFTypeDefString | Implementation class for AAFTypeDefString
#ifndef __ImplAAFTypeDefString_h__
#define __ImplAAFTypeDefString_h__

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

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefVectorProperty.h"
#include "OMWeakRefProperty.h"

class ImplAAFTypeDefString : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefString ();

protected:
  virtual ~ImplAAFTypeDefString ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // GetType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef) const;


  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pPropVal,

         // @parm [out] count of elements in the specified string property value
         aafUInt32 *  pCount);


  //****************
  // CreateValueFromCString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromCString
        (// @parm [in] pointer to compile-time C string containing data to use
		 aafMemPtr_t pInitData,

		 // @parm [in] size of data in pInitData, in bytes
		 aafUInt32  initDataSize,

		 // @parm [out] newly created property value
		 ImplAAFPropertyValue ** ppPropVal);


  //****************
  // SetCString()
  //
  //****************
  // SetCArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCString
        (// @parm [in] property value to write
         ImplAAFPropertyValue * pPropVal,

         // @parm [in, size_is(dataSize)] buffer from which C string data should be read
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);


  //****************
  // AppendElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElements
        (// @parm [in] property value corresponding to string to which elements are to
    // be appended
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] Null-terminated array of elements to be appended
         aafMemPtr_t  pElements);


  //****************
  // GetElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElements
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] array of values that are read
         aafMemPtr_t  pBuffer,

         // @parm [in] size of pBuffer, in bytes
         aafUInt32  bufferSize);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(OMByte* internalBytes,
							  size_t internalBytesSize) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(OMByte* externalBytes,
							  size_t externalBytesSize) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;


  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this array
         const ImplAAFTypeDef *pType,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);


  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMPropertyMBS (OMPropertyId pid,
							const char * name) const;

  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        (ImplAAFTypeDef ** ppRawTypeDef);

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;

private:
  OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;

  ImplAAFTypeDefSP _cachedBaseType;

  ImplAAFTypeDefSP BaseType (void) const;
};

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefString> ImplAAFTypeDefStringSP;

#endif // ! __ImplAAFTypeDefString_h__
