/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif




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
    ImplAAFFiller::Create (aafUID_t*	pDataDef,
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

extern "C" const aafClassID_t CLSID_AAFFiller;

OMDEFINE_STORABLE(ImplAAFFiller, CLSID_AAFFiller);

