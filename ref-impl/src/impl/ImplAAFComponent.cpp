

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#include <assert.h>
#include <AAFResult.h>


ImplAAFComponent::ImplAAFComponent ():
	_dataDef(	PID_IDENTIFICATION_DATADEF,	"dataDef"),
	_length(	PID_IDENTIFICATION_LENGTH,	"length")
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
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pLength == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pLength = _length;
	}
	return aafError;
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
 *      create new components.  It sets the length and datakind property
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
//	aafAssertValidFHdl(_file);
//	aafAssert((datakind != NULL), _file, OM_ERR_INVALID_DATAKIND);
//	assert(dataDef != NULL);
	
	if (pDataDef == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		_dataDef = *pDataDef;
		_length	= length;
	}
		
	return(AAFRESULT_SUCCESS);
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
											aafEffectChoice_t *effectChoice,
											ImplAAFComponent	*prevObject,
											ImplAAFComponent *nextObject,
#ifdef FULL_TOOLKIT
											AAFScopeStack *scopeStack,
#endif
											aafPosition_t *diffPos, aafLength_t *minLength,
											ImplAAFEffectDef **effeObject, aafInt32	*nestDepth,
											ImplAAFComponent **found, aafBool *foundTransition)
{
#ifdef FULL_TOOLKIT
  aafLength_t	tmpMinLen;
  
  XPROTECT(_file)
	{
	  *foundTransition = FALSE;
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
#endif
	return(AAFRESULT_SUCCESS);
}

extern "C" const aafClassID_t CLSID_AAFComponent;

OMDEFINE_STORABLE(ImplAAFComponent, CLSID_AAFComponent);

