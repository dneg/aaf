

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"

ImplAAFComponent::ImplAAFComponent ():
	_dataDef(	PID_Component_DataDefinition,	"DataDefinition"),
	_length(	PID_Component_Length,	"Length")
{
	_persistentProperties.put(   _dataDef.address());
	_persistentProperties.put(   _length.address());
}


ImplAAFComponent::~ImplAAFComponent ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::SetLength (aafLength_t *  pLength)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pLength == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		if ( *pLength < 0 )
			aafError = AAFRESULT_BAD_LENGTH;
		else
			_length = *pLength;
	}
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
    ImplAAFComponent::SetDataDef (aafUID_t*  pDataDef)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pDataDef == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		_dataDef = *pDataDef;
	}
	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetDataDef (aafUID_t*  pDataDef)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pDataDef == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pDataDef = _dataDef;
	}

	return aafError;
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
        aafUID_t *pDataDef)			/* IN - DataDef property value */
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	
	if (pDataDef == NULL)
	{
		return aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		_dataDef = *pDataDef;
		if ( length < 0 )
			aafError = AAFRESULT_BAD_LENGTH;
		else
			_length	= length;
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
											 ImplAAFMob *mob, ImplAAFMobSlot *track,
											 aafMediaCriteria_t *mediaCrit,
											 aafPosition_t currentObjPos,
											 aafOperationChoice_t *operationChoice,
											 ImplAAFComponent	*prevObject,
											 ImplAAFComponent *nextObject,
											 ImplAAFScopeStack *scopeStack,
											 aafPosition_t *diffPos, aafLength_t *minLength,
											 ImplAAFOperationGroup **groupObject, aafInt32	*nestDepth,
											 ImplAAFComponent **found, aafBool *foundTransition)
{
	aafLength_t	tmpMinLen;
	
	XPROTECT()
	{
		*foundTransition = AAFFalse;
		*found = this;
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


AAFRESULT ImplAAFComponent::ChangeContainedReferences(aafUID_t *from, aafUID_t *to)
{
	return AAFRESULT_SUCCESS;
}

