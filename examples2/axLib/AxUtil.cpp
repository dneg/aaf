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

#include "AxUtil.h"

#include <wchar.h>

#include <sstream>

#include <stdlib.h>
#include <string.h>

//=---------------------------------------------------------------------=

namespace {

AxString catUnkownStr( L"unkown" );

};

//=---------------------------------------------------------------------=

AxTypeCatMap::AxTypeCatMap()
{
#define MAP_ADD( T ) _map[ kAAFTypeCat##T ] = \
	AxString( AxStringUtil::mbtowc( "IAAFTypeDef" #T ) );

	MAP_ADD( Int )
	MAP_ADD( Character )
	MAP_ADD( StrongObjRef )
	MAP_ADD( WeakObjRef )
	MAP_ADD( Rename )
	MAP_ADD( Enum )
	MAP_ADD( FixedArray )
	MAP_ADD( VariableArray )
	MAP_ADD( Set )
	MAP_ADD( Record )
	MAP_ADD( Stream )
	MAP_ADD( String )
	MAP_ADD( ExtEnum )
	MAP_ADD( Indirect )
	MAP_ADD( Opaque )
	MAP_ADD( VariableArray )
	MAP_ADD( Unknown )
	MAP_ADD( Encrypted )

#undef MAP_ADD
}

AxTypeCatMap::~AxTypeCatMap()
{}

const AxTypeCatMap& AxTypeCatMap::getInstance()
{
	static AxTypeCatMap* instance = 0;

	if ( !instance ) {
		instance = new AxTypeCatMap();
	}

	return *instance;
}
 
const AxString& AxTypeCatMap::getStr( eAAFTypeCategory_t cat ) const
{
	std::map<eAAFTypeCategory_t,AxString>::const_iterator iter = _map.find( cat );
	
	if ( iter == _map.end() ) {
		return catUnkownStr;
	}

	return iter->second;

}

//=---------------------------------------------------------------------=

AxCmdLineArgs::AxCmdLineArgs( int argc, const char** argv )
:	_argc( argc ),
	_argv( argv )
{}

AxCmdLineArgs::~AxCmdLineArgs()
{}

std::pair<bool,int> AxCmdLineArgs::get( const char* opt )
{
	std::pair<bool,int> result(false,-1);

	// A Hash map would be better here....

	int i;
	for( i = 0; i < _argc; i++ ) {
		if ( strcmp( opt, _argv[i] ) == 0 ) {
			result.first = true;
			result.second = i;
			return result;
		}
	}

	return result;
}

std::pair<bool,const char*> AxCmdLineArgs::get( int n )
{
	std::pair<bool,const char*> result(false,0);

	if ( n < _argc ) {
		result.first = true;
		result.second = _argv[n];
	}

	return result;
}

//=---------------------------------------------------------------------=

AxString AxStringUtil::int2StrHex( aafUInt32 n )
{
        const unsigned int bufSize = 9;
	wchar_t buf[bufSize];

#if defined(OS_WINDOWS)
	swprintf( buf, L"%08x", n );
#else
	swprintf( buf, bufSize, L"%08x", n );
#endif

	return AxString( buf );
}

AxString AxStringUtil::int2Str( aafInt32 n )
{
	// 32 bit int requires 1 character for sign, plus
	// 10 digits, plus terminating null.
        const unsigned int bufSize = 12;
	wchar_t buf[bufSize];

#if defined(OS_WINDOWS)
	swprintf( buf, L"%d", n );
#else
	swprintf( buf, bufSize, L"%d", n );
#endif

	return AxString( buf );
}
//=---------------------------------------------------------------------=

AxString AxStringUtil::mbtowc( const char* cstr )
{
	std::wostringstream os;

	int i;
	for (i = 0; i < strlen(cstr); i++ ){ 
		wchar_t wc;
		if ( -1 != ::mbtowc( &wc, &cstr[i], 1 ) ) {
			os << wc;
		}
	}

	return AxString( os.str() );
}

//=---------------------------------------------------------------------=

