#ifndef __ImplAAFBaseClassFactory_h__
#define __ImplAAFBaseClassFactory_h__ 1

//!This file should be merged into other files over time

#include "AAFTypes.h"

// Forward declarations.
class OMFile; // obsolete
class ImplAAFObject;


class ImplAAFBaseClassFactory
{
public:
	ImplAAFBaseClassFactory(void);
	~ImplAAFBaseClassFactory(void);


	// Factory method for making instances of the built-in base classes.
	ImplAAFObject* createObject(const aafUID_t*	auid) const;

};


#endif /* __ImplAAFBaseClassFactory_h__	*/
