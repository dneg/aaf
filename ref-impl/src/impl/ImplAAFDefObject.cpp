//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
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

#include <assert.h>
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
		aafUID_t id = _identification;

#ifdef PATCH_DDEF_GETAUID
		const aafUID_t DDEF_Picture_v1 = { 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
		const aafUID_t DDEF_Sound_v1 = { 0x78E1EBE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
		const aafUID_t DDEF_Timecode_v1 = { 0x7F275E81, 0x77E5, 0x11D2, { 0x80, 0x7F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
		const aafUID_t DDEF_Picture_v11 =  { 0x01030202, 0x0100, 0x0000, { 0x6, 0xe, 0x2b, 0x34, 0x4, 0x1, 0x1, 0x1 } };
		const aafUID_t DDEF_Sound_v11 = { 0x01030202, 0x0200, 0x0000, { 0x6, 0xe, 0x2b, 0x34, 0x4, 0x1, 0x1, 0x1 } }; 
		const aafUID_t DDEF_Timecode_v11 = { 0x01030201, 0x0100, 0x0000, { 0x6, 0xe, 0x2b, 0x34, 0x4, 0x1, 0x1, 0x1 } };
#ifdef USE_V1_DDEFS
		// using v1.0 datadefs, map any v1.1 to v1.0
		if (memcmp(&id, &DDEF_Picture_v11, sizeof(id)) == 0)
			id = DDEF_Picture_v1;
		else if (memcmp(&id, &DDEF_Sound_v11, sizeof(id)) == 0)
			id = DDEF_Sound_v1;
		else if (memcmp(&id, &DDEF_Timecode_v11, sizeof(id)) == 0)
			id = DDEF_Timecode_v1;
#else
		// using v1.1 datadefs, map any v1.0 to v1.1
		if (memcmp(&id, &DDEF_Picture_v1, sizeof(id)) == 0)
			id = DDEF_Picture_v11;
		else if (memcmp(&id, &DDEF_Sound_v1, sizeof(id)) == 0)
			id = DDEF_Sound_v11;
		else if (memcmp(&id, &DDEF_Timecode_v1, sizeof(id)) == 0)
			id = DDEF_Timecode_v11;
#endif
#endif

	  *pAuid = id;
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
