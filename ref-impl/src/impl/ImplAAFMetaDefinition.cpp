/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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
 * @module AAFMetaDefinition | AAFMetaDefinition is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFMetaDefinition specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/



#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif


#ifndef __ImplAAFDictionary_h_
#include "ImplAAFDictionary.h"
#endif

//#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFMetaDefinition::ImplAAFMetaDefinition ()
: _name           (PID_MetaDefinition_Name,           L"Name"),
  _description    (PID_MetaDefinition_Description,    L"Description"),
  _identification (PID_MetaDefinition_Identification, L"Identification")
{
  _persistentProperties.put(_name.address());
  _persistentProperties.put(_description.address());
  _persistentProperties.put(_identification.address());
}


ImplAAFMetaDefinition::~ImplAAFMetaDefinition ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::Initialize (
    aafUID_constref id,
	  aafCharacter_constptr pName,
    aafCharacter_constptr pDescription)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
	//validate pName
  if (pName == NULL)
  {
    return AAFRESULT_NULL_PARAM;
  }

  result = SetIdentification (id);
  if (AAFRESULT_SUCCEEDED(result))
  {
    result = SetName (pName);
    if (AAFRESULT_SUCCEEDED(result))
    {
      if (pDescription)
      {
        result = SetDescription (pDescription);
      }
    }
  }

  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetAUID (
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
    ImplAAFMetaDefinition::SetName (
      aafCharacter_constptr  pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _name = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetName (
      aafCharacter * pName,
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
    ImplAAFMetaDefinition::GetNameBufLen (
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
    ImplAAFMetaDefinition::SetDescription (
      aafCharacter_constptr pDescription)
{
  if (! pDescription)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _description = pDescription;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetDescription (
      aafCharacter * pDescription,
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
    ImplAAFMetaDefinition::GetDescriptionBufLen (
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





// Associate the existing OMProperties with corresponding property definitions from
// the given class definition. NOTE: This call is recursive, it calls itself again
// for the parent class of the given class until current class is a "root" class.
void ImplAAFMetaDefinition::InitOMProperties (ImplAAFClassDef * pClassDef)
{
  // Since ImplAAFMetaDefinition is still a subclass of ImplAAFObject we
  // should just delegate property initialization:
  ImplAAFObject::InitOMProperties(pClassDef);
}

const OMUniqueObjectIdentification&
  ImplAAFMetaDefinition::identification(void) const
{
  return *reinterpret_cast<const OMUniqueObjectIdentification*>(&_identification.reference());
}



// Private method to install the AAFObjectModel definition associated with
// this meta definition.
AAFRESULT ImplAAFMetaDefinition::SetIdentification(aafUID_constref identification)
{
  _identification = identification;

  return AAFRESULT_SUCCESS;
}



// override from OMStorable.
const OMClassId& ImplAAFMetaDefinition::classId(void) const
{
  // This method needs to be overridden for all subclasses.
  // All meta definitions class id's are known statically.
  // The corresponding class defintions cannot be extended
  // at runtime.
  static OMObjectIdentification null = {0};
  return null;
}

// Override callbacks from OMStorable
void ImplAAFMetaDefinition::onSave(void* clientContext) const
{
  // TEMPORARY: Parent class will not always be ImplAAFObject!
  ImplAAFObject::onSave(clientContext);
}

void ImplAAFMetaDefinition::onRestore(void* clientContext) const
{
  // TEMPORARY: Parent class will not always be ImplAAFObject!
  ImplAAFObject::onRestore(clientContext);
}
