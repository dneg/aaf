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

#include "AxHeader.h"

#include <AAFResult.h>

#include <iostream>

AxHeader::AxHeader( IAAFHeaderSP spIaafHeader )
:	AxObject( AxQueryInterface<IAAFHeader,IAAFObject>(spIaafHeader) ),
	_spIaafHeader( spIaafHeader )
{}

AxHeader::AxHeader( const AxHeader& other )
:	AxObject( AxQueryInterface<IAAFHeader,IAAFObject>(other._spIaafHeader) ),
	_spIaafHeader( other._spIaafHeader )
{}

AxHeader::~AxHeader()
{}

IAAFDictionarySP AxHeader::GetDictionary() const
{
	IAAFDictionarySP spIaafDictionary;

	CHECK_HRESULT( _spIaafHeader->GetDictionary( &spIaafDictionary ) );

	return spIaafDictionary;
}
	
IAAFContentStorageSP AxHeader::GetContentStorage() const
{
	IAAFContentStorageSP spIaafContentStorage;

	CHECK_HRESULT( _spIaafHeader->GetContentStorage( &spIaafContentStorage ) );

	return spIaafContentStorage;
}

#if 0
// FIXME - Not ready to be used yet.
std::vector< IAAFIdentificationSP > AxHeader::LookupIdentifications()
{
	aafUInt32 count;

	hr = _spIaafHeader->CountIdentifications( &count );
	if ( !SUCCEEDED(hr) ) {
		throw AxExHResult( hr, L"IAAFHeader::CountIdentification" );
	}
	
	std::vector< IAAFIdentificationSP > idv = LookupIdentifications();
	while( count-- ) {

		IAAFIdentificationSP spId;

		hr = _spIaafHeader->GetIdentificationAt( count, &spId );
		if ( !SUCCEEDED(hr) ) {
			throw AxExHResult( hr, L"IAAFHeader::LookupIdentification" );
		}

		idv.push_back( spId );
	}

	return idv;
}
#endif

aafUInt32 AxHeader::CountMobs() const
{
	aafUInt32 mobCount;

	CHECK_HRESULT( _spIaafHeader->CountMobs( kAAFAllMob, &mobCount ) );

	return mobCount;
}

aafUInt32 AxHeader::CountMobs(aafMobKind_e mobKind) const
{
	aafUInt32 mobCount;

	CHECK_HRESULT( _spIaafHeader->CountMobs( mobKind, &mobCount ) );

	return mobCount;
}


// Gets all Mobs
IEnumAAFMobsSP AxHeader::GetMobs() const
{
	IEnumAAFMobsSP spMobs;
	CHECK_HRESULT(_spIaafHeader->GetMobs(NULL, &spMobs));

	return spMobs;
}

// Gets mobs according to the criteria set.
IEnumAAFMobsSP AxHeader::GetMobs(aafSearchCrit_t & searchCrit) const
{
	IEnumAAFMobsSP spMobs;
	CHECK_HRESULT(_spIaafHeader->GetMobs(&searchCrit, &spMobs));

	return spMobs;
}

aafUInt32 AxHeader::CountIdentifications()
{
	aafUInt32 idcount;

	CHECK_HRESULT(_spIaafHeader->CountIdentifications(&idcount));

	return idcount;
}

IAAFIdentificationSP AxHeader::GetIdentificationAt(aafUInt32 idno)
{
	IAAFIdentificationSP spIdent;

	CHECK_HRESULT(_spIaafHeader->GetIdentificationAt(idno, &spIdent));

	return spIdent;
}

aafUInt32 AxHeader::CountEssence() const
{
	aafUInt32 essenceCount;

	CHECK_HRESULT( _spIaafHeader->CountEssenceData( &essenceCount ) );

	return essenceCount;
}

void AxHeader::AddMob( IAAFMobSP spIaafMob )
{
	CHECK_HRESULT( _spIaafHeader->AddMob( spIaafMob ) );
}

std::wostream& AxHeader::dump( std::wostream& os ) const
{
	using namespace std;
		
	os << L"Header Summary:" << endl;
	os << L"\tMob Count:\t" << CountMobs() << endl;
	os << L"\tEssense Count:\t" << CountEssence() << endl;

	return os;
}


std::wostream& operator<<( std::wostream& os, const AxHeader& obj )
{
	return obj.dump( os );
}

AxIdentification::AxIdentification(IAAFIdentificationSP spIaafIdentification)
:	AxObject( AxQueryInterface<IAAFIdentification,IAAFObject>(spIaafIdentification) ),
	_spIaafIdentification(spIaafIdentification)
{};

aafUID_t AxIdentification::GetProductID()
{
	aafUID_t prodID;

	CHECK_HRESULT( _spIaafIdentification->GetProductID(&prodID));

	return prodID;
}
