#ifndef __headerGenUtils_h__
#define __headerGenUtils_h__

class ostream;

void printCopyright (ostream & os);

// Won't return (will exit(1)) if an error is found.
void validateArgs (int argc,
				   char ** argv,
				   char *& moduleName);


#endif // ! __headerGenUtils_h__
