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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Author : Oliver Morgan (oliver@metaglue.com)
// based on earlier work by Tim Bingham and Bob Tillman.

// moduleName on command line must be Class name of target subclass of DefinitionObject
// e.g. OperationDefinition

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

// this only does DefinitionObject on AAF_INSTANCE lines
// additional properties of subclasses not yet supported

struct inst_t {
  char* cls;
  char* name;
  char* alias;
  char* desc;
  aafUID_t identification;
};

inst_t instances[] =
#include "AAFMetaDictionary.h"

static void doFile (const char * moduleName, const char * prefix)
{
	assert (moduleName);
	printBeginGuard(moduleName, cout);
	printCopyright ("Metaglue Corporation", cout);

	cout << endl;
	cout << "#include \"AAFTypes.h\"" << endl;
	cout << endl;

	cout << "// AAF well-known " << moduleName << " instances" << endl
		 << "//" << endl << endl;

	size_t i = 0;

	for( i=0; instances[i].cls; i++ )
	{
		if( 0==strcmp( instances[i].cls, moduleName ) )
		{
			printDefinition("const aafUID_t",
							prefix,
							instances[i].name,
							0,
							instances[i].identification,
							cout);

			if( false )
			{
				// Name, Desc only wanted for registration code
				printDefinition("const aafCharacter*",
								prefix,
								instances[i].name,
								"_Name",
								instances[i].name,
								cout);

				if( strlen(instances[i].desc) )
				printDefinition("const aafCharacter*",
								prefix,
								instances[i].name,
								"_Desc",
								instances[i].desc,
								cout);
			}

			cout << endl;
		}
	}
 
	cout << "// AAF " << moduleName << " legacy aliases" << endl
		 << "//" << endl << endl;

	for( i=0; instances[i].cls; i++ )
	{
		if( 0==strcmp( instances[i].cls, moduleName ) )
		{
			if( strlen(instances[i].alias) )
					printAlias(	"const aafUID_t",
								instances[i].alias,
								prefix,
								instances[i].name,
								"",
								cout);
		}
	}

  printEndGuard(moduleName, cout);
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
