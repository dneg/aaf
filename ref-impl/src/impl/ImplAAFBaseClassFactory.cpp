#include <stdio.h>
#include <stdlib.h>
#include "Container.h"
#include "AAFObject.h"
#include "AAFUtils.h"
#include "aafansic.h"
#include "aafCvt.h"

#include "AAFHeader.h"
#include "OMFile.h"
#include "OMClassFactory.h"
#include "AAFIdentification.h"

// Later: These two functions set a global alignment in the
//file for writing properties.  Used only for media data
aafInt32 OMContainer::GetValueAlignment(void)
{
	return(0);
}

void OMContainer::SetValueAlignment(aafInt32 alignment)
{
}

// Later:  
aafInt16 OMContainer::GetDefaultByteOrder(void)
{
#if FULL_TOOLKIT
	return(OMNativeByteOrder);
#else
	return(0);
#endif
}
void OMContainer::SetDefaultByteOrder(aafInt16 byteOrder)
{
}

// Close and save the file
void OMContainer::OMLCloseContainer(void)
{
  if (_mode == writeMode) {
    *_file << *_head;
    _file->close();
  }
}

// Close without saving the file
void OMContainer::OMLAbortContainer(void)
{
}

OMStorable* makeHeader(void)
{
  AAFHeader* newAAFHeader = new AAFHeader;
  OMStorable* retval = dynamic_cast<OMStorable*>
    (newAAFHeader->GetRepObject());
  return retval;
}

OMStorable* makeIdentification(void)
{
  AAFIdentification* newAAFIdentification = new AAFIdentification;
  OMStorable* retval = dynamic_cast<OMStorable*>
    (newAAFIdentification->GetRepObject());
  return retval;
}

// Open a file
void OMContainer::OMLOpenContainer(OMLSession sessionData,
                                 OMLRefCon attributes,
                                 OMLconst_OMLGlobalName typeName, 
                                 OMLContainerUseMode useFlags)
{
	char *pathname;

  _mode = readMode;
	pathname = GetFileName(attributes);
  _file = new OMFile(pathname);
  _file->open();

  OMFile::classFactory()->add(CLSID_AAFHEADER, makeHeader);
  OMFile::classFactory()->add(CLSID_AAFIDENTIFICATION, makeIdentification);

  OMStorable* head = OMStorable::restoreFrom(_file, "head", _file->root());

}

//Will remove this!
void OMContainer::OMLSetContainerVersion1(void)
{
}

//Create a file
void OMContainer::OMLOpenNewContainer(OMLSession sessionData,
                                  OMLRefCon attributes,
                                  OMLconst_OMLGlobalName typeName, 
                                  OMLContainerUseMode useFlags,
                                  OMLGeneration generation,
                                  OMLContainerFlags containerFlags, ...)
{
	char *pathname;

  _mode = writeMode;
	pathname = GetFileName(attributes);
	_file = new OMFile(pathname);
  _file->create();
}

// OML Revision number
aafInt32 OMContainer::GetBentoMajorRevision(void)
{
	return 0; 		//!!!
}

// Submerge
OMLCount32 OMContainer::OMLCountValues(OMLObject object, OMLProperty property, OMLType type)
{
	return(0);
}

// Submerge
OMLValue OMContainer::OMLUseValue(OMLObject object, OMLProperty property, OMLType type)
{
	return(NULL);
}

// Submerge
OMLSize OMContainer::OMLGetValueDataOffset(OMLValue value, OMLCount offset, short *errVal)
{
	aafInt64	tmp;
	
	CvtInt32toInt64(0, &tmp);
	return(tmp);
}

// Submerge
// Size of a property in bytes
OMLSize OMContainer::OMLGetValueSize(OMLValue value)
{
	aafInt64	tmp;
	
	CvtInt32toInt64(0, &tmp);
	return(tmp);
}

// Property read
OMLSize32 OMContainer::OMLReadValueData(OMLValue value, OMLPtr buffer, OMLCount offset, OMLSize32 maxSize)
{
	return(0);
}

// Submerge : Bento needs to create a value for each property
OMLValue OMContainer::OMLNewValue(OMLObject object, OMLProperty property, OMLType type, ...)
{
	return(0);
}

// Property write
void OMContainer::OMLWriteValueData(OMLValue value, OMLPtr buffer, OMLCount offset, OMLSize32 size)
{
}

// Object delete
void OMContainer::OMLDeleteObject(OMLObject theObject)
{
}

// Delete property
void OMContainer::OMLDeleteObjectProperty(OMLObject theObject, OMLProperty theProperty)
{
}

// Follow objref (given an in-file object ref, turn into ptr to object
OMLObject  OMContainer::OMLGetReferencedObject(OMLValue value, OMLReference  theReferenceData)
{
	return(0);
}

// Iterate over properties on an objects
OMLProperty OMContainer::OMLGetNextObjectProperty(OMLObject theObject, OMLProperty currProperty)
{
	return(0);
}

//Submerge
void OMContainer::OMLGetValueInfo(OMLValue value, OMLObject *object, OMLProperty *property, OMLType *type)
{
}

