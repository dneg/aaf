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
		if(dict)
			dict->ReleaseReference();
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
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetValueBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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

OMDEFINE_STORABLE(ImplAAFVaryingValue, AUID_AAFVaryingValue);



