#ifndef __ImplAAFBaseClassFactory_h__
#define __ImplAAFBaseClassFactory_h__ 1

//!This file should be merged into other files over time

#include "AAFTypes.h"

// Forward declarations.
class ImplAAFObject;


class ImplAAFBaseClassFactory
{
public:
	ImplAAFBaseClassFactory(void);
	~ImplAAFBaseClassFactory(void);

  
  // Utility to find the code class id for the corresponding stored
  // object id.
  static const aafClassID_t* 
    LookupClassID(const aafUID_t* pAUID);
};


#endif /* __ImplAAFBaseClassFactory_h__	*/
