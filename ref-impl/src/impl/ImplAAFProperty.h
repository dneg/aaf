//@doc
//@class    AAFProperty | Implementation class for AAFProperty
#ifndef __ImplAAFProperty_h__
#define __ImplAAFProperty_h__


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

class ImplAAFPropertyDef;

class ImplAAFPropertyValue;

class OMProperty;





#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplAAFProperty : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFProperty ();

protected:
  virtual ~ImplAAFProperty ();

public:

  //****************
  // GetDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDefinition
        // @parm [out] This property's definition
        (ImplAAFPropertyDef ** ppPropDef);


  //****************
  // GetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValue
        // @parm [out] The Property Value object associated with this property.
        (ImplAAFPropertyValue ** ppValue);


public:
  //
  // non-published initialization method
  //
  AAFRESULT Initialize (ImplAAFPropertyDef * pPropDef,
						OMProperty * pOMProp);


private:
  ImplAAFPropertyDef *   _pPropDef;
  ImplAAFPropertyValue * _pPropVal;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFProperty> ImplAAFPropertySP;

#endif // ! __ImplAAFProperty_h__
