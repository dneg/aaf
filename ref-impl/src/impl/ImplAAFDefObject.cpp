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
#include "ImplAAFObjectCreation.h"
#include "aafErr.h"
#include "ImplAAFPluginDef.h"
#include "aafUtils.h"
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
	else
	{
		_identification = id;
		_name = pName;
		_description = pDesc;
	}
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::Initialize (
      const aafUID_t & id,
	  const aafCharacter * pName )
{
	//validate pName
	if (pName == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}

	_identification = id;
	_name = pName;

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
