//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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

#ifndef __ImplAAFDescriptiveFramework_h__
#include "ImplAAFDescriptiveFramework.h"
#endif

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDescriptiveMarker_h__
#include "ImplAAFDescriptiveMarker.h"
#endif

#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>

ImplAAFDescriptiveMarker::ImplAAFDescriptiveMarker ()
  : _descriptiveFramework( PID_DescriptiveMarker_DescriptiveFramework, L"DescriptiveFramework" )
{
  _persistentProperties.put( _descriptiveFramework.address() );
}


ImplAAFDescriptiveMarker::~ImplAAFDescriptiveMarker ()
{
  ImplAAFDescriptiveFramework* pFramework = _descriptiveFramework.clearValue();
  if ( pFramework ) {
    pFramework->ReleaseReference();
    pFramework = 0;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::Initialize ()
{
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::SetDescribedSlotIDs (
      aafUInt32  /*numberElements*/,
      aafUInt32*  /*pDescribedSlotIDs*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::GetDescribedSlotIDs (
      aafUInt32  /*numberElements*/,
      aafUInt32*  /*pDescribedSlotIDs*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::GetDescribedSlotIDsSize (
      aafUInt32* pNumberElements )
{
  if ( NULL == pNumberElements ) {
    return AAFRESULT_NULL_PARAM;
  }

  // FIXME - Set to zero pending implementation.
  *pNumberElements = 0;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::SetDescriptiveFramework (
      ImplAAFDescriptiveFramework * pDescriptiveFramework )
{
  if ( NULL == pDescriptiveFramework ) {
    return AAFRESULT_NULL_PARAM;
  }

  // Make sure another object does not already own pDescriptiveFramework.
  if ( pDescriptiveFramework->attached() ) {
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  }

  _descriptiveFramework = pDescriptiveFramework;
  _descriptiveFramework->AcquireReference();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::GetDescriptiveFramework (
      ImplAAFDescriptiveFramework ** ppDescriptiveFramework )
{
  if ( NULL == ppDescriptiveFramework ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !(_descriptiveFramework && _descriptiveFramework.isPresent()) ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  *ppDescriptiveFramework = _descriptiveFramework;
  _descriptiveFramework->AcquireReference();

  return AAFRESULT_SUCCESS;
}
