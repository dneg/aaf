#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "headerGenUtils.h"

void printCopyright (ostream & os)
{
  os <<
	"/***********************************************************************\n"
    " *\n"
    " *              Copyright (c) 1998-2000 Avid Technology, Inc.\n"
    " *\n"
    " * Permission to use, copy and modify this software and accompanying \n"
    " * documentation, and to distribute and sublicense application software\n"
    " * incorporating this software for any purpose is hereby granted, \n"
    " * provided that (i) the above copyright notice and this permission\n"
    " * notice appear in all copies of the software and related documentation,\n"
    " * and (ii) the name Avid Technology, Inc. may not be used in any\n"
    " * advertising or publicity relating to the software without the specific,\n"
    " * prior written permission of Avid Technology, Inc.\n"
    " *\n"
    " * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,\n"
    " * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY\n"
    " * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.\n"
    " * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,\n"
    " * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR\n"
    " * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF\n"
    " * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND\n"
    " * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES\n"
    " * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT\n"
    " * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF\n"
    " * LIABILITY.\n"
    " *\n"
    " ************************************************************************/\n";
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

