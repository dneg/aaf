#ifndef __headerGenUtils_h__
#define __headerGenUtils_h__

class ostream;

void printCopyright (ostream & os);

// Won't return (will exit(1)) if an error is found.
void validateArgs (int argc,
				   char ** argv,
				   char *& moduleName);


struct replacementSpec_t
{
  const char * from;
  const char * to;
};

// Checks to see if input exactly matches any "from" in the spec
// array; if so, prints the associated "to" to os.  If not, prints the
// input to os.
void replaceAndPrint (const char * input,
					  const replacementSpec_t * spec,
					  ostream & os);


#endif // ! __headerGenUtils_h__
