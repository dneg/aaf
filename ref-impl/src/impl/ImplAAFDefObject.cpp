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

#ifndef __ImplEnumAAFPluginDescriptors_h__
#include "ImplEnumAAFPluginDescriptors.h"
#endif

#ifndef __ImplAAFDictionary_h_
#include "ImplAAFDictionary.h"
#endif

#include <assert.h>
#include <string.h>
#include "ImplAAFObjectCreation.h"
#include "aafErr.h"
#include "ImplAAFPluginDescriptor.h"
#include "aafUtils.h"
#include "AAFDefUIDs.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginDescriptors;

ImplAAFDefObject::ImplAAFDefObject ()
: _name           (PID_DefinitionObject_Name,           "Name"),
  _description    (PID_DefinitionObject_Description,    "Description"),
  _identification (PID_DefinitionObject_Identification, "Identification"),
  _descriptors(    PID_DefinitionObject_PluginDescriptors, "PluginDescriptors")
{
  _persistentProperties.put(_name.address());
  _persistentProperties.put(_description.address());
  _persistentProperties.put(_identification.address());
  _persistentProperties.put(_descriptors.address());
		(void)AppendPluginDef (NULL);		// !!! TEMP Until optional properties
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

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::AppendPluginDef (
      ImplAAFPluginDescriptor *pPluginDescriptor)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldCount;
	aafInt32	newCount;
	static const aafUID_t nullID = { 0 };


//!!!	if(pPluginDescriptor == NULL)
//		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		oldCount = _descriptors.count();
		if (oldCount == 1) {
			aafUID_t first;
			_descriptors.getValueAt(&first, 0);
		  	if(EqualAUID(&first, &nullID))	//!!! Handle non-optional props
		  	{									//!!!
				oldCount = 0;					//!!!
			}									//!!!
		}
		newCount = oldCount + 1;
		if(pPluginDescriptor == NULL)	//!!!
			newUID = nullID;			//!!!
		else
		{
			CHECK(pPluginDescriptor->GetAUID(&newUID));
		}
		tmp = new aafUID_t[newCount];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldCount != 0)
		{
			_descriptors.copyToBuffer(tmp, oldCount * sizeof(aafUID_t));
		}
		tmp[newCount - 1] = newUID;
		_descriptors.setValue(tmp, newCount * sizeof(aafUID_t));
		delete [] tmp;
	}
	XEXCEPT
	{
		if(tmp != NULL)
			delete [] tmp;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::PrependPluginDef (
      ImplAAFPluginDescriptor *pPluginDescriptor)
{
	aafUID_t	*tmp = NULL, newUID;
	aafInt32	oldCount;
	aafInt32	newCount;

	if(pPluginDescriptor == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldCount = _descriptors.count();
		newCount = oldCount + 1;
		CHECK(pPluginDescriptor->GetAUID(&newUID));
		tmp = new aafUID_t[newCount];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldCount != 0)
			_descriptors.copyToBuffer(&tmp[1], oldCount * sizeof(aafUID_t));
		tmp[0] = newUID;
		_descriptors.setValue(tmp, newCount * sizeof(aafUID_t));
		delete [] tmp;
	}
	XEXCEPT
	{
		if(tmp != NULL)
			delete [] tmp;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetPluginDefs (
      ImplEnumAAFPluginDescriptors **ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFPluginDescriptors *)CreateImpl (CLSID_EnumAAFPluginDescriptors);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumProperty(this, &_descriptors);

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::CountPluginDefs
        (aafUInt32 * pResult)
{
	if (! pResult)
		return AAFRESULT_NULL_PARAM;
	return AAFRESULT_NOT_IMPLEMENTED;
// !!Put this back when we have weak references
//	*pResult = _descriptors.count();
//	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::InsertPluginDefAt
        (aafUInt32 index,
		 ImplAAFPluginDescriptor * pPluginDescriptor)
{
	if (! pPluginDescriptor)
		return AAFRESULT_NULL_PARAM;
	
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountPluginDefs (&count);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	if (index > count)
		return AAFRESULT_BADINDEX;
	
	return AAFRESULT_NOT_IMPLEMENTED;
// !!Put this back when we have weak references
//	_descriptors.insertAt(pPluginDescriptor, index);
//	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::GetPluginDefAt
        (aafUInt32 index,
		 ImplAAFPluginDescriptor ** ppPluginDescriptor)
{
	if (! ppPluginDescriptor)
		return AAFRESULT_NULL_PARAM;
	
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountPluginDefs (&count);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	return AAFRESULT_NOT_IMPLEMENTED;
// !!Put this back when we have weak references
//	_descriptors.getValueAt(ppPluginDescriptor, index);
//	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::RemovePluginDefAt
        (aafUInt32 index)
{
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountPluginDefs (&count);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	return AAFRESULT_NOT_IMPLEMENTED;
// !!Put this back when we have weak references
//	_descriptors.removeAt(index);
//	return AAFRESULT_SUCCESS;
}

const OMUniqueObjectIdentification&
  ImplAAFDefObject::identification(void) const
{
  return *reinterpret_cast<const OMUniqueObjectIdentification*>(&_identification.reference());
}
