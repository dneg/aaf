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

#include "MultiGenCommon.h"

#include <MultiGenTest.h>

#include <stdlib.h>
#include <errno.h>

#include <memory>
#include <sstream>

//=---------------------------------------------------------------------=

unsigned long int ToULong( const char* str )
{
  char *endptr = const_cast<char*>(str);
  unsigned long int val = strtoul( str, &endptr, 0 );

  // strtoul() man page documents error determination
  if ( !(str[0] != '\0' && *endptr == '\0') ||
       (val == ULONG_MAX && errno == ERANGE) ) {
    stringstream anError;
    anError << "Unable to convert \"" << str << "\" to an unsigned integer.";
    throw UsageEx( anError.str() );
  }
  
  return val;
}

//=---------------------------------------------------------------------=

wchar_t* ToWideString( const char* str )
{  
  int len = strlen( str );
  wchar_t *wstr = new wchar_t [ len + 1 ];

  int i;
  for( i = 0; i < len; i++ ) {
    mbtowc( &wstr[i], &str[i], 1 );
  }
  wstr[i] = 0;

  return wstr;
}

//=---------------------------------------------------------------------=

// True if identical
bool wstrcmp( const wchar_t* a, const wchar_t* b )
{
  if ( !(a && b) ) {
    return false;
  }

  while( (*a && *b) && ( *a == *b ) ) {
    a++;
    b++;
  }

  if ( *a || *b ) {
    return false;
  }
  else {
    return true;
  }
}


//=---------------------------------------------------------------------=

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      aafSearchCrit_t* criteria,
		      IAAFSmartPointer<IAAFMob>& mob ) 
{
  IAAFSmartPointer<IEnumAAFMobs> mobsEnumerator;
  CHECK_HRESULT( header->GetMobs( criteria, &mobsEnumerator ) );

  IAAFSmartPointer<IAAFMob> nextMob;
  HRESULT hr;

  auto_ptr<wchar_t> wname( ToWideString( name ) );

  for( hr = mobsEnumerator->NextOne( &nextMob );
       hr != AAFRESULT_NO_MORE_OBJECTS;
       hr = mobsEnumerator->NextOne( &nextMob ) ) {

    CHECK_HRESULT( hr );

    aafUInt32 bufSize;

    hr = nextMob->GetNameBufLen( &bufSize );
    if ( hr == AAFRESULT_PROP_NOT_PRESENT ) {
      continue;
    }
    CHECK_HRESULT( hr );

    // Convert bufSize from bytes to wide char;
    bufSize = (bufSize+1)/2;
    auto_ptr<wchar_t> buf( new wchar_t [ bufSize ] );
    CHECK_HRESULT( nextMob->GetName( buf.get(), sizeof(wchar_t)*bufSize ) );

    if ( wstrcmp(buf.get(), wname.get() ) ) {
      mob = nextMob;
      return true;
    }
  }

  if ( hr != AAFRESULT_NO_MORE_OBJECTS ) {
    CHECK_HRESULT( hr );
  }

  return false;
}


//=---------------------------------------------------------------------=

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      IAAFSmartPointer<IAAFMasterMob>& masmobret ) 
{
  aafSearchCrit_t	criteria;
  criteria.searchTag = kAAFByMobKind;
  criteria.tags.mobKind = kAAFMasterMob;

  IAAFSmartPointer<IAAFMob> mob; 
  if ( get_mob_by_name( header, name, &criteria, mob ) ) {
    // This cast should work, an exception here would indicate a
    // serious problem.
    IAAFSmartPointer<IAAFMasterMob> masmob; 
    CHECK_HRESULT( mob->QueryInterface( IID_IAAFMasterMob,
					ToVoid( &masmob ) ) );
    masmobret = masmob;
    return true;
  }
  else {
    return false;
  }
}

//=---------------------------------------------------------------------=

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      IAAFSmartPointer<IAAFMob>& mob ) 
{
  if ( get_mob_by_name( header, name, 0, mob ) ) {
    return true;
  }
  else {
    return false;
  }
}

//=---------------------------------------------------------------------=

bool find_comment( IAAFSmartPointer<IAAFMob> mob,
		   const wchar_t* category,
		   const wchar_t* comment,
		   IAAFSmartPointer<IAAFTaggedValue>& taggedValue )
{
  IAAFSmartPointer<IEnumAAFTaggedValues> enumerator;

  CHECK_HRESULT( mob->GetComments( &enumerator ) );

  HRESULT hr;
  IAAFSmartPointer<IAAFTaggedValue> next;

  for( hr = enumerator->NextOne( &next );
       hr != AAFRESULT_NO_MORE_OBJECTS;
       hr = enumerator->NextOne( &next ) ) {

    CHECK_HRESULT( hr );

    aafUInt32 bufSize;
    CHECK_HRESULT( next->GetNameBufLen( &bufSize ) );
    // Convert bufSize from bytes to wide char;
    bufSize = (bufSize+1)/2;
    auto_ptr<wchar_t> buf( new wchar_t [ bufSize ] );
    CHECK_HRESULT( next->GetName( buf.get(), sizeof(wchar_t)*bufSize ) );

    if ( !wstrcmp( buf.get(), category ) ) {
      continue;
    }

    aafUInt32 valueSize;
    CHECK_HRESULT( next->GetValueBufLen( &valueSize ) );
    valueSize = (valueSize+1)/2;
    auto_ptr<wchar_t> valBuf( new wchar_t [ valueSize ] );
    aafUInt32 actualBytesRead;
    CHECK_HRESULT( next->GetValue( sizeof(wchar_t)*valueSize,
		   reinterpret_cast<aafDataBuffer_t>(valBuf.get()),
		   &actualBytesRead ) );

    if ( wstrcmp( valBuf.get(), comment ) ) {
      taggedValue = next;
      return true;
    }
    else {
      return false;
    }
  }

  if ( hr != AAFRESULT_NO_MORE_OBJECTS ) {
    CHECK_HRESULT( hr );
  }

  return false;
}

//=---------------------------------------------------------------------=

//=---------------------------------------------------------------------=

//=---------------------------------------------------------------------=


