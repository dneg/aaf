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

#if !defined(OS_WINDOWS)
bool operator==( const tagGUID& uidL, const tagGUID& uidR );
inline bool operator!=( const tagGUID& uidL, const tagGUID& uidR )
{
	return !(uidL == uidR);
}
#endif

// AxAutoPtr - Just like auto_ptr, does not have const related
// problems (using gcc 3.0.3) when using in maps, pairs, etc.
template <class T>
class AxAutoPtr
{
public:
  AxAutoPtr()
  : _p( 0 )
  {}

  explicit AxAutoPtr( T* p )
    : _p(p)
  {}

  ~AxAutoPtr()
  { delete _p; }

  AxAutoPtr( const AxAutoPtr& other )
  {
    AxAutoPtr& ncother = const_cast<AxAutoPtr&>(other);
    _p = ncother.release();
  }

  AxAutoPtr& operator=( const AxAutoPtr& other )
  {
    AxAutoPtr& ncother = const_cast<AxAutoPtr&>(other);
    if ( this != &other ) {
      _p = ncother.release();
    }
	return *this;
  }

  T* operator->() const
  {
	return _p;
  }

  T& operator*() const
  {
	return *_p;
  }

  T* get() const
  { return _p; }

  T* release()
  { T* p = _p; _p = 0; return p; }

private:
  T* _p;
};



#endif