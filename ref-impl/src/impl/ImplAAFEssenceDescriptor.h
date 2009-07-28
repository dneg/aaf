//@doc
//@class    AAFEssenceDescriptor | Implementation class for AAFEssenceDescriptor
#ifndef __ImplAAFEssenceDescriptor_h__
#define __ImplAAFEssenceDescriptor_h__



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

class ImplAAFLocator;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFLocator> ImplEnumAAFLocators;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif
#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#include "OMStrongRefVectorProperty.h"


class ImplAAFEssenceDescriptor : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssenceDescriptor ();
  virtual ~ImplAAFEssenceDescriptor ();



  //****************
  // CountLocators()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountLocators
		// @parm [out] Returns the number of locators
        (aafUInt32 *  pCount);
  //@comm The number of locators may be zero if the essence is in the current file.

  //****************
  // AppendLocator()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendLocator
		// @parm [in] Locator to append
        (ImplAAFLocator * pLocator);
  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a new primary location).

  //****************
  // PrependLocator()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependLocator
		// @parm [in] Locator to append
        (ImplAAFLocator * pLocator);
  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a secondary location for the essence).


  //****************
  // InsertLocatorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertLocatorAt
		// @parm [in] place to insert locator
        (aafUInt32 index,
		// @parm [in] Locator to insert
		 ImplAAFLocator * pLocator);


  //****************
  // GetLocatorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLocatorAt
		// @parm [in] index of locator to get
        (aafUInt32 index,
		// @parm [in] returned locator
		 ImplAAFLocator ** ppLocator);


  //****************
  // GetLocatorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveLocatorAt
		// @parm [in] index of locator to remove
        (aafUInt32 index);


  //****************
  // RemoveLocator()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveLocator
		// @parm [in] Locator to remove
        (ImplAAFLocator * pLocator);

  //****************
  // GetLocators()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLocators
		// @parm [out] An enumerator to the locators on this essence descriptor
        (ImplEnumAAFLocators ** ppEnum);
  //@comm The number of locators may be zero if the essence is in the current file.


public:
	// Functions internal to the toolkit
	virtual AAFRESULT STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

	virtual AAFRESULT
		GetNthLocator (aafInt32 index, ImplAAFLocator **ppLocator);

private:
    OMStrongReferenceVectorProperty<ImplAAFLocator> _locators;
};

#endif // ! __ImplAAFEssenceDescriptor_h__

