#ifndef __AxHeader_h__
#define __AxHeader_h__

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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

#include <iosfwd>

class AxHeader : public AxObject {

public:

	AxHeader( IAAFHeaderSP spIaafHeader );
	AxHeader( const AxHeader& );
	virtual ~AxHeader();

	IAAFDictionarySP	 GetDictionary() const;
	IAAFContentStorageSP GetContentStorage() const;

	aafUInt32 CountMobs() const;
	aafUInt32 CountMobs(aafMobKind_e mobKind) const;

	IEnumAAFMobsSP GetMobs() const;
	IEnumAAFMobsSP GetMobs(aafSearchCrit_t & searchCrit) const;

	IAAFMobSP LookupMob(aafMobID_constref mobid) const;

	aafUInt32 CountIdentifications();
	IAAFIdentificationSP GetIdentificationAt(aafUInt32 idno);

	aafUInt32 CountEssence() const;

	void AddMob( IAAFMobSP spIaafMob );
	
#if 0
	// FIXME - Not ready to use this yet.
	std::vector< IAAFIdentificationSP > LookupIdentifications();
#endif

	operator IAAFHeaderSP ()
	{ return _spIaafHeader; }

	std::wostream& dump( std::wostream& os ) const;

private:

	AxHeader();
	AxHeader& operator=( const AxHeader& );

	mutable IAAFHeaderSP _spIaafHeader;
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

