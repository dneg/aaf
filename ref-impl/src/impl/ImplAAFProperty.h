//@doc
//@class    AAFProperty | Implementation class for AAFProperty
#ifndef __ImplAAFProperty_h__
#define __ImplAAFProperty_h__


//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFPropertyDef;

class ImplAAFPropertyValue;

class OMProperty;


#include "OMDataTypes.h"
#include "OMObject.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplAAFProperty : public ImplAAFRoot,
                        public OMObject
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


  AAFRESULT pvtSetValue (ImplAAFPropertyValue * pValue);
    
  // non-public unique identification
  const OMPropertyId& identification(void) const;


private:
  ImplAAFPropertyDef *   _pPropDef;
  ImplAAFPropertyValue * _pPropVal;
  OMPropertyId  _pid; // key returned by identification().
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
