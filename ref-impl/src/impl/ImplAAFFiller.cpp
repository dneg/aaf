/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"


#ifndef __ImplAAFFiller_h__
#include "ImplAAFFiller.h"
#endif

#include <assert.h>
#include "AAFResult.h"


ImplAAFFiller::ImplAAFFiller ()
{}


ImplAAFFiller::~ImplAAFFiller ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFiller::Initialize (aafUID_t*	pDataDef,
                           aafLength_t		length)
{
	if (pDataDef == NULL)
		return AAFRESULT_NULL_PARAM;
	else
		return( SetNewProps( length, pDataDef ) );
}

AAFRESULT ImplAAFFiller::TraverseToClip(aafLength_t length,
										 ImplAAFSegment **sclp,
										 ImplAAFPulldown **pulldownObj,
										 aafInt32 *pulldownPhase,
										 aafLength_t *sclpLen,
										 aafBool *isMask)
{
	return ( AAFRESULT_FILL_FOUND );
}

OMDEFINE_STORABLE(ImplAAFFiller, AUID_AAFFiller);
