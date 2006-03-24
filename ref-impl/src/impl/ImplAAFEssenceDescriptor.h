//@doc
//@class    AAFEssenceDescriptor | Implementation class for AAFEssenceDescriptor
#ifndef __ImplAAFEssenceDescriptor_h__
#define __ImplAAFEssenceDescriptor_h__



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

class ImplAAFLocator;
class ImplAAFSubDescriptor;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFLocator> ImplEnumAAFLocators;
typedef ImplAAFEnumerator<ImplAAFSubDescriptor> ImplEnumAAFSubDescriptors;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif
#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif
#ifndef __ImplAAFSubDescriptor_h__
#include "ImplAAFSubDescriptor.h"
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


//****************
  // CountSubDescriptors()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountSubDescriptors
		// @parm [out] Returns the number of subdescriptors
        (aafUInt32 *  pCount);
  //@comm The number of subdescriptors may be zero if the essence is in the current file.

  //****************
  // AppendSubDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendSubDescriptor
		// @parm [in] SubDescriptor to append
        (ImplAAFSubDescriptor * pSubDescriptor);
  //@comm    Use this function to add a subdescriptor to be scanned first when searching for
  // the essence (a new primary location).

  //****************
  // PrependSubDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependSubDescriptor
		// @parm [in] SubDescriptor to append
        (ImplAAFSubDescriptor * pSubDescriptor);
  //@comm    Use this function to add a subdescriptor to be scanned first when searching for
  // the essence (a secondary location for the essence).


  //****************
  // InsertSubDescriptorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertSubDescriptorAt
		// @parm [in] place to insert subdescriptor
        (aafUInt32 index,
		// @parm [in] SubDescriptor to insert
		 ImplAAFSubDescriptor * pSubDescriptor);


  //****************
  // GetSubDescriptorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSubDescriptorAt
		// @parm [in] index of subdescriptor to get
        (aafUInt32 index,
		// @parm [in] returned subdescriptor
		 ImplAAFSubDescriptor ** ppSubDescriptor);


  //****************
  // GetSubDescriptorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSubDescriptorAt
		// @parm [in] index of subdescriptor to remove
        (aafUInt32 index);


  //****************
  // RemoveSubDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSubDescriptor
		// @parm [in] SubDescriptor to remove
        (ImplAAFSubDescriptor * pSubDescriptor);

  //****************
  // GetSubDescriptors()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSubDescriptors
		// @parm [out] An enumerator to the subdescriptors on this essence descriptor
        (ImplEnumAAFSubDescriptors ** ppEnum);
  //@comm The number of subdescriptors may be zero if the essence is in the current file.

public:
	// Functions internal to the toolkit
	virtual AAFRESULT STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

	virtual AAFRESULT
		GetNthLocator (aafInt32 index, ImplAAFLocator **ppLocator);

	virtual AAFRESULT
		GetNthSubDescriptor (aafInt32 index, ImplAAFSubDescriptor **ppSubDescriptor);

private:
    OMStrongReferenceVectorProperty<ImplAAFLocator> _locators;
    OMStrongReferenceVectorProperty<ImplAAFSubDescriptor> _subdescriptors;
};

#endif // ! __ImplAAFEssenceDescriptor_h__

