#ifndef MODULE_TEST_COMMON_H
#define MODULE_TEST_COMMON_H

#include "AAF.h"


typedef enum unitTestMode
{
	kAAFUnitTestReadWrite, kAAFUnitTestReadOnly
} testMode_t;
#endif



#ifdef __cplusplus
extern "C" {
#endif



//
// Feature support
//

// Return the current version of the AAF toolkit shared library and
// the version of the toolkit used to create the file.
// NOTE: The parameter fileAAFVersion may be NULL.
HRESULT GetAAFVersions(IAAFHeader * pHeader, 
                       aafProductVersion_t * currentAAFVersion, 
                       aafProductVersion_t * fileAAFVersion);


// Support for Sets (strong reference sets) added in DR4
bool StrongReferenceSetsSupported(aafProductVersion_constref toolkitVersion);

// Support for weak references was added after DR4
bool WeakReferencesSupported(aafProductVersion_constref toolkitVersion);

// Support for weak reference sets was added after DR4
bool WeakReferenceSetsSupported(aafProductVersion_constref toolkitVersion);

// Support for extending AAFObject was added after DR4
bool ExtendingAAFObjectSupported(aafProductVersion_constref toolkitVersion);

#ifdef __cplusplus
}
#endif
