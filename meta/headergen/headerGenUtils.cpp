#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "headerGenUtils.h"

void printCopyright (ostream & os)
{
  os <<
"//=---------------------------------------------------------------------=\n"
"//\n"
"// The contents of this file are subject to the AAF SDK Public\n"
"// Source License Agreement (the \"License\"); You may not use this file\n"
"// except in compliance with the License.  The License is available in\n"
"// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF\n"
"// Association or its successor.\n"
"// \n"
"// Software distributed under the License is distributed on an \"AS IS\"\n"
"// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See\n"
"// the License for the specific language governing rights and limitations\n"
"// under the License.\n"
"// \n"
"// The Original Code of this file is Copyright 1998-2001, Licensor of the\n"
"// AAF Association.\n"
"// \n"
"// The Initial Developer of the Original Code of this file and the\n"
"// Licensor of the AAF Association is Avid Technology.\n"
"// All rights reserved.\n"
"//\n"
"//=---------------------------------------------------------------------=\n";
}


static void usage (const char * progname)
{
  assert (progname);

  cerr << "usage: " << progname << "moduleName" << endl;
}


void validateArgs (int argc,
				   char ** argv,
				   char *& moduleName)
{
  const char * progname = argv[0];

  if (argc < 2)
	{
	  usage (progname);
	  exit (1);
	}

  moduleName = argv[1];
}

