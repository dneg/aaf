//@doc
//@class    AAFEssenceDescriptor | Implementation class for AAFEssenceDescriptor
#ifndef __ImplAAFEssenceDescriptor_h__
#define __ImplAAFEssenceDescriptor_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

class ImplAAFLocator;

class ImplEnumAAFLocators;







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif
#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif



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
  // GetNumLocators()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumLocators
		// @parm [out] Returns the number of locators
        (aafInt32 *  pCount);
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
  // RemoveLocator()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveLocator
		// @parm [in] Locator to remove
        (ImplAAFLocator * pLocator);

  //****************
  // EnumAAFAllLocators()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFAllLocators
		// @parm [out] An enumerator to the locators on this essence descriptor
        (ImplEnumAAFLocators ** ppEnum);
  //@comm The number of locators may be zero if the essence is in the current file.




public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEssenceDescriptor)

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

