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


/*************************************************************************
 * 
 * @module AAFDefObject | AAFDefObject is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFDefObject specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/




#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFDefObject::ImplAAFDefObject ()
: _ID(				PID_DefinitionObject_Identification,"Identification"),
  _name(			PID_DefinitionObject_Name,			"Name"),
  _description(		PID_DefinitionObject_Description,	"Description")
{	_persistentProperties.put(_ID.address());
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_description.address());
}


ImplAAFDefObject::~ImplAAFDefObject ()
{}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetAUID (
      aafUID_t *pAuid)
{
	if (pAuid == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pAuid = _ID;
	}

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::SetAUID (
      aafUID_t *pAuid)
{
	if (pAuid == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		_ID = *pAuid;
	}
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetName (aafWChar *pName,
	aafInt32 bufSize)
{
	bool stat;

	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	stat = _name.copyToBuffer(pName, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

//****************
// GetNameBufLen()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFDefObject::GetNameBufLen
        (aafInt32 *  pSize)  //@parm [in,out] Definition Name length
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pSize = _name.size();
	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::SetName (aafWChar *pName)
{	
	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	_name = pName;

	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetDescription (aafWChar *pName,
	aafInt32 bufSize)
{
	bool stat;

	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	stat = _description.copyToBuffer(pName, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

//****************
// GetNameBufLen()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFDefObject::GetDescriptionBufLen
        (aafInt32 *  pSize)  //@parm [in,out] Definition Name length
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pSize = _description.size();
	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::SetDescription (aafWChar *pName)
{	
	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	_description = pName;

	return(AAFRESULT_SUCCESS); 
}

OMDEFINE_STORABLE(ImplAAFDefObject, AUID_AAFDefObject);


