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
		(void)AppendPluginDescriptor (NULL);		// !!! TEMP Until optional properties
}


ImplAAFDefObject::~ImplAAFDefObject ()
{
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

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDefObject::AppendPluginDescriptor (
      ImplAAFPluginDescriptor *pPluginDescriptor)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;

//!!!	if(pPluginDescriptor == NULL)
//		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		oldBufSize = _descriptors.size();
		newBufSize = oldBufSize + sizeof(aafUID_t);
		if(pPluginDescriptor == NULL)	//!!!
			newUID = NilMOBID;			//!!!
		else
		{
			CHECK(pPluginDescriptor->GetAUID(&newUID));
		}
		tmp = new aafUID_t[newBufSize];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldBufSize != 0)
		{
			_descriptors.copyToBuffer(tmp, oldBufSize);
			if(EqualAUID(tmp, &NilMOBID))		//!!! Handle non-optional props
			{									//!!!
				oldBufSize = 0;					//!!!
				newBufSize -= sizeof(aafUID_t);	//!!!
			}									//!!!
		}
		tmp[oldBufSize/sizeof(aafUID_t)] = newUID;
		_descriptors.setValue(tmp, newBufSize);
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
    ImplAAFDefObject::PrependPluginDescriptor (
      ImplAAFPluginDescriptor *pPluginDescriptor)
{
	aafUID_t	*tmp = NULL, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;
	aafInt32	n;

	if(pPluginDescriptor == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _descriptors.size();
		newBufSize = oldBufSize + sizeof(aafUID_t);
		CHECK(pPluginDescriptor->GetAUID(&newUID));
		tmp = new aafUID_t[newBufSize];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldBufSize != 0)
			_descriptors.copyToBuffer(tmp, oldBufSize);
		for(n = oldBufSize/sizeof(aafUID_t); n >= 0; n--)
		{
			tmp[n+1] = tmp[n];
		}
		tmp[0] = newUID;
		_descriptors.setValue(tmp, newBufSize);
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
    ImplAAFDefObject::EnumPluginDescriptors (
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


OMDEFINE_STORABLE(ImplAAFDefObject, AUID_AAFDefObject);
