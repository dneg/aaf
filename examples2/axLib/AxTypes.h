#ifndef __AxTypes_h__
#define __AxTypes_h__


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

#include <AAFTypes.h>
#include <AAFCOMPlatformTypes.h>

#include <iosfwd>
#include <memory>
#include <string>

// Could have just used wstring, but the AAF SDK goes to the trouble of 
// typedef'ing aafCharacter, so AxString follows that example.
typedef ::std::basic_string<aafCharacter> AxString;
typedef ::std::char_traits<aafCharacter> AxCharTraits;

class AxProductIdentification 
{
 public:

  AxProductIdentification();

  AxProductIdentification(
    const AxString& companyName,
    const AxString& productName,
    const AxString& productVersionString,
    const AxString& platform,
    const aafUID_t& productID,
    const aafProductVersion_t& productVersion
    );
    
  virtual ~AxProductIdentification();
  
  const aafProductIdentification_t* getProductId() const;

  std::wostream& dump( std::wostream& os ) const;

 private:

  // prohibited ops
  // None - default copy and assignment are okay.

  // private methods
  void init(  const aafUID_t& productID );
    
  // private data

  /* The product ident has pointers to strings stored here. */
  AxString _companyName;
  AxString _productName;
  AxString _productVersionString;
  AxString _platform;

  /* The product ident has a pointer to a product version stored here. */
  aafProductVersion_t _productVersion;

  /* This stores pointers to the private data members above, and
     stores the uuid. */
  aafProductIdentification_t _productId;
};

std::wostream& operator<<( std::wostream& os, const AxProductIdentification& id );
std::wostream& operator<<( std::wostream& os, const aafProductVersion_t& ver );
std::wostream& operator<<( std::wostream& os, const aafUID_t& uid );

bool operator==( const aafUID_t& uidL, const aafUID_t& uidR );
inline bool operator!=( const aafUID_t& uidL, const aafUID_t& uidR )
{
	return !(uidL == uidR);
}

// This has no real meaning, but permits use of aafUID_t
// as the key of a std::map<>
bool operator<( const aafUID_t& uidL, const aafUID_t& uidR );

#if !defined(OS_WINDOWS)
bool operator==( const tagGUID& uidL, const tagGUID& uidR );
inline bool operator!=( const tagGUID& uidL, const tagGUID& uidR )
{
	return !(uidL == uidR);
}
#endif


// Preserve auto_ptr ownership semantics but add a size
// data member so that a buffer pointer and its size can travel
// together.

// FIXME - The auto_ptr data member is declared mutable because SGI's
// compiler will not recognize the copy constructor and assignment
// operator if the const declaration is not used.  This should be
// fixed.  The copy constructor and assignment operator should not
// require a const reference.

template <class T>
class AxBuffer {
public:

        AxBuffer()
        {}

	// Size should be be in units of sizeof(T).
	AxBuffer( std::auto_ptr<T> ptr,  int size )
		: _ptr( ptr ),
		  _size( size )
	{}

	AxBuffer( const AxBuffer& other )
		: _ptr( other._ptr ),
		  _size( other._size )
	{}

	~AxBuffer()
	{}

	AxBuffer& operator=( const AxBuffer& rhs )
	{
		if ( this != &rsh ) {
			_ptr = rhs._ptr;
			_size = rhs._size;
		}

		return *this;
	}

	int GetSize() const
	{
		return _size;
	}

	// Caller takes ownership at their discretion.
	std::auto_ptr<T>& GetPtr()
	{
		return _ptr;
	}

private:
	mutable std::auto_ptr<T> _ptr;
	int _size;
};


#endif


