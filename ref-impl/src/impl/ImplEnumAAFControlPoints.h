//@doc
//@class    EnumAAFControlPoints | Implementation class for EnumAAFControlPoints
#ifndef __ImplEnumAAFControlPoints_h__
#define __ImplEnumAAFControlPoints_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/

class ImplAAFControlPoint;


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


typedef OMVariableSizeProperty<aafUID_t> ControlPointWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFControlPoint> ControlPointStrongRefArrayProp_t;

class ImplEnumAAFControlPoints : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFControlPoints ();

protected:
  virtual ~ImplEnumAAFControlPoints ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next container Definition
        (ImplAAFControlPoint ** ppPluggableDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of Pluggable definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive container definitions
         ImplAAFControlPoint ** ppControlPoints,

         // @parm [out,ref] number of actual container definitions fetched into ppControlPoints array
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
        (ImplEnumAAFControlPoints ** ppEnum);

public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, ControlPointStrongRefArrayProp_t *pProp);

private:
	aafUInt32							_current;
	ImplAAFObject						*_enumObj;
	ControlPointStrongRefArrayProp_t	*_enumStrongProp;
};

#endif // ! __ImplEnumAAFControlPoints_h__


