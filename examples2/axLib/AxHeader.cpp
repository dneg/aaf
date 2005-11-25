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
//=---------------------------------------------------------------------=

#include "AxHeader.h"

#include <AAFResult.h>

#include <iostream>

AxHeader::AxHeader( IAAFHeader2SP spIaafHeader )
:	AxObject( AxQueryInterface<IAAFHeader2, IAAFObject>(spIaafHeader) ),
	_spIaafHeader( spIaafHeader )
{}

AxHeader::AxHeader( IAAFHeaderSP spIaafHeader )
: AxObject( AxQueryInterface<IAAFHeader, IAAFObject>( spIaafHeader ) ),
    _spIaafHeader( AxQueryInterface<IAAFHeader, IAAFHeader2>( spIaafHeader ) )
{}

AxHeader::AxHeader( const AxHeader& other )
:	AxObject( AxQueryInterface<IAAFHeader2,IAAFObject>(other._spIaafHeader) ),
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

IAAFMobSP AxHeader::LookupMob(aafMobID_constref mobid) const
{
	IAAFMobSP spmob;

	CHECK_HRESULT(_spIaafHeader->LookupMob(mobid, &spmob));

	return spmob;
}

IAAFEssenceDataSP AxHeader::LookupEssenceData(aafMobID_constref mobID)
{
	IAAFEssenceDataSP spessdata;

	CHECK_HRESULT(_spIaafHeader->LookupEssenceData(mobID, &spessdata));

	return spessdata;
}

aafUInt32 AxHeader::CountEssenceData() const
{
        aafUInt32 result;

        CHECK_HRESULT(_spIaafHeader->CountEssenceData(&result));

        return result;
}

