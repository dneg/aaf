//@doc
//@class    EnumAAFProperties | Implementation class for EnumAAFProperties
#ifndef __ImplEnumAAFProperties_h__
#define __ImplEnumAAFProperties_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFProperty;


template <typename T> class ImplAAFCollection;
template <typename T> class ImplAAFEnumerator;


#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplEnumAAFProperties : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFProperties ();

protected:
  virtual ~ImplEnumAAFProperties ();

public:

  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next Property
        (ImplAAFProperty ** ppProperty);


  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of properties requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pNumFetched)] array to receive properties
         ImplAAFProperty ** ppProperties,

         // @parm [out,ref] number of actual Properties fetched into ppProperties array
         aafUInt32 *  pNumFetched);


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
        (ImplEnumAAFProperties ** ppEnum);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFPropertiesTest.cpp.
  static AAFRESULT test();

  AAFRESULT Initialize (ImplAAFCollection<ImplAAFProperty*> * pProperties);

private:
  ImplAAFEnumerator<ImplAAFProperty*> * _rep;
};


#endif // ! __ImplEnumAAFProperties_h__


