/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

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

#ifndef __ImplAAFDictionary_h_
#include "ImplAAFDictionary.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFDefObject::ImplAAFDefObject ()
: _name           (PID_DefinitionObject_Name,           "Name"),
  _description    (PID_DefinitionObject_Description,    "Description"),
  _identification (PID_DefinitionObject_Identification, "Identification"),
  _pDict (0)
{
  _persistentProperties.put(_name.address());
  _persistentProperties.put(_description.address());
  _persistentProperties.put(_identification.address());
}


ImplAAFDefObject::~ImplAAFDefObject ()
{
  if (_pDict)
	{
	  // BobT hack!!!  See comment in SetDict() below...
	  // _pDict->ReleaseReference ();
	  _pDict = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::Init (
      aafUID_t *pAuid, aafWChar *pName, aafWChar *pDesc)
{
	if (pAuid == NULL || pName == NULL || pDesc == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		_identification = *pAuid;
		_name = pName;
		_description = pDesc;
	}
	return AAFRESULT_SUCCESS;
}
AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::SetName (
      wchar_t *  pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _name = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetName (
      wchar_t *  pName,
      aafUInt32  bufSize)
{
  bool stat;
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _name.copyToBuffer(pName, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetNameBufLen (
      aafUInt32 *  pBufSize)  //@parm [in,out] Definition Name length
{
  if (! pBufSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pBufSize = _name.size();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::SetDescription (
      wchar_t * pDescription)
{
  if (! pDescription)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _description = pDescription;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetDescription (
      wchar_t * pDescription,
      aafUInt32 bufSize)
{
  bool stat;
  if (! pDescription)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _description.copyToBuffer(pDescription, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetDescriptionBufLen (
      aafUInt32 * pBufSize)  //@parm [in,out] Definition Name length
{
  if (! pBufSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pBufSize = _description.size();
  return AAFRESULT_SUCCESS;
}


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
	  *pAuid = _identification;
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
	  _identification = *pAuid;
	}
  return AAFRESULT_SUCCESS;
}


void ImplAAFDefObject::SetDict (ImplAAFDictionary * pDict)
{
  _pDict = pDict;

  // BobT Hack!!!! dict hasn't been fully init'd yet when this is
  // called, so can't AcquireReference.  Must not ReleaseReference on
  // destruction...
  // _pDict->AcquireReference();
}


ImplAAFDictionary * ImplAAFDefObject::GetDict ()
{
  assert (_pDict);
  return _pDict;
}


OMDEFINE_STORABLE(ImplAAFDefObject, AUID_AAFDefObject);
