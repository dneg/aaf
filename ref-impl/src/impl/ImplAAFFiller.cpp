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


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"


#ifndef __ImplAAFFiller_h__
#include "ImplAAFFiller.h"
#endif

#include "AAFResult.h"


ImplAAFFiller::ImplAAFFiller ()
{}


ImplAAFFiller::~ImplAAFFiller ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFiller::Initialize (ImplAAFDataDef * pDataDef,
							   aafLength_t		length)
{
  if (! pDataDef)
	return AAFRESULT_NULL_PARAM;

  return( SetNewProps( length, pDataDef ) );
}

AAFRESULT ImplAAFFiller::TraverseToClip(aafLength_t /*length*/,
										 ImplAAFSegment ** /*sclp*/,
										 ImplAAFPulldown ** /*pulldownObj*/,
										 aafInt32 * /*pulldownPhase*/,
										 aafLength_t * /*sclpLen*/,
										 aafBool * /*isMask*/)
{
	return ( AAFRESULT_FILL_FOUND );
}

