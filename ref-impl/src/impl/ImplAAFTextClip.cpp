/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/







#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTextClip_h__
#include "ImplAAFTextClip.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTextClip::ImplAAFTextClip ()
{}


ImplAAFTextClip::~ImplAAFTextClip ()
{}



OMDEFINE_STORABLE(ImplAAFTextClip, AUID_AAFTextClip);


