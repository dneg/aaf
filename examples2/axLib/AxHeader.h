#ifndef __AxHeader_h__
#define __AxHeader_h__

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

class AxHeader : public AxObject {

public:

	AxHeader( IAAFHeaderSP spIaafHeader );
    AxHeader( IAAFHeader2SP spIaafHeader );
	AxHeader( const AxHeader& );
	virtual ~AxHeader();

	IAAFDictionarySP	 GetDictionary() const;
	IAAFContentStorageSP GetContentStorage() const;

	aafUInt32 CountMobs() const;
	aafUInt32 CountMobs(aafMobKind_e mobKind) const;

	IEnumAAFMobsSP GetMobs() const;
	IEnumAAFMobsSP GetMobs(aafSearchCrit_t & searchCrit) const;

	IAAFMobSP LookupMob(aafMobID_constref mobid) const;
	IAAFEssenceDataSP LookupEssenceData(aafMobID_constref mobID);
	aafUInt32 CountEssenceData() const;
	aafBoolean_t IsEssenceDataPresent(aafMobID_constref mobID) const;

	aafUInt32 CountIdentifications();
	IAAFIdentificationSP GetIdentificationAt(aafUInt32 idno);

	aafUInt32 CountEssence() const;

	void AddMob( IAAFMobSP spIaafMob );

    void RemoveMob( IAAFMobSP spIaafMob );

    IEnumAAFEssenceDataSP EnumEssenceData();
    void AddEssenceData( IAAFEssenceDataSP spIaafEssenceData );
    void RemoveEssenceData( IAAFEssenceDataSP spIaafEssenceData );
    
    IAAFIdentificationSP GetLastIdentification();
    IAAFIdentificationSP LookupIdentification( aafUID_constref generation );
    IEnumAAFIdentificationsSP GetIdentifications();
    void AppendIdentification( IAAFIdentificationSP spIaafIdent );
    
    aafProductVersion_t GetRefImplVersion();
    aafVersionType_t GetFileRevision();
    aafTimeStamp_t GetLastModified();
    
    IAAFMobSP GetPrimaryMob();
    void SetPrimaryMob( IAAFMobSP spIaafPrimaryMob );
    
    aafUID_t GetOperationalPattern();
    void SetOperationalPattern( aafUID_constref operationalPatternID );
    
    void UpdateEssenceContainers();
    std::vector<aafUID_t> GetEssenceContainers();
    aafBoolean_t IsEssenceContainerPresent( aafUID_constref essenceContainerID );
    
    std::vector<aafUID_t> GetDescriptiveSchemes();
    aafBoolean_t IsDescriptiveSchemePresent( aafUID_constref descriptiveSchemeID );
    void AddDescriptiveScheme( aafUID_constref descriptiveSchemeID );
    void RemoveDescriptiveScheme( aafUID_constref descriptiveSchemeID );
	
#if 0
	// FIXME - Not ready to use this yet.
	std::vector< IAAFIdentificationSP > LookupIdentifications();
#endif

    inline operator IAAFHeaderSP ()
    { return AxQueryInterface<IAAFHeader2,IAAFHeader>( _spIaafHeader ); }

	inline operator IAAFHeader2SP ()
	{ return _spIaafHeader; }

	std::wostream& dump( std::wostream& os ) const;

private:

	AxHeader();
	AxHeader& operator=( const AxHeader& );

	mutable IAAFHeader2SP _spIaafHeader;
};

std::wostream& operator<<( std::wostream& os, const AxHeader& obj );

class AxIdentification : public AxObject  
{
public:
	AxIdentification(IAAFIdentificationSP spIaafIdentification);
	virtual ~AxIdentification(){};
	aafUID_t GetProductID();
private:
	AxIdentification();
	mutable IAAFIdentificationSP _spIaafIdentification;
};

#endif

