//@doc
//@class    AAFVaryingValue | Implementation class for AAFVaryingValue
#ifndef __ImplAAFVaryingValue_h__
#define __ImplAAFVaryingValue_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

class ImplAAFDataDef;
class ImplAAFControlPoint;
class ImplEnumAAFControlPoints;





#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif


class ImplAAFVaryingValue : public ImplAAFParameter
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFVaryingValue ();

protected:
  virtual ~ImplAAFVaryingValue ();

public:


  //****************
  // AddPoint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddPoint
        // @parm [in] pointer to IAAFControlPoint object
        (ImplAAFControlPoint * pPoint);


  //****************
  // GetControlPoints()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetControlPoints
        // @parm [out,retval] Parameter definition enumeration
        (ImplEnumAAFControlPoints ** ppEnum);

  //****************
  // GetValueBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValueBufLen
        // @parm [out] Mob Name
        (aafInt32 *  pLen);

  //****************
  // GetInterpolatedValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInterpolatedValue
        (// @parm [in] Position along the effect
         aafRational_t  inputValue,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  pValue,

         // @parm [out] Number of actual bytes read
         aafInt32*  bytesRead);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFVaryingValue)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFVaryingValueTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFVaryingValue_h__



