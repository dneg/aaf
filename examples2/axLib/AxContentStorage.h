#ifndef __AxContentStorage_h__
#define __AxContentStorage_h__

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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

#include <iosfwd>

class AxContentStorage : public AxObject {

public:
	
	AxContentStorage( IAAFContentStorageSP spIaafContentStorage );
	virtual ~AxContentStorage();

	IAAFMobSP LookupMob( const aafMobID_t& );

	IAAFEssenceDataSP LookupEssenceData( const aafMobID_t& id );
	aafUInt32 CountEssenceData() const;
	aafBoolean_t IsEssenceDataPresent( const aafMobID_t& id ) const;

	IEnumAAFMobsSP GetMobs(  const aafSearchCrit_t* pCriteria = 0 ) const;

	IEnumAAFEssenceDataSP EnumEssenceData() const;

	std::wostream& dump( std::wostream& os ) const;

private:

	AxContentStorage();
	AxContentStorage( const AxContentStorage& );
	AxContentStorage& operator=( const AxContentStorage& );

	mutable IAAFContentStorageSP _spIaafContentStorage;
};

std::wostream& operator<<( std::wostream& os, const AxContentStorage& obj );

#endif
