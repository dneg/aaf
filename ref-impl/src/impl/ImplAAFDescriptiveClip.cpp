//=---------------------------------------------------------------------=
//
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDescriptiveClip_h__
#include "ImplAAFDescriptiveClip.h"
#endif

#include "AAFPropertyIDs.h"

#include <string.h>


ImplAAFDescriptiveClip::ImplAAFDescriptiveClip ()
: _sourceTrackIDs( PID_DescriptiveClip_SourceTrackIDs, L"SourceTrackIDs" )
{
	_persistentProperties.put( _sourceTrackIDs.address() );
}


ImplAAFDescriptiveClip::~ImplAAFDescriptiveClip ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::Initialize (ImplAAFDataDef *    pDataDef,
								  const aafLength_t &    length,
								  const aafSourceRef_t & sourceRef)
{
    if( isInitialized() )
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }
    
    // Call parent class' Initialize.
    AAFRESULT  ar = ImplAAFSourceClip::Initialize(pDataDef, length, sourceRef);

    // Initialize this class required properties
    // and set the object initialized.
    if( ar == AAFRESULT_SUCCESS )
    {
        setInitialized();
    }
    
    return ar;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::SetSourceTrackIDs (
      aafUInt32  numberElements,
      aafUInt32*  pSourceTrackIDs)
{
  if (!pSourceTrackIDs) {
    return AAFRESULT_NULL_PARAM;
  }

  _sourceTrackIDs.clear();
  unsigned int i;
  for ( i = 0; i < numberElements; ++i ) {
    _sourceTrackIDs.insert( pSourceTrackIDs[i] );
  }

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::GetSourceTrackIDs (
      aafUInt32  numberElements,
      aafUInt32*  pSourceTrackIDs)
{
  if ( !pSourceTrackIDs ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !_sourceTrackIDs.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  if ( _sourceTrackIDs.count() > numberElements ) {
    return AAFRESULT_SMALLBUF;
  }
	
  aafUInt32* pNextSourceTrackID = pSourceTrackIDs;
  OMSetPropertyIterator<aafUInt32> iter( _sourceTrackIDs, OMBefore );
  while( ++iter ) {
    *pNextSourceTrackID = iter.value();
    pNextSourceTrackID++;
  }

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::GetSourceTrackIDsSize (
      aafUInt32 *  numberElements)
{
  if ( NULL == numberElements ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !_sourceTrackIDs.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  *numberElements = _sourceTrackIDs.count();

  return AAFRESULT_SUCCESS;
}




