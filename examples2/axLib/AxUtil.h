#ifndef __AxUtil_h__
#define __AxUtil_h__

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

#include <AAFTypes.h>
#include <AAFResult.h>

#include <vector>
#include <memory>
#include <utility>

#include <map>
#include <set>

// Useful constants that appear over and over again.
class AxConstants
{
 public:
  static const aafMobID_t NULL_MOBID;
  static const aafUID_t   NULL_UID;
};

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
	
	AxCmdLineArgs( int argc, char** argv );
	~AxCmdLineArgs();

	// Get index of option "opt" at or after argv[n].  True if found.
	std::pair<bool,int> get( const char* opt, int n=0 );

	// Return argv[n].  True if m <= n < argc.
	std::pair<bool,const char*> get( int n, int m=0 );
	
	// Return true if n < m or if n is an argument that has been
	// fetched by one of the get methods.
	bool IsFetched( int n, int m=0 );

	void MarkFetched( int n );	

private:
	int _argc;
	char** _argv;
	std::set<int> _fetchSet;
};

// Useful collection of string processing functions.

class AxStringUtil {

public:

	static AxString bool2Str( bool value );
	static AxString bool2Str( aafBoolean_t value );
	
	static AxString int2StrHex( aafUInt32 n );

	static AxString int2Str( aafInt32 n );

	static AxString uid2Str(const aafUID_t & uid);

	static AxString mobid2Str(const aafMobID_t & mobid);

	static AxString mbtowc( const char* cstr );

	static std::string wctomb( const AxString& s );

	// Converts using base zero - see the strtol man page.
	// This means that s can be decimal, hex, or octal using
	// the usual notations.  Throws an exception if conversion
	// failed.
	static long strtol( const char* s );
	static unsigned long strtoul( const char* s );

	// This will convert to 8 bit chars, then convert.
	static long strtol( const AxString& s );
	static unsigned long strtoul( const AxString& s );
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
	std::vector< aafCharacter > buf( sizeInChars );

	CHECK_HRESULT( spDef->GetName( &buf[0], sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( &buf[0] );

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
	std::vector< aafCharacter > buf( sizeInChars );

	CHECK_HRESULT( sp->GetName( &buf[0], sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( &buf[0] );

	return name;
}

template <class Type>
AxString AxNameToString( IAAFSmartPointer< Type >& sp, const AxString& defaul )
{
	try
	{
		return AxNameToString<Type>(sp);
	}
	catch ( const AxExHResult& ex )
	{
		if ( AAFRESULT_PROP_NOT_PRESENT == ex.getHResult() )
		{
			return defaul;
		}
		else
		{
			throw;
		}
	}
}


template <class Type>
std::pair<bool,AxString> AxExistsNameToString( IAAFSmartPointer< Type >& sp )
{
	std::pair<bool,AxString> result(false,L"");

  try
  {
    result.second = AxNameToString<Type>(sp);
    result.first = true;
  }
  catch ( const AxExHResult& ex )
  {
    if ( AAFRESULT_PROP_NOT_PRESENT != ex.getHResult() )
    {
      throw;
    }
  }

  return result;
}

AxString AxTaggedValueToString( IAAFTaggedValueSP& sp );

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
	std::vector< aafCharacter > buf( sizeInChars );

	CHECK_HRESULT( sp->GetDescription( &buf[0], sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( &buf[0] );

	return name;
}

// Some of the AAF interfaces (e.g. TapeDescriptor, Event) have
// a number of  different string values that can be set/get.
// Each has it's own GetNAMEBufLen/GetNAMEBuf pair.   That
// can't be handled easily with a template.  Resort
// to a macro.

#define AX_ANY_TO_STRING_ONEARG( AXSTRING_RET, POINTER, GETBUFLEN, GETBUF, ARG1 ) \
{ \
	aafUInt32 _sizeInBytes = 0; \
	CHECK_HRESULT( ((POINTER)->GETBUFLEN)( (ARG1), &_sizeInBytes ) ); \
	int _sizeInChars = (_sizeInBytes /sizeof(aafCharacter)) + 1; \
	std::vector< aafCharacter > _buf( _sizeInChars ); \
	CHECK_HRESULT( ((POINTER)->GETBUF)( (ARG1), &_buf[0], _sizeInChars*sizeof(aafCharacter) ) ); \
	AxString tmp( &_buf[0] ); \
	AXSTRING_RET = tmp; \
}

#define AX_ANY_TO_STRING( AXSTRING_RET, POINTER, GETBUFLEN, GETBUF ) \
{ \
	aafUInt32 _sizeInBytes = 0; \
	CHECK_HRESULT( ((POINTER)->GETBUFLEN)( &_sizeInBytes ) ); \
	int _sizeInChars = (_sizeInBytes /sizeof(aafCharacter)) + 1; \
	std::vector< aafCharacter > _buf( _sizeInChars ); \
	CHECK_HRESULT( ((POINTER)->GETBUF)( &_buf[0], _sizeInChars*sizeof(aafCharacter) ) ); \
	AxString tmp( &_buf[0] ); \
	AXSTRING_RET = tmp; \
}

#endif
