//@doc
//@class    AAFWeakRefSetValue | Implementation class for AAFWeakRefSetValue
#ifndef __ImplAAFWeakRefSetValue_h__
#define __ImplAAFWeakRefSetValue_h__


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



#ifndef __ImplAAFRefSetValue_h__
#include "ImplAAFRefSetValue.h"
#endif

class ImplAAFTypeDefSet;

class ImplAAFWeakRefSetValue : public ImplAAFRefSetValue
{
public:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFWeakRefSetValue ();
  
protected:
  virtual ~ImplAAFWeakRefSetValue ();

  
  //
  // ImplAAFRefContainer overrides
  //
  
public:
  // Return the type of object references in the container.
  virtual ImplAAFTypeDefObjectRef * GetElementType(void) const; // the result is NOT reference counted.
  
protected:
  // Perform specialized validation for an object before it is added
  // to a container.
  virtual AAFRESULT ValidateNewObject(ImplAAFStorable *pNewObject) const;
  
  // Perform any specialized cleanup of any object after it has been removed
  // from the container.
  virtual bool usesReferenceCounting(void) const;
  
public:
  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefSet *referenceType,
                        OMProperty *property);

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFWeakRefSetValue> ImplAAFWeakRefSetValueSP;

#endif // ! __ImplAAFWeakRefSetValue_h__
