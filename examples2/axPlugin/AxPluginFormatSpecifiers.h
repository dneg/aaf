#ifndef __AxPluginFormatSpecifiers_h__
#define __AxPluginFormatSpecifiers_h__

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

#include "AxPluginUtil.h"

#include <AAFTypes.h>

#include <map>

#include <assert.h>

// Raw untyped format specifier storage.

class AxPluginUntypedFormatSpecifier {
public:

	AxPluginUntypedFormatSpecifier( int size );
	AxPluginUntypedFormatSpecifier( aafUID_t uid, int size, bool readOnly = false );
	virtual ~AxPluginUntypedFormatSpecifier() = 0;

	void SetUID( const aafUID_t& );
	aafUID_t GetUID() const;
	bool IsEqual( const aafUID_t& );

	
	
	// SetValBuf allocates a buffer of "size" bytes suitably aligned for
	// any data type and copies "size" bytes from valBuf to the new buffer.
	// A buffer allocated as a result of a previous call is destoyed.
	// If valBuf is null, an exception is thrown.
	void SetValBuf( const aafUInt8* valBuf, int size );

	// Return a bare pointer to the underlying value buffer, and
	// it's size.  The caller does *not* assume ownership of the
	// pointer. A call to SetValBuf(), or the destructor, invalidates
	// the returned pointer (and size).
	// If SetValBuf() has not been called to set the value, then
	// this call will throw an exception.
	aafUInt8* GetValBuf();

	// Return the size of the underlying buffer.  This value is
	// valid even if no value has been set because it is required
	// by the constructors.
	int GetValBufSize();

	bool IsValSet();

	// This exists simply as a means for a codec to generically test if
	// a specifier is intended to be read only.  Enforcing this is entirely
	// the responsibility of the codec.

	void SetReadOnly( bool readOnly );
	bool IsReadOnly();

protected:

	// Allocate and clear a value buffer of the specified size.
	void SetValZero( int size );

private:

	// prohibited
	AxPluginUntypedFormatSpecifier();
	AxPluginUntypedFormatSpecifier( const AxPluginUntypedFormatSpecifier& );
	AxPluginUntypedFormatSpecifier& operator=( const AxPluginUntypedFormatSpecifier& );

	bool _readOnly;
	aafUID_t _uid;
	aafUInt8* _valBuf;
	int _valBufSize;
};

// Type format specifiers built on top of the raw untyped specifier class.

template <typename T>
class AxPluginFormatSpecifier : public AxPluginUntypedFormatSpecifier {
public:

	typedef T SpecifierType;

	AxPluginFormatSpecifier()
		: AxPluginUntypedFormatSpecifier( sizeof(T) )
	{
		SetValZero( sizeof(T) );
	}

		// Create a format specifier, and create a zero initial value.
	AxPluginFormatSpecifier( aafUID_t uid )
		: AxPluginUntypedFormatSpecifier( uid, sizeof(T) )
	{
		SetValZero( sizeof(T) );
	}

	AxPluginFormatSpecifier( aafUID_t uid, const T& val )
		: AxPluginUntypedFormatSpecifier( uid, sizeof(T) )
	{
		SetVal( val );
	}

	void SetVal( const T& val )
	{
		SetValBuf( &val, sizeof(T) );
	}

	T& GetVal()
	{
		T* pVal = reinterpret_cast<T*>( GetValBuf() );

		return *pVal;
	}

	// Assigned a value of type T directly.
	T& operator=(const T& rhs)
	{
		SetValBuf( reinterpret_cast<const aafUInt8*>(&rhs), sizeof(T) );
		return *this;
	}
	
	// Cast operator permits the object to behave like type T
	// when necessary.
	operator T& ()
	{
		return GetVal();
	}

};

// Format specifier map.  Useful in codec implementations.
// Add a few useful methods to std::map<>;

// This depends on the operator<(const aafUID_t&, const aafUID_t&)
// implementation in AxPluginUtil.h

class AxPluginSpecifierMap
	: public std::map<aafUID_t, AxPluginUntypedFormatSpecifier*> 
{
public:

	typedef std::map<aafUID_t, AxPluginUntypedFormatSpecifier*>::iterator IterType;
	typedef std::pair<aafUID_t, AxPluginUntypedFormatSpecifier*>		  PairType;
	
	// Use this to double check if a uid is in the map before
	// using operator[]().
	bool IsFound( const aafUID_t& uid );

	// Returns largest value buffer size for all specifiers in the map.
	int GetMaxValBufSize();
};

#endif
