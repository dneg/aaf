//@doc
//@class    AAFTypeDefIndirect | Implementation class for AAFTypeDefIndirect
#ifndef __ImplAAFTypeDefIndirect_h__
#define __ImplAAFTypeDefIndirect_h__


/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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



// Forward declarations:
class ImplAAFPropertyValue;




class ImplAAFTypeDefIndirect : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefIndirect ();

protected:
  virtual ~ImplAAFTypeDefIndirect ();

public:

  //****************
  // CreateValueFromActualValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromActualValue
        (// @parm [in] pointer to actual value
         ImplAAFPropertyValue * pActualValue,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);

  //****************
  // CreateValueFromActualData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromActualData
        (// @parm [in] pointer to actual type
         ImplAAFTypeDef * pActualType,

         // @parm [in, size_is(initDataSize)] pointer to buffer containing data to use
         aafMemPtr_t  pInitData,

         // @parm [in] size of data in pInitData
         aafUInt32  initDataSize,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetActualValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualValue
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] value that is read
         ImplAAFPropertyValue ** ppOutPropVal);

  //****************
  // GetActualSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualSize
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pPropVal,
         
         // @parm [out] actual data size
         aafUInt32 * dataSize);

  //****************
  // GetActualType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualType
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pPropVal,
         
         // @parm [out] actual data type
         ImplAAFTypeDef ** ppActualType);

  //****************
  // GetActualData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualData
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pPropVal,

         // @parm [out, size_is(dataSize)] buffer into which actual data should be written
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType (/*[out]*/ ImplAAFTypeDef ** ppRawTypeDef);

public:

  //
  // Methods used internally by the SDK.
  //

  //
  // OMType methods
  //
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
                              size_t externalSize) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;



  //****************
  // pvtInitialize() 
  //   Called when we initialize as one of the "builtin" types.
  //
  AAFRESULT
    pvtInitialize
        (// @parm [in, ref] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in, ref, string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);


  //
  // Utilities to extract information from an "indirect" property.
  // Hopefully this is temporary and this routine will be removed
  // when there is a specific "indirect" OM property.
  //
  static AAFRESULT GetActualPropertySize (
	  const OMProperty & indirectProperty,
	  aafUInt32 * pActualValueSize);

  static AAFRESULT GetActualPropertyValue (
	  const OMProperty & indirectProperty,
    aafDataBuffer_t value,
    aafUInt32 valueSize,
	  aafUInt32 * bytesRead);

  static AAFRESULT SetActualPropertyValue (
	  const OMProperty & indirectProperty,
    ImplAAFTypeDef *pActualType,
    aafDataBuffer_t value,
    aafUInt32 valueSize);

  static AAFRESULT GetActualPropertyType (
	  const OMProperty & property,
	  ImplAAFTypeDef ** pActualType); // REFERENCE COUNTED!

  static AAFRESULT GetActualPropertyTypeID (
	  const OMProperty & property,
	  aafUID_t * pTypeID);



  // Called internally by the dm because there is NO OM property to hide this
  // from the DM.
  aafUInt32 GetIndirectValueOverhead (void) const;

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

protected:
  // Utility (possibly temporary) that returns true if the given 
  // actual type can be used in an indirect type property.
  bool supportedActualType (ImplAAFTypeDef *pActualType, aafUInt32 level = 0);

  // Find the actual type definition from the dictionary.
  virtual AAFRESULT LookupActualType (aafUID_constref typeID, ImplAAFTypeDef ** ppActualType) const;

	// Utility to extract common information from the given indirect value.
  AAFRESULT GetIndirectValueInfo (
      ImplAAFPropertyValue * pIndirectValue,
			aafUInt32 & indirectValueSize,
			aafMemPtr_t & pIndirectValueDataBits,
      ImplAAFTypeDef ** ppActualType,
			aafUInt32 * actualValueSize = NULL,
      aafUID_t *actualTypeID = NULL);



protected:
  bool _initialized;
  ImplAAFDictionary *_dictionary;
  ImplAAFTypeDef *_typeDefAUID;
  aafUInt32 _internalAUIDSize;
  aafUInt32 _externalAUIDSize;
  aafUInt32 _internalIndirectSize;
  aafUInt32 _externalIndirectSize;
};

#endif // ! __ImplAAFTypeDefIndirect_h__