aafBoolean_t AxHeader::IsEssenceDataPresent(aafMobID_constref mobID) const
{
    	aafBoolean_t result;

	CHECK_HRESULT(_spIaafHeader->IsEssenceDataPresent(mobID,kAAFEssence,&result));

	return result;
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

void AxHeader::RemoveMob( IAAFMobSP spIaafMob )
{
    CHECK_HRESULT( _spIaafHeader->RemoveMob( spIaafMob ) );
}


IEnumAAFEssenceDataSP AxHeader::EnumEssenceData()
{
    IEnumAAFEssenceDataSP spEssence;

    CHECK_HRESULT(_spIaafHeader->EnumEssenceData(&spEssence));

    return spEssence;
}

void AxHeader::AddEssenceData( IAAFEssenceDataSP spIaafEssenceData )
{
    CHECK_HRESULT( _spIaafHeader->AddEssenceData( spIaafEssenceData ) );
}

void AxHeader::RemoveEssenceData( IAAFEssenceDataSP spIaafEssenceData )
{
    CHECK_HRESULT( _spIaafHeader->RemoveEssenceData( spIaafEssenceData ) );
}

IAAFIdentificationSP AxHeader::GetLastIdentification()
{
    IAAFIdentificationSP spIaafIdent;

    CHECK_HRESULT( _spIaafHeader->GetLastIdentification( &spIaafIdent ) );

    return spIaafIdent;
}

IAAFIdentificationSP AxHeader::LookupIdentification( aafUID_constref generation )
{
    IAAFIdentificationSP spIaafIdent;

    CHECK_HRESULT( _spIaafHeader->LookupIdentification( generation, &spIaafIdent ) );

    return spIaafIdent;
}

IEnumAAFIdentificationsSP AxHeader::GetIdentifications()
{
    IEnumAAFIdentificationsSP spIdentifications;

    CHECK_HRESULT(_spIaafHeader->GetIdentifications(&spIdentifications));

    return spIdentifications;
}

void AxHeader::AppendIdentification( IAAFIdentificationSP spIaafIdent )
{
    CHECK_HRESULT( _spIaafHeader->AppendIdentification( spIaafIdent ) );
}

aafProductVersion_t AxHeader::GetRefImplVersion()
{
    aafProductVersion_t productVersion;

    CHECK_HRESULT( _spIaafHeader->GetRefImplVersion( &productVersion ) );

    return productVersion;
}

aafVersionType_t AxHeader::GetFileRevision()
{
    aafVersionType_t versionType;

    CHECK_HRESULT( _spIaafHeader->GetFileRevision( &versionType ) );

    return versionType;
}

aafTimeStamp_t AxHeader::GetLastModified()
{
    aafTimeStamp_t timeStamp;

    CHECK_HRESULT( _spIaafHeader->GetLastModified( &timeStamp ) );

    return timeStamp;
}

IAAFMobSP AxHeader::GetPrimaryMob()
{
    IAAFMobSP spIaafMob;

    CHECK_HRESULT( _spIaafHeader->GetPrimaryMob( &spIaafMob ) );

    return spIaafMob;
}

void AxHeader::SetPrimaryMob( IAAFMobSP spIaafPrimaryMob )
{
    CHECK_HRESULT( _spIaafHeader->SetPrimaryMob( spIaafPrimaryMob ) );
}

aafUID_t AxHeader::GetOperationalPattern()
{
    aafUID_t operationalPattern;

    CHECK_HRESULT( _spIaafHeader->GetOperationalPattern( &operationalPattern ) );

    return operationalPattern;
}

void AxHeader::SetOperationalPattern( aafUID_constref operationalPatternID )
{
    CHECK_HRESULT( _spIaafHeader->SetOperationalPattern( operationalPatternID ) );
}

void AxHeader::UpdateEssenceContainers()
{
    CHECK_HRESULT( _spIaafHeader->UpdateEssenceContainers() );
}

std::vector<aafUID_t> AxHeader::GetEssenceContainers()
{
    aafUInt32 numberOfContainers = 0;

    CHECK_HRESULT( _spIaafHeader->CountEssenceContainers( &numberOfContainers ) );
    
    std::vector<aafUID_t> essenceContainers( numberOfContainers ); 

    CHECK_HRESULT( _spIaafHeader->GetEssenceContainers( numberOfContainers, &(essenceContainers[0]) ) ); 
    
    return essenceContainers;
}

aafBoolean_t AxHeader::IsEssenceContainerPresent( aafUID_constref essenceContainerID )
{
    aafBoolean_t isContainerPresent;

    CHECK_HRESULT( _spIaafHeader->IsEssenceContainerPresent( essenceContainerID, &isContainerPresent ) );

    return isContainerPresent;
}

std::vector<aafUID_t> AxHeader::GetDescriptiveSchemes()
{
    aafUInt32 numberOfSchemes = 0;

    CHECK_HRESULT( _spIaafHeader->CountDescriptiveSchemes( &numberOfSchemes ) );
    
    std::vector<aafUID_t> descriptiveSchemes( numberOfSchemes ); 

    CHECK_HRESULT( _spIaafHeader->GetDescriptiveSchemes( numberOfSchemes, &(descriptiveSchemes[0]) ) ); 
    
    return descriptiveSchemes;
}

aafBoolean_t AxHeader::IsDescriptiveSchemePresent( aafUID_constref descriptiveSchemeID )
{
    aafBoolean_t isSchemePresent;

    CHECK_HRESULT( _spIaafHeader->IsDescriptiveSchemePresent( descriptiveSchemeID, &isSchemePresent ) );

    return isSchemePresent;
}

void AxHeader::AddDescriptiveScheme( aafUID_constref descriptiveSchemeID )
{
    CHECK_HRESULT( _spIaafHeader->AddDescriptiveScheme( descriptiveSchemeID ) );
}

void AxHeader::RemoveDescriptiveScheme( aafUID_constref descriptiveSchemeID )
{
    CHECK_HRESULT( _spIaafHeader->RemoveDescriptiveScheme( descriptiveSchemeID ) );
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
