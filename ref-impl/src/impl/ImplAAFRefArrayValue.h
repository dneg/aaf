//@doc
//@class    AAFRefArrayValue | Implementation class for AAFRefArrayValue
#ifndef __ImplAAFRefArrayValue_h__
#define __ImplAAFRefArrayValue_h__


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

class ImplAAFTypeDefArray;


class ImplAAFRefArrayValue : public ImplAAFRefContainerValue
{
protected:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefArrayValue ();
  
  virtual ~ImplAAFRefArrayValue ();
  
  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefArray *containerType,
                        OMProperty *property,
                        bool fixedSize);

public:

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);

private:
  bool _fixedSize; // true if the array property is fixed size
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefArrayValue> ImplAAFRefArrayValueSP;

#endif // ! __ImplAAFRefArrayValue_h__
