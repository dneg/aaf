//@doc
//@class    AAFVaryingValue | Implementation class for AAFVaryingValue
#ifndef __ImplAAFVaryingValue_h__
#define __ImplAAFVaryingValue_h__


/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] // Parameter definition for this object (this determines the type of the constant value)
         ImplAAFParameterDef * pParameterDef,
         
         // @parm [in] Interpolation definition for this object
         ImplAAFInterpolationDef * pInterpolationDef);



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
  // AddControlPoint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddControlPoint
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
  // CountControlPoints()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountControlPoints
        // @parm [out,retval] Parameter definition enumeration
        (aafUInt32 * pResult);


  //****************
  // GetControlPointAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetControlPointAt
        // @parm [in] index of control point to retrieve
        (aafUInt32 index,
		 // @parm [out,retval] retrieved control point
		 ImplAAFControlPoint ** ppControlPoint);


  //****************
  // RemoveControlPointAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveControlPointAt
        // @parm [in] index of control point to remove
        (aafUInt32 index);


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

private:
	OMFixedSizeProperty<aafUID_t>						 _interpolation;
  OMStrongReferenceVectorProperty<ImplAAFControlPoint> _controlPoints;

  ImplAAFTypeDef * _cachedTypeDef; // NOT REFERENCE COUNTED!
};	

#endif // ! __ImplAAFVaryingValue_h__



