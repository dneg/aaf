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
//
//=---------------------------------------------------------------------

#include <AxFile.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxMetaDef.h>
#include <AxMob.h>
#include <AxEx.h>
#include <AxUtil.h>
#include <AxKLVData.h>

#include <AAFStoredObjectIDs.h>
#include <AAFTypeDefUIDs.h>

#include <vector>
#include <iostream>

// Each of our KLV objects require a unique key.  Here they are:
// UUIDs were generated with Microsoft VC's UUIDGEN.EXE tool:

// 4353dc46-5a56-42f0-a4d8-9516dd98335f
const aafUID_t KLVKey_AxExampleData_A =
{ 0x4353dc46, 0x5a56, 0x42f0, { 0xa4, 0xd8, 0x95, 0x16, 0xdd, 0x98, 0x33, 0x5f } };
// 6def0b26-d055-41f9-892b-c7528fdae103
const aafUID_t KLVKey_AxExampleData_B =
{ 0x6def0b26, 0xd055, 0x41f9, {0x89, 0x2b, 0xc7, 0x52, 0x8f, 0xda, 0xe1, 0x03 } };



// class KLV - a simple container for KLV values.

namespace {

class KLV {

private:
	const aafUID_t& _K;
	int _xL;
	const aafUInt8* _V;

public:
	
	KLV( const aafUID_t& K, int L, const aafUInt8* V )
		: _K(K), _xL(L), _V(V)
	{}

	~KLV()
	{}

	const aafUID_t& K() const
	{ return _K; }

	int L() const
	{ return _xL; }

	const aafUInt8* V() const
	{ return _V; } 
};


// class MobDescription - contains all information required to create on
// sample mob, i.e., name, comments, and klv data.

class MobDescription {
public:
	MobDescription(
		const wchar_t* name,
		const wchar_t* comment_category_a,
		const wchar_t* comment_a,
		const wchar_t* comment_category_b,
		const wchar_t* comment_b,
		const KLV&  klv_a,
		const KLV&  klv_b
		)
		:
		_name(name),
		_comment_category_a( comment_category_a ),
		_comment_a( comment_a ),
		_comment_category_b( comment_category_b ),
		_comment_b( comment_b ),
		_klv_a( klv_a ),
		_klv_b( klv_b )
	{}

	~MobDescription()
	{}

	const AxString& name() const
	{ return _name; }

	const AxString& comment_category_a() const
	{ return _comment_category_a; }

	const AxString& comment_a() const
	{ return _comment_a; }

	const AxString& comment_category_b() const
	{ return _comment_category_b; }

	const AxString& comment_b() const
	{ return _comment_b; }

	const KLV& klv_a() const
	{ return _klv_a; }

	const KLV& klv_b() const
	{ return _klv_b; }

private:

	AxString _name;
	AxString _comment_category_a;
	AxString _comment_a;
	AxString _comment_category_b;
	AxString _comment_b;
	const KLV& _klv_a;
	const KLV& _klv_b;
};

// AddMasterMob - Creates, initializes, and adds one master mob.

void AddMasterMob( AxHeader& axHeader,
				   AxDictionary& axDictionary,
				   const MobDescription& desc )
{
	AxMasterMob axMasterMob( AxCreateInstance<IAAFMasterMob>( axDictionary ) );

	axMasterMob.Initialize();

	axMasterMob.SetName( desc.name() );

	axMasterMob.AppendComment( desc.comment_category_a(),
							   desc.comment_a() );
	axMasterMob.AppendComment( desc.comment_category_b(),
							   desc.comment_b() );


	{
		AxKLVData axKLVData( AxCreateInstance<IAAFKLVData>( axDictionary ) );
		axKLVData.Initialize( desc.klv_a().K(), desc.klv_a().L(),
							  const_cast<aafUInt8*>(desc.klv_a().V()) ); 
		axMasterMob.AppendKLVData( axKLVData );
	}

	{
		AxKLVData axKLVData( AxCreateInstance<IAAFKLVData>( axDictionary ) );

		axKLVData.Initialize( desc.klv_b().K(), desc.klv_b().L(),
							  const_cast<aafUInt8*>(desc.klv_b().V()) ); 
		axMasterMob.AppendKLVData( axKLVData );
	}

	// That's it, add the mob.
	axHeader.AddMob( axMasterMob );
}

} // end of namespace


// AxCreateMetaDataExample - Add four master mobs the file.  Two named Video A/B, two
// named Audio A/B.  Another test module will find these mobs, and add essence data.

void AxCreateMetaDataExample( AxFile& axFile,
							  AxCmdLineArgs& args )
{
	AxHeader axHeader( axFile.getHeader() );
	AxDictionary axDictionary( axHeader.GetDictionary() );

	axDictionary.RegisterKLVDataKey(
		KLVKey_AxExampleData_A,
		axDictionary.LookupTypeDef( kAAFTypeID_UInt8Array ) );

	axDictionary.RegisterKLVDataKey(
		KLVKey_AxExampleData_B,
		axDictionary.LookupTypeDef( kAAFTypeID_UInt8Array ) );

	aafUInt16 v_a = 0xabcd;
	KLV klv_a( KLVKey_AxExampleData_A, sizeof(v_a), reinterpret_cast<aafUInt8*>(&v_a) );

	aafUInt32 v_b = 0x87654321;
	KLV klv_b( KLVKey_AxExampleData_A, sizeof(v_b), reinterpret_cast<aafUInt8*>(&v_b) );

	vector< MobDescription* > descV;

	descV.push_back( 
		new MobDescription( L"Audio Mob A",
				    L"Meta Comment", L"Metadata added.",
				    L"Essence Comment", L"No essence data yet.",
				    klv_a, klv_b  ) );

	descV.push_back( 
		new MobDescription( L"Audio Mob B",
				    L"Meta Comment", L"Metadata added.",
				    L"Essence Comment", L"No essence data yet.",
				    klv_a, klv_b  ) );

	descV.push_back( 
		new MobDescription( L"Video Mob A",
				    L"Meta Comment", L"Metadata added.",
				    L"Essence Comment", L"No essence data yet.",
				    klv_a, klv_b  ) );

	descV.push_back( 
		new MobDescription( L"Video Mob B",
				    L"Meta Comment", L"Metadata added.",
				    L"Essence Comment", L"No essence data yet.",
				    klv_a, klv_b  ) );

	vector< MobDescription* >::const_iterator iter;
	for( iter = descV.begin(); iter != descV.end(); ++iter ) {
		AddMasterMob( axHeader, axDictionary, **iter );
	}

	/* Must delete "manually".  It would be nice not use a bare 
	   pointer so that this could be avoided. */
	for( iter = descV.begin(); iter != descV.end(); ++iter ) {
	  delete *iter;
	}
}
