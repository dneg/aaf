#ifndef __AxEx_h__
#define __AxEx_h__

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

// Similar to standard C++ "exception" base class, but uses wide characters.
// Not derived from "exception" so that "what()" method can return wide 
// character.  Could be derived from "exception" if desired... with effort.

#include "AxTypes.h"

#include <AAF.h>

#include <wchar.h>

#define CHECK_HRESULT( expr )				       \
{							       \
	HRESULT _hr = (expr);				       \
	if ( !SUCCEEDED( _hr ) ) {			       \
		throw AxExHResult( _hr, __FILE__, __LINE__ );  \
	}						       \
}

class AxEx {

public:

	AxEx();
	AxEx( const wchar_t* what );
	virtual ~AxEx();
	virtual const wchar_t* what() const;

private:

	AxString _what;
};

class AxExHResult : public AxEx {

public:

	AxExHResult();
	AxExHResult( HRESULT hr );
	AxExHResult( HRESULT hr, const wchar_t* what );
	AxExHResult( HRESULT hr, const char* file, int line );
	virtual ~AxExHResult();
	virtual const wchar_t* what() const;

	HRESULT getHResult() const;
	int getLine() const;
	const char* getFile() const;

private:
	const char* _file;
	int _line;
	AxString _what;
	HRESULT _hresult;
};

class AxExSmartPointer : public AxEx {

public:

	AxExSmartPointer();
	virtual ~AxExSmartPointer();
};

class AxExBadImp : public AxEx {

public:

	AxExBadImp( const wchar_t* what );
	virtual ~AxExBadImp();
};

#endif
