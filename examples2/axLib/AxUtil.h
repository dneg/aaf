#ifndef __AxUtil_h__
#define __AxUtil_h__

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

#include <AAFTypes.h>

#include <vector>
#include <memory>
#include <utility>

#include <map>

// Map eAAFTypeCategory_t to string

class AxTypeCatMap {
public:
	~AxTypeCatMap();

	static const AxTypeCatMap& getInstance();

	const AxString& getStr( eAAFTypeCategory_t cat ) const;

private:
	AxTypeCatMap();

	std::map<eAAFTypeCategory_t,AxString> _map;
};

// Simple command line arguments class

class AxCmdLineArgs {
public:
	
	AxCmdLineArgs( int argc, const char ** argv );
	~AxCmdLineArgs();

	// Get index of option "opt" at or after argv[n].  True if found.
	std::pair<bool,int> get( const char* opt, int n=0 );

	// Return argv[n].  True if n < argc.
	std::pair<bool,const char*> get( int n );
	
private:
	int _argc;
	const char** _argv;
};

// Useful collection of string processing functions.

class AxStringUtil {

public:

	static AxString bool2Str( bool value );
	static AxString bool2Str( aafBoolean_t value );
	
	static AxString int2StrHex( aafUInt32 n );

	static AxString int2Str( aafInt32 n );

	static AxString uid2Str(const aafUID_t & uid);

	static AxString mbtowc( const char* cstr ); 

	static std::string wctomb( const AxString& s );

	// Converts using base zero - see the strtol man page.
	// This means that s can be decimal, hex, or octal using
	// the usual notations.  Throws an exception if conversion
	// failed.
	static long strtol( const char* s );

	// This will convert to 8 bit chars, then convert.
	static long strtol( const AxString& s );
};

// Function template that will return a type name given a smart
// pointer of type "Type".

template <class Type, class DefType>
AxString AxDefNameToString( IAAFSmartPointer< Type >& sp )
{
	IAAFSmartPointer< DefType > spDef;

	CHECK_HRESULT( sp->GetDefinition( &spDef ) );

	aafUInt32 sizeInBytes = 0;
	CHECK_HRESULT( spDef->GetNameBufLen( &sizeInBytes ) );

	// size is in bytes!  Divide by sizeof(aafCharacter) to allocate correctly
	// sized aafCharacter array.  Add one to account for possible rounding.

	int sizeInChars = (sizeInBytes /sizeof(aafCharacter)) + 1;
	std::auto_ptr< aafCharacter >
		buf( new aafCharacter[ sizeInChars ] );

	CHECK_HRESULT( spDef->GetName( buf.get(), sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( buf.get() );

	return name;
}


// Function template that will return a name given a smart
// pointer of type "Type".  Type must have a "GetNameBufLen",
// and a "GetName" method.

template <class Type>
AxString AxNameToString( IAAFSmartPointer< Type >& sp )
{
	aafUInt32 sizeInBytes = 0;

	CHECK_HRESULT( sp->GetNameBufLen( &sizeInBytes ) );

	// size is in bytes!  Divide by sizeof(aafCharacter) to allocate correctly
	// sized aafCharacter array.  Add one to account for possible rounding.

	int sizeInChars = (sizeInBytes /sizeof(aafCharacter)) + 1;
	std::auto_ptr< aafCharacter >
		buf( new aafCharacter[ sizeInChars ] );

	CHECK_HRESULT( sp->GetName( buf.get(), sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( buf.get() );

	return name;
}

// Function template that will return a description given a smart
// pointer of type "Type".  Type must have a "GetDescriptionBufLen",
// and a "GetDescription" method.

template <class Type>
AxString AxDescriptionToString( IAAFSmartPointer< Type >& sp )
{
	aafUInt32 sizeInBytes = 0;

	CHECK_HRESULT( sp->GetDescriptionBufLen( &sizeInBytes ) );

	// size is in bytes!  Divide by sizeof(aafCharacter) to allocate correctly
	// sized aafCharacter array.  Add one to account for possible rounding.

	int sizeInChars = (sizeInBytes /sizeof(aafCharacter)) + 1;
	std::auto_ptr< aafCharacter >
		buf( new aafCharacter[ sizeInChars ] );

	CHECK_HRESULT( sp->GetDescription( buf.get(), sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( buf.get() );

	return name;
}

// Some of the AAF interfaces (e.g. TapeDescriptor, Event) have
// a number of  different string values that can be set/get.
// Each has it's own GetNAMEBufLen/GetNAMEBuf pair.   That
// can't be handled easily with a template.  Resort
// to a macro.
#define AX_ANY_TO_STRING( AXSTRING_RET, POINTER, GETBUFLEN, GETBUF ) \
{ \
	aafUInt32 _sizeInBytes = 0; \
	CHECK_HRESULT( ((POINTER)->GETBUFLEN)( &_sizeInBytes ) ); \
	int _sizeInChars = (_sizeInBytes /sizeof(aafCharacter)) + 1; \
	std::auto_ptr< aafCharacter > \
		_buf( new aafCharacter[ _sizeInChars ] ); \
    CHECK_HRESULT( ((POINTER)->GETBUF)( _buf.get(), \
			                              _sizeInChars*sizeof(aafCharacter) ) ); \
	AxString tmp( _buf.get() ); \
	AXSTRING_RET = tmp; \
}


#endif
