//@doc
//@class    AAFEssenceDescriptor | Implementation class for AAFEssenceDescriptor
#ifndef __ImplAAFEssenceDescriptor_h__
#define __ImplAAFEssenceDescriptor_h__



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

