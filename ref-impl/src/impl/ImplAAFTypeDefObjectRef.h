//@doc
//@class    AAFTypeDefObjectRef | Implementation class for AAFTypeDefObjectRef
#ifndef __ImplAAFTypeDefObjectRef_h__
#define __ImplAAFTypeDefObjectRef_h__


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

class ImplAAFClassDef;





#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefObjectRef : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefObjectRef ();

protected:
  virtual ~ImplAAFTypeDefObjectRef ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t *  pID,

         // @parm [in] class def of objects permitted to be referenced
         ImplAAFClassDef * pObjType,

         // @parm [in, string] friendly name of this type definition
         wchar_t *  pTypeName);


  //****************
  // CreateValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValue
        (// @parm [in] object with which to initialize this object reference
         ImplAAFObject * pObj,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetObjectType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObjectType
        // @parm [out] class def of objects permitted to be referenced
        (ImplAAFClassDef ** ppObjType);


  //****************
  // GetObject()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObject
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pPropVal,

         // @parm [out] pointer to object value
         ImplAAFObject ** ppObject);


  //****************
  // SetObject()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetObject
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pPropVal,

         // @parm [in] pointer to object value
         ImplAAFObject * ppObject);



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


  // Similar to Initialize(), but doesn't require a real referenced
  // object type; can merely pass along the ref'd object's AUID.  NOT
  // FOR CLIENT CONSUMPTION!  This is only here to break a bootstrap
  // dependency when instantiating the first class definition:
  //
  // ClassDef requires
  // ObjRefArray (array of strong object references), which requires
  // ObjectReference, which requires
  // ClassDef (for use as referenced type).
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t *  pID,

         // @parm [in] class def of objects permitted to be referenced
         const aafUID_t * pRefdObjID,

         // @parm [in, string] friendly name of this type definition
         wchar_t *  pTypeName);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefObjectRef> ImplAAFTypeDefObjectRefSP;

#endif // ! __ImplAAFTypeDefObjectRef_h__
