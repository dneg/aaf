//@doc
//@class    AAFVaryingValue | Implementation class for AAFVaryingValue
#ifndef __ImplAAFVaryingValue_h__
#define __ImplAAFVaryingValue_h__


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
class ImplAAFInterpolationDef;




#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#include "ImplEnumAAFControlPoints.h"

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
  // SetInterpolationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetInterpolationDefinition
        (ImplAAFInterpolationDef * pPoint);


  //****************
  // GetInterpolationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInterpolationDefinition
        (ImplAAFInterpolationDef ** ppDef);

    //****************
  // AppendPoint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendPoint
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
	// SDK-private methods
	virtual AAFRESULT STDMETHODCALLTYPE
		GetTypeDef(ImplAAFTypeDef **ppTypeDef);

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFVaryingValue)
private:
	OMFixedSizeProperty<aafUID_t>						 _interpolation;
    OMStrongReferenceVectorProperty<ImplAAFControlPoint> _controlPoints;
};

#endif // ! __ImplAAFVaryingValue_h__



