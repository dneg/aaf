//@doc
//@class    EnumAAFEffectDefs | Implementation class for EnumAAFEffectDefs
#ifndef __ImplEnumAAFEffectDefs_h__
#define __ImplEnumAAFEffectDefs_h__


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


class ImplAAFEffectDef;







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> effectDefWeakRefArrayProp_t;


class ImplEnumAAFEffectDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFEffectDefs ();

protected:
  virtual ~ImplEnumAAFEffectDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next EffectDefinition
        (ImplAAFEffectDef ** ppEffectDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of effect definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive effect definitions
         ImplAAFEffectDef ** ppEffectDefs,

         // @parm [out,ref] number of actual EffectDefs fetched into ppEffectDefs array
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
        (ImplEnumAAFEffectDefs ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFEffectDefs::SetEnumProperty( ImplAAFObject *pObj, effectDefWeakRefArrayProp_t *pProp);

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFEffectDefsTest.cpp.
  static AAFRESULT test();
private:
	aafUInt32					_current;
	ImplAAFObject				*_enumObj;
	effectDefWeakRefArrayProp_t	*_enumProp;
};

#endif // ! __ImplEnumAAFEffectDefs_h__


