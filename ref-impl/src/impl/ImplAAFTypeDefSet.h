//@doc
//@class    AAFTypeDefSet | Implementation class for AAFTypeDefSet
#ifndef __ImplAAFTypeDefSet_h__
#define __ImplAAFTypeDefSet_h__


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

#include "OMWeakRefProperty.h"

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
         aafUID_constref  id,

         // @parm [in] type of each element to be contained in this set
         ImplAAFTypeDef * pTypeDef,

         // @parm [in,string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);

  //****************
  // GetElementType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef);

  //****************
  // AddElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddElement
        (// @parm [in] property value corresponding to set to which element is added
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] value to be added to this set
         ImplAAFPropertyValue * pElementPropertyValue);

  //****************
  // RemoveElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveElement
        (// @parm [in] property value corresponding to set from which element is removed
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] value to be removed from this set
         ImplAAFPropertyValue * pElementPropertyValue);

  //****************
  // ContainsElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ContainsElement
        (// @parm [in] property value corresponding to set to which element is added
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] value whose presence is being tested in this set
         ImplAAFPropertyValue * pElementPropertyValue,

         // @parm [out] value to be added to this set
         aafBoolean_t*  pContainsElement);



  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [out] count of elements in the specified set property value
         aafUInt32 *  pCount);


  //****************
  // CreateKey()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateKey
        (// @parm [in,size_is(length)] Pointer to the key value bytes
         aafDataBuffer_t  pKeyPtr,

         // @parm [in] The size of the key in bytes
         aafUInt32  length,

         // @parm [out] An interface which may be passed to LookupElement() or ContainsKey()
         ImplAAFPropertyValue ** ppKey);


  //****************
  // LookupElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupElement
        (// @parm [in] property value of set
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] A key returned from CreateKey()
         ImplAAFPropertyValue * pKey,

         // @parm [out] The returned property value
         ImplAAFPropertyValue ** ppElementPropertyValue);

  //****************
  // ContainsKey()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ContainsKey
        (// @parm [in] property value of set
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] A key returned from CreateKey()
         ImplAAFPropertyValue * pKey,

         // @parm [out] Value returned is AAFTrue if an entry with the correct key is present
         aafBoolean_t*  pContainsKey);


  //****************
  // GetElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElements
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [out] enumerator across property values
         ImplEnumAAFPropertyValues ** ppEnum);


public:
  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this set
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;



  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

private:
  //
  // Persistent properties
  //
  OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;
};

#endif // ! __ImplAAFTypeDefSet_h__
