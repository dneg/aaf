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

#include "AxPluginFormatSpecifiers.h"

#include <AxTypes.h>

#include <string.h>

#include <memory>
#include <algorithm>

namespace {

class MaxIntFunc {
public:
	MaxIntFunc()
		: _val( 1 << (8*sizeof(int) - 1) )
	{}

	~MaxIntFunc()
	{}

	void operator()( AxPluginSpecifierMap::PairType pair )
	{
		int size = pair.second->GetValBufSize();
		if ( size > _val ) {
			_val = size;
		}
	}

	int GetVal()
	{
		return _val;
	}

private:
	int _val;
};

const aafUID_t NULL_UID;
};

AxPluginUntypedFormatSpecifier::AxPluginUntypedFormatSpecifier( int size )
: _uid( NULL_UID ),
  _valBuf( 0 ),
  _valBufSize( size ),
  _readOnly( false )
{}

AxPluginUntypedFormatSpecifier::AxPluginUntypedFormatSpecifier( aafUID_t uid, int size, bool readOnly )
: _uid( uid ),
  _valBuf( 0 ),
  _valBufSize( size ),
  _readOnly( readOnly )
{}

AxPluginUntypedFormatSpecifier::~AxPluginUntypedFormatSpecifier()
{
	if ( _valBuf ) {
		delete _valBuf;
	}
}

void AxPluginUntypedFormatSpecifier::SetUID( const aafUID_t& uid )
{
	_uid = uid;
}
		
aafUID_t AxPluginUntypedFormatSpecifier::GetUID() const
{
	return _uid;
}

bool AxPluginUntypedFormatSpecifier::IsEqual( const aafUID_t& other )
{
	return other == _uid;
}

void AxPluginUntypedFormatSpecifier::SetValZero( int size )
{
	std::auto_ptr<aafUInt8> buf( new aafUInt8[ size ] );
	memset( buf.get(), 0, size );
	SetValBuf( buf.get(), size );
}

void AxPluginUntypedFormatSpecifier::SetValBuf( const aafUInt8* valBuf, int size )
{
	if ( !valBuf ) {
		// FIXME - require exception
		// throw 
	}
	
	if ( size <= 0 ) {
		// FIXME - require exception
		// throw 
	}

	if ( _valBuf ) {
		delete _valBuf;
	}

	_valBuf = new aafUInt8[ size ];

	memcpy( _valBuf, valBuf, size );

	_valBufSize = size;
}

aafUInt8* AxPluginUntypedFormatSpecifier::GetValBuf()
{
	if ( !_valBuf ) {
		// FIXME - require exception
		// throw 
	}
	
	return _valBuf;
}

int AxPluginUntypedFormatSpecifier::GetValBufSize()
{
	return _valBufSize;
}

bool AxPluginUntypedFormatSpecifier::IsValSet()
{
	return _valBuf ? true : false;
}

void AxPluginUntypedFormatSpecifier::SetReadOnly( bool readOnly )
{
	_readOnly = readOnly;
}

bool AxPluginUntypedFormatSpecifier::IsReadOnly()
{
	return _readOnly;
}

//=---------------------------------------------------------------------=

bool AxPluginSpecifierMap::IsFound( const aafUID_t& uid )
{
	if ( find(uid) == end() ) {
		return false;
	}
	else {
		return true;
	}
}


int AxPluginSpecifierMap::GetMaxValBufSize()
{
	MaxIntFunc max;

	std::for_each( begin(), end(), max );

	return max.GetVal();
}

