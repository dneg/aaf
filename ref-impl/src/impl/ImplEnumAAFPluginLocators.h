//@doc
//@class    EnumAAFPluginLocators | Implementation class for EnumAAFPluginLocators
#ifndef __ImplEnumAAFPluginLocators_h__
#define __ImplEnumAAFPluginLocators_h__


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


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


class ImplAAFLocator;






#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "ImplAAFPluginDescriptor.h"

class ImplEnumAAFPluginLocators : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFPluginLocators ();

protected:
  virtual ~ImplEnumAAFPluginLocators ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next AAFLocator
        (ImplAAFLocator ** ppAAFLocator);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of AAFLocators requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive AAFLocators
         ImplAAFLocator ** ppAAFLocators,

         // @parm [out,ref] number of actual AAFLocators fetched into ppAAFLocators array
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
        (ImplEnumAAFPluginLocators ** ppEnum);



public:
// Internal to the toolkit
AAFRESULT
    SetPluginDescriptor(ImplAAFPluginDescriptor *pEDesc);

private:
	aafInt32					_current;
	ImplAAFPluginDescriptor	*_cPluginDesc;
};

#endif // ! __ImplEnumAAFPluginLocators_h__