//Creatre a reference
OMLReference  *  OMContainer::OMLNewReference(OMLValue value,
                                                 OMLObject referencedObject,
                                                 OMLReference  theReferenceData)
{
	return(0);
}

//Property delete
void OMContainer::OMLDeleteValueData(OMLValue value, OMLCount offset, OMLSize size)
{
}

// Register a type (by name)  Will need to change to take a AUID
OMLType OMContainer::OMLRegisterType(OMLconst_OMLGlobalName name)
{
	return(0);
}

// Register a property (by name)  Will need to change to take a AUID
OMLProperty OMContainer::OMLRegisterProperty(OMLconst_OMLGlobalName name)
{
	return(0);
}

//??
OMLGlobalName OMContainer::OMLGetGlobalName(OMLObject theObject)
{
	return(0);
}

// DEcrement the reference count
void OMContainer::OMLReleaseObject(OMLObject theObject)
{
}

//Used to iterate over types defined in container, pass in NULL to start
OMLType OMContainer::OMLGetNextType(OMLType currType)
{
	return(0);
}

//Used to iterate over properties defined in container, pass in NULL to start
OMLProperty OMContainer::OMLGetNextProperty(OMLProperty currProperty)
{
	return(0);
}

//Bento uses these as internal per-file-unique object IDs.  I think that only the dumper
//uses these.
aafInt32 OMContainer::GetObjectID(OMLObject obj)
{
	return(0);
}

// Given an internal object ID, return the object.  I don't know WHO would use this.
TOCObjectPtr OMContainer::cmFindObject(const OMLObjectID id)
{
	return(0);
}

// 'Nuff said
void * OMLMalloc(OMLContainer container, OMLSize32 size, OMLSession sessionData)
{
	return(malloc(size));
}

// 'Nuff said
void OMLFree(OMContainer * container, OMLPtr ptr, OMLSession sessionData)
{
	free(ptr);
}

// 'Nuff said
void aafOMLError(OMLSession sessionData, OMLErrorString message, ...)
{
}

// In Bento, you create an object, then WE add a field to say classID
OMLObject OMContainer::OMLNewObject(void)
{
	return(0);
}

// Iterate over values on a property.  Bento allows >1, but never used in 2.0 files.
OMLValue OMContainer::OMLGetNextValue(OMLObject object, OMLProperty property, OMLValue currValue)
{
	return(0);
}

#if FULL_TOOLKIT
// Get the file from an object pointer
OMLContainer OMContainer::OMLGetObjectContainer(AAFObject *theObject)
{
	return(0);
}

// Types and properties in Bento are objects
aafBool OMContainer::OMLIsType(AAFObject *theObject)
{
	return(0);
}
// Types and properties in Bento are objects
aafBool OMContainer::OMLIsProperty(AAFObject *theObject)
{
	return(0);
}

// Iterate over all objects in a file, pass in NULL to start.
AAFObject *OMContainer::OMLGetNextObject(AAFObject *currObject)
{
	return(0);
}
#endif

// May not need to implement.  These functions together tell how property values
// exist in the file (absolute file positions).  A segment (ouch..) is a section of a 
// property which is contiguous.
aafInt32 OMContainer::GetNumSegments(OMLObject	object,
			OMLProperty    	cprop,
			OMLType         	ctype)
{
	return(0);
}

// May not need to implement.  These functions together tell how property values
// exist in the file (absolute file positions).  A segment (ouch..) is a section of a 
// property which is contiguous.
void OMContainer::GetSegmentSizeLen(OMLObject	object,
			OMLProperty    	cprop,
			OMLType         	ctype,
			aafInt32		index,
			aafPosition_t	*startPos,	/* OUT -- where tdoe sthe segment begin, */
			aafLength_t		*length)	/* OUT -- and how long is it? */
{
}

#if FULL_TOOLKIT
void OMContainer::SetHead(const AAFHeader* head)
{
  _head = head;
}
#endif

/*****/
// Called once at program start for your real globals
OMLSession OMLStartSession(OMLMetaHandler metaHandler, OMLRefCon sessionRefCon)
{
	return(0);
}

// Called once at client program exit, to clean up.
void OMLEndSession(OMLSession sessionData, aafBool closeOpenContainers)
{
}

//Bail at end of program, no cleaning up
void OMLAbortSession(OMLSession sessionData)
{
}

// These functions are used to set particular handlers to aafansic.c
OMLHandlerAddr OMLSetMetaHandler(OMLSession sessionData,
                                            OMLconst_OMLGlobalName typeName,
                                            OMLMetaHandler metaHandler)
{
	return(0);
}

// These functions are used to set particular handlers to aafansic.c
MetaHandlerPtr OMLLookupMetaHandler(const unsigned char *typeName,
                                   struct SessionGlobalData *sessionData)
{
	return(0);
}

// 'Nuff said
OMLErrorString OMLVGetErrorString(OMLErrorString errorString, OMLSize32 maxLength, 
                                             OMLErrorNbr errorNumber, va_list inserts)
{
	return(0);
}

void OMContainer::SetHead(const ImplAAFHeader* head)
{
  _head = head;
}

