//@doc
//@class    EnumAAFLocators | Implementation class for EnumAAFLocators
#ifndef __ImplEnumAAFLocators_h__
#define __ImplEnumAAFLocators_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFLocator;







#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "ImplAAFEssenceDescriptor.h"

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
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFLocatorsTest.cpp.
  static AAFRESULT test();

public:
// Internal to the toolkit
AAFRESULT
    SetEssenceDesc(ImplAAFEssenceDescriptor *pEDesc);

private:
	aafInt32					_current;
	ImplAAFEssenceDescriptor	*_cEssenceDesc;

};

#endif // ! __ImplEnumAAFLocators_h__

