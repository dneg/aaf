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
//=---------------------------------------------------------------------=

#include <AxFile.h>
#include <AxTypes.h>
#include <AxInit.h>
#include <AxObject.h>
#include <AxMetaDef.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxIterator.h>
#include <AxProperty.h>
#include <AxContentStorage.h>
#include <AxPropertyValueDump.h>
#include <AxBaseObj.h>
#include <AxBaseObjIter.h>
#include <AxUtil.h>
#include <AxEx.h>

#include <AAFStoredObjectIDs.h>
#include <AAFTypeDefUIDs.h>

#include <iostream>
#include <sstream>

namespace {

void throwUsage()
{
	wostringstream msg;
	msg << L"Usage: " << L" -file filename [-objects] [-properties] [-values] [-records] [-verbose]";
	throw AxEx( msg.str().c_str() );
}

//=---------------------------------------------------------------------=

class Level {
public:

	inline Level(int l)
		:_l(l) {};

	inline Level& operator=( const Level& rhs )
	{
		_l = rhs._l;
		return *this;
	}

	inline int get() const
	{return _l;}

	inline int& getRef()
	{ return _l; }

private:
	int _l;
};

std::wostream& operator<<( std::wostream& os, const Level& level )
{
	int l = level.get();

	while( l-- >= 0 ) {
		os << L"  ";
	}

	return os;
}

//=---------------------------------------------------------------------=

void dumpBaseObjects( AxDictionary& axDict,
			 AxBaseObjRecIter& recIter,
			 AxCmdLineArgs& args )
{
	using namespace std;

	pair<bool,auto_ptr<AxBaseObj> > next;
	Level level(0);

	// FIXME - options require refinement.
	pair<bool,int> verboseOpt = args.get("-verbose");
	bool not_verbose = !verboseOpt.first;

	pair<bool,int> objectDump = args.get("-objects");
	pair<bool,int> propertyDump = args.get("-properties");
	pair<bool,int> valueDump = args.get("-values");
	pair<bool,int> recordDump = args.get("-records");

	// For each object, cast to a known type and process accordingly.

	// Worth noting:
	//
	//		- If the object is, ahh... an object, then simply output
	//		  the class name.
	//
	//		- If the object is a property we ask it if it the dictionary
	//		  and if it is then stop recursion down that subtree.  Note,
	//		  a operator== implementation exists that could be used instead
	//		  of AxProperty::isA().
	//
	//		- If the object is a propety value then process it by calling
	//		  the AxProperty::Process() using AxPropertyValueDump as the
	//		  processing protocol instance.  Of course, any instance of
	//		  of AxPropertyValuePrtcl could be used here.
	//
	//		- If the iterator passes out an exception, then just dump the
	//		  exception details and keep on iterating.
	//
	//		- Note the means by which the dictionary property is identified,
	//		  and its subtree not descended.
	
	
	for( next.first  = recIter.NextOne( next.second, level.getRef() );
		 next.first;
		 next.first = recIter.NextOne( next.second, level.getRef() ) ) {

		 if ( objectDump.first &&
			  dynamic_cast<AxObject*>( next.second.get() ) ) {

			auto_ptr<AxObject> obj( 
				 dynamic_cast<AxObject*>( next.second.release() ) );

			wcout << level.get() << L"\tObject\t" << level <<
				*obj->GetClassName() << endl;
		}

		else if ( dynamic_cast<AxProperty*>( next.second.get() ) ) {

			auto_ptr<AxProperty> prop( 
				 dynamic_cast<AxProperty*>( next.second.release() ) );

			if ( not_verbose &&
				 ( *prop == kAAFPropID_Header_Dictionary ||
				   *prop == kAAFPropID_Header_Version ||
				   *prop == kAAFPropID_Header_IdentificationList ) ) {
			     recIter.PopStack();
			}

			if ( propertyDump.first ) {
				auto_ptr< AxString > name = prop->GetName();
				wcout << level.get()<< L"\tProp\t" << level << *name;
				wcout << endl;
			}

			
		}
		else if ( valueDump.first && 
				  dynamic_cast<AxPropertyValue*>( next.second.get() ) ) {

			auto_ptr<AxPropertyValue> propVal( 
				 dynamic_cast<AxPropertyValue*>( next.second.release() ) );

			AxPropertyValueDump axPropValueDump( std::wcout );

			wcout << level.get() << L"\tValue\t" << level;
			propVal->Process( axPropValueDump );
			wcout << endl;
		}

		else if ( dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( next.second.get() ) ) {

			auto_ptr< AxBaseObjAny<AxRecordIterator::Pair> > recPair(
				dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( next.second.release() ) );

			if ( recordDump.first ) {
				wcout << level.get() << L"\tRecMem\t" << level;
				wcout << recPair->get().first << endl;
			}
			else {
				// If we are not dumping records, then don't even bother
				// continuing the recursion.			
				recIter.PopStack();
			}
		}

		else if ( dynamic_cast<AxBaseObjAny<AxExHResult>*>( next.second.get() ) ) {
			auto_ptr< AxBaseObjAny<AxExHResult> > ex (
				dynamic_cast<AxBaseObjAny<AxExHResult>*>( next.second.release() ) );

			wcout << level.get() << L"\tExcep\t" << level;
			wcout << ex->get().what() << endl;
		}

	}
} // end of dumpBaseObjects

//=---------------------------------------------------------------------=


class AxPropValueRenamePeskyOpaques	: public AxPropertyValueNoopPrtcl {
public:

