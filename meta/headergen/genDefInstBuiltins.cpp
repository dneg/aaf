//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
#include <cstring>

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
  const char* cls;
  const char* name;
  const char* alias;
  const char* desc;
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
			if ( 0!=strcmp( instances[i].name, "DataDef_Unknown") ) {
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
