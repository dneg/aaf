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

#include <MultiGenTest.h>

namespace {

class Noop : public MultiGenTest
{ 
public:
  Noop( const char* name,
	const char* desc,
	const char* usage,
	const char* notes )
    : MultiGenTest( name, desc, usage, notes )
  {}

  virtual ~Noop()
  {}

  virtual void RunTest( CmdState& state, int argc, char** argv );
};

void Noop::RunTest( CmdState&, int, char** )
{
}

MULTIGEN_TEST_FACTORY( NoopFactory,
		       Noop,
		       "No Operation",
		       "No Usage",
		       "No Notes" )

// Global static instance of factory.  Ctor will register this
// instance with the MultiGenTestRegistry.
NoopFactory factory;

} // end of namespace
