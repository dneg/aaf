// AxShellExtFolder.cpp

#include "StdAfx.h"

#include "AxShellExt.h"
#include "AAFShellExtDebug.h"
#include "AAFShellExtPIDL.h"

#include "AxFile.h"
#include "AxHeader.h"
#include "AxDictionary.h"
#include "AxProperty.h"
#include "AxPropertyValue.h"
#include "AxPropertyValueDump.h"
#include "AxObject.h"
#include "AxMetaDef.h"

#include <AAFStoredObjectIDs.h>
#include <AAFTypeDefUIDs.h>

#include <PidlMgr.h>

#include <sstream>

#include <vector>

LPCITEMIDLIST FindLastUnkownPidl( LPCITEMIDLIST rootPidl )
{
	CPidlMgr pidlMgr;
	LPITEMIDLIST lastUnknown = 0;
	LPITEMIDLIST pidlNext;

	int last;
	int i;

	for( pidlNext = const_cast<LPITEMIDLIST>(rootPidl), i = 1, last = -1;
		 pidlNext && pidlNext->mkid.cb != 0;
		 pidlNext = pidlMgr.GetNextItem( pidlNext ), i++ ) {

		AAFShellExtPidl* pOurPidlData = 0;

		if ( pidlMgr.GetDataPointer( pidlNext, &pOurPidlData ) &&
			 memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
		}
		else {
			last = i;
			lastUnknown = pidlNext;
		}
	}

	AAFShellExtLog( "Last unkown found at: " );
	AAFShellExtLog( last );
	AAFShellExtLog( "\n" );

	return lastUnknown;
}

void DumpPidlDebugInfo( LPCITEMIDLIST pidl )
{
	CPidlMgr pidlMgr;

	int i;
	LPITEMIDLIST pidlNext;

	AAFShellExtLog( "DumpPidlDebugInfo()\n" );

	for( pidlNext = const_cast<LPITEMIDLIST>(pidl), i = 1;
		 pidlNext && pidlNext->mkid.cb != 0;
		 pidlNext = pidlMgr.GetNextItem( pidlNext ), i++ ) {

		std::wostringstream os;
		os << L"Pidl " << i << L": ";

		AAFShellExtPidl* pOurPidlData = 0;

		if ( pidlMgr.GetDataPointer( pidlNext, &pOurPidlData ) && 
  			    memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
			os << L"AAF: " << pOurPidlData->pDisplayText;
		}
		else if ( pidlNext->mkid.cb == 0 ) {
			os << "empty pidl";
		}
		else {
			os << L"unkown";
		}

		AAFShellExtLog( "\t" );
		AAFShellExtLog( os.str().c_str() );
		AAFShellExtLog( "\n" );
	}
}

bool FilePathFromRootPidl( LPCITEMIDLIST rootPidl, AxString& retFileName )
{
	CPidlMgr pidlMgr;

	// Search for first known pidl or end.
	// If known pidl found, make a copy, zero that pidl,
	// Get the filename, and restore.
	// If end found, just get filename.

	LPITEMIDLIST pidl = pidlMgr.Copy( rootPidl );

	LPITEMIDLIST pidlNext;

	for( pidlNext = pidl;
		 pidlNext && pidlNext->mkid.cb != 0;
		 pidlNext = pidlMgr.GetNextItem( pidlNext ) ) {

		AAFShellExtPidl* pOurPidlData = 0;

		if ( pidlMgr.GetDataPointer( pidlNext, &pOurPidlData ) ) {

			if ( memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
				memset( pidlNext, 0, pidlNext->mkid.cb );
				// It is ours.
				break;
			}

		}
		else {
			// This should never happen.  Just in case, return false.
			return false;
		}
	}
	


	TCHAR szPath[MAX_PATH];

	AxString fileName;
	
	bool retVal = false;

	if( pidl->mkid.cb > 0 && SHGetPathFromIDList( pidl, szPath ) ) {
		retFileName = AxString( szPath );
		retVal = true;
	}

	pidlMgr.Delete( pidl );

	return retVal;
}



