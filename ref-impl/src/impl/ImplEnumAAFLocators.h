//@doc
//@class    EnumAAFLocators | Implementation class for EnumAAFLocators
#ifndef __ImplEnumAAFLocators_h__
#define __ImplEnumAAFLocators_h__


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







#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "ImplAAFEssenceDescriptor.h"

typedef OMStrongReferenceVectorProperty<ImplAAFLocator> locatorStrongRefArrayProp_t;

class ImplEnumAAFLocators : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFLocators ();
  virtual ~ImplEnumAAFLocators ();



  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
		// @parm [out,retval] The Next Locator
        (ImplAAFLocator ** ppLocator);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of locators requested
		 aafUInt32  count,

		 // @parm [out, size_is(count), length_is(*pFetched)] array to receive locators
		 ImplAAFLocator ** ppLocators,

		 // @parm [out,ref] number of actual Locators fetched into ppLocators array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
		// @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
		// @parm [out,retval] new enumeration
        (ImplEnumAAFLocators ** ppEnum);

public:
// Internal to the toolkit
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, locatorStrongRefArrayProp_t *pProp);

private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	locatorStrongRefArrayProp_t		*_enumStrongProp;
};

#endif // ! __ImplEnumAAFLocators_h__

