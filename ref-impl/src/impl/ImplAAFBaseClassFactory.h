#ifndef _AAF_BENTO_REPLACEMENT_
#define _AAF_BENTO_REPLACEMENT_ 1

#include <stdarg.h>
#include "AAFTypes.h"
#include "OMFile.h"
#include "ImplAAFContentStorage.h"

//#include "CMAPIErr.h"	/* !!! */
//#include "CMAPIIDS.h"	/* !!! */

class ImplAAFFile;

/* Flags */
#define kOMLWriting        0x0002U                 /* container was opened for writing   */
#define kOMLReuseFreeSpace 0x0004U                 /* try to reuse freed space           */

/* The following flags are passed to OMLOpenNewContainer() as OMLContainerFlags.  These   */
/* flags are stored in the container label and define some constant attributes about the*/
/* container.  The container flags may be retrieved by calling the OMLGetContainerInfo() */
/* API routine.                                                                         */

#define kOMLLittleEndian   0x0101U                 /* little-endian byte ordering        */

typedef aafUInt8  OML_UCHAR;	// !!!
typedef aafUInt16  OML_USHORT;	// !!!
typedef aafUInt32  OML_ULONG;	// !!!
typedef char    *OMLErrorString;          /* error message string pointers      */

typedef OML_USHORT OMLEofStatus;                    /* "feof()" handler result status     */
typedef OML_UCHAR  OMLBoolean;                      /* boolean values/functs. (0==>false) */
typedef OML_UCHAR   *OMLMagicBytes;           /* magic byte pointers                */
typedef OML_UCHAR  *OMLDataBuffer;           /* ptr to data buffer for handlers    */
typedef OML_UCHAR  *OMLPrivateData;          /* ptr to private OML data for handlers*/

typedef aafUInt32  OMLObjectID;                     /* object ID's                        */
typedef aafUInt16 OMLContainerUseMode;             /* container open use mode flags      */
typedef struct CMSession_    		*OMLSession;    /* pointer to session (task) data     */
typedef char    *OMLGlobalName;           /* global unique name pointers        */
typedef aafInt64  OMLSize;                         /* sizes                              */
typedef aafUInt32  OMLCount32;
typedef struct CMValue_     *OMLValue;      /* "refNum" for values                */
typedef aafInt64  OMLCount;                        /* amounts or counts                  */
typedef aafUInt32  OMLSize32;
typedef aafUInt8  OMLSeekMode;                     /* container "fseek()" handler modes  */

typedef struct CMObject_    		*OMLObject;     /* "refNum" for objects               */
typedef OMLObject                    OMLProperty;   /* "refNum" for property objects      */
typedef OMLObject                    OMLType;       /* "refNum" for type objects          */
typedef struct CMValue_      		*OMLValue;      /* "refNum" for values                */
typedef aafUInt8  OMLReference[4];                 /* referenced object data pointers    */
typedef const char              *OMLconst_OMLGlobalName;
typedef void       *OMLRefCon;               /* reference constants ("refCon"s)    */
typedef aafUInt32  OMLGeneration;                   /* container generation numbers       */
typedef aafUInt16 OMLContainerFlags;               /* container label flags              */
typedef void       *OMLPtr;                  /* arbitrary data pointers            */

typedef struct Container Container, *ContainerPtr;
typedef const struct CMContainer_  *OMLconst_OMLContainer;
typedef struct CMContainer_  *OMLContainer;
typedef struct TOCObject TOCObject, *TOCObjectPtr;
typedef enum
{
	kSwabIfNeeded, kNeverSwab
}               aafSwabCheck_t;
typedef char    *OMLOpenMode;             /* handler open mode string pointers  */

typedef void (*OMLHandlerAddr)();     /* handler address pointers           */
     
     #define kOMLSeekSet        0x00                    /* "fseek()" handler mode (pos)       */
#define kOMLSeekCurrent    0x01                    /* "fseek()" handler mode (curr+pos)  */
#define kOMLSeekEnd        0x02                    /* "fseek()" handler mode (end+pos)   */
                               
typedef aafInt32   OMLErrorNbr;                     /* error handler error numbers        */


