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

#include "AxEx.h"
#include "AxUtil.h"
#include "AxHrMap.h"

#include <wchar.h>

#include <sstream>

namespace {

AxString hresult( L"HRESULT" );
AxString spc( L" " );
AxString zerox( L"0x" );
AxString colon( L":" );

};

//=---------------------------------------------------------------------=

AxEx::AxEx() throw()
: _what()
{}

AxEx::AxEx( const wchar_t* what )
: _what( what )
{}

AxEx::~AxEx()
{}
	
const wchar_t* AxEx::what() const
{
	return _what.c_str();
}

//=---------------------------------------------------------------------=

AxExHResult::AxExHResult()
: AxEx(),
  _what(0),
  _hresult( 0 )
{}

AxExHResult::AxExHResult( HRESULT hr )
: AxEx(),
  _what( L"" ),
  _hresult( hr )
{}

AxExHResult::AxExHResult( HRESULT hr, const wchar_t* what )
: AxEx(),
  _what( ),
  _hresult( hr )
{
	std::wostringstream os;

	os << hresult << spc;
	os << zerox << AxStringUtil::int2StrHex( hr );
	os << spc << AxHrMap::getInstance().getStr( hr ) << spc;
	os << AxString( what );

	_what = os.str();
}

AxExHResult::AxExHResult( HRESULT hr, const char* file, int line )
: AxEx(),
  _file( file ),
  _line( line ),	
  _hresult( hr )
{
	std::wostringstream os;

	os << hresult << spc;
	os << AxStringUtil::mbtowc( file );
	os << colon << line << spc;
	os << zerox << AxStringUtil::int2StrHex( hr );
	os << spc << AxHrMap::getInstance().getStr( hr );

	_what = os.str();
}

AxExHResult::~AxExHResult()
{}

const wchar_t* AxExHResult::what() const
{
	return _what.c_str();
}

HRESULT AxExHResult::getHResult() const
{
	return _hresult;
}

int AxExHResult::getLine() const
{
	return _line;
}

const char* AxExHResult::getFile() const
{
	return _file;
}


//=---------------------------------------------------------------------=

AxExSmartPointer::AxExSmartPointer()
:	AxEx( L"AAFSmartPointer assertion failed" )
{}

AxExSmartPointer::~AxExSmartPointer()
{}

//=---------------------------------------------------------------------=

AxExBadImp::AxExBadImp( const wchar_t* what )
:	AxEx( what )
{}

AxExBadImp::~AxExBadImp()
{}
