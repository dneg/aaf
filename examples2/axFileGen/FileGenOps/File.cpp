//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#include "ParamMaps.h"

#include <axFileGen.h>

#include <AxFile.h>
#include <AxUtil.h>

#include <AAFFileKinds.h>

#include <stdio.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  RemoveFile,
  L"RemoveFile",
  L"Delete a file from the filesystem.",
  L"file_name",
  L"Only works with 8 bit file names.  No error checking is performed.",
  2,
  2 );

RemoveFile::~RemoveFile()
{}

void RemoveFile::Execute( const std::vector<AxString>& args )
{
	::remove( AxStringUtil::wctomb(args[1]).c_str() );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CreateNewModify,
  L"CreateNewModify",
  L"Create a new AAF file.",
  L"file_name",
  L"Referenced by file_name.",
  2,
  2 );

CreateNewModify::~CreateNewModify()
{}

void CreateNewModify::Execute( const std::vector<AxString>& args )
{
	AxFile axFile;
	axFile.OpenNewModify( args[1] );
	
	RegisterInstance( args[1] );
	IAAFFileSP file = axFile;
	SetCOM( file );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  OpenExistingRead,
  L"OpenExistingRead",
  L"Create a new AAF file.",
  L"file_name",
  L"Referenced by file_name.",
  2,
  2 );

OpenExistingRead::~OpenExistingRead()
{}

void OpenExistingRead::Execute( const std::vector<AxString>& args )
{
	AxFile axFile;
	axFile.OpenExistingRead( args[1] );
	
	RegisterInstance( args[1] );
	IAAFFileSP file = axFile;
	SetCOM( file );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SaveFile,
  L"SaveFile",
  L"Save an AAF file.",
  L"file_name",
  L"References file_name.",
  2,
  2 );

SaveFile::~SaveFile()
{}

void SaveFile::Execute( const std::vector<AxString>& args )
{
    AxFGOp& file = GetInstance( args[1] );  
	IAAFFileSP spIaafFile;
	file.GetCOM( spIaafFile );
	AxFile axFile( spIaafFile );
	axFile.Save();
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CloseFile,
  L"CloseFile",
  L"Close an AAF file.",
  L"file_name",
  L"References file_name.",
  2,
  2 );

CloseFile::~CloseFile()
{}

void CloseFile::Execute( const std::vector<AxString>& args )
{
    AxFGOp& file = GetInstance( args[1] );  
	IAAFFileSP spIaafFile;
	file.GetCOM( spIaafFile );
	AxFile axFile( spIaafFile );
	axFile.Close();
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SaveCopyAs,           
  L"SaveCopyAs",
  L"Copy all objects in source file to destination file.",
  L"SrcFile DstFile",
  L"Destination file must be empty.",
  3,
  3 ) 

SaveCopyAs::~SaveCopyAs()
{}

void SaveCopyAs::Execute( const std::vector<AxString>& argv )
{
	AxString srcFileName = argv[1];
	AxString dstFileName = argv[2];

	IAAFFileSP spSrcFile;
	GetInstance( srcFileName ).GetCOM( spSrcFile );
	AxFile axSrcFile( spSrcFile );

	IAAFFileSP spDstFile;
	GetInstance( dstFileName ).GetCOM( spDstFile );

	axSrcFile.SaveCopyAs( spDstFile );	
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SaveAsFile,           
  L"SaveAsFile",
  L"Save an IAAFRandomFile as an ordinary file.",
  L"SrcFile DstFile",
  L"Destination file must be empty. SrcFile assumes the persona of DstFile.",
  3,
  3 ) 

SaveAsFile::~SaveAsFile()
{}

void SaveAsFile::Execute( const std::vector<AxString>& argv )
{
	AxString srcFileName = argv[1];
	AxString dstFileName = argv[2];

	IAAFRandomFileSP spSrcFile;
	GetInstance( srcFileName ).GetCOM( spSrcFile );
	AxRandomFile axSrcFile( spSrcFile );

	IAAFFileSP spDstFile;
	GetInstance( dstFileName ).GetCOM( spDstFile );

	axSrcFile.SaveAsFile( spDstFile );	
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CreateRawStorageMemory,           
  L"CreateRawStorageMemory",
  L"Creates an IAAFRawStorage on memory.",
  L"StorageName file_access",
  L"",
  3,
  3 ) 

CreateRawStorageMemory::~CreateRawStorageMemory()
{}

void CreateRawStorageMemory::Execute( const std::vector<AxString>& argv )
{
	AxString storageName = argv[1];
	AxString accessMode  = argv[2];

	IAAFRawStorageSP spRawStorage;
	CHECK_HRESULT(
		AAFCreateRawStorageMemory( 
			FileAccessParams::GetInstance().Find( *this, accessMode ),
			&spRawStorage )
		);

	SetCOM( spRawStorage );
	RegisterInstance( storageName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CreateAAFFileOnRawStorage,           
  L"CreateAAFFileOnRawStorage",
  L"Create an IAAFile out of an IAAFRawStorage",
  L"FileName StorageName",
  L"existence = new, access = writable, kind = SSBin512, ident = AxProductItentification, mode flags = 0",
  3,
  3 ) 

CreateAAFFileOnRawStorage::~CreateAAFFileOnRawStorage()
{}

void CreateAAFFileOnRawStorage::Execute( const std::vector<AxString>& argv )
{
	AxString fileName    = argv[1];
	AxString storageName = argv[2];

	IAAFRawStorageSP spRawStorage;
	GetInstance( storageName ).GetCOM( spRawStorage );

	AxProductIdentification ident;
	IAAFFileSP spFile;
	CHECK_HRESULT( AAFCreateAAFFileOnRawStorage( spRawStorage,
												 kAAFFileExistence_new,
												 kAAFFileAccess_write,
												 &aafFileKindAaf4KBinary,
												 0,
												 ident.getProductId(),
												 &spFile ) );
	

	CHECK_HRESULT( spFile->Open() );
	
	SetCOM( spFile );
	RegisterInstance( fileName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  OpenTransient,           
  L"OpenTransient",
  L"Create a transient, memory backed, file.",
  L"FileName",
  L"The same thing can be accompolished using CreateRawStorageMemory and CreateAAFFileOnRawStorage.",
  2,
  2 ) 

OpenTransient::~OpenTransient()
{}

void OpenTransient::Execute( const std::vector<AxString>& argv )
{
	AxString name = argv[1];

	AxFile axFile;
	axFile.OpenTransient();

	IAAFFileSP spFile = axFile;
	SetCOM( spFile );
	RegisterInstance( name );
}

} // end of namespace

