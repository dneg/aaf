//@doc
//@class    AAFRefArrayValue | Implementation class for AAFRefArrayValue
#ifndef __ImplAAFRefSetValue_h__
#define __ImplAAFRefSetValue_h__


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



#ifndef __ImplAAFRefContainerValue_h__
#include "ImplAAFRefContainerValue.h"
#endif

class ImplAAFTypeDefSet;
class ImplAAFTypeDefRecord;
class OMReferenceSetProperty;


class ImplAAFRefSetValue : public ImplAAFRefContainerValue
{
protected:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefSetValue ();
  
  virtual ~ImplAAFRefSetValue ();
  
  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefSet *containerType,
                        OMProperty *property);
  
  // Retrieve the property as an OMReferenceSetProperty.
  OMReferenceSetProperty * referenceSetProperty(void) const;

public:

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);


  // Remove the object identified by identification from the 
  // reference set.
  virtual AAFRESULT STDMETHODCALLTYPE 
    Remove(void* identification);

  // Does the reference set contain an object identified
  // by identification?
  virtual AAFRESULT STDMETHODCALLTYPE 
    Contains(void* identification,
             aafBoolean_t* pResult) const;

  // Find the object in the reference set property identified by 
  // identification.  If the object is found it is returned in object.
  // If the object is not found the result is false.
  virtual AAFRESULT STDMETHODCALLTYPE 
    FindObject(void* identification,
               ImplAAFStorable **pObject,
               aafBoolean_t* pResult) const;

  
  // Find the element associated with the given key.
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupElement(ImplAAFPropertyValue * pKey,
                  ImplAAFPropertyValue ** ppElementPropertyValue);

  // Test for containment by key.
  virtual AAFRESULT STDMETHODCALLTYPE
    ContainsKey(ImplAAFPropertyValue * pKey,
                aafBoolean_t*  pContainsKey);

private:  
  // Load the key (if valid) into the _keyBuffer
  void LoadKey(ImplAAFPropertyValue * pKey,
               AAFRESULT& result);

private:
  ImplAAFTypeDefRecord* _uidType; // cached type for the unique identifier property.
  aafUInt8* _keyBuffer;
  aafUInt32 _keyBufferSize;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefSetValue> ImplAAFRefSetValueSP;

#endif // ! __ImplAAFRefSetValue_h__
