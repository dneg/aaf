//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

#ifndef __ImplEnumAAFPluginDefs_h__
#include "ImplEnumAAFPluginDefs.h"
#endif

#ifndef __ImplAAFDictionary_h_
#include "ImplAAFDictionary.h"
#endif

#include <string.h>
#include <wchar.h>
#include "ImplAAFObjectCreation.h"
#include "aafErr.h"
#include "ImplAAFPluginDef.h"
#include "AAFUtils.h"
#include "AAFDefUIDs.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginDescriptors;

ImplAAFDefObject::ImplAAFDefObject ()
: _name           (PID_DefinitionObject_Name,           L"Name"),
  _description    (PID_DefinitionObject_Description,    L"Description"),
  _identification (PID_DefinitionObject_Identification, L"Identification")
{
  _persistentProperties.put(_name.address());
  _persistentProperties.put(_description.address());
  _persistentProperties.put(_identification.address());
}


ImplAAFDefObject::~ImplAAFDefObject ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::pvtInitialize (
      const aafUID_t & id,
	  const aafWChar * pName,
	  const aafWChar * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		return AAFRESULT_BAD_SIZE;
	}
	if (wcslen(pDesc)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		return AAFRESULT_BAD_SIZE;
	}

	_identification = id;
	_name = pName;
	_description = pDesc;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::Initialize (
      const aafUID_t & id,
	  const aafCharacter * pName,
	  const aafCharacter * pDesc )
{
	//validate pName
	if (pName == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		return AAFRESULT_BAD_SIZE;
	}

        // Validate pDesc
	if (pDesc != NULL)
	{
		if (wcslen(pDesc)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		{
			return AAFRESULT_BAD_SIZE;
		}
	}

	_identification = id;
	_name = pName;
	
	if (pDesc != NULL)
	{
		_description = pDesc;
	}

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::SetName (
      const aafCharacter *  pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
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
      const aafCharacter * pDescription)
{
  if (! pDescription)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (wcslen(pDescription)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
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
	if (!_description.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
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
	if (!_description.isPresent())
		*pBufSize = 0;
	else
		*pBufSize = _description.size();
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetAUID (
      aafUID_t *pAuid) const
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
      const aafUID_t & id)
{
  _identification = id;

  return AAFRESULT_SUCCESS;
}



const OMUniqueObjectIdentification&
  ImplAAFDefObject::identification(void) const
{
  return *reinterpret_cast<const OMUniqueObjectIdentification*>(&_identification.reference());
}
