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

#include <axFileGen.h>

#include <AxFile.h>

namespace {

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

//=---------------------------------------------------------------------=


} // end of namespace

