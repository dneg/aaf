#ifndef __AxFile_h__
#define __AxFile_h__

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

class AxFile {

 public:

	// Is it an aaf file?
    static bool isAAFFile( const AxString& name );
	 
	AxFile();

	AxFile( IAAFFileSP spIaafFile );
	
	AxFile( const AxFile& other );

	virtual ~AxFile();

	AxFile& operator=( const AxFile& rhs );

	void OpenNewModify( const AxString& name,
						aafUInt32 mode = 0,
						const AxProductIdentification& ident = AxProductIdentification() );

	void OpenExistingModify( const AxString& name,
				 aafUInt32 mode = 0,
				 const AxProductIdentification& ident = AxProductIdentification() );

	void OpenExistingRead( const AxString& name,
						   aafUInt32 mode = 0 );
	
	void Save();

	void Close();

	const AxString& getName() const;

	IAAFHeaderSP getHeader() const;

	operator IAAFFileSP ()
	{ return _spIaafFile; }
	
	std::wostream& dump( std::wostream& os ) const;

 private:
  
	void init( const AxFile& other );
	 
	aafUInt32 _mode;
	AxString _name;

	mutable IAAFFileSP _spIaafFile;
};

std::wostream& operator<<( std::wostream& os, const AxFile& file );

#endif
