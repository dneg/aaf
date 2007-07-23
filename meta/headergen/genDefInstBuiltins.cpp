//=---------------------------------------------------------------------=
//
// $Id$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Author : Oliver Morgan (oliver@metaglue.com)
// based on earlier work by Tim Bingham and Bob Tillman.

// moduleName on command line must be Class name of target subclass of DefinitionObject
// e.g. DataDefinition or ContainerDefinition

#include <iostream>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

#define AAF_SYMBOL(symbol, name, alias, desc) \
           #symbol, alias

#define AAF_INSTANCE_TABLE_BEGIN()			\
  {

#define AAF_INSTANCE(cls, name, id, desc)	\
    {#cls, name, desc, id},

#define AAF_INSTANCE_TABLE_END()			\
    {0,0,0,0,{0}}								\
  };

struct inst_t {
  char* cls;
  char* name;
  char* alias;
  char* desc;
  uid identification;
};

inst_t instances[] =
#include "AAFMetaDictionary.h"

static void doFile (const char * moduleName, const char * prefix)
{
	assert (moduleName);
	printCopyright ("Avid Technology", cout);

	cout << endl;
	cout << "// AAF well-known " << moduleName << " instances" << endl;
	cout << "// This file should be #included within ImplAAFDictionary::InitBuiltins()" << endl;
	cout << "// " << endl;
	cout << endl << endl;

	size_t i = 0;

	for( i=0; instances[i].cls; i++ )
	{
		if( 0==strcmp( instances[i].cls, moduleName ) )
		{
			// DataDef_Unknown appears in AAFDataDefs.h but should not be built-in
			if (instances[i].name != "DataDef_Unknown") {
				cout << "Init" << moduleName << " (" << endl;
				cout << "  " << prefix << instances[i].name << ", " << endl;
				cout << "  L\"" << instances[i].name << "\", " << endl;
				cout << "  L\"" << instances[i].desc << "\");" << endl;
				cout << endl;
			}
		}
	}
}


int main (int argc, char ** argv)
{
  char * moduleName = 0;
  char * prefix = 0;
  validateArgs (argc, argv, moduleName, prefix);
  assert (moduleName);
  assert (prefix);
  doFile (moduleName,prefix);
  return 0;
}
