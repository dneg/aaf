//@doc
//@class    EnumAAFInterpolationDefs | Implementation class for EnumAAFInterpolationDefs
#ifndef __ImplEnumAAFInterpolationDefs_h__
#define __ImplEnumAAFInterpolationDefs_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

class ImplAAFInterpolationDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> interpDefWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFInterpolationDef> interpDefStrongRefArrayProp_t;


class ImplEnumAAFInterpolationDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFInterpolationDefs ();

protected:
  virtual ~ImplEnumAAFInterpolationDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next PluggableDefinition
        (ImplAAFInterpolationDef ** ppPluggableDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of Pluggable definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive Pluggable definitions
         ImplAAFInterpolationDef ** ppPluggableDefs,

         // @parm [out,ref] number of actual PluggableDefs fetched into ppPluggableDefs array
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
        (ImplEnumAAFInterpolationDefs ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, interpDefWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, interpDefStrongRefArrayProp_t *pProp);

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFInterpolationDefsTest.cpp.
  static AAFRESULT test();
private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	interpDefWeakRefArrayProp_t		*_enumProp;
	interpDefStrongRefArrayProp_t		*_enumStrongProp;
};

#endif // ! __ImplEnumAAFInterpolationDefs_h__



