//@doc
//@class    EnumAAFParameterDefs | Implementation class for EnumAAFParameterDefs
#ifndef __ImplEnumAAFParameterDefs_h__
#define __ImplEnumAAFParameterDefs_h__


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


class ImplAAFParameterDef;







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> parmDefWeakRefArrayProp_t;

class ImplEnumAAFParameterDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFParameterDefs ();

protected:
  virtual ~ImplEnumAAFParameterDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next ParameterDefinition
        (ImplAAFParameterDef ** ppParameterDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of control code definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive control code definitions
         ImplAAFParameterDef ** ppParameterDefs,

         // @parm [out,ref] number of actual ParameterDefs fetched into ppParameterDefs array
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
        (ImplEnumAAFParameterDefs ** ppEnum);



public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::SetEnumProperty( ImplAAFObject *pObj, parmDefWeakRefArrayProp_t *pProp);

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFParameterDefsTest.cpp.
  static AAFRESULT test();
private:
	aafUInt32					_current;
	ImplAAFObject				*_enumObj;
	parmDefWeakRefArrayProp_t	*_enumProp;
};

#endif // ! __ImplEnumAAFParameterDefs_h__


