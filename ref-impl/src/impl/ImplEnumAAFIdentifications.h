//@doc
//@class    EnumAAFIdentifications | Implementation class for EnumAAFIdentifications
#ifndef __ImplEnumAAFIdentifications_h__
#define __ImplEnumAAFIdentifications_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFIdentification;


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> identWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFIdentification> identStrongRefArrayProp_t;

class ImplEnumAAFIdentifications : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFIdentifications ();

protected:
  virtual ~ImplEnumAAFIdentifications ();

public:

  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out, retval] The Next Identification
        (ImplAAFIdentification ** ppIdentification);


  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of identifications requested
         aafInt32  count,

         // @parm [out, size_is(count), length_is(*pNumFetched)] array to receive identification objects
         ImplAAFIdentification ** ppIdentifications,

         // @parm [out] number of actual Identifications fetched into ppIdentifications array
         aafInt32 *  pNumFetched);


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
        // @parm [out, retval] new enumeration
        (ImplEnumAAFIdentifications ** ppEnum);

public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, identWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, identStrongRefArrayProp_t *pProp);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFIdentificationsTest.cpp.
  static AAFRESULT test();
private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	identWeakRefArrayProp_t			*_enumProp;
	identStrongRefArrayProp_t		*_enumStrongProp;
};

#endif // ! __ImplEnumAAFIdentifications_h__


