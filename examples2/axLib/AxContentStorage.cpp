//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
//=---------------------------------------------------------------------=

#include "AxContentStorage.h"

#include "AxEx.h"

#include <ostream>

AxContentStorage::AxContentStorage( IAAFContentStorageSP spIaafContentStorage )
:	AxObject( AxQueryInterface<IAAFContentStorage,IAAFObject>(spIaafContentStorage) ),
	_spIaafContentStorage( spIaafContentStorage )
{}

AxContentStorage::~AxContentStorage()
{}

IAAFMobSP AxContentStorage::LookupMob( const aafMobID_t& id )
{
	IAAFMobSP spIaafMob;

	CHECK_HRESULT( _spIaafContentStorage->LookupMob( id, &spIaafMob ) );

	return spIaafMob;
}

IEnumAAFMobsSP AxContentStorage::GetMobs(  const aafSearchCrit_t* pCriteria ) const
{
    IEnumAAFMobsSP spIEnumAAFMobs;
	HRESULT hr;

	hr = _spIaafContentStorage->GetMobs( const_cast<aafSearchCrit_t*>(pCriteria),
										 &spIEnumAAFMobs );
	if ( !SUCCEEDED(hr) ) {
		throw AxExHResult( hr, L"IAAFContentStorage::GetMobs()" );
	}

	return spIEnumAAFMobs;
}

IEnumAAFEssenceDataSP AxContentStorage::EnumEssenceData() const
{
	IEnumAAFEssenceDataSP spIEnumAAFEssenceData;
	HRESULT hr;

	hr = _spIaafContentStorage->EnumEssenceData( &spIEnumAAFEssenceData );
	if ( !SUCCEEDED(hr) ) {
		throw AxExHResult( hr, L"IAAFContentStorage::EnumEssenceData()" );
	}

	return spIEnumAAFEssenceData;
}

std::wostream& AxContentStorage::dump( std::wostream& os ) const
{
	using namespace std;

	os << L"AxContentStorage:" << endl;
	os << L"\tnot implemented" << endl;

	return os;
}

std::wostream& operator<<( std::wostream& os, const AxContentStorage& obj )
{
	return obj.dump( os );
}

