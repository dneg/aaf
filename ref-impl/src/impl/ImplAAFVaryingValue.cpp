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
 * prior written permission of Avid Technology, Inc.
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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFVaryingValue_h__
#include "ImplAAFVaryingValue.h"
#endif

#include "ImplAAFControlPoint.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFPluginManager.h"
#include "AAFPlugin.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFControlPoints;

ImplAAFVaryingValue::ImplAAFVaryingValue ()
: _controlPoints(         PID_VaryingValue_PointList,          L"PointList"),
  _interpolation(         PID_VaryingValue_Interpolation,      L"Interpolation", L"/Header/Dictionary/InterpolationDefinitions", PID_DefinitionObject_Identification)
{
	  aafReferenceType_t	ref = kAAFRefLimitMinimum;
	 aafInt32				zero = 0;
	 
	  _persistentProperties.put(_controlPoints.address());
	  _persistentProperties.put(_interpolation.address());
}


ImplAAFVaryingValue::~ImplAAFVaryingValue ()
{
	// Release all of the locator pointers.
	size_t count = _controlPoints.count();
	for (size_t i = 0; i < count; i++)
	{
		ImplAAFControlPoint *pControl = _controlPoints.clearValueAt(i);
		if (pControl)
		{
		  pControl->ReleaseReference();
		  pControl = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::Initialize
    (// @parm [in] // Parameter definition for this object (this determines the type of the constant value)
     ImplAAFParameterDef * pParameterDef,
     
     // @parm [in] Interpolation definition for this object
     ImplAAFInterpolationDef * pInterpolationDef)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (!pParameterDef || !pInterpolationDef)
    return (AAFRESULT_NULL_PARAM);

  result = SetParameterDefinition (pParameterDef);
  if (AAFRESULT_SUCCEEDED (result))
    result = SetInterpolationDefinition (pInterpolationDef);

  return result;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::AddControlPoint (
      ImplAAFControlPoint *pPoint)
{
  ImplAAFTypeDef *parameterType = NULL;
  ImplAAFTypeDef *controlPointsType = NULL;
  if(pPoint == NULL)
	return(AAFRESULT_NULL_PARAM);

	if (pPoint->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  XPROTECT()
	{
    // Validate the the control point has been initialized with the same
    // type as this varying value.
    CHECK (GetTypeDefinition (&parameterType));
    CHECK (pPoint->GetTypeDefinition (&controlPointsType));
    // We should be able to compare object pointers.
    if (parameterType != controlPointsType)
      RAISE (AAFRESULT_TYPE_SEMANTIC);
    controlPointsType->ReleaseReference();
    controlPointsType = NULL;
    parameterType->ReleaseReference();
    parameterType = NULL;

    _controlPoints.appendValue(pPoint);
    pPoint->AcquireReference();
	}
	XEXCEPT
	{
		if (controlPointsType)
		  controlPointsType->ReleaseReference();
		if (parameterType)
		  parameterType->ReleaseReference();
		return(XCODE());
	}
	XEND;

  return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetControlPoints (
      ImplEnumAAFControlPoints **ppEnum)
{
	ImplEnumAAFControlPoints		*theEnum = (ImplEnumAAFControlPoints *)CreateImpl (CLSID_EnumAAFControlPoints);
		
	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFControlPoint>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFControlPoint>(_controlPoints);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFControlPoints, this, iter));
		
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::CountControlPoints (
      aafUInt32 * pResult)
{
  if(! pResult) return(AAFRESULT_NULL_PARAM);

  *pResult = _controlPoints.count();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetControlPointAt (
      aafUInt32 index,
	  ImplAAFControlPoint ** ppControlPoint)
{
  if(! ppControlPoint) return(AAFRESULT_NULL_PARAM);

  aafUInt32 count;
  AAFRESULT ar;
  ar = CountControlPoints (& count);
  if (AAFRESULT_FAILED (ar)) return ar;
  if (index >= count)
	return AAFRESULT_BADINDEX;

  ImplAAFControlPoint *pPoint;
  _controlPoints.getValueAt(pPoint,index);

  assert(pPoint);
  pPoint->AcquireReference();
  (*ppControlPoint)=pPoint;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::RemoveControlPointAt (
      aafUInt32 index)
{
  aafUInt32 count;
  AAFRESULT hr;
  hr = CountControlPoints (& count);
  if (AAFRESULT_FAILED (hr)) return hr;
  if (index >= count)
	return AAFRESULT_BADINDEX;

	ImplAAFControlPoint *pPoint = NULL;
	pPoint = 	_controlPoints.removeAt(index);
	if (pPoint)
	{
		// We have removed an element from a "stong reference container" so we must
		// decrement the objects reference count. This will not delete the object
		// since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
		pPoint->ReleaseReference ();
	}
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::SetInterpolationDefinition (
      ImplAAFInterpolationDef *pDef)
{
	if(pDef == NULL)
		return AAFRESULT_NULL_PARAM;

	_interpolation = pDef;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetInterpolationDefinition (
      ImplAAFInterpolationDef **ppDef)
{
	if(ppDef == NULL)
		return AAFRESULT_NULL_PARAM;

	*ppDef = _interpolation;
	(*ppDef)->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetValueBufLen (
      aafInt32 *pLen)
{
	ImplEnumAAFControlPoints	*theEnum = NULL;
	ImplAAFControlPoint			*point = NULL;
	aafUInt32					oneSize, largest = 0;

	if(pLen == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		CHECK(GetControlPoints(&theEnum));
		while(theEnum->NextOne(&point) == AAFRESULT_SUCCESS)
		{
			CHECK(point->GetValueBufLen (&oneSize));
			if(oneSize > largest)
				largest = oneSize;
			point->ReleaseReference();
			point = NULL;
		}
		theEnum->ReleaseReference();
		theEnum = NULL;
		*pLen = largest;
	}
	XEXCEPT
	{
		if(point)
		  point->ReleaseReference();
		point = 0;
		if(theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFVaryingValue::GetInterpolatedValue (
      aafRational_t  inputValue,
      aafInt32  valueSize,
      aafDataBuffer_t  pValue,
      aafInt32*  bytesRead)
{
	ImplAAFInterpolationDef		*interpDef = NULL;
	aafUID_t					interpID;
	IAAFPlugin					*plugin = NULL;
	ImplAAFPluginManager		*mgr = NULL;
	IAAFInterpolator			*iInterp = NULL;
	IAAFParameter				*iParm = NULL;
	IUnknown					*iUnk = NULL;
	ImplAAFTypeDef				*intDef;
	IAAFTypeDef					*iTypeDef = NULL;
	XPROTECT()
	{
		CHECK(GetInterpolationDefinition(&interpDef));
		CHECK(interpDef->GetAUID (&interpID));
		mgr = ImplAAFPluginManager::GetPluginManager();
		CHECK(mgr->GetPluginInstance(interpID, &plugin));
		CHECK(plugin->QueryInterface(IID_IAAFInterpolator, (void **)&iInterp));
		/**/
		iUnk = static_cast<IUnknown *> (this->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFParameter, (void **)&iParm));
		CHECK(iInterp->SetParameter(iParm));
		CHECK(GetTypeDefinition(&intDef));

		iUnk = static_cast<IUnknown *> (intDef->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFTypeDef, (void **)&iTypeDef));
		CHECK(iInterp->SetTypeDefinition(iTypeDef));
		/**/
		CHECK(iInterp->InterpolateOne(&inputValue, valueSize, pValue, (aafUInt32 *)bytesRead));

		iTypeDef->Release();
		iTypeDef = NULL;

		intDef->ReleaseReference();
		intDef = NULL;
		iParm->Release();
		iParm = NULL;
		iInterp->Release();
		iInterp = NULL;
		plugin->Release();
		plugin = NULL;
		mgr->ReleaseReference();
		mgr = NULL;
		interpDef->ReleaseReference();
		interpDef = NULL;
	}
	XEXCEPT
	{
		if (iTypeDef)
			iTypeDef->Release();
		if (intDef)
			intDef->ReleaseReference();
		if(iParm)
			iParm->Release();
		if(iInterp)
			iInterp->Release();
		if(plugin)
			plugin->Release();
		if(mgr)
		  mgr->ReleaseReference();
		mgr = 0;
		if (interpDef)
			interpDef->ReleaseReference();
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}

