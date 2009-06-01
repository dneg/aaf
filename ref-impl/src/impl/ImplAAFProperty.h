//@doc
//@class    AAFProperty | Implementation class for AAFProperty
#ifndef __ImplAAFProperty_h__
#define __ImplAAFProperty_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFProperty> ImplAAFPropertySP;

#endif // ! __ImplAAFProperty_h__
