//=---------------------------------------------------------------------=
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
//=---------------------------------------------------------------------=

#include "AxKLVData.h"

AxKLVData::AxKLVData( IAAFKLVDataSP spIaafKLVData )
: AxObject( AxQueryInterface<IAAFKLVData, IAAFObject>(spIaafKLVData) ),
	_spIaafKLVData( spIaafKLVData )
{}

AxKLVData::~AxKLVData()
{}

void AxKLVData::Initialize( aafUID_t  key,
							aafUInt32 length,
							const aafDataBuffer_t pValue )
{
	CHECK_HRESULT( _spIaafKLVData->Initialize( key, length,
						   const_cast<aafDataBuffer_t>(pValue) ) );
}

aafUID_t AxKLVData::GetKey()
{
    aafUID_t key;
    
    CHECK_HRESULT( _spIaafKLVData->GetKey( &key ) );
    
    return key;
}

void AxKLVData::GetValue( aafUInt32 valueSize, aafDataBuffer_t pValue, aafUInt32* bytesRead)
{
    CHECK_HRESULT( _spIaafKLVData->GetValue( valueSize, pValue, bytesRead ) );
}

void AxKLVData::SetValue( aafUInt32 valueSize, aafDataBuffer_t pValue )
{
    CHECK_HRESULT( _spIaafKLVData->SetValue( valueSize, pValue ) );
}


