//@doc
//@class    AAFParameter | Implementation class for AAFParameter
#ifndef __ImplAAFParameter_h__
#define __ImplAAFParameter_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

class ImplAAFParameterDef;
class ImplAAFTypeDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFParameter : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFParameter ();

protected:
  virtual ~ImplAAFParameter ();

public:
  virtual AAFRESULT STDMETHODCALLTYPE
    SetParameterDefinition (
      ImplAAFParameterDef *pParmDef);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefinition (
      ImplAAFParameterDef **ppParmDef);	

  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef);

    virtual const OMUniqueObjectIdentification& identification(void) const;

private:

	OMFixedSizeProperty<aafUID_t>				_parmDef;
};

#endif // ! __ImplAAFParameter_h__




