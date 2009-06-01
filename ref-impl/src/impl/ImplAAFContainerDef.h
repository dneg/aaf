//@doc
//@class    AAFContainerDef | Implementation class for AAFContainerDef
#ifndef __ImplAAFContainerDef_h__
#define __ImplAAFContainerDef_h__


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


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

class ImplAAFContainerDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFContainerDef ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const aafCharacter *name,
		 const aafCharacter *description);

protected:
  virtual ~ImplAAFContainerDef ();

public:


  //****************
  // EssenceIsIdentified()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EssenceIsIdentified
        // @parm [out] The EssenceIsIdentified
        (aafBool *  pEssenceIsIdentified);


  //****************
  // SetEssenceIsIdentified()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEssenceIsIdentified
        // @parm [in] The EssenceIsIdentified flag
        (aafBool  EssenceIsIdentified);

private:
	OMFixedSizeProperty<aafBool>					_isIdentified;
};

#endif // ! __ImplAAFContainerDef_h__


