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
namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  Alias,           
  L"Alias",
  L"Create a reference that aliases another reference.",
  L"current_name alias_name",
  L"Use \"alias_name\" to refer to \"current_name\".",
  3,
  3 ) 

Alias::~Alias()
{}

void Alias::Execute( const std::vector<AxString>& argv )
{
	AxString name  = argv[1];
	AxString alias = argv[2];

	AxFGOp& op = GetInstance( name );
	op.RegisterInstance( alias );
}


///////////////////////////////////////////////////////////////////////////


} // end of namespace


