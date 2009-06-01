//=---------------------------------------------------------------------=
//
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
: _describedSlotIDs( PID_DescriptiveClip_DescribedSlotIDs, L"DescribedSlotIDs" )
{
    _persistentProperties.put( _describedSlotIDs.address() );
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
    ImplAAFDescriptiveClip::CountDescribedSlotIDs (
      aafUInt32*  pCount)
{
    if (NULL == pCount) {
        return AAFRESULT_NULL_PARAM;
    }

    if (!_describedSlotIDs.isPresent()) {
        return AAFRESULT_PROP_NOT_PRESENT;
    }

    *pCount = _describedSlotIDs.count();

    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::GetDescribedSlotIDs (
      aafUInt32  maxDescribedSlotIDCount,
      aafUInt32 *  pDescribedSlotIDs)
{
    if (!pDescribedSlotIDs) {
        return AAFRESULT_NULL_PARAM;
    }

    if (_describedSlotIDs.count() > maxDescribedSlotIDCount) {
        return AAFRESULT_SMALLBUF;
    }

    if (!_describedSlotIDs.isPresent()) {
        return AAFRESULT_PROP_NOT_PRESENT;
    }

    aafUInt32* pNextDescribedSlotID = pDescribedSlotIDs;
    OMSetPropertyIterator<aafUInt32> iter( _describedSlotIDs, OMBefore );
    while (++iter) {
        *pNextDescribedSlotID = iter.value();
        pNextDescribedSlotID++;
    }

    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::IsDescribedSlotIDPresent (
      aafUInt32  describedSlotID,
      aafBoolean_t*  pIsPresent)
{
    if (NULL == pIsPresent) {
        return AAFRESULT_NULL_PARAM;
    }

    if (!_describedSlotIDs.isPresent()) {
        return AAFRESULT_PROP_NOT_PRESENT;
    }
    
    *pIsPresent = _describedSlotIDs.contains(describedSlotID) ? kAAFTrue :
                                                            kAAFFalse;

    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::AddDescribedSlotID (
      aafUInt32  describedSlotID)
{
    if (_describedSlotIDs.isPresent()) {
        if (_describedSlotIDs.contains(describedSlotID))
            return AAFRESULT_INVALID_PARAM;
    }
    
    _describedSlotIDs.insert(describedSlotID);
    
    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveClip::RemoveDescribedSlotID (
      aafUInt32  describedSlotID)
{
    if (!_describedSlotIDs.isPresent())
        return AAFRESULT_PROP_NOT_PRESENT;
    
    if (!_describedSlotIDs.contains(describedSlotID))
        return AAFRESULT_INVALID_PARAM;
    
    _describedSlotIDs.remove(describedSlotID);
    
    if (_describedSlotIDs.count() == 0) {
        _describedSlotIDs.removeProperty();
    }
    
    return AAFRESULT_SUCCESS;
}

