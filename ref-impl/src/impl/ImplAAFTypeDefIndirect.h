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



class ImplAAFPropertyValue;






#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


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
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in, ref] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in] the actual type of the data
         ImplAAFTypeDef * pActualType,

         // @parm [in, ref, string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);


  //****************
  // GetActualType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualType
        // @parm [out] the actual type definition
        (ImplAAFTypeDef ** ppActualType);

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
        (// @parm [in, size_is(initDataSize)] pointer to buffer containing data to use
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


};

#endif // ! __ImplAAFTypeDefIndirect_h__