bool ObjectPathFromRootPidl( LPCITEMIDLIST rootPidl, vector<AxString>& objPath )
{
	CPidlMgr pidlMgr;

	LPCITEMIDLIST lastUnknownPidl = FindLastUnkownPidl( rootPidl );

	LPITEMIDLIST pidlNext;

	// oops... if the last unknown is null, that means they	
	// are all known... start iterating at root.
	if ( !lastUnknownPidl ) {
		lastUnknownPidl = rootPidl;
	}

	for( pidlNext = const_cast<LPITEMIDLIST>(lastUnknownPidl);
		 pidlNext && pidlNext->mkid.cb != 0;
		 pidlNext = pidlMgr.GetNextItem( pidlNext ) ) {

		AAFShellExtPidl* pOurPidlData = 0;

		if ( pidlMgr.GetDataPointer( pidlNext, &pOurPidlData ) ) {

			if ( memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
				// It is ours.
				break;
			}

		}
		else {
			// This should never happen.  Just in case, return false.
			return false;
		}
	}

	/* Extract strings from remaining pidls, and push onto vector. */
	for( ;
		 pidlNext && pidlNext->mkid.cb != 0;
		 pidlNext = pidlMgr.GetNextItem( pidlNext ) ) {

		AAFShellExtPidl* pOurPidlData = 0;

		if ( pidlMgr.GetDataPointer( pidlNext, &pOurPidlData ) ) {

			if ( memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
				objPath.push_back( AxString( pOurPidlData->pDisplayText ) );			
			}
			else {
				// Should only find our own pidls.
				return false;
			}
		}
		else {
			// Should never fail to get a data pointer.
			return false;
		}
	}

	return true;
}

AxString ObjPathStringFromRootPidl( LPCITEMIDLIST rootPidl )
{
	vector<AxString> objPath;

	if ( ObjectPathFromRootPidl( rootPidl, objPath ) ){
		std::wostringstream os;
		int i;
		os << L"\\\\";
		for( i = 0; i < objPath.size(); i++ ){
			os << objPath[i] << L"\\";
		}
		return os.str();
	}
	else {
		return AxString( L"\\\\" );
	}
}

AxString CompletePathFromPidl( LPCITEMIDLIST rootPidl )
{
	std::wostringstream os;

	AxString filePath;
	if ( FilePathFromRootPidl( rootPidl, filePath ) ) {
		os << filePath;
	}

	os << ObjPathStringFromRootPidl( rootPidl );

	return os.str();
}

LPCITEMIDLIST GetFirstAAFPidl( LPCITEMIDLIST rootPidl ) 
{
	CPidlMgr pidlMgr;

	LPITEMIDLIST pidlNext;

	for( pidlNext = const_cast<LPITEMIDLIST>(rootPidl);
		 pidlNext && pidlNext->mkid.cb != 0;
		 pidlNext = pidlMgr.GetNextItem( pidlNext ) ) {

		AAFShellExtPidl* pOurPidlData = 0;

		if ( pidlMgr.GetDataPointer( pidlNext, &pOurPidlData ) ) {

			if ( memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
				return pidlNext;
			}
		}
	}

   return 0;
}

