#ifndef __ImplAAFClassFactory_h__
#define __ImplAAFClassFactory_h__ 1

//!This file should be merged into other files over time


// Forward declarations.
class OMFile;


class ImplAAFBaseClassFactory
{
public:
	ImplAAFBaseClassFactory(void);
	~ImplAAFBaseClassFactory(void);

	void RegisterPredefinedClasses(OMFile* file);
};


#endif /* __ImplAAFClassFactory_h__	*/