class OMContainer
{
	public:
                OMContainer(void);
                ~OMContainer(void);
		aafInt32 GetValueAlignment(void);
		void SetValueAlignment(aafInt32 alignment);
		aafInt16 GetDefaultByteOrder(void);
		void SetDefaultByteOrder(aafInt16 byteOrder);
		void OMLCloseContainer(void);
		void OMLAbortContainer(void);
		void OMLOpenContainer(OMLSession sessionData,
                                         OMLRefCon attributes,
                                         OMLconst_OMLGlobalName typeName, 
                                         OMLContainerUseMode useFlags,
                                         ImplAAFHeader*& header);
		void OMLSetContainerVersion1(void);
		void OMLOpenNewContainer(ImplAAFHeader* head,
                                          OMLSession sessionData,
                                          OMLRefCon attributes,
                                          OMLconst_OMLGlobalName typeName, 
                                          OMLContainerUseMode useFlags,
                                          OMLGeneration generation,
                                          OMLContainerFlags containerFlags, ...);
		aafInt32 GetBentoMajorRevision(void);
		OMLCount32 OMLCountValues(OMLObject object, OMLProperty property, OMLType type);
		OMLValue OMLUseValue(OMLObject object, OMLProperty property, OMLType type);
		OMLSize OMLGetValueDataOffset(OMLValue value, OMLCount offset, short *errVal);
		OMLSize OMLGetValueSize(OMLValue value);
		OMLSize32 OMLReadValueData(OMLValue value, OMLPtr buffer, OMLCount offset, OMLSize32 maxSize);
		OMLValue OMLNewValue(OMLObject object, OMLProperty property, OMLType type, ...);
		void OMLWriteValueData(OMLValue value, OMLPtr buffer, OMLCount offset, OMLSize32 size);
		void OMLDeleteObject(OMLObject theObject);
		void OMLDeleteObjectProperty(OMLObject theObject, OMLProperty theProperty);
		aafInt32 GetObjectID(OMLObject obj);
		OMLObject  OMLGetReferencedObject(OMLValue value, OMLReference  theReferenceData);

OMLReference  *  OMLNewReference(OMLValue value,
                                                 OMLObject referencedObject,
                                                 OMLReference  theReferenceData);
void OMLDeleteValueData(OMLValue value, OMLCount offset, OMLSize size);
/***/
OMLType OMLRegisterType(OMLconst_OMLGlobalName name);
OMLProperty OMLRegisterProperty(OMLconst_OMLGlobalName name);
OMLGlobalName OMLGetGlobalName(OMLObject theObject);
void OMLReleaseObject(OMLObject theObject);
OMLType OMLGetNextType(OMLType currType);
OMLProperty OMLGetNextProperty(OMLProperty currProperty);
OMLObject OMLNewObject(void);
OMLValue OMLGetNextValue(OMLObject object, OMLProperty property, OMLValue currValue);
		TOCObjectPtr cmFindObject(const OMLObjectID id);
void GetSegmentSizeLen(OMLObject	object,
			OMLProperty    	cprop,
			OMLType         	ctype,
			aafInt32		index,
			aafPosition_t	*startPos,	/* OUT -- where tdoe sthe segment begin, */
			aafLength_t		*length);	/* OUT -- and how long is it? */
aafInt32 GetNumSegments(OMLObject	object,
			OMLProperty    	cprop,
			OMLType         	ctype);
OMLContainer OMLGetObjectContainer(ImplAAFObject *theObject);
ImplAAFObject *OMLGetNextObject(ImplAAFObject *currObject);
aafBool OMLIsType(ImplAAFObject *theObject);
aafBool OMLIsProperty(ImplAAFObject *theObject);
OMLProperty OMLGetNextObjectProperty(OMLObject theObject, OMLProperty currProperty);
void OMLGetValueInfo(OMLValue value, OMLObject *object, OMLProperty *property, OMLType *type);

  private:
    OMFile* _file;
};

#define OMLGetReferenceData(v, o, d) OMLNewReference(v, (OMLObject)o, d)

typedef OMLHandlerAddr (*OMLMetaHandler)(OMLType,             /* metahandlers */
                                                    OMLconst_OMLGlobalName);

typedef OMLType (*GetUpdatingTargetType)(OMLContainer theUpdatingContainer);

OMLSession OMLStartSession(OMLMetaHandler metaHandler, OMLRefCon sessionRefCon);
void OMLEndSession(OMLSession sessionData, aafBool closeOpenContainers);
void OMLAbortSession(OMLSession sessionData);
void OMLReleaseObject(OMLObject theObject);

                                         
                                     
                                     // Bento function takes TOC pointer

OMLType OMLRegisterType(Container * targetContainer, OMLconst_OMLGlobalName name);
OMLProperty OMLRegisterProperty(Container * targetContainer,
                                           OMLconst_OMLGlobalName name);
OMLGlobalName OMLGetGlobalName(OMLObject theObject);
OMLProperty OMLGetNextProperty(Container * targetContainer,
                                          OMLProperty currProperty);
OMLType OMLGetNextType(Container * targetContainer, OMLType currType);

OMLRefCon createRefConForMyHandlers(OMLSession sessionData,
                                                const char  *pathname,
                                                GetUpdatingTargetType getTargetType,
                                                ImplAAFFile *file);

void * OMLMalloc(OMLContainer container, OMLSize32 size, OMLSession sessionData);
void OMLFree(OMContainer * container, OMLPtr ptr, OMLSession sessionData);