	AxPropValueRenamePeskyOpaques( AxDictionary& axDict )
		: _axDict( axDict ),
  		  _count(0)
	{}

	virtual ~AxPropValueRenamePeskyOpaques()
	{}

	virtual void process( IAAFPropertyValueSP& spPropVal,
						  IAAFTypeDefOpaqueSP& spTypeDefOpaque )
	{
		AxTypeDefOpaque axOpaque( spTypeDefOpaque );

		aafUID_t typeUid = axOpaque.GetActualTypeID( spPropVal );

		if ( !_axDict.isKnownTypeDef( typeUid ) ) {

			AxTypeDef axBaseTypeDef( _axDict.LookupTypeDef( kAAFTypeID_UInt8Array ) );

			AxTypeDefRename axTypeDefRename(
				AxCreateMetaInstance<IAAFTypeDefRename>(
							_axDict, AUID_AAFTypeDefRename, IID_IAAFTypeDefRename ) );

			axTypeDefRename.Initialize( typeUid, axBaseTypeDef, AxString( L"Opaque Data" ) );

			// The query iterface can be removed if base class cast is implemented.
			_axDict.RegisterOpaqueTypeDef(
				AxQueryInterface<IAAFTypeDefRename,IAAFTypeDef>(
					axTypeDefRename, IID_IAAFTypeDef ) );

			_count++;
		}
	}

	int GetCount()
	{return _count;}

private:
	AxDictionary& _axDict;
	int _count;
};



int renamePeskyOpaques( AxDictionary& axDict,
						AxBaseObjRecIter& recIter )
{
	using namespace std;

	pair<bool, auto_ptr< AxBaseObj > > next;
	Level level(0);

	int count = 0;

	for( next.first = recIter.NextOne( next.second, level.getRef() );
		 next.first;
		 next.first = recIter.NextOne( next.second, level.getRef() ) ) {

		if ( dynamic_cast<AxPropertyValue*>( next.second.get() ) ) {

			auto_ptr<AxPropertyValue> propVal( 
				 dynamic_cast<AxPropertyValue*>( next.second.release() ) );

			AxPropValueRenamePeskyOpaques axPropValueRenamePeskyOpaques( axDict );

			propVal->Process( axPropValueRenamePeskyOpaques );

			count += axPropValueRenamePeskyOpaques.GetCount();
		}
	}

	return count;
}


} // end of namepspace

//=---------------------------------------------------------------------=

int main( int argc, const char** argv )
{
	using namespace std;
	
	try {

		// Second command line argument is the filename
		AxCmdLineArgs args( argc, argv );

		pair<bool, const char*> fileArg = args.get(1);

		pair<bool, int> fileOpArg = args.get( "-file" );
		if ( !fileOpArg.first ) {
			throwUsage();
		}

		pair<bool, const char*> fileNameArg = args.get( fileOpArg.second + 1 );
		if ( !fileNameArg.first ) {
			throwUsage();
		}


		AxString fileName( AxStringUtil::mbtowc( fileNameArg.second ) );

		// One time init stuff.  Including loading the com api library.
		AxInit initObj;
	
		// Open the file.
		AxFile axFile;
		axFile.OpenExistingRead( fileName );

		
		// Get the header to use as a root object from which to begin iteration.
		// Any object, property, or property value can be used as a root object.
		// Generally, you probably not start with the header - you would start
		// with the ContentStorage object, or the Mobs or EssenceData properties
		// of the ContentStorage object.
		AxHeader axHeader( axFile.getHeader() );
		wcout << axHeader << endl;

		AxDictionary axDictionary( axHeader.GetDictionary() );

		// The header must be wrapped up by a AxBaseSolitaryObjIter instance
		// in order to be used a root for the recursive iterator.
		// If you follow the code, you will see that the header object is, ultimately,
		// cast to AxObject so that the IAAFObject interface of the header can be used
		// to access its properties.
	
		{
			auto_ptr< AxBaseObjIterPrtcl > axHeaderIter(
				new AxBaseSolitaryObjIter<AxHeader>(axHeader) );

			// Create a recursive iterator...
			AxBaseObjRecIter recIter( axHeaderIter );

			// ... and run through all values registering
			// a renamed type for each opaque type.
			wcout << L"Rename Pesky Opaques:" << endl;
			int count = renamePeskyOpaques( axDictionary, recIter );
			wcout << L"\tOpaques reported by dictionary: " << axDictionary.CountOpaqueTypeDefs() << endl;
			wcout << L"\tOpaques found and renamed: " << count << endl;
		}

		{
			auto_ptr< AxBaseObjIterPrtcl > axHeaderIter(
			new AxBaseSolitaryObjIter<AxHeader>(axHeader) );

		    // Create a recursive iterator...
			AxBaseObjRecIter recIter( axHeaderIter );

			// ... and dump all objects.
			wcout << endl << L"Recursive Dump:" << endl;
			wcout << endl << L"Level   Desc.       Detail" << endl;
			dumpBaseObjects( axDictionary, recIter, args );
		}

		// That's all folks.
		axFile.Close();
		wcout << L"That's all folks." << endl;
	}

	catch ( const AxEx& ex ) {
		wcout << ex.what() << endl;
	}

	return 0;
}
