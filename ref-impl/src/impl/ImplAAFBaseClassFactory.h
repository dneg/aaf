#ifndef _AAF_CONTAINER_REPLACEMENT_
#define _AAF_CONTAINER_REPLACEMENT_ 1

//!This file should be merged into other files over time

#include <stdarg.h>
#include "AAFTypes.h"
#include "ImplAAFContentStorage.h"


class ImplAAFFile;
class OMFile;

#if 1
typedef char    *OMLErrorString;          /* error message string pointers      */
typedef void       *OMLRefCon;               /* reference constants ("refCon"s)    */
typedef void    		*OMLSession;    /* pointer to session (task) data     */
typedef void  *OMLContainer;
typedef const char              *OMLconst_OMLGlobalName;
typedef aafUInt16 OMLContainerUseMode;             /* container open use mode flags      */

/* Flags */
#define kOMLWriting        0x0002U                 /* container was opened for writing   */
#define kOMLReuseFreeSpace 0x0004U                 /* try to reuse freed space           */

typedef struct CMObject_    		*OMLObject;     /* "refNum" for objects               */
typedef aafUInt32  OMLGeneration;                   /* container generation numbers       */

typedef aafUInt16 OMLContainerFlags;               /* container label flags              */

typedef aafInt32   OMLErrorNbr;                     /* error handler error numbers        */
#endif
typedef enum
{
	kOmCreate, kOmModify, kOmOpenRead, kOmUndefined = -1
}				openType_t;

class OMContainer
{
	public:
                OMContainer(void);
                ~OMContainer(void);
		void OMLCloseContainer(void);
		void OMLAbortContainer(void);
		void OMLOpenContainer(aafWChar* stream,
                                         OMLSession sessionData,
                                         OMLRefCon attributes,
                                         OMLconst_OMLGlobalName typeName, 
                                         OMLContainerUseMode useFlags,
										 openType_t type,
                                         ImplAAFHeader*& header);
		void OMLSetContainerVersion1(void);
		void OMLOpenNewContainer(aafWChar* stream,
                                          ImplAAFHeader* head,
                                          OMLSession sessionData,
                                          OMLRefCon attributes,
                                          OMLconst_OMLGlobalName typeName, 
                                          OMLContainerUseMode useFlags,
                                          OMLGeneration generation,
                                          OMLContainerFlags containerFlags, ...);

/***/
void OMLReleaseObject(OMLObject theObject);

  private:
    OMFile* _file;
};


void OMLReleaseObject(OMLObject theObject);

                                         
                                     

void aafOMLError(OMLSession sessionData, OMLErrorString message, ...);

OMLErrorString OMLVGetErrorString(OMLErrorString errorString, aafInt32 maxLength, 
                                             OMLErrorNbr errorNumber, va_list inserts);


#endif /* _AAF_CONTAINER_REPLACEMENT_	*/
