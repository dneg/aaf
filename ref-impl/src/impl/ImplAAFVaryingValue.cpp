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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFVaryingValue_h__
#include "ImplAAFVaryingValue.h"
#endif

#include "ImplAAFControlPoint.h"
#include "ImplAAFHeader.h"
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
: _controlPoints(         PID_VaryingValue_PointList,          "PointList"),
  _interpolation(         PID_VaryingValue_Interpolation,      "Interpolation")
{
	  _persistentProperties.put(_interpolation.address());
	  _persistentProperties.put(_controlPoints.address());
}


ImplAAFVaryingValue::~ImplAAFVaryingValue ()
{
	// Release all of the locator pointers.
	size_t size = _controlPoints.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFControlPoint *pControl = _controlPoints.setValueAt(0, i);
		if (pControl)
		{
		  pControl->ReleaseReference();
		  pControl = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::AppendPoint (
      ImplAAFControlPoint *pPoint)
{
	if(pPoint == NULL)
		return(AAFRESULT_NULL_PARAM);

	_controlPoints.appendValue(pPoint);
	pPoint->AcquireReference();

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetControlPoints (
      ImplEnumAAFControlPoints **ppEnum)
{
	ImplEnumAAFControlPoints		*theEnum = (ImplEnumAAFControlPoints *)CreateImpl (CLSID_EnumAAFControlPoints);
		
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_controlPoints));
		CHECK(theEnum->Reset());
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
    ImplAAFVaryingValue::SetInterpolationDefinition (
      ImplAAFInterpolationDef *pDef)
{
	aafUID_t			newUID;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(pDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pDef->GetAUID(&newUID));
		CHECK(pDef->MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
// This is a weak reference, not yet counted
//		if(dict->LookupParameterDef(&newUID, &def) == AAFRESULT_SUCCESS)
//			def->ReleaseReference();

		_interpolation = newUID;
//		pDef->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
		  head->ReleaseReference();
		head = 0;
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetInterpolationDefinition (
      ImplAAFInterpolationDef **ppDef)
{
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;
	aafUID_t			interpID;

	if(ppDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		interpID = _interpolation;
		CHECK(dict->LookupInterpolationDefinition(&interpID, ppDef));
//		(*ppDef)->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
		  head->ReleaseReference();
		head = 0;
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND;

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
		CHECK(GetTypeDef(&intDef));

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
		plugin->Release();
		plugin = NULL;
		iInterp->Release();
		iInterp = NULL;
		mgr->ReleaseReference();
		mgr = NULL;
		interpDef->ReleaseReference();
		interpDef = NULL;
	}
	XEXCEPT
	{
		if(plugin)
			plugin->Release();
		if(iUnk)
			iUnk->Release();
		if(iInterp)
			iInterp->Release();
		if(iParm)
			iParm->Release();
		if(mgr)
		  mgr->ReleaseReference();
		mgr = 0;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}


// SDK-private methods
AAFRESULT STDMETHODCALLTYPE
ImplAAFVaryingValue::GetTypeDef(ImplAAFTypeDef **ppTypeDef)
{
	if(ppTypeDef == NULL)
		return(AAFRESULT_NULL_PARAM);		// Assumes that all types are equal, can we assume this?

	ImplAAFControlPoint	*obj = NULL;
	_controlPoints.getValueAt(obj, 0);
	if (obj)
	{
		obj->GetTypeDefinition(ppTypeDef);
//		obj->AcquireReference();
	}
	else
		return AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???

	return AAFRESULT_SUCCESS;
}
