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
//


#include "AxFile.h"
#include "AxEx.h"

#include <iostream>

//=---------------------------------------------------------------------=

bool AxFile::isAAFFile( const AxString& name )
{
	aafBool isA;
	aafUID_t filekind;  // don't care

	CHECK_HRESULT( AAFFileIsAAFFile( name.c_str(), &filekind, &isA ) );

	return isA ? true : false;
}
	
AxFile::AxFile()
{}

AxFile::AxFile( const AxFile& other )
{
	init( other );
}

AxFile::~AxFile()
{}

AxFile&  AxFile::operator=( const AxFile& rhs )
{
	if ( this != &rhs ) {
		init( rhs );
	}

	return *this;
}

void AxFile::OpenNewModify( const AxString& name,
			    aafUInt32 mode,
			    const AxProductIdentification& ident )
{
	IAAFFileSP spIaafFile;

	CHECK_HRESULT( AAFFileOpenNewModify( name.c_str(), mode,
					     const_cast<aafProductIdentification_t*>( ident.getProductId() ),
					     &spIaafFile ) );

    _spIaafFile = spIaafFile;
    _name       = name;
    _mode	= mode;
}

// AAF_RESULT_NOT_IN_CURRENT_VERSION
void AxFile::OpenExistingModify( const AxString& name,
				 aafUInt32 mode,
				 const AxProductIdentification& ident )
{
	IAAFFileSP spIaafFile;

	CHECK_HRESULT( AAFFileOpenExistingModify( name.c_str(), mode,
						  const_cast<aafProductIdentification_t*>( ident.getProductId() ),
						  &spIaafFile ) );

    _spIaafFile = spIaafFile;
    _name       = name;
    _mode	= mode;
}

void AxFile::OpenExistingRead( const AxString& name,
			       aafUInt32 mode )
{
	IAAFFileSP spIaafFile;

	CHECK_HRESULT( AAFFileOpenExistingRead( name.c_str(), mode, &spIaafFile ) );

	_spIaafFile = spIaafFile;
	_name = name;
	_mode = mode;
}


void AxFile::Save()
{
	CHECK_HRESULT( _spIaafFile->Save() );
}

void AxFile::Close()
{
	CHECK_HRESULT( _spIaafFile->Close() );
}

const AxString& AxFile::getName() const
{
	return _name;
}

IAAFHeaderSP AxFile::getHeader() const
{
	IAAFHeaderSP spIaafHeader;

	CHECK_HRESULT( _spIaafFile->GetHeader( &spIaafHeader ) );
	
	return spIaafHeader;
}

std::wostream& AxFile::dump( std::wostream& os ) const
{
	using namespace std;


	os << L"AxFile:" << endl;
	os << L"\t" << _name << endl;
	os << L"\tIncomplete implementation." << endl;

	return os;
}

void AxFile::init( const AxFile& other )
{
	_name = other._name;
	_mode = other._mode;
	_spIaafFile = other._spIaafFile;
}

std::wostream& operator<<( std::wostream& os, AxFile& axFile )
{	
	return axFile.dump( os );
}