const AAFShellExtPidl* GetEndAAFPidl( LPCITEMIDLIST rootPidl ) 
{
	CPidlMgr pidlMgr;

	LPITEMIDLIST pidlEnd = pidlMgr.GetLastItem( rootPidl );

	AAFShellExtPidl* pOurPidlData = 0;

	if ( pidlMgr.GetDataPointer( pidlEnd, &pOurPidlData ) ) {

		if ( memcmp( &pOurPidlData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ) {
			return pOurPidlData;
		}
	}

   return 0;
}

LPCITEMIDLIST GetAbsolutePidl( LPCITEMIDLIST pidl )
{
	const AAFShellExtPidl* last = GetEndAAFPidl( pidl );

	if ( last ) {
		return last->absPidl;
	}
	else {
		return pidl;
	}
}

//==============================================================================================

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

int IterateOverAllAndRenamePeskyOpaques( AxDictionary& axDict,
										 AxBaseObjRecIter& recIter )
{
	using namespace std;

	pair<bool, auto_ptr< AxBaseObj > > next;
	int unusedLevel;

	int count = 0;

	for( next.first = recIter.NextOne( next.second, unusedLevel );
		 next.first;
		 next.first = recIter.NextOne( next.second, unusedLevel ) ) {

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


void RenamePeskyOpaques( IAAFHeaderSP spIaafHeader )
{
	AxHeader axHeader( spIaafHeader );
	AxDictionary axDictionary( axHeader.GetDictionary() );
	
	auto_ptr< AxBaseObjIterPrtcl > axHeaderIter(
		new AxBaseSolitaryObjIter<AxHeader>(axHeader) );

	// Create a recursive iterator...
	AxBaseObjRecIter recIter( axHeaderIter );

		// ... and run through all values registering
	// a renamed type for each opaque type.
	std::wostringstream os;
	os << L"Rename Pesky Opaques:" << endl;
	int count = IterateOverAllAndRenamePeskyOpaques( axDictionary, recIter );
	os << L"\tOpaques reported by dictionary: " << axDictionary.CountOpaqueTypeDefs() << endl;
	os << L"\tOpaques found and renamed: " << count << endl;
	AAFShellExtLog( os.str().c_str() );
}

//==============================================================================================

AxShellExtIDList::AxShellExtIDList( LPCITEMIDLIST rootPidl )
: _rootPidl(rootPidl),
  _levelToIterateOver(-1)
{
	AAFShellExtLog( "Opening: " );
	AAFShellExtPidlLog( _rootPidl );
	AAFShellExtLog( "\n" );


	// Everything from the beginning of to our first pidl is assumed to
	// be the file name.  Everything after that identifies an element
	// in the AAF file.  The filePidl can never be null.  the objectPidl
	// may be null, indicating we should start browing at the header.
	_fileName = L"no path";
	FilePathFromRootPidl( _rootPidl, _fileName );
	// FIXME - inadequate error handling.
	
	// Now get the obj path into the file.
	_objPath.clear();
	ObjectPathFromRootPidl( _rootPidl, _objPath );

	// ... and the lid;
	const AAFShellExtPidl* endData = GetEndAAFPidl( rootPidl );
	if ( endData ) {
		_startLid = endData->lid;
	}
	else {
		_startLid = -1;
	}
}

AxShellExtIDList::~AxShellExtIDList()
{}

void AxShellExtIDList::Initialize()
{
	std::auto_ptr<AxInit> initObj( new AxInit );
	_initObj = initObj;
	
	// From Ctor....
	AAFShellExtLog( "About to open: " );
	AAFShellExtLog( _fileName.c_str() );
	AAFShellExtLog( "\n" );

	std::auto_ptr<AxFile> axFile( new AxFile );
	_axFile = axFile;

	_axFile->OpenExistingRead( _fileName );

	AAFShellExtLog( "Opened successfully\n" );

	AxHeader axHeader( _axFile->getHeader() );

	RenamePeskyOpaques( axHeader );

	std::auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxHeader>(axHeader) );
	
	std::auto_ptr< AxBaseObjRecIter > recIter( new AxBaseObjRecIter( iter ) );
	
	_recIter = recIter;

	DescendObjectPath();

	_levelToIterateOver = _recIter->GetLevel();
}

#if 1
void AxShellExtIDList::DescendObjectPath()
{
	std::vector<AxString>::const_iterator iter = _objPath.begin();

	int lid;

	while( iter != _objPath.end() ) {

		AxString desc;
		bool isFolder;

		AAFShellExtLog( "AxShellExtIDList::DescendObjectPath() looking for: " );
		AAFShellExtLog( (*iter).c_str() );
		AAFShellExtLog( "\n" );

		int unusedLevel = -1;

		if ( RealNext( desc, isFolder, unusedLevel, lid ) ) {
			if ( desc == *iter ) {
				++iter;
			}
			else if ( isFolder ) {
				_recIter->PopStack();
			}
		}
		else {
			// It should not happen, what to do?  return and let the chip fall
			return;
		}
	}
}
#else
void AxShellExtIDList::DescendObjectPath()
{
	std::vector<AxString>::const_iterator iter = _objPath.begin();

	int lid = -1;

	AAFShellExtLog( "AxShellExtIDList::DescendObjectPath() searching for _startLid = " );
	AAFShellExtLog( _startLid );
	AAFShellExtLog( "\n" );

	AxString desc;
	bool isFolder;

	while( lid != _startLid ) {


		int unusedLevel = -1;

		if ( RealNext( desc, isFolder, unusedLevel, lid ) ) {
			; // noop
		}
		else {
			// It should not happen, what to do?  return and let the chip fall
			DebugBreak();
			return;
		}
	}

	AAFShellExtLog( "\tFound it at: " );
	AAFShellExtLog( desc.c_str() );
	AAFShellExtLog( "\n" );
}
#endif

void AxShellExtIDList::Finalize()
{
	// from dtor
	// Ensure this is done before the file is closed.
	delete _recIter.release();

	_axFile->Close();
	delete _axFile.release();

	delete _initObj.release();
}

bool AxShellExtIDList::Next( AxString& desc, bool& isFolder, int& lid )
{
	
	int level = -1;
	bool retVal = RealNext( desc, isFolder, level, lid );

	if ( retVal ) {

		AAFShellExtLog( "AxShellExtIDList::Next level/iterLevel = " );
		AAFShellExtLog( level );
		AAFShellExtLog( " " );
		AAFShellExtLog( _recIter->GetLevel() );
		AAFShellExtLog( "\n" );
		
		if ( _recIter->GetLevel() > _levelToIterateOver ) {
			_recIter->PopStack();
		}

		if ( level == _levelToIterateOver ) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

bool AxShellExtIDList::RealNext( AxString& desc, bool& isFolder, int& level, int& lid )
{

	bool retVal = false;

	std::wostringstream os;

	std::pair<bool,auto_ptr<AxBaseObj> > next;

		next.first = _recIter->NextOne( next.second, level );

		retVal = next.first;

		if ( retVal ) {

			_levelCounters[level]++;
			lid = ((level<<16)&0xffff0000) | (_levelCounters[level]&0x0000ffff);

			// List guarantees that the display text is unique for every
			// item at every level.  If they are not unique, they will not
			// be displayed!
			os << L"[" << level << L":" << _levelCounters[level] << L"] ";

			if ( dynamic_cast<AxObject*>( next.second.get() ) ) {
			
				auto_ptr<AxObject> obj( 
					 dynamic_cast<AxObject*>( next.second.release() ) );
	
				AxString name( obj->GetClassName() );
	
				os << L"Object " << name;
	
			}
	
			else if ( dynamic_cast<AxProperty*>( next.second.get() ) ) {

				auto_ptr<AxProperty> prop( 
					 dynamic_cast<AxProperty*>( next.second.release() ) );

					AxString name = prop->GetName();

					os << L"Prop " << name;
			
			}

			else if ( dynamic_cast<AxPropertyValue*>( next.second.get() ) ) {

				auto_ptr<AxPropertyValue> propVal(
					 dynamic_cast<AxPropertyValue*>( next.second.release() ) );

				AxPropertyValueDump axPropValueDump( os );

				os << L"Value ";
				propVal->Process( axPropValueDump );
			}

			else if ( dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( next.second.get() ) ) {

				auto_ptr< AxBaseObjAny<AxRecordIterator::Pair> > recPair(
					dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( next.second.release() ) );

				os << L"RecMem " << recPair->get().first;
			}

			else if ( dynamic_cast<AxBaseObjAny<AxExHResult>*>( next.second.get() ) ) {

				auto_ptr< AxBaseObjAny<AxExHResult> > ex (
					dynamic_cast<AxBaseObjAny<AxExHResult>*>( next.second.release() ) );

				os << L"Excep" << ex->get().what();
			}


				desc = os.str();

				if ( level < _recIter->GetLevel() ) {;
					isFolder = true;
				}
				else {
					isFolder = false;
				}

				AAFShellExtLog( "AxShellExtIDList::Next: " );
				AAFShellExtLog( os.str().c_str() );
				AAFShellExtLog( "\n" );
		}
	
	return retVal;
}
