

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


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



#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"

#include "ImplAAFDictionary.h"
#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;
typedef ImplAAFSmartPointer<ImplAAFDataDef>    ImplAAFDataDefSP;
extern "C" const aafClassID_t CLSID_EnumAAFKLVData;

ImplAAFComponent::ImplAAFComponent ():
  _dataDef( PID_Component_DataDefinition,
            L"DataDefinition", 
            L"/Header/Dictionary/DataDefinitions",
            PID_DefinitionObject_Identification),
  _length( PID_Component_Length,
           L"Length"),
  _KLVData( PID_Component_KLVData,
            L"KLVData")
{
	_persistentProperties.put(   _dataDef.address());
	_persistentProperties.put(   _length.address());
	_persistentProperties.put(   _KLVData.address());
}


ImplAAFComponent::~ImplAAFComponent ()
{
	if(_KLVData.isPresent())
	{
		size_t size = _KLVData.count();
		for (size_t j = 0; j < size; j++)
		{
			ImplAAFKLVData* pKLVData = _KLVData.clearValueAt(j);
			if (pKLVData)
			  pKLVData->ReleaseReference();
			pKLVData = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::SetLength (const aafLength_t & length)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if ( length < 0 )
	  aafError = AAFRESULT_BAD_LENGTH;
	else
	  _length = length;
	return aafError;
}


/*************************************************************************
 * Function: GetLength()
 *
 * 		This function returns the length of a component.  
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetLength (aafLength_t * pLength)
{
  if (pLength == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (! _length.isPresent())
	{
	  return AAFRESULT_PROP_NOT_PRESENT;
	}
	
  *pLength = _length;
  return AAFRESULT_SUCCESS;
}

	
AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::SetDataDef (ImplAAFDataDef * pDataDef)
{
  if (! pDataDef)
    return AAFRESULT_NULL_PARAM;
  if (!pDataDef->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;

  _dataDef = pDataDef;

  return AAFRESULT_SUCCESS;
//	assert(_dataDef.isVoid());
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetDataDef (ImplAAFDataDef ** ppDataDef)
{
  if (! ppDataDef)
	return AAFRESULT_NULL_PARAM;

   if(_dataDef.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFDataDef *pDataDef = _dataDef;

  *ppDataDef = pDataDef;
  assert (*ppDataDef);
  (*ppDataDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::AppendKLVData (ImplAAFKLVData * pData)
{
	if (NULL == pData)
		return AAFRESULT_NULL_PARAM;
  if (pData->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_KLVData.appendValue(pData);
	pData->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// RemoveKLVData()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFComponent::RemoveKLVData
        (ImplAAFKLVData * pData)
{
	if (! pData)
		return AAFRESULT_NULL_PARAM;
  if (!pData->attached ()) // object could not possibly be in container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;
	if(!_KLVData.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
  size_t index;
  if (_KLVData.findIndex (pData, index))
  {
	  _KLVData.removeAt(index);
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pData->ReleaseReference ();
  }
  else
  {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::CountKLVData (aafUInt32*  pNumComments)
{
	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_KLVData.isPresent())
	{	// If the userComments property is not present then
		// number of user comments is zero!
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumComments = _KLVData.count();
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetKLVData (ImplEnumAAFKLVData** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFKLVData *theEnum = (ImplEnumAAFKLVData *)CreateImpl (CLSID_EnumAAFKLVData);
	
  XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFKLVData>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFKLVData>(_KLVData);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFKLVData, this, iter));
	  *ppEnum = theEnum;
	}
  XEXCEPT
	{
	  if (theEnum)
		{
		  theEnum->ReleaseReference();
		  theEnum = 0;
		}
	}
  XEND;
	
  return(AAFRESULT_SUCCESS);
}
/*************************************************************************
 * Private Function: SetNewProps()
 *
 *      This is an internal function that is called by functions that
 *      create new components.  It sets the length and data definition property
 *      values on the input component object.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT ImplAAFComponent::SetNewProps(
        aafLength_t length,			/* IN - Length property value */
        ImplAAFDataDef * pDataDef)			/* IN - DataDef property value */
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (! pDataDef)
	  return AAFRESULT_NULL_PARAM;

	if ( length < 0 )
	  aafError = AAFRESULT_BAD_LENGTH;
	else
	  {
		_length	= length;
		_dataDef = pDataDef;
	  }
	return aafError;
}

AAFRESULT ImplAAFComponent::AccumulateLength(aafLength_t *pLength)
{
	if (pLength == NULL)
	{
 		return AAFRESULT_NULL_PARAM;
	}

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFComponent::GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
											 ImplAAFMob */*mob*/, ImplAAFMobSlot */*track*/,
											 aafMediaCriteria_t */*mediaCrit*/,
											 aafPosition_t currentObjPos,
											 aafOperationChoice_t */*operationChoice*/,
											 ImplAAFComponent	*/*prevObject*/,
											 ImplAAFComponent */*nextObject*/,
											 ImplAAFScopeStack */*scopeStack*/,
											 aafPosition_t *diffPos, aafLength_t *minLength,
											 ImplAAFOperationGroup **/*groupObject*/, aafInt32	*/*nestDepth*/,
											 ImplAAFComponent **found, aafBool *foundTransition)
{
	aafLength_t	tmpMinLen;
	
	XPROTECT()
	{
		*foundTransition = kAAFFalse;
		*found = this;
    AcquireReference(); // We are returning a reference so bump the reference count!
		CHECK(GetLength(&tmpMinLen));
		if (Int64Less(tmpMinLen, rootLen))
		{
			*minLength = tmpMinLen;
			if(diffPos != NULL)
			{
				/* Figure out diffPos */
				*diffPos = rootPos;
				SubInt64fromInt64(currentObjPos, diffPos);
			}
		}
		else
		{
			*minLength = rootLen;
			if(diffPos != NULL)
				CvtInt32toInt64(0, diffPos);
		}
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFComponent::ChangeContainedReferences(aafMobID_constref /*from*/,
													  aafMobID_constref /*to*/)
{
	return AAFRESULT_SUCCESS;
}