OMLHandlerAddr sessionRoutinesMetahandler(OMLType targetType, OMLconst_OMLGlobalName operationType);
OMLHandlerAddr OMLSetMetaHandler(OMLSession sessionData,
                                            OMLconst_OMLGlobalName typeName,
                                            OMLMetaHandler metaHandler);
                                            
OMLHandlerAddr containerMetahandler(OMLType targetType, OMLconst_OMLGlobalName operationType);
void aafOMLError(OMLSession sessionData, OMLErrorString message, ...);

OMLErrorString OMLVGetErrorString(OMLErrorString errorString, OMLSize32 maxLength, 
                                             OMLErrorNbr errorNumber, va_list inserts);

/*** META-HANDLER code.  Used to use the Bento file I/O to do raw I/O also ***/
typedef struct SymbolLinks SymbolLinks, *SymbolLinksPtr;

typedef struct MetaHandler MetaHandler, *MetaHandlerPtr;

typedef struct HandlerOps HandlerOps;

MetaHandlerPtr OMLLookupMetaHandler(const unsigned char *typeName,
                                   struct SessionGlobalData *sessionData);

/*-------------------------------------*
 | Predfined Editing Metahandler Types |
 *-------------------------------------*
 
 These define a type names to be associated with special metahandler packages.  Currently 
 there is only one (!), which defines the type known to the Container Manager, that must
 be associated with target container handlers whenever a target container is opened by an
 updating container.  The target container handlers must be registered as the handler
 package assoicated with this type.
*/

#define CMTargetHandlersTypeName  "Apple:Target_Handlers_TypeName"


/*---------------------------------------------------------------------*
 | Metahandler Operation Types For Container Special Global Operations |
 *---------------------------------------------------------------------*

 These are the defined strings the API passes to a metahandler (passed to CMStartSession())
 to get the handler routine addresses for special global (to all containers) operations.
*/

#define CMErrorOpType           "Apple:ReportError"
#define CMAllocOpType           "Apple:AllocMemory"
#define CMFreeOpType            "Apple:FreeMemory"
#define CMFreeCtrOpType         "Apple:FreeCtrMemory"


/*----------------------------------------------------------*
 | Metahandler Operation Types For Container I/O Operations |
 *----------------------------------------------------------*

 These are the defined strings the API passes to a metahandler to get the handler routine
 addresses for the I/O operations the API needs to do on a container.
*/

#define CMOpenOpType            "Apple:OpenStream"
#define CMCloseOpType           "Apple:CloseStream"
#define CMFlushOpType           "Apple:FlushStream"
#define CMSeekOpType            "Apple:SetPosStream"
#define CMTellOpType            "Apple:GetPosStream"
#define CMReadOpType            "Apple:ReadStream"
#define CMWriteOpType           "Apple:WriteStream"
#define CMEofOpType             "Apple:EofStream"
#define CMTruncOpType           "Apple:TruncateStream"
#define CMSizeOpType            "Apple:SizeStream"
#define CMReadLblOpType         "Apple:ReadLabel"
#define CMWriteLblOpType        "Apple:WriteLabel"
#define CMParentOpType          "Apple:ReturnParentValue"
#define CMContainerOpName       "Apple:ReturnContainerName"
#define CMTargetTypeOpType      "Apple:TargetContainerDynamicValueType"
#define CMExtractDataOpType     "Apple:ExtractData"
#define CMFormatDataOpType      "Apple:FormatData"


/*------------------------------------------------*
 | Metahandler Operation Types For Dynamic Values |
 *------------------------------------------------*
 
 These are the defined strings that CMUseValue() and CMNewValue() pass to a metahandler if
 the type of the value is that of a globally unique name and that name has a "use value"
 matahandler defined for it.  These are used to create dynamic values.
*/

#define CMDefineMetaDataOpType  "Apple:DefineMetaData"
#define CMNewValueOpType        "Apple:NewValue"
#define CMUseValueOpType        "Apple:UseValue"


/*----------------------------------------------------------*
 | Metahandler Operation Types For Dynamic Value Operations |
 *----------------------------------------------------------*

 These are the defined strings the API passes to a dynamic value metahandler (returned
 from a "use value" handler) to get the dynamic value handler addresses for value
 operations.
*/

#define CMGetValueSizeOpType    "Apple:CMGetValueSize"
#define CMReadValueDataOpType   "Apple:CMReadValueData"
#define CMWriteValueDataOpType  "Apple:CMWriteValueData"
#define CMInsertValueDataOpType "Apple:CMInsertValueData"
#define CMDeleteValueDataOpType "Apple:CMDeleteValueData"
#define CMGetValueInfoOpType    "Apple:CMGetValueInfo"
#define CMSetValueTypeOpType    "Apple:CMSetValueType"
#define CMSetValueGenOpType     "Apple:CMSetValueGeneration"
#define CMReleaseValueOpType    "Apple:CMReleaseValue"

#endif /* _AAF_BENTO_REPLACEMENT_	*/
