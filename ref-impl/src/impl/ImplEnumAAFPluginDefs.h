//@doc
//@class    EnumAAFPluginDescriptors | Implementation class for EnumAAFPluginDescriptors
#ifndef __ImplEnumAAFPluginDescriptors_h__
#define __ImplEnumAAFPluginDescriptors_h__


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


class ImplAAFPluginDescriptor;






#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplEnumAAFPluginDescriptors : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFPluginDescriptors ();

protected:
  virtual ~ImplEnumAAFPluginDescriptors ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next AAFPluginDescriptor
        (ImplAAFPluginDescriptor ** ppAAFPluginDescriptor);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of AAFPluginDescriptors requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive AAFPluginDescriptors
         ImplAAFPluginDescriptor ** ppAAFPluginDescriptors,

         // @parm [out,ref] number of actual AAFPluginDescriptors fetched into ppAAFPluginDescriptors array
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
        (ImplEnumAAFPluginDescriptors ** ppEnum);



public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFPluginDescriptorsTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplEnumAAFPluginDescriptors_h__


