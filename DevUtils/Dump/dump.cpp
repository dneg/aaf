//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
//
// Low-level AAF file dumper.
//

// Tim Bingham 05-May-1998 Tim_Bingham@avid.com
//             19-June-1998
//             12-August-1998
//                April-2000
//
// Tom Ransdell 28-Sept-1998 Tom_Ransdell@avid.com
//              Adapt to building on the Macintosh with CodeWarrior Pro3
//              24-Jan-2000
//              Added new conditional macro IOS_FMT_FLAGS for CodeWarrior Pro5.
//
// Terry Skotz 4-27-2000 Terry_Skotz@avid.com
//             added support for DataInput.h so dump can get input from text
//             file.

//
// Usage:
//
//  $ dump [-x -r -p -a -c -s -z <pid> -m <n> -l <n> -v -h] files...
//
//    -x       = hex dump, works for any file.
//    -r       = raw dump, works for any structured storage file.
//    -p       = property dump, works for files using the AAF stored
//                 object format.
//    -a       = AAF file dump, works only for AAF files.
//    -c       = print raw reference counts
//    -s       = print statistics.
//    -z <pid> = dump properties with pid <pid> (hex) as all zeroes.
//    -m <n>   = dump only the first <n> bytes (dec) of media streams. 
//    -l <n>   = dump only the first <n> bytes (dec) of the file.
//    -v       = validate the structure of the file
//    -h       = print help.
//
//  Notes:
//
//    1) -x, -r, -p and -a are mutually exclusive.
//    2) -s is valid with -r, -p and -a. When combined with either -p or -a
//       statistics on objects and properties are displayed, when combined
//       with -r statistics on IStorages, IStreams and bytes are displayed.
//    3) -z is not valid with -r. Multiple -z flags may be supplied.
//    4) <pid> must be specified in hex. Examples are 1a00 0x1a00 0x1A00 
//    5) -m is not valid with -r.
//    6) -v is only valid with -p or -a, it is not valid with -x or -r
//

#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <string.h>

// Determine host operating system.
//
#if defined(_WIN32)
#define OM_OS_WINDOWS
#elif defined(_MAC) || defined(macintosh)
#define OM_OS_MACOS
#elif defined(__sgi) || defined(__linux__) || defined (__FreeBSD__) || \
      defined (__APPLE__) || defined(__CYGWIN__)
#define OM_OS_UNIX
#else
#error "Can't determine host operating system"
#endif

// Determine which implementation of structured storage to use.
//
#if defined(OM_OS_WINDOWS)
#define OM_WINDOWS_SS
#elif defined(OM_OS_MACOS)
#define OM_MACINTOSH_SS
#elif defined(OM_OS_UNIX)
#define OM_REFERENCE_SS
#else
#error "Don't know which implementation of structured storage to use."
#endif

// Include the structured storage headers. These are different
// depending on the implementation.
//
#if defined(OM_WINDOWS_SS)
#include <objbase.h>
#elif defined(OM_MACINTOSH_SS)
#include "wintypes.h"
#include "macdef.h"
#include "macpub.h"
#include "compobj.h"
#include "storage.h"
#include "initguid.h"
#include "coguid.h"
#elif defined(OM_REFERENCE_SS)
#include "h/storage.h"
#endif

// Determine whether or not UNICODE versions of the APIs are in use.
//
#if defined(OM_OS_WINDOWS) && defined(UNICODE)
#define OM_UNICODE_APIS
#endif

// OMCHAR is used for all character arguments to functions whose
// prototype changes when UNICODE is defined.
//
#if defined(OM_UNICODE_APIS)
typedef wchar_t OMCHAR;
#else
typedef char OMCHAR;
#endif

// Console input for Macintosh (two different methods).
//
#if defined(OM_OS_MACOS)
#if !defined(USE_DATAINPUT)
#include <console.h>
#else
#include "DataInput.h"
#endif
#endif

// TRR:2000-JAN-24: Added the following conditional code support 
// CodeWarrior Pro 5 MSL changes.
#if defined(__MWERKS__)
#if defined(__MSL_CPP__) && (__MSL_CPP__ >= 0x5300)
#define IOS_FMT_FLAGS ios_base::fmtflags
#else
#define IOS_FMT_FLAGS long int
#endif
#else
#define IOS_FMT_FLAGS long int
#endif

// Stored forms
//
const int SF_DATA                                   = 0x82;
const int SF_DATA_STREAM                            = 0x42;
const int SF_STRONG_OBJECT_REFERENCE                = 0x22;
const int SF_STRONG_OBJECT_REFERENCE_VECTOR         = 0x32;
const int SF_STRONG_OBJECT_REFERENCE_SET            = 0x3A;
const int SF_WEAK_OBJECT_REFERENCE                  = 0x02;
const int SF_WEAK_OBJECT_REFERENCE_VECTOR           = 0x12;
const int SF_WEAK_OBJECT_REFERENCE_SET              = 0x1A;
const int SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID = 0x03;
const int SF_UNIQUE_OBJECT_ID                       = 0x86;
const int SF_OPAQUE_STREAM                          = 0x40;

// Integral types
//
typedef signed char        OMInt8;
typedef signed short int   OMInt16;
typedef signed long int    OMInt32;

typedef unsigned char      OMUInt8;
typedef unsigned short int OMUInt16;
typedef unsigned long int  OMUInt32;

typedef OMUInt8 OMByte;
typedef OMUInt16 OMCharacter;
typedef OMUInt16 OMPropertyId;

typedef struct {
  OMUInt8 SMPTELabel[12];
  OMUInt8 length;
  OMUInt8 instanceHigh;
  OMUInt8 instanceMid;
  OMUInt8 instanceLow;
  CLSID material;
} UMID;

// Structure of property index header
//
typedef struct {
  OMUInt16 _byteOrder;
  OMUInt32 _formatVersion;
  OMUInt32 _entryCount;
} IndexHeader;

// Structure of a property index entry
//
typedef struct {
  OMUInt32 _pid;
  OMUInt32 _type;
  OMUInt32 _offset;
  OMUInt32 _length;
} IndexEntry;

// Structure of a vector index entry
//
typedef struct {
  OMUInt32 _elementName;
} VectorIndexEntry;

// Structure of a set index entry
//
typedef struct {
  OMUInt32 _elementName;
  OMUInt32 _referenceCount;
  CLSID _key;
} SetIndexEntry;

// Structure of a weak collection index entry
//
typedef struct {
  CLSID _key;
} WeakCollectionIndexEntry;

// Byte ordering
//
typedef OMUInt16 ByteOrder;
const ByteOrder unspecifiedEndian = 0;
const ByteOrder littleEndian      = 0x4949;
const ByteOrder bigEndian         = 0x4d4d;

const CLSID nullCLSID = {0,0,0, {0,0,0,0,0,0,0,0}};
const UMID nullUMID   = {{0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0, 0, 0,
                         {0,0,0, {0,0,0,0,0,0,0,0}}};

// Stream names and punctuation
//
const char* const propertiesStreamName = "properties";
const char* const openArrayKeySymbol = "{";
const char* const closeArrayKeySymbol = "}";

// The file kind for structured storage binary AAF files.
//
const CLSID aafFileKindAafSSBinary = 
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// The file kind for structured storage binary MXF files.
//
const CLSID aafFileKindMxfSSBinary = 
{0x4246584d, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// The file kind for XML text AAF files.
//
const CLSID aafFileKindAafXmlText = 
{0x58464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// The file kind for XML text MXF files.
//
const CLSID aafFileKindMxfXmlText = 
{0x5846584d, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// The signature for structured storage binary AAF files. This includes
// the structured storage file signature.
//
unsigned char aafFileSignatureAafSSBinary[] = {
  0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1,
  0x41, 0x41, 0x46, 0x42, 0x0d, 0x00, 0x4f, 0x4d,
  0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff
};

// The signature for structured storage binary MXF files. This includes
// the structured storage file signature.
//
unsigned char aafFileSignatureMxfSSBinary[] = {
  0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1,
  0x4d, 0x58, 0x46, 0x42, 0x0d, 0x00, 0x4f, 0x4d,
  0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff
};

// The signature for XML text AAF files.
//
unsigned char aafFileSignatureAafXmlText[] = {
  0x3c, 0x3f, 0x78, 0x6d, 0x6c, 0x20, 0x76, 0x65,
  0x72, 0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x22, 0x31,
  0x2e, 0x30, 0x22, 0x3f, 0x3e, 0x3c, 0x3f, 0x41,
  0x41, 0x46, 0x20, 0x73, 0x69, 0x67, 0x6e, 0x61,
  0x74, 0x75, 0x72, 0x65, 0x3d, 0x22, 0x7b, 0x35,
  0x38, 0x34, 0x36, 0x34, 0x31, 0x34, 0x31, 0x2d,
  0x46, 0x46, 0x30, 0x44, 0x2d, 0x34, 0x44, 0x34,
  0x46, 0x2d, 0x30, 0x36, 0x30, 0x45, 0x2d, 0x32,
  0x42, 0x33, 0x34, 0x30, 0x31, 0x30, 0x31, 0x30,
  0x31, 0x30, 0x30, 0x7d, 0x22, 0x3f, 0x3e
};

// The signature for XML text MXF files.
//
unsigned char aafFileSignatureMxfXmlText[] = {
  0x3c, 0x3f, 0x78, 0x6d, 0x6c, 0x20, 0x76, 0x65,
  0x72, 0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x22, 0x31,
  0x2e, 0x30, 0x22, 0x3f, 0x3e, 0x3c, 0x3f, 0x41,
  0x41, 0x46, 0x20, 0x73, 0x69, 0x67, 0x6e, 0x61,
  0x74, 0x75, 0x72, 0x65, 0x3d, 0x22, 0x7b, 0x35,
  0x38, 0x34, 0x36, 0x35, 0x38, 0x34, 0x44, 0x2d,
  0x46, 0x46, 0x30, 0x44, 0x2d, 0x34, 0x44, 0x34,
  0x46, 0x2d, 0x30, 0x36, 0x30, 0x45, 0x2d, 0x32,
  0x42, 0x33, 0x34, 0x30, 0x31, 0x30, 0x31, 0x30,
  0x31, 0x30, 0x30, 0x7d, 0x22, 0x3f, 0x3e
};

// Table of valid signatures. Signatures are found at the beginning of
// the file and are variable in size. There are multiple signatures
// since there are multiple external representations (or "types") of
// AAF file. e.g. "structured storage binary" and "XML text".
//
struct format {
  unsigned char* signature;
  size_t signatureSize;
  // Could add the offset of the signature here
  const GUID* fileKind;
} formatTable[] = {
  {aafFileSignatureAafSSBinary,
   sizeof(aafFileSignatureAafSSBinary),
   &aafFileKindAafSSBinary},
  {aafFileSignatureMxfSSBinary,
   sizeof(aafFileSignatureMxfSSBinary),
   &aafFileKindMxfSSBinary},
  {aafFileSignatureAafXmlText,
   sizeof(aafFileSignatureAafXmlText),
   &aafFileKindAafXmlText},
  {aafFileSignatureMxfXmlText,
   sizeof(aafFileSignatureMxfXmlText),
   &aafFileKindMxfXmlText},
};

static size_t signatureSize(void);
static size_t maxSignatureSize = signatureSize();


// A note on file format versions.
//
// History
//
// Version : Changes
//
//  0.01   : initial version
//  0.02   : add byte order flag to the property index, change stream names
//  0.03   : change property ids to be unique within a file (this
//           includes using automatically generated PIDs and changes to
//           property names)
//  0.04   : change to the use of SMPTE conformant stored class ids
//  0.05   : change the value of many pids because of the addition of the
//           new plugin related classes
//  0.06   : change property names to match those in the dictionary
//  0.07   : change AAFSourceMob::MediaDescription to
//           AAFSourceMob::EssenceDescription.
//  0.08   : remove AAFPluggableDef. Renumbered TID_* values.
//  0.09   : Effect -> OperationGroup, EffectDefinition -> OperationDefinition,
//           PlugInDescriptors -> PluginDescriptors.
//  0.10   : remove OperationDefinition::PluginLocator and
//           OperationDefinition::ManufacturerID,
//           add TypeDefinitionExtendibleEnumeration.
//  0.11   : remove classes AIFCData, HTMLData, ImageData, JPEGImageData,
//           MIDIFileData, TIFFData and WAVEData. Move FrameIndex property
//           to EssenceData and rename to SampleIndex. Change type of
//           DefinitionObject::PluginDescriptors from ObjRefArray to
//           RefAUIDArray.
//  0.12   : TIFFDescriptor now descended from FileDescriptor instead of
//           DigitalImageDescriptor. Several other minor changes.
//  0.13   : Change PropertDefinition::pid to
//           PropertyDefinition::LocalIdentification.
//  0.14   : Implement all types, except for Stream, as defined in
//           AAFMetaDictionary.h .  Previously most types were aliases
//           to a more basic type (aafUIntXX for enums,
//           InterchangeObject as target for all object references,
//           and variable-array-of-uint8s for all others).  Now all
//           types (except Stream) are implemented as defined in the
//           MetaDictionary.  Note that *some* of the built-in defs
//           (class and type) are *not* entered into the dictionary,
//           so these files are not entirely self-describing.  This
//           was done as a work-around, and should change in the
//           future.  Stay tuned for a new version when this is
//           fixed.
//  0.15   : Change AUIDs for PullDownKindType, StringArray and
//           PositionArray since these were not unique !
//  0.16   : Version 0.15 used in DR1. Bump to 0.16 to continue
//           development.
//  0.17   : changed format of aafTimeStamp_t struct to reflect
//           reality.
//  0.18   : Changed PluginDescriptor::Locators to be a
//           StrongReferenceVector (was a StrongReferenceSet).
//  0.19   : Introduced singleton weak references, added
//           "referenced properties".
//  0.20   : Set of objects with unique identifiers other than GUIDs.
//           Add keyPid and keySize to set index header.
//  0.21   : Combine "property index" and "property values" streams
//           into a single "properties" stream.
//  0.22   : Put the count field first (instead of the high water mark)
//           in strong reference vector and set indexes.
//  0.23   : Remove high water mark from strong reference vector and
//           strong reference set, replace with first free key and last
//           free key.
//  0.24   : Squeeze fields in the Object Manager meta-data as follows
//
//           OMPropertyId    32 -> 16
//           OMPropertySize  32 -> 16
//           OMPropertyCount 32 -> 16
//           OMStoredForm    32 -> 16
//           OMVersion       32 ->  8
//           OMKeySize       32 ->  8
//           OMByteOrder     16 ->  8
//           OMPropertyTag   32 -> 16
//
//           Remove the offset filed field from stored property set index
//           entries.
//  0.25   : Change stored froms to use bit values.
//  0.26   : Use 2 byte characters for names in Object Manager meta-data
//  0.27   : Use a string of pids instead of a string of 2-byte characters
//           for entries in the referenced properties table.
//  0.28   : Add stored byte order for data streams (1 byte preceeding
//           stream name).
//  0.32   : RC1
//

// The following may change at run time depending on the file format
// version.
//
char* _propertyValueStreamName = (char*)propertiesStreamName;
char* _propertyIndexStreamName = (char*)propertiesStreamName;
char* _openArrayKeySymbol = (char*)openArrayKeySymbol;
char* _closeArrayKeySymbol = (char*)closeArrayKeySymbol;

// Old values for stored forms
//

// version 0.08 and above
const int TID_DATA                                   =  0;
const int TID_DATA_STREAM                            =  1;
const int TID_STRONG_OBJECT_REFERENCE                =  2;
const int TID_STRONG_OBJECT_REFERENCE_VECTOR         =  3;
const int TID_STRONG_OBJECT_REFERENCE_SET            =  4;
const int TID_WEAK_OBJECT_REFERENCE                  =  5;
const int TID_WEAK_OBJECT_REFERENCE_VECTOR           =  6;
const int TID_WEAK_OBJECT_REFERENCE_SET              =  7;
const int TID_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID =  8;
const int TID_UNIQUE_OBJECT_ID                       =  9;
const int TID_OPAQUE_STREAM                          = 10;

// version 0.07 and below
const int OLD_TID_DATA                           = 0;
const int OLD_TID_STRONG_OBJECT_REFERENCE        = 1;
const int OLD_TID_STRONG_OBJECT_REFERENCE_VECTOR = 2;
const int OLD_TID_WEAK_OBJECT_REFERENCE          = 3;
const int OLD_TID_WEAK_OBJECT_REFERENCE_VECTOR   = 5;
const int OLD_TID_DATA_STREAM                    = 4;

// Highest version of file/index format recognized by this dumper
//
const OMUInt32 HIGHVERSION = 32;

// Output format requested
//
enum optionType {hexadecimal, raw, property, aaf};
enum optionType option = raw; // default
bool cFlag = false;
bool zFlag = false;
bool mFlag = false;
bool lFlag = false;
bool vFlag = false;
unsigned long int mLimit = 0;
unsigned long int lLimit = 0;

// Statistics gathering
//
size_t totalStorages;
size_t totalStreams;

size_t totalPropertyBytes;
size_t totalObjects;
size_t totalProperties;

size_t totalStreamBytes;
size_t totalFileBytes;

// Validity checking
//
size_t warningCount = 0;
size_t errorCount = 0;

// Prototypes for local functions.
//
static char* baseName(char* fullName);
static void reportAssertionFailure(char* name,
                                   char* expressionString,
                                   char* fileName,
                                   size_t lineNumber);
static void checkSizes(void);
static ByteOrder hostByteOrder(void);
static const char* byteOrder(ByteOrder bo);
static void formatError(DWORD errorCode);
static void fatalError(char* routineName, char* message);
static void error(char* routineName, char* message);
static void warning(char* routineName, char* message);
static void printError(const char* prefix,
                       const char* fileName,
                       DWORD errorCode);
static int check(const char* fileName, DWORD resultCode);
static int checks(DWORD resultCode);
#if defined(OM_UNICODE_APIS)
static void convert(wchar_t* wcName, size_t length, const char* name);
static void convert(char* cName, size_t length, const wchar_t* name);
#else
static void convert(char* cName, size_t length, const OMCharacter* name);
#endif
static void convert(char* cName, size_t length, const char* name);
static void convertName(char* cName,
                        size_t length,
                        OMCHAR* wideName,
                        char** tag);
static void indent(int level);
static void getClass(IStorage* storage, CLSID* clsid, const char* fileName);
static void printClsid(const CLSID& clsid);
static void printRawKey(OMByte* key, size_t keySize);
static void printUMID(const UMID& umid);
static void openStream(IStorage* storage,
                       const char* streamName,
                       IStream** stream);
static HRESULT openStreamTry(IStorage* storage,
                             const char* streamName,
                             IStream** stream);
static size_t sizeOfStream(IStream* stream, const char* streamName);
static void printStat(STATSTG* statstg, char* tag);
static void dumpStream(IStream* stream, STATSTG* statstg, char* pathName);
static void dumpStorage(IStorage* storage,
                        STATSTG* statstg,
                        char* pathName,
                        int isRoot);
static void read(IStream* stream, void* address, size_t size);
static void read(IStream* stream, size_t offset, void* address, size_t size);
static void readUInt8(IStream* stream, OMUInt8* value);
static void readUInt16(IStream* stream, OMUInt16* value, bool swapNeeded);
static void readUInt32(IStream* stream, OMUInt32* value, bool swapNeeded);
static void swapUInt16(OMUInt16* value);
static void swapUInt32(OMUInt32* value);
static void readOMString(IStream* stream,
                         OMCharacter* string,
                         size_t characterCount,
                         bool swapNeeded);
static void printOMString(const OMCharacter* string);
static void swapOMString(OMCharacter* string,
                         size_t characterCount);
static void readPidString(IStream* stream,
                          OMPropertyId* string,
                          size_t pidCount,
                          bool swapNeeded);
static void printPidString(const OMPropertyId* string);
static void swapPidString(OMPropertyId* string,
                          size_t pidCount);
static void readCLSID(IStream* stream, CLSID* value, bool swapNeeded);
static void swapCLSID(CLSID* value);
static void dumpIndexEntry(OMUInt32 i, IndexEntry* indexEntry);
static void printIndex(IndexEntry* index, OMUInt32 entries);
static void readIndexEntry(IStream* stream,
                           IndexEntry* entry,
                           bool swapNeeded,
                           OMUInt32 version);
static IndexEntry* readIndex(IStream* stream,
                             OMUInt32 count,
                             bool swapNeeded,
                             OMUInt32 version);
void reportBadIndex(char* pathName,
                    OMUInt16 order,
                    OMUInt32 version,
                    OMUInt32 entryCount,
                    OMUInt32 expectedSize,
                    OMUInt32 actualSize);
static void reportBadIndexEntry(OMUInt32 i,
                                const IndexEntry* entry);
static bool isValid(const IndexEntry* index, const OMUInt32 entries);
static size_t valueStreamSize(const IndexEntry* index, const OMUInt32 entries);
static char* typeName(OMUInt32 type);
static void openStorage(IStorage* parentStorage,
                        char* storageName,
                        IStorage** subStorage);
static void dumpVectorIndexEntry(OMUInt32 i,
                                 VectorIndexEntry* vectorIndexEntry);
static void printVectorIndex(VectorIndexEntry* vectorIndex,
                             OMUInt32 count,
                             OMUInt32 highWaterMark,
                             OMUInt32 lowWaterMark,
                             OMUInt32 version);
static void readVectorIndexEntry(IStream* stream,
                           VectorIndexEntry* entry,
                           bool swapNeeded);
static VectorIndexEntry* readVectorIndex(IStream* stream,
                                         OMUInt32 count,
                                         bool swapNeeded);
static void dumpSetIndexEntry(OMUInt32 i,
                              SetIndexEntry* setIndexEntry,
                              bool printKey);
static void printReferenceCount(OMUInt32 referenceCount);
static void printSetIndex(SetIndexEntry* setIndex,
                          OMUInt32 count,
                          OMUInt32 highWaterMark,
                          OMUInt32 lowWaterMark,
                          OMUInt32 keyPid,
                          OMUInt32 keySize,
                          OMUInt32 version);
static void printSetIndex(SetIndexEntry* setIndex,
                          OMUInt32 count,
                          OMUInt32 highWaterMark,
                          OMUInt32 lowWaterMark,
                          OMUInt32 keyPid,
                          OMUInt32 keySize,
                          OMByte* keys,
                          OMUInt32 version);
static void printWeakCollectionIndex(int containerType,
                                     WeakCollectionIndexEntry* collectionIndex,
                                     OMUInt32 count,
                                     OMUInt32 tag,
                                     OMUInt32 keyPid,
                                     OMUInt32 keySize,
                                     OMUInt32 version);
static void readSetIndexEntry(IStream* stream,
                              SetIndexEntry* entry,
                              bool swapNeeded);
static SetIndexEntry* readSetIndex(IStream* stream,
                                   OMUInt32 count,
                                   bool swapNeeded);
static SetIndexEntry* readSetIndex(IStream* stream,
                                   OMUInt32 count,
                                   OMUInt32 keyPid,
                                   OMUInt32 keySize,
                                   OMByte** keys,
                                   bool swapNeeded);
static WeakCollectionIndexEntry* readWeakCollectionIndex(IStream* stream,
                                                         OMUInt32 count,
                                                         bool swapNeeded);
static void dumpObject(IStorage* storage,
                       char* pathName,
                       int isRoot,
                       OMUInt32 version);
static OMUInt32 typeOf(IndexEntry* entry, OMUInt32 version);
OMUInt32 objectCount(IStorage* storage,
                     IStream* propertiesStream,
                     IndexEntry* index,
                     OMUInt32 version,
                     bool swapNeeded);
void checkObject(IStorage* storage,
                 IStream* propertiesStream,
                 IndexEntry* index,
                 OMUInt32 entries,
                 OMUInt32 version,
                 char* pathName,
                 int isRoot,
                 bool swapNeeded);
static void dumpContainedObjects(IStorage* storage,
                                 IStream* propertiesStream,
                                 IndexEntry* index,
                                 OMUInt32 entries,
                                 OMUInt32 version,
                                 char* pathName,
                                 int isRoot,
                                 bool swapNeeded);
static void dumpDataStream(IStream* stream,
                           const char* pathName,
                           const char* streamName,
                           OMUInt32 version,
                           OMUInt8 byteOrder);
static void dumpProperties(IStorage* storage,
                           IStream* stream,
                           IndexEntry* index,
                           OMUInt32 entries,
                           OMUInt32 version,
                           char* pathName,
                           int isRoot,
                           bool swapNeeded);
static void openStorage(char* fileName, IStorage** storage);
static void dumpFileHex(char* fileName);
static void dumpFile(char* fileName);
static OMUInt16 determineVersion(IStorage* storage);
static void dumpFileProperties(char* fileName, const char* label);
static void dumpReferencedProperties(IStorage* root, OMUInt16 version);
static FILE* wfopen(const wchar_t* fileName, const wchar_t* mode);
static int readSignature(FILE* file,
                         unsigned char* signature,
                         size_t signatureSize);
static bool isRecognizedSignature(unsigned char* signature,
                                  size_t signatureSize,
                                  GUID* fileKind);
static int isAnAAFFile(const wchar_t* fileName,
                       GUID* fileKind,
                       bool* fileIsAAFFile);
static void usage(void);

static void printInteger(const size_t value, char* label);
static void printFixed(const double value, char* label);
static void printFixedPercent(const double value, char* label);

static void resetStatistics(void);
static void printStatistics(void);
static double divide(size_t dividend, size_t divisor);
static double percent(size_t whole, size_t part);

static bool ignoring(OMUInt32 pid);
static void ignore(OMUInt32 pid);

static void initializeCOM(void);
static void finalizeCOM(void);

static char* readName(IStream* stream,
                      OMUInt32 nameOffset,
                      OMUInt32 nameSize,
                      OMUInt32 version,
                      bool swapNeeded)
{
  char* result;
  char* buffer;
  buffer = new char[nameSize];
  read(stream,
       nameOffset,
       buffer,
       nameSize);
  if (version < 26) {
    result = buffer;
  } else {
    // name consists of 2-byte characters
    size_t characterCount = nameSize / 2;
    if (swapNeeded) {
      swapOMString((OMCharacter*)buffer, characterCount);
    }
    char* name = new char[characterCount];
    convert(name, characterCount, (OMCharacter*)buffer);
    delete [] buffer;
    result = name;
  }
  return result;
}

static size_t fileSize(const char* fileName)
{
  FILE* f = fopen(fileName, "r");
  if (f == 0) {
    fatalError("fileSize", "Can't open file");
  }
  int status = fseek(f, 0L, SEEK_END);
  if (status != 0) {
    fatalError("fileSize", "seek() failed");
  }
  errno = 0;
  size_t result = ftell(f);
  if ((result == (size_t)-1) && (errno != 0)) {
    fatalError("fileSize", "ftell() failed");
  }
  fclose(f);
  return result;
}

// Hexadecimal/ASCII dumper.
//
class Dumper {
public:

  Dumper(void);
  ~Dumper(void);

  // Dump a single byte.
  //
  void print(unsigned char ch);

  // Flush any pending dumped bytes.
  //
  void flush(void);

private:

  void output(void);
  void spaces(int n);

  enum {BYTESPERLINE  = 16}; // Number of dumped bytes to put on a line.
  enum {SEPARATION    =  3}; // Separation between hex and ASCII dumps.
  enum {BYTESIZE      =  3}; // Output chars used for a byte dumped in hex.
  enum {LEADINGSPACES =  4}; // Separation between address and hex dump. 

  unsigned char* _buffer;
  int _count;
  int _line;

  static char table[128];

  char map(int c);

};

// Interpret values 0x00 - 0x7f as ASCII characters.
//
char Dumper::table[128] = {
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
' ',  '!',  '"',  '#',  '$',  '%',  '&', '\'',
'(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
'8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_',
'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
'x',  'y',  'z',  '{',  '|',  '}',  '~',  '.'};

Dumper::Dumper(void)
  : _buffer(new unsigned char[BYTESPERLINE]), _count(0), _line(0)
{
}

Dumper::~Dumper(void)
{
  flush();
  delete [] _buffer;
}

void Dumper::output(void)
{
  int width = 8;
  int i;
  
  cout << setw(width) << _line * BYTESPERLINE;
  spaces(LEADINGSPACES);
  _line++;
  
  IOS_FMT_FLAGS savedFlags = cout.setf(ios::basefield);
  char savedFill = cout.fill();
  
  for (i = 0; i < _count; i++) {
    cout << hex << setw(2) << setfill('0') << (int)_buffer[i] << " ";
  }
  
  cout.setf(savedFlags, ios::basefield);
  cout.fill(savedFill);
  
  for (i = _count; i < BYTESPERLINE; i++) {
    spaces(BYTESIZE);
  }
  
  spaces(SEPARATION);
  
  for (i = 0; i < _count; i++) {
    int c = (unsigned char)_buffer[i];
    cout << map(c);
  }
  
  cout << endl;
}

void Dumper::spaces(int n)
{
  for (int i = 0; i < n; i++ ) {
    cout << ' ';
  }
}

void Dumper::print(unsigned char ch)
{ 
  _buffer[_count++] = ch;
  
  if ( _count == BYTESPERLINE) {
    output();
    _count = 0;
  }
}

void Dumper::flush(void)
{
  if (_count > 0) {
    output();
  }
  _count = 0;
  _line = 0;
}

char Dumper::map(int c)
{
  char result;
  if (c < 0x80) {
    result = table[c & 0x7f];
  } else {
    result = '.';
  }
  return result;
}

Dumper dumper;

char* baseName(char* fullName)
{
  char* result;
#if defined(OM_OS_WINDOWS)
  const int delimiter = '\\';
#elif defined(OM_OS_MACOS)
  const int delimiter = ':';
#else
  const int delimiter = '/';
#endif
  result = strrchr(fullName, delimiter);
  if (result == 0) {
    result = fullName;
  } else if (strlen(result) == 0) {
    result = fullName;
  } else {
    result++;
  }

  return result;
}

void reportAssertionFailure(char* name,
                            char* expressionString,
                            char* fileName,
                            size_t lineNumber)
{
  cerr << "Assertion \"" << name << "\" failed." << endl;
  cerr << "The failure occurred at line "
       << lineNumber << " in file \"" << fileName << "\"." << endl;
  cerr << "The condition \"" << expressionString << "\" was violated." << endl;

  exit(EXIT_FAILURE);
}

// Assertions are on unless they are explicitly turned off.
//
#if !defined(NODEBUG)
#define ASSERT(name, expression)  \
  (expression) \
    ? (void)0  \
    : reportAssertionFailure(name, #expression,  __FILE__, __LINE__)

#else
#define ASSERT(name, expression)
#endif

// Consistency check of data type sizes.
//
void checkSizes(void)
{
  ASSERT("Correct definition for OMUInt8",  sizeof(OMUInt8)  == 1);
  ASSERT("Correct definition for OMUInt16", sizeof(OMUInt16) == 2);
  ASSERT("Correct definition for OMUInt32", sizeof(OMUInt32) == 4);

  if (sizeof(OMUInt8) != 1) {
    fatalError("checkSizes", "Incorrect definition for OMUInt8.");
  }
  if (sizeof(OMUInt16) != 2) {
    fatalError("checkSizes", "Incorrect definition for OMUInt16.");
  }
  if (sizeof(OMUInt32) != 4) {
    fatalError("checkSizes", "Incorrect definition for OMUInt32.");
  }
}

ByteOrder hostByteOrder(void)
{
  OMUInt16 word = 0x1234;
  OMUInt8  byte = *((OMUInt8*)&word);
  ByteOrder result;

  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));

  if (byte == 0x12) {
    result = bigEndian;
  } else {
    result = littleEndian;
  }
  return result;
}

const char* byteOrder(ByteOrder bo)
{
  char* result = "unknown";

  if (bo == littleEndian) {
    result = "little endian";
  } else if (bo == bigEndian) {
    result = "big endian";
  }
  return result;
}

void formatError(DWORD errorCode)
{
#if defined(OM_OS_WINDOWS)
  OMCHAR buffer[256];
  int status = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errorCode,
    LANG_SYSTEM_DEFAULT,
    buffer,
    sizeof(buffer)/sizeof(buffer[0]),
    NULL);
  char message[256];
  convert(message, 256, buffer);
  if (status != 0) {
    int length = strlen(message);
    if (length >= 2) {
      message[length - 2] = '\0';
    }
    cerr << message << endl;
  } else {
    cerr << hex << errorCode << dec << endl;
  }
#else
  cerr << "Error code = " << hex << errorCode << dec << endl;
#endif
}

char* programName;

void fatalError(char* routineName, char* message)
{
  cerr << programName
       << ": Fatal error in routine \"" << routineName << "\". "
       << message << endl;

  exit(EXIT_FAILURE);
}

void error(char* routineName, char* message)
{
  cerr << programName
       << ": Error in routine \"" << routineName << "\". "
       << message << endl;

  errorCount = errorCount + 1;
}

void warning(char* routineName, char* message)
{
  cerr << programName
       << ": Warning in routine \"" << routineName << "\". "
       << message << endl;
}

void printError(const char* prefix, const char* fileName, DWORD errorCode)
{
  cerr << prefix << ": Error : \"" << fileName << "\" : ";
  formatError(errorCode);
}

int check(const char* fileName, DWORD resultCode)
{
  if (FAILED(resultCode)) {
    printError(programName, fileName, resultCode);
    return FALSE;
  } else {
    return TRUE;
  }
}

int checks(DWORD resultCode)
{
  if (FAILED(resultCode)) {
    cerr << programName << ": Error : ";
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }
}

#if defined(OM_UNICODE_APIS)

void convert(wchar_t* wcName, size_t length, const char* name)
{
  size_t status  = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    fatalError("convert", "Conversion failed.");
  }
}

void convert(char* cName, size_t length, const wchar_t* name)
{
  size_t status  = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fatalError("convert", "Conversion failed.");
  }
}

#else

// For use when wchar_t and OMCharacter are incompatible.
// e.g. when sizeof(wchar_t) != sizeof(OMCharacter)
void convert(char* cName, size_t length, const OMCharacter* name)
{
  for (size_t i = 0; i < length; i++) {
    char ch = name[i]; // truncate
    cName[i] = ch;
    if (ch == 0) {
      break;
    }
  }
}
#endif

void convert(char* cName, size_t length, const char* name)
{
  size_t sourceLength = strlen(name);
  if (sourceLength < length - 1) {
    strncpy(cName, name, length);
  } else {
    fatalError("convert", "Conversion failed.");
  }
}

void convertName(char* cName, size_t length, OMCHAR* wideName, char** tag)
{
  char name[256];
  convert(name, 256, wideName);
  unsigned int first = name[0];
  char* pName;

  if (first < 0x03) {
    *tag = "OLE"; // OLE-managed
    pName = &name[1];
  } else if (first == 0x03) {
    *tag = "parent"; // parent-owned
    pName = &name[1];
  } else if (first == 0x04) {
    *tag = "SS reserved"; // for exclusive use of Structured Storage
    pName = &name[1];
  } else if (first <= 0x1f) {
    *tag = "OLE & O/S reserved"; // reserved for OLE and O/S
    pName = &name[1];
  } else {
    *tag = "application";
    pName = &name[0];
  }
  // Insert check for non-printable characters here.
  strncpy(cName, pName, length);
}

void indent(int level)
{
  for (int i = 0; i < level; i++) {
    cout << " ";
  }
}

#if defined(OM_REFERENCE_SS)

static const unsigned char guidMap[] =
{ 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15 }; 
static const wchar_t digits[] = L"0123456789ABCDEF"; 

#define GUIDSTRMAX 38 

int StringFromGUID2(const GUID& guid, OMCHAR* buffer, int bufferSize) 
{
  const unsigned char* ip = (const unsigned char*) &guid; // input pointer
  OMCHAR* op = buffer;                                    // output pointer

  *op++ = L'{'; 
 
  for (size_t i = 0; i < sizeof(guidMap); i++) { 

    if (guidMap[i] == '-') { 
      *op++ = L'-'; 
    } else { 
      *op++ = digits[ (ip[guidMap[i]] & 0xF0) >> 4 ]; 
      *op++ = digits[ (ip[guidMap[i]] & 0x0F) ]; 
    } 
  } 
  *op++ = L'}'; 
  *op = L'\0'; 
 
  return GUIDSTRMAX; 
} 

#endif

void getClass(IStorage* storage, CLSID* clsid, const char* fileName)
{
  STATSTG statstg;
  HRESULT result = storage->Stat(&statstg, STATFLAG_DEFAULT);
  if (!check(fileName, result)) {
    fatalError("getClass", "IStorage::Stat() failed.");
  }
  *clsid = statstg.clsid;
}

void printClsid(const CLSID& clsid)
{
  char cs[256];

  OMCHAR s[256];
  int result = StringFromGUID2(clsid, s, 256);
  if (result <= 0) {
    strcpy(cs, "unknown");
  } else {
    convert(cs, 256, s);
  }
  cout << cs;
}

void printRawKey(OMByte* key, size_t keySize)
{
  IOS_FMT_FLAGS savedFlags = cout.setf(ios::basefield);
  char savedFill = cout.fill();

  for (OMUInt32 j = 0; j < keySize; j++) {
    cout << hex << setw(2) << setfill('0') << (int)key[j];
  }
  cout.setf(savedFlags, ios::basefield);
  cout.fill(savedFill);
}

void printUMID(const UMID& umid)
{
  IOS_FMT_FLAGS savedFlags = cout.setf(ios::basefield);
  char savedFill = cout.fill();
  cout << "{";
  for (size_t i = 0; i < sizeof(umid.SMPTELabel); i++) {
    cout << setfill('0') << setw(2) << hex << (int)umid.SMPTELabel[i];
  }
  cout << "-";
  cout << setfill('0') << setw(2) << hex << (int)umid.length;
  cout << "-";
  cout << setfill('0') << setw(2) << hex << (int)umid.instanceHigh;
  cout << "-";
  cout << setfill('0') << setw(2) << hex << (int)umid.instanceMid;
  cout << "-";
  cout << setfill('0') << setw(2) << hex << (int)umid.instanceLow;
  cout << "-";
  printClsid(umid.material);
  cout << "}";
  cout.setf(savedFlags, ios::basefield);
  cout.fill(savedFill);
}

void openStream(IStorage* storage, const char* streamName, IStream** stream)
{
  *stream = 0;
  OMCHAR wcStreamName[256];
  convert(wcStreamName, 256, streamName);

  HRESULT result = storage->OpenStream(
    wcStreamName,
    NULL,
    STGM_SHARE_EXCLUSIVE | STGM_READ,
    0,
    stream);
  if (!check(streamName, result)) {
    fatalError("openStream", "Failed to open stream.");
  }

}

HRESULT openStreamTry(IStorage* storage,
                      const char* streamName,
                      IStream** stream)
{
  *stream = 0;
  OMCHAR wcStreamName[256];
  convert(wcStreamName, 256, streamName);

  HRESULT result = storage->OpenStream(
    wcStreamName,
    NULL,
    STGM_SHARE_EXCLUSIVE | STGM_READ,
    0,
    stream);

  return result;
}

size_t sizeOfStream(IStream* stream, const char* streamName)
{
  STATSTG statstg;
  HRESULT result = stream->Stat(&statstg, STATFLAG_NONAME);
  if (!check(streamName, result)) {
    fatalError("sizeOfStream", "Failed to Stat() stream.");
  }
  unsigned long int streamBytes = statstg.cbSize.LowPart;
  if (statstg.cbSize.HighPart != 0) {
    warning("sizeOfStream", "Large streams not handled.");
    streamBytes = (size_t)-1;
  }
  return streamBytes;
}

void printStat(STATSTG* statstg, char* tag)
{
  indent(4);
  switch (statstg->type) {
    
  case STGTY_STORAGE:
    cout << "storage";
    break;
    
  case STGTY_STREAM:
    cout << "stream";
    break;
    
  case STGTY_LOCKBYTES:
    cout << "lock bytes";
    break;
    
  case STGTY_PROPERTY:
    cout << "property";
    break;
    
  default:
    cout << "unknown";
    break;
  }
  
  cout << " (" << tag << ")" << endl;

  if (statstg->type == STGTY_STORAGE) {
    indent(6);
    cout << "clsid  = ";
    printClsid(statstg->clsid);
    cout << endl;
  }

  if ((statstg->type == STGTY_STREAM) || (statstg->type == STGTY_LOCKBYTES)) {
    unsigned long int byteCount = statstg->cbSize.LowPart;
    if (statstg->cbSize.HighPart != 0) {
      warning("printStat", "Large streams not handled.");
      byteCount = (size_t)-1;
    }
    
    indent(6);
    cout << "size   = " << byteCount << " bytes." << endl;
  }

#if defined (STGFMT_FIELD_IMPLEMENTED)
  // The field "dwStgFmt" is mentioned in the documentation
  // but does not appear to be implemented in the compound
  // document (docfile) implementation of structured storage.
  // Perhaps it is the "reserved" field.
  //
  indent(6);
  cout << "format = ";
  switch (statstg->dwStgFmt) {
  case STGFMT_DOCUMENT:
    cout << "document";
    break;
  case STGFMT_DIRECTORY:
    cout << "directory";
    break;
  case STGFMT_CATALOG:
    cout << "catalog";
    break;
  case STGFMT_FILE:
    cout << "file";
    break;
  default:
    cout << "unknown";
    break;
  }
  cout << endl;
#endif
}

void dumpStream(IStream* stream, STATSTG* statstg, char* pathName)
{
  totalStreams = totalStreams + 1; // Count this stream

  OMCHAR* name = statstg->pwcsName;
  char cName[256];
  char* tag;

  convertName(cName, 256, name, &tag);

  cout << endl;
  cout << pathName << cName << endl;
  printStat(statstg, tag);

  char ch;
  HRESULT result;
  unsigned long int bytesRead;

  unsigned long int byteCount = statstg->cbSize.LowPart;
  if (statstg->cbSize.HighPart != 0) {
    warning("dumpStream", "Large streams not handled.");
    byteCount = (size_t)-1;
  }
  totalStreamBytes = totalStreamBytes + byteCount;
  
  for (unsigned long int i = 0; i < byteCount; i++) {
    result = stream->Read(&ch, 1, &bytesRead);
    if (!check(pathName, result)) {
      fatalError("dumpStream", "IStream::Read() failed.");
    }
    if (bytesRead != 1) {
      fatalError("dumpStream", "IStream::Read() read wrong number of bytes.");
    }
    dumper.print(ch);
  }
  
  dumper.flush();

}

void dumpStorage(IStorage* storage,
                 STATSTG* statstg,
                 char* pathName,
                 int isRoot)
{
  totalStorages = totalStorages + 1; // Count this storage

  OMCHAR* name = statstg->pwcsName;
  char cName[256];
  char myPathName[256];
  char* tag;

  // Compute my name.
  //
  strcpy(myPathName, pathName);

  if (isRoot) {
    convertName(cName, 256, name, &tag); // To fill in tag
    strcpy(cName, "");
  } else {
    convertName(cName, 256, name, &tag);
  }

  strcat(myPathName, cName);

  // Display my name.
  //
  cout << endl;
  cout << myPathName << endl;
  printStat(statstg, tag);

  // Compute my name as a path.
  //
  if (!isRoot) {
    strcat(myPathName, "/");
  }

  HRESULT result;
  size_t status;

  IEnumSTATSTG* enumerator;
  result = storage->EnumElements(0, NULL, 0, &enumerator);
  if (!check(pathName, result)) {
    fatalError("dumpStorage", "IStorage::EnumElements() failed.");
  }
  
  result = enumerator->Reset();
  if (!check(pathName, result)) {
    fatalError("dumpStorage", "IStorage::Reset() failed.");
  }
  
  do {
    STATSTG statstg;
    status = enumerator->Next(1, &statstg, NULL);
    if (status == S_OK) {
      
      switch (statstg.type) {

      case STGTY_STORAGE:
        IStorage* subStorage;
        result = storage->OpenStorage(
          statstg.pwcsName,
          NULL,
          STGM_SHARE_EXCLUSIVE | STGM_READ,
          NULL,
          0,
          &subStorage);
        if (!check(pathName, result)) {
          fatalError("dumpStorage", "IStorage::OpenStorage() failed.");
        }
        dumpStorage(subStorage, &statstg, myPathName, 0);
        subStorage->Release();
        subStorage = 0;
        break;

      case STGTY_STREAM:
        IStream* subStream;
        result = storage->OpenStream(
          statstg.pwcsName,
          NULL,
          STGM_SHARE_EXCLUSIVE | STGM_READ,
          0,
          &subStream);
        if (!check(pathName, result)) {
          fatalError("dumpStorage", "IStorage::OpenStream() failed.");
        }
        dumpStream(subStream, &statstg, myPathName);
        subStream->Release();
        subStream = 0;
        break;

      case STGTY_LOCKBYTES:
        cout << "data    ";
        // FIXME printName(statstg.pwcsName);
        break;

      case STGTY_PROPERTY:
        cout << "property";
        // FIXME printName(statstg.pwcsName);
        break;

      default:
        cout << "unknown ";
        // FIXME printName(statstg.pwcsName);
        break;
      }
      
      // Having called EnumElements()/Next() we are responsible for
      // freeing statstg.pwcsName.
      //
      CoTaskMemFree(statstg.pwcsName);
    }
  } while (status == S_OK);
}

void read(IStream* stream, void* address, size_t size)
{
  DWORD bytesRead;
  HRESULT result = stream->Read(address, size, &bytesRead);
  if (!checks(result)) {
    fatalError("read", "IStream::Read() failed.");
  }
  if (bytesRead != size) {
    fatalError("read", "IStream::Read() read wrong number of bytes.");
  }
  
}

void read(IStream* stream, size_t offset, void* address, size_t size)
{
  LARGE_INTEGER newPosition = {offset, 0};
  ULARGE_INTEGER oldPosition;
  HRESULT status = stream->Seek(newPosition, STREAM_SEEK_SET, &oldPosition);
  if (!checks(status)) {
    fatalError("read", "IStream::Seek() failed.");
  }

  read(stream, address, size);
}

void readUInt8(IStream* stream, OMUInt8* value)
{
  read(stream, value, sizeof(OMUInt8));
}

void readUInt16(IStream* stream, OMUInt16* value, bool swapNeeded)
{
  read(stream, value, sizeof(OMUInt16));
  if (swapNeeded) {
    swapUInt16(value);
  }
}

void readUInt32(IStream* stream, OMUInt32* value, bool swapNeeded)
{
  read(stream, value, sizeof(OMUInt32));
  if (swapNeeded) {
    swapUInt32(value);
  }
}

void swapUInt16(OMUInt16* value)
{
  OMUInt8* p = (OMUInt8*)value;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[1];
  p[1] = temp;

}

void swapUInt32(OMUInt32* value)
{
  OMUInt8* p = (OMUInt8*)value;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[3];
  p[3] = temp;

  temp = p[1];
  p[1] = p[2];
  p[2] = temp;
}

void readOMString(IStream* stream,
                  OMCharacter* string,
                  size_t characterCount,
                  bool swapNeeded)
{
  read(stream, string, characterCount * sizeof(OMCharacter));
  if (swapNeeded) {
    swapOMString(string, characterCount);
  }
}

void printOMString(const OMCharacter* string)
{
  const OMCharacter* p = string;
  while (*p != 0) {
    OMCharacter c = *p;
    char ch = c & 0xff;
    cout << ch;
    ++p;
  }
}

void swapOMString(OMCharacter* string,
                  size_t characterCount)
{
  for (size_t index = 0; index < characterCount; index++) {
    swapUInt16(&string[index]);
  }
}

void readPidString(IStream* stream,
                   OMPropertyId* string,
                   size_t pidCount,
                   bool swapNeeded)
{
  read(stream, string, pidCount * sizeof(OMPropertyId));
  if (swapNeeded) {
    swapPidString(string, pidCount);
  }
}

void printPidString(const OMPropertyId* string)
{
  IOS_FMT_FLAGS savedFlags = cout.setf(ios::basefield);
  char savedFill = cout.fill();

  const OMPropertyId* p = string;
  while (*p != 0) {
  
    cout << hex << setw(4) << setfill('0') << *p << " ";
  
    ++p;
  }
  cout.setf(savedFlags, ios::basefield);
  cout.fill(savedFill);
}

void swapPidString(OMPropertyId* string,
                   size_t pidCount)
{
  for (size_t index = 0; index < pidCount; index++) {
    swapUInt16(&string[index]);
  }
}

static void readCLSID(IStream* stream, CLSID* value, bool swapNeeded)
{
  read(stream, value, sizeof(CLSID));
  if (swapNeeded) {
    swapCLSID(value);
  }
}

void swapCLSID(CLSID* value)
{
  swapUInt32(&value->Data1);
  swapUInt16(&value->Data2);
  swapUInt16(&value->Data3);
  // no need to swap Data4
}

void dumpIndexEntry(OMUInt32 i, IndexEntry* indexEntry)
{
  ASSERT("Valid index entry", indexEntry != 0);
  cout << setw(12) << i;
  cout << setw(12) << hex
                   << indexEntry->_pid
                   << dec;
  cout << setw(12) << hex
                   << indexEntry->_type
                   << dec;
  cout << setw(12) << indexEntry->_offset;
  cout << setw(12) << indexEntry->_length;
  cout << endl;
}

void printIndex(IndexEntry* index, OMUInt32 entries)
{
  ASSERT("Valid index", index != 0);
  if (entries > 0) {
    cout << setw(12) << "property";
    cout << setw(12) << "pid (hex)";
    cout << setw(12) << "form (hex)";
    cout << setw(12) << "offset";
    cout << setw(12) << "length";
    cout << endl;
    for (OMUInt32 i = 0; i < entries; i++) {
      dumpIndexEntry(i, &index[i]);
    }
  } else {
    cout << "empty" << endl;
  }
  cout << endl;
}

void readIndexEntry(IStream* stream,
                    IndexEntry* entry,
                    bool swapNeeded,
                    OMUInt32 version)
{
  IndexEntry newEntry;
  if (version >= 23) {
    OMUInt16 pid;
    OMUInt16 type;
    OMUInt16 length;
    readUInt16(stream, &pid, swapNeeded);
    readUInt16(stream, &type, swapNeeded);
    readUInt16(stream, &length, swapNeeded);
    newEntry._pid = pid;
    newEntry._type = type;
    newEntry._length = length;
    newEntry._offset = 0;
  } else {
    readUInt32(stream, &newEntry._pid, swapNeeded);
    readUInt32(stream, &newEntry._type, swapNeeded);
    readUInt32(stream, &newEntry._offset, swapNeeded);
    readUInt32(stream, &newEntry._length, swapNeeded);
  }
  memcpy(entry, &newEntry, sizeof(IndexEntry));
}

IndexEntry* readIndex(IStream* stream,
                      OMUInt32 count,
                      bool swapNeeded,
                      OMUInt32 version)
{
  IndexEntry* result = new IndexEntry[count];
  ASSERT("Successfully allocated index array", result != 0);
  for (OMUInt32 i = 0; i < count; i++) {
    readIndexEntry(stream, &result[i], swapNeeded, version);
  }
  if (version >= 23) {
    // compute offsets for in memory index
    OMUInt32 offset = 4 + (count * 6);
    for (OMUInt32 j = 0; j < count; j++) {
      result[j]._offset = offset;
      offset = offset + result[j]._length;
    }
  }
  return result;
}

void reportBadIndex(char* pathName,
                    OMUInt16 order,
                    OMUInt32 version,
                    OMUInt32 entryCount,
                    OMUInt32 expectedSize,
                    OMUInt32 actualSize)
{
  char* endianity;
  if (order == hostByteOrder()) {
    endianity = "native";
  } else {
    endianity = "foreign";
  }
  cerr << "Error in property index for \"" << pathName << ".\"" << endl;
  cerr << "( Byte order = " << byteOrder(order)
       << " (" << endianity << ")"
       << ", Version = " << version
       << ", Number of entries = " << entryCount << " )" << endl;
  cerr << "Expected index stream size = " << expectedSize
       << ", Actual index stream size = " << actualSize << "." << endl;
}

void reportBadIndexEntry(OMUInt32 i,
                         const IndexEntry* entry)
{
  cerr << "Property set index entry is invalid." << endl;
  cerr << setw(12) << "property";
  cerr << setw(12) << "pid (hex)";
  cerr << setw(12) << "form (hex)";
  cerr << setw(12) << "offset";
  cerr << setw(12) << "length";
  cerr << endl;
  cerr << setw(12) << i;
  cerr << setw(12) << hex
                   << entry->_pid
                   << dec;
  cerr << setw(12) << hex
                   << entry->_type
                   << dec;
  cerr << setw(12) << entry->_offset;
  cerr << setw(12) << entry->_length;
  cerr << endl;
}

bool isValid(const IndexEntry* index, const OMUInt32 entries)
{
  bool result = true;

  size_t position;
  size_t previousOffset;
  size_t currentOffset;
  size_t currentLength;

  for (size_t i = 0; i < entries; i++) {
    currentOffset = index[i]._offset;
    currentLength = index[i]._length;
    // Check length
    if (currentLength == 0) {
      reportBadIndexEntry(i, &index[i]);
      error("isValid", "Property set index entry has zero length.");
    }
    if (i == 0) {
      // First entry
      previousOffset = currentOffset;
      position = currentOffset + currentLength;
    } else {
      // Subsequent entries
      if (currentOffset < previousOffset) {
        reportBadIndexEntry(i, &index[i]);
        warning("isValid", "Property set index entries out of order.");
        result = false;
        break;
      } else if (position > currentOffset) {
        reportBadIndexEntry(i, &index[i]);
        warning("isValid", "Property set index entries overlap.");
        result = false;
        break; 
      } else {
        // this entry is valid
        previousOffset = currentOffset;
        position = position + currentLength;
      }
    }
  }

  return result;
}

// Minimum value stream size for a value stream with this index.
//
size_t valueStreamSize(const IndexEntry* index, const OMUInt32 entries)
{
  size_t result = 0;

  for (size_t i = 0; i < entries; i++) {
    result = result + index[i]._length;
  }
  return result;
}

char* typeName(OMUInt32 type)
{
  char * result;
  
  switch (type) {
    
  case SF_DATA:
    result = "data";
    break;
    
  case SF_DATA_STREAM:
    result = "data stream";
    break;

  case SF_STRONG_OBJECT_REFERENCE:
    result = "strong object reference";
    break;
    
  case SF_STRONG_OBJECT_REFERENCE_VECTOR:
    result = "strong object reference vector";
    break;

  case SF_STRONG_OBJECT_REFERENCE_SET:
    result = "strong object reference set";
    break;
    
  case SF_WEAK_OBJECT_REFERENCE:
    result = "weak object reference";
    break;

  case SF_WEAK_OBJECT_REFERENCE_VECTOR:
    result = "weak object reference vector";
    break;

  case SF_WEAK_OBJECT_REFERENCE_SET:
    result = "weak object reference set";
    break;

  case SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID:
    result = "stored object identification";
    break;

  case SF_UNIQUE_OBJECT_ID:
    result = "unique object identification";
    break;

  case SF_OPAQUE_STREAM:
    result = "opaque stream";
    break;

  default:
    result = "unknown";
    break;
  }

  return result;
  
}

void openStorage(IStorage* parentStorage,
                 char* storageName,
                 IStorage** subStorage)
{
  *subStorage = 0;
  OMCHAR wcStorageName[256];
  convert(wcStorageName, 256, storageName);

  HRESULT result = parentStorage->OpenStorage(
    wcStorageName,
    NULL,
    STGM_SHARE_EXCLUSIVE | STGM_READ,
    NULL,
    0,
    subStorage);
  if (!check(storageName, result)) {
    fatalError("openStorage", "IStorage::OpenStorage() failed.");
  }
}

void dumpVectorIndexEntry(OMUInt32 i, VectorIndexEntry* vectorIndexEntry)
{
  cout << setw(8) << i
       << " : "
       << setw(10) << vectorIndexEntry->_elementName << endl;
}

void printVectorIndex(VectorIndexEntry* vectorIndex,
                      OMUInt32 count,
                      OMUInt32 highWaterMark,
                      OMUInt32 lowWaterMark,
                      OMUInt32 version)
{
  if (version >= 23) {
    cout << "Dump of vector index" << endl;
    cout << "( Number of entries    = " << count << "," << endl
         << "  First free local key = " << highWaterMark
         << ", Last free local key = " << lowWaterMark << " )" << endl;
  } else {
    cout << "Dump of vector index" << endl;
    cout << "( Number of entries = " << count
         << ", High water mark = " << highWaterMark << " )" << endl;
  }

  if (count > 0) {
    cout << setw(8) << "ordinal"
         << "   "
         << setw(10) << "local key" << endl;

    for (OMUInt32 i = 0; i < count; i++) {
      dumpVectorIndexEntry(i, &vectorIndex[i]);
    }
  } else {
    cout << "empty" << endl;
  }
}

void readVectorIndexEntry(IStream* stream,
                          VectorIndexEntry* entry,
                          bool swapNeeded)
{
  // Reading native vector index entries not yet supported.
  // Instead read the entire vector index.
  //
  ASSERT("Swap needed", swapNeeded);
  if (!swapNeeded) {
    // NYI
  } else {
    VectorIndexEntry newEntry;
    readUInt32(stream, &newEntry._elementName, swapNeeded);
    memcpy(entry, &newEntry, sizeof(VectorIndexEntry));
  }

}

VectorIndexEntry* readVectorIndex(IStream* stream,
                                  OMUInt32 count,
                                  bool swapNeeded)
{
  VectorIndexEntry* result = new VectorIndexEntry[count];
  ASSERT("Successfully allocated vector index array", result != 0);
  if (!swapNeeded) {
    read(stream, result, sizeof(VectorIndexEntry) * count);
  } else {
    for (OMUInt32 i = 0; i < count; i++) {
      readVectorIndexEntry(stream, &result[i], swapNeeded);
    }
  }
  return result;
}

void dumpSetIndexEntry(OMUInt32 i,
                       SetIndexEntry* setIndexEntry,
                       bool printKey)
{
  cout << setw(8) << i
       << " : "
       << setw(10) << setIndexEntry->_elementName
       << "     ";
  printReferenceCount(setIndexEntry->_referenceCount);
  cout << "     ";
  if (printKey) {
    printClsid(setIndexEntry->_key);
  } else {
    printClsid(nullCLSID);
  }
  cout << endl;
}

void printReferenceCount(OMUInt32 referenceCount)
{
  if (cFlag) {
      cout << setw(8) << referenceCount;
  } else {
    OMUInt32 count = referenceCount - 2;
    if (count == 0xffffffff) {
      cout << setw(8) << "sticky";
    } else {
      cout << setw(8) << count;
    }
  }
}

void printSetIndex(SetIndexEntry* setIndex,
                   OMUInt32 count,
                   OMUInt32 highWaterMark,
                   OMUInt32 lowWaterMark,
                   OMUInt32 keyPid,
                   OMUInt32 keySize,
                   OMUInt32 version)
{
  cout << "Dump of set index" << endl;
  if (version > 19) {
    if (version >= 23) {
      cout << "( Number of entries    = " << count << "," << endl
           << "  First free local key = " << highWaterMark
           << ", Last free local key = " << lowWaterMark << endl
           << "  Key pid = "    << hex << keyPid
           << ", Key size = "   << dec << keySize<< " )" << endl;
    } else {
      cout << "( Number of entries = "   << count
           << ", High water mark = " << highWaterMark
           << ", Key pid = "    << hex << keyPid
           << ", Key size = "   << dec << keySize<< " )" << endl;
    }
  } else {
    cout << "( Number of entries = " << count
         << ", High water mark = " << highWaterMark << " )" << endl;
  }

  if (count > 0) {
    cout << setw(8) << "ordinal"
         << "   "
         << setw(10) << "local key"
         << "   "
         << setw(8) << "references"
         << "     "
         << setw(8) << "unique key"
         << endl;

    for (OMUInt32 i = 0; i < count; i++) {
      dumpSetIndexEntry(i, &setIndex[i], !ignoring(keyPid));
    }
  } else {
    cout << "empty" << endl;
  }
}

void printSetIndex(SetIndexEntry* setIndex,
                   OMUInt32 count,
                   OMUInt32 highWaterMark,
                   OMUInt32 lowWaterMark,
                   OMUInt32 keyPid,
                   OMUInt32 keySize,
                   OMByte* keys,
                   OMUInt32 version)
{
  cout << "Dump of set index" << endl;
  if (version >= 23) {
    cout << "( Number of entries    = " << count << "," << endl
         << "  First free local key = " << highWaterMark
         << ", Last free local key = " << lowWaterMark << endl
         << "  Key pid = "    << hex << keyPid
         << ", Key size = "   << dec << keySize<< " )" << endl;
  } else {
    cout << "( Number of entries = "   << count
         << ", High water mark = " << highWaterMark
         << ", Key pid = "    << hex << keyPid
         << ", Key size = "   << dec << keySize<< " )" << endl;
  }

  if (count > 0) {
    cout << setw(8) << "ordinal"
         << "   "
         << setw(10) << "local key"
         << "   "
         << setw(8) << "references"
         << "     "
         << setw(8) << "unique key"
         << endl;

    for (OMUInt32 i = 0; i < count; i++) {
      cout << setw(8) << i
           << " : "
           << setw(10) << setIndex[i]._elementName
           << "     ";
      printReferenceCount(setIndex[i]._referenceCount);
      cout << "     ";
      cout << endl;
      cout << "  ";
      if (keySize == 32) {
        if (!ignoring(keyPid)) {
          printUMID((const UMID&)keys[i * keySize]);
        } else {
          printUMID(nullUMID);
        }
      } else {
        printRawKey(&keys[i * keySize], keySize);
      }
      cout << endl;
    }
  } else {
    cout << "empty" << endl;
  }
}

void printWeakCollectionIndex(int containerType,
                              WeakCollectionIndexEntry* collectionIndex,
                              OMUInt32 count,
                              OMUInt32 tag,
                              OMUInt32 keyPid,
                              OMUInt32 keySize,
                              OMUInt32 version)
{
  //TRACE("printWeakCollectionIndex");
  ASSERT("Valid container type",
                          (containerType == SF_WEAK_OBJECT_REFERENCE_SET) ||
                          (containerType == SF_WEAK_OBJECT_REFERENCE_VECTOR));

  if (containerType == SF_WEAK_OBJECT_REFERENCE_SET) {
    cout << "Dump of set index" << endl;
  } else if (containerType == SF_WEAK_OBJECT_REFERENCE_VECTOR) {
    cout << "Dump of vector index" << endl;
  }

  if (version > 19) {
    cout << "( Tag = " << tag
         << ", Number of entries = " << count
         << ", Key pid = "    << hex << keyPid
         << ", Key size = "   << dec << keySize<< " )" << endl;
  } else {
    cout << "( Tag = " << tag
         << ", Number of entries = " << count << " )" << endl;
  }

  if (count > 0) {
    cout << setw(8) << "ordinal"
         << "   "
         << setw(8) << "unique key"
         << endl;

    for (OMUInt32 i = 0; i < count; i++) {
      cout << setw(8) << i;
      cout << " : ";
      printClsid(collectionIndex[i]._key);
      cout << endl;
    }
  } else {
    cout << "empty" << endl;
  }
}

void readSetIndexEntry(IStream* stream,
                          SetIndexEntry* entry,
                          bool swapNeeded)
{
  // Reading native set index entries not yet supported.
  // Instead read the entire set index.
  //
  ASSERT("Swap needed", swapNeeded);
  if (!swapNeeded) {
    // NYI
  } else {
    SetIndexEntry newEntry;
    readUInt32(stream, &newEntry._elementName, swapNeeded);
    readUInt32(stream, &newEntry._referenceCount, swapNeeded);
    readCLSID(stream, &newEntry._key, swapNeeded);
    memcpy(entry, &newEntry, sizeof(SetIndexEntry));
  }

}

SetIndexEntry* readSetIndex(IStream* stream,
                                  OMUInt32 count,
                                  bool swapNeeded)
{
  SetIndexEntry* result = new SetIndexEntry[count];
  ASSERT("Successfully allocated set index array", result != 0);
  if (!swapNeeded) {
    read(stream, result, sizeof(SetIndexEntry) * count);
  } else {
    for (OMUInt32 i = 0; i < count; i++) {
      readSetIndexEntry(stream, &result[i], swapNeeded);
    }
  }
  return result;
}

SetIndexEntry* readSetIndex(IStream* stream,
                            OMUInt32 count,
                            OMUInt32 /* keyPid */,
                            OMUInt32 keySize,
                            OMByte** keys,
                            bool swapNeeded)
{
  SetIndexEntry* result = new SetIndexEntry[count];
  ASSERT("Successfully allocated set index array", result != 0);
  *keys = new OMByte[count * keySize];
  ASSERT("Successfully allocated key array", *keys != 0);
  for (OMUInt32 i = 0; i < count; i++) {
    readUInt32(stream, &result[i]._elementName, swapNeeded);
    readUInt32(stream, &result[i]._referenceCount, swapNeeded);
    memset(&result[i]._key, 0, sizeof(result[i]._key)); // Gak !!
    read(stream, &((*keys)[i * keySize]), keySize);
  }
  return result;
}

WeakCollectionIndexEntry* readWeakCollectionIndex(IStream* stream,
                                                  OMUInt32 count,
                                                  bool swapNeeded)
{
  WeakCollectionIndexEntry* result = new WeakCollectionIndexEntry[count];
  ASSERT("Successfully allocated collection index array", result != 0);
  if (!swapNeeded) {
    read(stream, result, sizeof(WeakCollectionIndexEntry) * count);
  } else {
    for (OMUInt32 i = 0; i < count; i++) {
      readCLSID(stream, &result[i]._key, swapNeeded);
    }
  }
  return result;
}

OMUInt32 typeOf(IndexEntry* entry, OMUInt32 version)
{
  OMUInt32 result;

  if (version > 24) {
    result = entry->_type;
  } else if (version > 7) {
    switch(entry->_type) {
      case TID_DATA:
        result = SF_DATA;
        break;
      case TID_DATA_STREAM:
        result = SF_DATA_STREAM;
        break;
      case TID_STRONG_OBJECT_REFERENCE:
        result = SF_STRONG_OBJECT_REFERENCE;
        break;
      case TID_STRONG_OBJECT_REFERENCE_VECTOR:
        result = SF_STRONG_OBJECT_REFERENCE_VECTOR;
        break;
      case TID_STRONG_OBJECT_REFERENCE_SET:
        result = SF_STRONG_OBJECT_REFERENCE_SET;
        break;
      case TID_WEAK_OBJECT_REFERENCE:
        result = SF_WEAK_OBJECT_REFERENCE;
        break;
      case TID_WEAK_OBJECT_REFERENCE_VECTOR:
        result = SF_WEAK_OBJECT_REFERENCE_VECTOR;
        break;
      case TID_WEAK_OBJECT_REFERENCE_SET:
        result = SF_WEAK_OBJECT_REFERENCE_SET;
        break;
      case TID_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID:
        result = SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID;
        break;
      case TID_UNIQUE_OBJECT_ID:
        result = SF_UNIQUE_OBJECT_ID;
        break;
      case TID_OPAQUE_STREAM:
        result = SF_OPAQUE_STREAM;
        break;
    }
  } else {
    switch (entry->_type) {
      case OLD_TID_DATA:
        result = SF_DATA; 
        break;
      case OLD_TID_STRONG_OBJECT_REFERENCE:
        result = SF_STRONG_OBJECT_REFERENCE;
        break;
      case OLD_TID_STRONG_OBJECT_REFERENCE_VECTOR:
        result = SF_STRONG_OBJECT_REFERENCE_VECTOR;
        break;
      case OLD_TID_WEAK_OBJECT_REFERENCE:
        result = SF_WEAK_OBJECT_REFERENCE;
        break;
      case OLD_TID_WEAK_OBJECT_REFERENCE_VECTOR:
        result = SF_WEAK_OBJECT_REFERENCE_VECTOR;
        break;
      case OLD_TID_DATA_STREAM:
        result = SF_DATA_STREAM;
        break;
      default:
        break;
    }
  }
  return result;
}

OMUInt32 objectCount(IStorage* storage,
                     IStream* propertiesStream,
                     IndexEntry* index,
                     OMUInt32 version,
                     bool swapNeeded)
{
  // get name of collection index
  //
  char* suffix = " index";
  char* collectionName = readName(propertiesStream,
                                  index->_offset,
                                  index->_length,
                                  version,
                                  swapNeeded);

  size_t size = strlen(collectionName) + strlen(suffix) + 1;
  char* collectionIndexName = new char[size];
  strcpy(collectionIndexName, collectionName);
  strcat(collectionIndexName, suffix);

  // open the collection index stream
  //
  IStream* subStream = 0;
  openStream(storage, collectionIndexName, &subStream);
  if (subStream == 0) {
    fatalError("objectCount", "openStream() failed.");
  }

  OMUInt32 count = 0;
  OMUInt32 highWaterMark = 0;
  if (version >= 22) {
    readUInt32(subStream, &count, swapNeeded);
    readUInt32(subStream, &highWaterMark, swapNeeded);
  } else {
    readUInt32(subStream, &highWaterMark, swapNeeded);
    readUInt32(subStream, &count, swapNeeded);
  }
  delete [] collectionName;
  delete [] collectionIndexName;
  subStream->Release();
  return count;
}

void checkObject(IStorage* storage,
                 IStream* propertiesStream,
                 IndexEntry* index,
                 OMUInt32 entries,
                 OMUInt32 version,
                 char* pathName,
                 int isRoot, 
                 bool swapNeeded)
{
  // Count expected storages and streams
  //
  size_t storageCount = 0;
  size_t streamCount = 1; // For the "properties" stream
  if (isRoot) {
    streamCount = streamCount + 1; // For the "referenced properties" stream
  }
  for (size_t i = 0; i < entries; i++) {
    switch(typeOf(&index[i], version)) {
      case SF_DATA_STREAM:
        streamCount = streamCount + 1;
        break;
      case SF_STRONG_OBJECT_REFERENCE:
        storageCount = storageCount + 1;
        break;
      case SF_STRONG_OBJECT_REFERENCE_VECTOR:
        storageCount = storageCount + objectCount(storage,
                                                  propertiesStream,
                                                  &index[i],
                                                  version,
                                                  swapNeeded);
        streamCount = streamCount + 1;
        break;
      case SF_STRONG_OBJECT_REFERENCE_SET:
        storageCount = storageCount + objectCount(storage,
                                                  propertiesStream,
                                                  &index[i],
                                                  version,
                                                  swapNeeded);
        streamCount = streamCount + 1;
        break;
      case SF_WEAK_OBJECT_REFERENCE_VECTOR:
        streamCount = streamCount + 1;
        break;
      case SF_WEAK_OBJECT_REFERENCE_SET:
        streamCount = streamCount + 1;
        break;
    }
  }
  // Count actual storages and streams
  //
  HRESULT result;
  size_t status;

  IEnumSTATSTG* enumerator;
  result = storage->EnumElements(0, NULL, 0, &enumerator);
  if (!check(pathName, result)) {
    fatalError("checkObject", "IStorage::EnumElements() failed.");
  }
  
  result = enumerator->Reset();
  if (!check(pathName, result)) {
    fatalError("checkObject", "IStorage::Reset() failed.");
  }
  
  OMUInt32 actualStorageCount = 0;
  OMUInt32 actualStreamCount = 0;
  do {
    STATSTG statstg;
    status = enumerator->Next(1, &statstg, NULL);
    if (status == S_OK) {

      switch (statstg.type) {
      case STGTY_STORAGE:
        actualStorageCount = actualStorageCount + 1;
        break;
      case STGTY_STREAM:
        actualStreamCount = actualStreamCount + 1;
        break;
      }
      CoTaskMemFree(statstg.pwcsName);
    }
  } while (status == S_OK);
  if (actualStorageCount != storageCount) {
    cerr << programName
         << ": Warning : Incorrect IStorage count for \"" << pathName
         << "\", expected " << storageCount << " IStorages, found "
         << actualStorageCount << "." << endl;
    warningCount = warningCount + 1;
  }
  if (actualStreamCount != streamCount) {
    cerr << programName
         << ": Warning : Incorrect IStream count for \"" << pathName
         << "\", expected " << streamCount << " IStreams, found "
         << actualStreamCount << "." << endl;
    warningCount = warningCount + 1;
  }
}

void dumpContainedObjects(IStorage* storage,
                          IStream* propertiesStream,
                          IndexEntry* index,
                          OMUInt32 entries,
                          OMUInt32 version,
                          char* pathName,
                          int isRoot,
                          bool swapNeeded)
{

  if (vFlag) {
    checkObject(storage,
                propertiesStream,
                index,
                entries,
                version,
                pathName,
                isRoot,
                swapNeeded);
  }

  for (OMUInt32 i = 0; i < entries; i++) {

    int containerType = typeOf(&index[i], version);
    switch (containerType) {
      
    case SF_DATA:
      // value is dumped when the property value stream is dumped
      break;

    case SF_DATA_STREAM: {
      OMUInt32 nameStart = index[i]._offset;
      OMUInt32 nameLength = index[i]._length;
      OMUInt8 byteOrder = 0x55; // unspecified
      if (version >= 28) {
        // byte order
        read(propertiesStream, index[i]._offset, &byteOrder, 1);
        nameStart = nameStart + 1;
        nameLength = nameLength - 1;
      }
      char* subStreamName = readName(propertiesStream,
                                     nameStart,
                                     nameLength,
                                     version,
                                     swapNeeded);
      
      // Compute the pathname for this stream
      //
      char thisPathName[256];
      strcpy(thisPathName, pathName);
      if (!isRoot) {
        strcat(thisPathName, "/");
      }
      strcat(thisPathName, subStreamName);
      IStream* stream = 0;
      openStream(storage, subStreamName, &stream);
      dumpDataStream(stream, thisPathName, subStreamName, version, byteOrder);

      stream->Release();
      stream = 0;

      delete [] subStreamName;
      subStreamName = 0;
    }
    break;

    case SF_STRONG_OBJECT_REFERENCE: {
      // get name of sub-storage
      //
      char* subStorageName = readName(propertiesStream,
                                      index[i]._offset,
                                      index[i]._length,
                                      version,
                                      swapNeeded);
      
      // Compute the pathname for this object
      //
      char thisPathName[256];
      strcpy(thisPathName, pathName);
      if (!isRoot) {
        strcat(thisPathName, "/");
      }
      strcat(thisPathName, subStorageName);

      // open the storage
      //
      IStorage* subStorage = 0;
      openStorage(storage, subStorageName, &subStorage);
      if (storage == 0) {
        fatalError("dumpContainedObjects", "OpenStorage() failed.");
      }
      // dump the object
      //
      dumpObject(subStorage, thisPathName, 0, version);

      delete [] subStorageName;
      subStorageName = 0;

      subStorage->Release();
      subStorage = 0;
    }
    break;

    case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
      // get name of vector index
      //
      char* suffix = " index";
      char* vectorName = readName(propertiesStream,
                                  index[i]._offset,
                                  index[i]._length,
                                  version,
                                  swapNeeded);

      size_t size = strlen(vectorName) + strlen(suffix) + 1;
      char* vectorIndexName = new char[size];
      strcpy(vectorIndexName, vectorName);
      strcat(vectorIndexName, suffix);

      // Compute the pathname for this object
      //
      char thisPathName[256];
      strcpy(thisPathName, pathName);
      if (!isRoot) {
        strcat(thisPathName, "/");
      }
      strcat(thisPathName, vectorName);

      // open the vector index stream
      //
      IStream* subStream = 0;
      openStream(storage, vectorIndexName, &subStream);
      if (subStream == 0) {
        fatalError("dumpContainedObjects", "openStream() failed.");
      }
      size_t vectorIndexStreamSize = sizeOfStream(subStream, vectorIndexName);
      totalStreamBytes = totalStreamBytes + vectorIndexStreamSize;

      OMUInt32 _count = 0;
      OMUInt32 _highWaterMark = 0;
      OMUInt32 _lowWaterMark = 0;
      if (version >= 22) {
        readUInt32(subStream, &_count, swapNeeded);
        readUInt32(subStream, &_highWaterMark, swapNeeded);
        if (version >= 23) {
          readUInt32(subStream, &_lowWaterMark, swapNeeded);
        }
      } else {
        readUInt32(subStream, &_highWaterMark, swapNeeded);
        readUInt32(subStream, &_count, swapNeeded);
      }

      // Read the vector index.
      //
      VectorIndexEntry* vectorIndex = readVectorIndex(subStream,
                                                      _count,
                                                      swapNeeded);

      // dump the vector index
      //
      cout << endl;
      cout << thisPathName << endl;
      printVectorIndex(vectorIndex,
                       _count,
                       _highWaterMark,
                       _lowWaterMark,
                       version);

      // for each vector index entry
      //
      for (OMUInt32 entry = 0; entry < _count; entry++) {
        //   compute storage name
        char* elementName = new char[strlen(vectorName) + 1 + 1];
        strcpy(elementName, vectorName);
        strcat(elementName, _openArrayKeySymbol);
        
        char number[256];
        sprintf(number, "%lx", vectorIndex[entry]._elementName);
        
        size_t size = strlen(elementName) + strlen(number) + 1 + 1;
        char* subStorageName = new char[size];
        strcpy(subStorageName, elementName);
        strcat(subStorageName, number);
        strcat(subStorageName, _closeArrayKeySymbol);

        // Compute the path name for this element
        //
        char thisPathName[256];
        strcpy(thisPathName, pathName);
        if (!isRoot) {
          strcat(thisPathName, "/");
        }
        strcat(thisPathName, subStorageName);
        
        // open the storage
        //
        IStorage* subStorage = 0;
        openStorage(storage, subStorageName, &subStorage);
        if (storage == 0) {
          fatalError("dumpContainedObjects", "openStorage() failed.");
        }
        // dump the object
        //
        dumpObject(subStorage, thisPathName, 0, version);

        delete [] elementName;
        elementName = 0;

        delete [] subStorageName;
        subStorageName = 0;

        subStorage->Release();
        subStorage = 0;
      }

      delete [] vectorName;
      vectorName = 0;

      delete [] vectorIndexName;
      vectorIndexName = 0;

      delete [] vectorIndex;
      vectorIndex = 0;

      subStream->Release();
      subStream = 0;
    }
    break;
    case SF_STRONG_OBJECT_REFERENCE_SET : {
      // get name of set index
      //
      char* suffix = " index";
      char* setName = readName(propertiesStream,
                               index[i]._offset,
                               index[i]._length,
                               version,
                               swapNeeded);

      size_t size = strlen(setName) + strlen(suffix) + 1;
      char* setIndexName = new char[size];
      strcpy(setIndexName, setName);
      strcat(setIndexName, suffix);

      // Compute the pathname for this object
      //
      char thisPathName[256];
      strcpy(thisPathName, pathName);
      if (!isRoot) {
        strcat(thisPathName, "/");
      }
      strcat(thisPathName, setName);

      // open the set index stream
      //
      IStream* subStream = 0;
      openStream(storage, setIndexName, &subStream);
      if (subStream == 0) {
        fatalError("dumpContainedObjects", "openStream() failed.");
      }
      size_t setIndexStreamSize = sizeOfStream(subStream, setIndexName);
      totalStreamBytes = totalStreamBytes + setIndexStreamSize;

      OMUInt32 _count = 0;
      OMUInt32 _highWaterMark = 0;
      OMUInt32 _lowWaterMark = 0;
      if (version >= 22) {
        readUInt32(subStream, &_count, swapNeeded);
        readUInt32(subStream, &_highWaterMark, swapNeeded);
        if (version >= 23) {
          readUInt32(subStream, &_lowWaterMark, swapNeeded);
        }
      } else {
        readUInt32(subStream, &_highWaterMark, swapNeeded);
        readUInt32(subStream, &_count, swapNeeded);
      }

      OMUInt32 keyPid = 0;
      OMUInt32 keySize = 16;

      if (version > 23) {
        // Read the key pid.
        //
        OMUInt16 pid;
        readUInt16(subStream, &pid, swapNeeded);
        keyPid = pid;

        // Read the key size.
        //
        OMUInt8 size;
        readUInt8(subStream, &size);
        keySize = size;
      } else if (version > 19) {
        // Read the key pid.
        //
        readUInt32(subStream, &keyPid, swapNeeded);

        // Read the key size.
        //
        readUInt32(subStream, &keySize, swapNeeded);
      }

      SetIndexEntry* setIndex = 0;
      if (keySize == 16) {
        // Read the set index.
        //
        setIndex = readSetIndex(subStream, _count, swapNeeded);

        // dump the set index
        //
        cout << endl;
        cout << thisPathName << endl;
        printSetIndex(setIndex,
                      _count,
                      _highWaterMark,
                      _lowWaterMark,
                       keyPid,
                       keySize,
                       version);
      } else {
        // Read the set index.
        //
        OMByte*keys = 0;
        setIndex = readSetIndex(subStream,
                                _count,
                                keyPid,
                                keySize,
                                &keys,
                                swapNeeded);

        // dump the set index
        //
        cout << endl;
        cout << thisPathName << endl;
        printSetIndex(setIndex,
                      _count,
                      _highWaterMark,
                      _lowWaterMark,
                       keyPid,
                       keySize,
                       keys,
                       version);
        delete [] keys;
      }

      // for each set index entry
      //
      for (OMUInt32 entry = 0; entry < _count; entry++) {
        //   compute storage name
        char* elementName = new char[strlen(setName) + 1 + 1];
        strcpy(elementName, setName);
        strcat(elementName, _openArrayKeySymbol);
        
        char number[256];
        sprintf(number, "%lx", setIndex[entry]._elementName);
        
        size_t size = strlen(elementName) + strlen(number) + 1 + 1;
        char* subStorageName = new char[size];
        strcpy(subStorageName, elementName);
        strcat(subStorageName, number);
        strcat(subStorageName, _closeArrayKeySymbol);

        // Compute the path name for this element
        //
        char thisPathName[256];
        strcpy(thisPathName, pathName);
        if (!isRoot) {
          strcat(thisPathName, "/");
        }
        strcat(thisPathName, subStorageName);
        
        // open the storage
        //
        IStorage* subStorage = 0;
        openStorage(storage, subStorageName, &subStorage);
        if (storage == 0) {
          fatalError("dumpContainedObjects", "openStorage() failed.");
        }
        // dump the object
        //
        dumpObject(subStorage, thisPathName, 0, version);

        delete [] elementName;
        elementName = 0;

        delete [] subStorageName;
        subStorageName = 0;

        subStorage->Release();
        subStorage = 0;
      }

      delete [] setName;
      setName = 0;

      delete [] setIndexName;
      setIndexName = 0;

      delete [] setIndex;
      setIndex = 0;

      subStream->Release();
      subStream = 0;
    }
    break;
    case SF_WEAK_OBJECT_REFERENCE:
      // value is dumped when the property value stream is dumped
      break;

    case SF_WEAK_OBJECT_REFERENCE_VECTOR:
    case SF_WEAK_OBJECT_REFERENCE_SET: {
      // get name of index
      //
      char* suffix = " index";
      char* setName = readName(propertiesStream,
                               index[i]._offset,
                               index[i]._length,
                               version,
                               swapNeeded);

      size_t size = strlen(setName) + strlen(suffix) + 1;
      char* setIndexName = new char[size];
      strcpy(setIndexName, setName);
      strcat(setIndexName, suffix);

      // Compute the pathname for this object
      //
      char thisPathName[256];
      strcpy(thisPathName, pathName);
      if (!isRoot) {
        strcat(thisPathName, "/");
      }
      strcat(thisPathName, setName);

      // open the index stream
      //
      IStream* subStream = 0;
      openStream(storage, setIndexName, &subStream);
      if (subStream == 0) {
        fatalError("dumpContainedObjects", "openStream() failed.");
      }
      size_t setIndexStreamSize = sizeOfStream(subStream, setIndexName);
      totalStreamBytes = totalStreamBytes + setIndexStreamSize;

      OMUInt32 _count;
      readUInt32(subStream, &_count, swapNeeded);

      OMUInt32 _tag;
      OMUInt32 keyPid = 0;
      OMUInt32 keySize = 16;

      if (version > 23) {
        OMUInt16 tag;
        readUInt16(subStream, &tag, swapNeeded);
        _tag = tag;

        // Read the key pid.
        //
        OMUInt16 pid;
        readUInt16(subStream, &pid, swapNeeded);
        keyPid = pid;

        // Read the key size.
        //
        OMUInt8 size;
        readUInt8(subStream, &size);
        keySize = size;
      } else if (version > 19) {
        readUInt32(subStream, &_tag, swapNeeded);
        // Read the key pid.
        //
        readUInt32(subStream, &keyPid, swapNeeded);

        // Read the key size.
        //
        readUInt32(subStream, &keySize, swapNeeded);
      }

      // Read the index.
      //
      WeakCollectionIndexEntry* collectionIndex =
                                           readWeakCollectionIndex(subStream,
                                                                   _count,
                                                                   swapNeeded);

      // dump the index
      //
      cout << endl;
      cout << thisPathName << endl;
      printWeakCollectionIndex(containerType, collectionIndex, _count, _tag, keyPid, keySize, version);

      delete [] setName;
      setName = 0;

      delete [] setIndexName;
      setIndexName = 0;

      delete [] collectionIndex;
      collectionIndex = 0;

      subStream->Release();
      subStream = 0;
      }
      break;

    case SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID:
    case SF_UNIQUE_OBJECT_ID:
    case SF_OPAQUE_STREAM:
      // TBS
      break;

    default:
      break;
    }

  }

}

void dumpDataStream(IStream* stream,
                    const char* pathName,
                    const char* streamName,
                    OMUInt32 version,
                    OMUInt8 byteOrder)
{
  cout << endl;
  cout << pathName << endl;
  cout << "Dump of data stream" << endl;

  STATSTG statstg;
  HRESULT result = stream->Stat(&statstg, STATFLAG_NONAME);
  if (!check(streamName, result)) {
    fatalError("dumpdataStream", "Failed to Stat() stream.");
  }
  unsigned long int streamBytes = statstg.cbSize.LowPart;
  if (statstg.cbSize.HighPart != 0) {
    warning("dumpDataStream", "Large streams not handled.");
    streamBytes = (size_t)-1;
  }
  totalStreamBytes = totalStreamBytes + streamBytes;
  totalPropertyBytes = totalPropertyBytes + streamBytes;

  unsigned long int limit = streamBytes;
  if (mFlag) {
    if (mLimit <= limit) {
      limit = mLimit;
      cout << "( Size = "
           << streamBytes
           << ", output limited to "
           << mLimit;
    } else {
      cout << "( Size = "
           << streamBytes
           << ", smaller than output limit of "
           << mLimit;
    }
  } else {
    cout << "( Size = "
         << streamBytes;
  }

  if (version >= 28) {
    char* s;
    switch (byteOrder) {
    case 'L':
      s = "little endian";
      break;
    case 'B':
      s = "big endian";
      break;
    case 'U':
      s = "unspecified";
      break;
    default:
      s = "unknown"; // error
      break;
    }
    cout << ", Byte order = " << s;
  }
  cout << " )"
       << endl;

  if (streamBytes > 0) {
    for (unsigned long int byteCount = 0;
         byteCount < limit;
         byteCount++) {

      unsigned char ch;
      DWORD bytesRead;
      HRESULT result = stream->Read(&ch, 1, &bytesRead);

      if (!checks(result)) {
        fatalError("dumpDataStream", "IStream::Read() failed.");
      }

      if (bytesRead != 1) {
        fatalError("dumpDataStream",
                   "IStream::Read() read wrong number of bytes.");
      }
      dumper.print(ch);
    }
    dumper.flush();
  } else {
    cout << "empty" << endl;
  }
}

void dumpProperties(IStorage* storage,
                    IStream* stream,
                    IndexEntry* index,
                    OMUInt32 entries,
                    OMUInt32 version,
                    char* pathName,
                    int isRoot,
                    bool swapNeeded)
{
  if (version < 21) {
    openStream(storage, _propertyValueStreamName, &stream);
    if (stream == 0) {
      fatalError("dumpProperties", "openStream() failed.");
    }
  }

  // Check that the property value stream is the correct size for the
  // given index.
  //
  size_t actualStreamSize = sizeOfStream(stream, _propertyValueStreamName);
  size_t expectedStreamSize = valueStreamSize(index, entries);

  if (actualStreamSize < expectedStreamSize) {
    fatalError("dumpProperties", "Property value stream too small.");
  }

  if (version > 23) {
    size_t correctSize = expectedStreamSize + 4 + (entries * 6);
    if (actualStreamSize != correctSize) {
      cerr << programName
           << ": Warning : Property value stream wrong size"
           << " (actual = " << actualStreamSize
           << ", expected = " << correctSize <<" )." << endl;
      warningCount = warningCount + 1;
    }
  }

  // Add in the size of the property value stream
  //
  if (version >= 21) {
    totalStreamBytes = totalStreamBytes + expectedStreamSize;
  } else {
    totalStreamBytes = totalStreamBytes + actualStreamSize;
  }

  for (OMUInt32 i = 0; i < entries; i++) {

    // Count this property
    //
    totalProperties = totalProperties + 1;

    // Count the bytes in this property.
    // Note that this byte count doesn't include any gaps there may be
    // between property values, nor does it include overhead such as
    // the space occupied by the property index.
    //
    totalPropertyBytes = totalPropertyBytes + index[i]._length;

    bool ignore = ignoring(index[i]._pid);

    cout << endl;
    IOS_FMT_FLAGS savedFlags = cout.setf(ios::basefield);
    char savedFill = cout.fill();
    cout << "property " << i << " "
         << "( pid = "
         << hex << setw(4) << setfill('0') << index[i]._pid << dec
         << ", form = "
         << typeName(typeOf(&index[i], version))
         << " )" << endl;

    cout.setf(savedFlags, ios::basefield);
    cout.fill(savedFill);

    for (OMUInt32 byteCount = 0; byteCount < index[i]._length; byteCount++) {

      unsigned char ch;
      DWORD bytesRead;
      HRESULT result = stream->Read(&ch, 1, &bytesRead);

      if (!checks(result)) {
        fatalError("dumpProperties", "IStream::Read() failed.");
      }

      if (bytesRead != 1) {
        fatalError("dumpProperties",
                   "IStream::Read() read wrong number of bytes.");
      }
      if (!ignore) {
        dumper.print(ch);
      } else {
        dumper.print(0);
      }
    }
    dumper.flush();
  }
  
  dumpContainedObjects(storage,
                       stream,
                       index,
                       entries,
                       version,
                       pathName,
                       isRoot,
                       swapNeeded);

  if (version < 21) {
    stream->Release();
    stream = 0;
  }
}

ByteOrder fileByteOrder = unspecifiedEndian;

void dumpObject(IStorage* storage,
                char* pathName,
                int isRoot,
                OMUInt32 version)
{
  totalObjects = totalObjects + 1; // Count this object

  IStream* stream = 0;
  openStream(storage, _propertyIndexStreamName, &stream);

  if (stream == 0) {
    fatalError("dumpObject", "Property index stream not found.");
  }

  // Check that the stream is not empty.
  //
  size_t indexStreamSize = sizeOfStream(stream, _propertyIndexStreamName);
  if (indexStreamSize == 0){
    fatalError("dumpObject", "Property index stream empty.");
  }

  OMUInt16 _byteOrder;
  if (version >= 23) {
    OMUInt8 byteOrder;
    readUInt8(stream, &byteOrder);
    ASSERT("Valid byte order", ((byteOrder == 'L') || (byteOrder == 'B')));
    if (byteOrder == 'L') {
      _byteOrder = littleEndian;
    } else {
      _byteOrder = bigEndian;
    }
  } else if (version > 1) {
    OMUInt16 byteOrder;
    readUInt16(stream, &byteOrder, false);
    _byteOrder = byteOrder;
  }
  ASSERT("Valid byte order",
                    (_byteOrder == littleEndian) || (_byteOrder == bigEndian));

  if (isRoot) {
    // The byte ordering of the root property index specifies the byte
    // ordering for the whole file.
    //
    fileByteOrder = _byteOrder;

  } else {
    if (_byteOrder != fileByteOrder) {
      // This is an illegal AAF file, but in the dumper we treat it
      // as a warning and byte swap the index and the property
      // values anyway. The goal is to dump as much useful
      // information as possible, this could be the bug that the
      // user is looking for.
      //
      warning("dumpObject", "Illegal AAF File.");
      cerr << "Byte ordering for index of \"" << pathName << "\" ("
           << byteOrder(_byteOrder)
           << ") does not match that of the file ("
           << byteOrder(fileByteOrder) << ")." << endl;
    }
  }

  // We must know the file and index byte orders to correctly proceed.
  //
  ASSERT("File byte order known",
         (fileByteOrder == littleEndian) || (fileByteOrder == bigEndian));
  ASSERT("Index byte order known",
         (_byteOrder == littleEndian) || (_byteOrder == bigEndian));

  bool swapNeeded;
  if (_byteOrder != hostByteOrder()) {
    swapNeeded = true;
  } else {
    swapNeeded = false;
  }

  OMUInt32 _entryCount;
  if (version >= 23) {
    OMUInt8 _formatVersion;
    readUInt8(stream, &_formatVersion);

    OMUInt16 entryCount;
    readUInt16(stream, &entryCount, swapNeeded);
    _entryCount = entryCount;
  } else {
    OMUInt32 _formatVersion;
    readUInt32(stream, &_formatVersion, swapNeeded);

    OMUInt32 entryCount;
    readUInt32(stream, &entryCount, swapNeeded);
    _entryCount = entryCount;
  }

  // Compute the header size which is as follows ...
  //
  //   _byteOrder     2 (_formatVersion >= 2)
  //   _formatVersion 4
  //   _entryCount    4
  //
  size_t headSize;
  if (version < 2) {
    headSize = 8;
  } else {
    headSize = 10;
  }
  
  // Check that the stream size is consistent with the entry count
  //
  if (version >= 21) {
    // stream must be at least big enough to contain the index
    size_t expectedSize;
    if (version >= 24) {
      expectedSize = 4 + (_entryCount * 6);
    } else {
      expectedSize = headSize + (_entryCount * sizeof(IndexEntry));
    }
    if (indexStreamSize < expectedSize) {
      reportBadIndex(pathName,
                     _byteOrder,
                     version,
                     _entryCount,
                     expectedSize,
                     indexStreamSize);
      fatalError("dumpObject", "Property stream too small.");
    }
    // Add in the size of the index stream
    totalStreamBytes = totalStreamBytes + expectedSize;
  } else {
    size_t expectedSize = headSize + (_entryCount * sizeof(IndexEntry));
    if (indexStreamSize < expectedSize) {
      fatalError("dumpObject", "Property index stream too small.");
    } else if (indexStreamSize > expectedSize) {
      warning("dumpObject", "Property index stream too large.");
    }
    // Add in the size of the index stream
    totalStreamBytes = totalStreamBytes + indexStreamSize;
  }

  if ((isRoot) && (version < 3)) {
    if (zFlag) {
      cout << programName
           << ": Warning : Specifying -x with format version "
           << version
           << " is not supported." << endl;
    }
  }

  if (version > HIGHVERSION) {
    cout << programName
         << ": Error : Unrecognized format version ("
         << version
         << "). Highest recognized version is " << HIGHVERSION << "." << endl;
    exit(EXIT_FAILURE);
  }

  // Should compare the format version for the file (the version of
  // the root object) with that for this object. Are files containing
  // mixed versions allowed ?

  cout << endl;
  cout << pathName << endl;

  CLSID clsid;
  getClass(storage, &clsid, pathName);
  printClsid(clsid);
  cout << endl;

  char* endianity;
  if (_byteOrder == hostByteOrder()) {
    endianity = "native";
  } else {
    endianity = "foreign";
  }

  cout << "Dump of property index" << endl
       << "( Byte order = " << byteOrder(_byteOrder)
       << " (" << endianity << ")"
       << ", Version = " << version
       << ", Number of entries = " << _entryCount << " )" << endl;

  IndexEntry* index = readIndex(stream, _entryCount, swapNeeded, version);
  printIndex(index, _entryCount);

  if (!isValid(index, _entryCount)) {
    fatalError("dumpObject", "Invalid property set index.");
  }

  cout << "Dump of properties" << endl;
  dumpProperties(storage,
                 stream,
                 index,
                 _entryCount,
                 version,
                 pathName,
                 isRoot,
                 swapNeeded);
  if (_entryCount == 0) {
    cout << "empty" << endl;
  }

  delete [] index;
  index = 0;

  stream->Release();
  stream = 0;
}

void openStorage(char* fileName, IStorage** storage)
{
  HRESULT result;
  OMCHAR wcFileName[256];
  convert(wcFileName, 256, fileName);
  
  result = StgIsStorageFile(wcFileName);
  
  switch (result) {
    
  case STG_E_INVALIDNAME:
    cerr << programName << ": Error: "
         << "\"" << fileName << "\" is not a valid file name."
         << endl;
    exit(EXIT_FAILURE);
    break;
    
  case STG_E_FILENOTFOUND:
    cerr << programName <<": Error: "
         << "File \"" << fileName << "\" not found."
         << endl;
    exit(EXIT_FAILURE);
    break;
  case STG_E_PATHNOTFOUND:
    cerr << programName <<": Error: "
         << "Path \"" << fileName << "\" not found."
         << endl;
    exit(EXIT_FAILURE);
    break;
    
  }
  
  if (!check(fileName, result)) {
    fatalError("openStorage", "StgIsStorageFile() failed.");
  }
  
  if (result != S_OK) {
    cout << "\"" << fileName << "\" is NOT a structured storage file." << endl;
    exit(EXIT_FAILURE);
  }
  
  result = StgOpenStorage(
    wcFileName,
    NULL,
    STGM_READ | STGM_SHARE_DENY_WRITE,
    NULL,
    0,
    storage);
  if (!check(fileName, result)) {
    fatalError("openStorage", "StgOpenStorage() failed.");
  }

}

void dumpFileHex(char* fileName)
{ 
  FILE* infile;
  int ch;
  unsigned long byteCount = 0;

  infile = fopen(fileName, "rb");
  if (infile != NULL) {
    cout << "Hex dump." << endl;  
    while((ch = fgetc(infile)) != EOF) {
      if ((lFlag) && (++byteCount > lLimit)) {
        break;
      }
      dumper.print((char)ch);
    }
 
    dumper.flush();
    fclose(infile);
  } else {
    cerr << programName <<": Error: "
         << "File \"" << fileName << "\" not found."
         << endl;
    exit(EXIT_FAILURE);
  }
}

void dumpFile(char* fileName)
{
  resetStatistics();

  initializeCOM();
  IStorage* storage = 0;
  openStorage(fileName, &storage);

  if (storage == 0) {
    fatalError("dumpFile", "openStorage() failed.");
  }

  STATSTG statstg;
  HRESULT result = storage->Stat(&statstg, STATFLAG_DEFAULT);
  if (!check(fileName, result)) {
    fatalError("dumpFile", "IStorage::Stat() failed.");
  }

  cout << "Raw dump." << endl;  
  dumpStorage(storage, &statstg, "/", 1);
  
  // Having called Stat() specifying STATFLAG_DEFAULT
  // we are responsible for freeing statstg.pwcsName.
  //
  CoTaskMemFree(statstg.pwcsName);
    
  // Releasing the last reference to the root storage closes the file.
  storage->Release();
  storage = 0;
  finalizeCOM();

  totalFileBytes = fileSize(fileName);
  printStatistics();
}

OMUInt16 determineVersion(IStorage* storage)
{
  OMUInt16 result;
  IStream* s = 0;
  // Version >= 21 (or < 2)
  HRESULT r = openStreamTry(storage, "properties", &s);
  if (s == 0) {
    // Version 2 - 20 (inclusive)
    r = openStreamTry(storage, "property index", &s);
  }

  if (s != 0) {
    OMUInt8 byteOrder;
    OMUInt8 version;
    read(s, &byteOrder, sizeof(byteOrder));
    read(s, &version, sizeof(version));
    if ((byteOrder != 'L') && (byteOrder != 'B')) {
      OMUInt16 byteOrder;
      OMUInt16 version;
      
      read(s, 0, &byteOrder, sizeof(byteOrder));
      read(s, sizeof(byteOrder), &version, sizeof(version));
 
      ASSERT("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
      if (byteOrder != hostByteOrder()) {
        swapUInt16(&version);
      }
      result = version;
    } else {
      result = version;
    }
    s->Release();
  } else {
    fatalError("determineVersion",
               "Can't find property index of root object");
  }
  return result;
}

void dumpFileProperties(char* fileName, const char* label)
{
  resetStatistics();

  initializeCOM();
  IStorage* storage = 0;
  openStorage(fileName, &storage);

  if (storage == 0) {
    fatalError("dumpFileProperties", "openStorage() failed.");
  }

  OMUInt16 _version = determineVersion(storage);

  ASSERT("Valid version", _version < 255);
  if (_version >= 21) {
    _propertyIndexStreamName = "properties";
    _propertyValueStreamName = "properties";
  } else {
    _propertyIndexStreamName = "property index";
    _propertyValueStreamName = "property values";
  }
  cout << label << endl;
  dumpObject(storage, "/", 1, _version);

  dumpReferencedProperties(storage, _version);

  // Releasing the last reference to the root storage closes the file.
  storage->Release();
  storage = 0;
  finalizeCOM();

  totalFileBytes = fileSize(fileName);
  printStatistics();

}

static void dumpReferencedProperties(IStorage* root, OMUInt16 version)
{
  if (version > 18) {
    IStream *stream = 0;
    openStream(root, "referenced properties", &stream);
    if (stream == 0) {
      fatalError("dumpReferencedProperties", "openStream() failed.");
    }
    ByteOrder bo;
    OMByte _byteOrder;
    read(stream, 0, &_byteOrder, sizeof(_byteOrder));
    ASSERT("Valid byte order", (_byteOrder == 'L') || (_byteOrder == 'B'));
    if (_byteOrder == 'L') {
      bo = littleEndian;
    } else if (_byteOrder == 'B') {
      bo = bigEndian;
    }
    bool swap;
    char* endianity;
    if (bo == hostByteOrder()) {
      swap = false;
      endianity = "native";
    } else {
      swap = true;
      endianity = "foreign";
    }
    OMUInt16 nameCount;
    readUInt16(stream, &nameCount, swap);
    if ((version >= 19) && (version <= 25)) {
      // version 19 - 25 "names" are 1-byte character strings
    } else if (version == 26) {
      // version 26 - 26 "names" are 2-byte character strings
      OMUInt32 characterCount;
      readUInt32(stream, &characterCount, swap);
      OMCharacter* buffer = new OMCharacter[characterCount];
      readOMString(stream, buffer, characterCount, swap);

      OMCharacter** names = new OMCharacter*[nameCount];
      OMCharacter* p = buffer;
      size_t index;
      for (index = 0; index < nameCount; index++) {
        names[index] = p;
        while (*p != 0) {
         ++p;
        }
        ++p;
      }

      cout << endl;
      cout << "Dump of Referenced Properties" << endl;
      cout << "( Byte order = " << byteOrder(bo)
           << " (" << endianity << ")";
      cout << ", Number of entries = " << nameCount << " )" << endl;
  
      if (nameCount > 0) {
        cout << setw(8) << "tag"
             << "   "
             << "name" << endl;
      }

      for (index = 0; index < nameCount; index++) {
        cout << setw(8) << index << " : ";
        printOMString(names[index]);
        cout << endl;
      }

      delete [] names;
      delete [] buffer;
    } else if (version > 26) {
      // version 27 -    "names" are 2-byte property ids
      OMUInt32 pidCount;
      readUInt32(stream, &pidCount, swap);
      OMPropertyId* buffer = new OMPropertyId[pidCount];
      readPidString(stream, buffer, pidCount, swap);

      OMPropertyId** names = new OMPropertyId*[nameCount];
      OMPropertyId* p = buffer;
      size_t index;
      for (index = 0; index < nameCount; index++) {
        names[index] = p;
        while (*p != 0) {
         ++p;
        }
        ++p;
      }

      cout << endl;
      cout << "Dump of Referenced Properties" << endl;
      cout << "( Byte order = " << byteOrder(bo)
           << " (" << endianity << ")";
      cout << ", Number of entries = " << nameCount << " )" << endl;
  
      if (nameCount > 0) {
        cout << setw(8) << "tag"
             << "   "
             << "path to target (hex pid list)" << endl;
      }

      for (index = 0; index < nameCount; index++) {
        cout << setw(8) << index << " : ";
        printPidString(names[index]);
        cout << endl;
      }

      delete [] names;
      delete [] buffer;
    }
    stream->Release();
  }    // version  0 - 18 no referenced properties table
}

// _wfopen() and _wremove are in the W32 API on Windows 95, 98 and ME
// but with an implementation that always fails. By default we use
// _wfopen() when compiled on/for the W32 API, this can be overridden
// by defining NO_W32_WFUNCS.

#if !defined(NO_W32_WFUNCS)
#if defined(_WIN32) || defined(WIN32)
#define W32_WFOPEN
#define W32_WREMOVE
#endif
#endif

// Just like fopen() except for wchar_t* file names.
//
FILE* wfopen(const wchar_t* fileName, const wchar_t* mode)
{
  FILE* result = 0;
#if defined(W32_WFOPEN)
  result = _wfopen(fileName, mode);
#else
  char cFileName[FILENAME_MAX];
  size_t status = wcstombs(cFileName, fileName, FILENAME_MAX);
  ASSERT("Convert succeeded", status != (size_t)-1);

  char cMode[FILENAME_MAX];
  status = wcstombs(cMode, mode, FILENAME_MAX);
  ASSERT("Convert succeeded", status != (size_t)-1);

  result = fopen(cFileName, cMode);
#endif
  return result;
}

// Read the file signature. Assumes that no valid file may be shorter
// than the longest signature.
//
int readSignature(FILE* file,
                  unsigned char* signature,
                  size_t signatureSize)
{
  int result = 0;
  unsigned char* sig = new unsigned char[signatureSize];

  size_t status = fread(sig, signatureSize, 1, file);
  if (status == 1) {
    memcpy(signature, sig, signatureSize);
    result = 0;
  } else {
    result = 1;  // Can't read signature
  }

  delete [] sig;
  return result;
}

// The number of bytes to read to be sure of getting the signature.
//
size_t signatureSize(void)
{
  size_t result = 0;
  for (size_t i = 0; i < sizeof(formatTable)/sizeof(formatTable[0]); i++) {
    if (formatTable[i].signatureSize > result) {
      result = formatTable[i].signatureSize;
    }
  }
  return result;
}

// Try to recognize a file signature. Assumes that no signature is a
// prefix of any other signature.
//
bool isRecognizedSignature(unsigned char* signature,
                           size_t signatureSize,
                           GUID* fileKind)
{
  bool result = false;

  for (size_t i = 0; i < sizeof(formatTable)/sizeof(formatTable[0]); i++) {
    if (formatTable[i].signatureSize <= signatureSize) {
      if (memcmp(formatTable[i].signature,
                 signature,
                 formatTable[i].signatureSize) == 0) {
        result = true;
        memcpy(fileKind, formatTable[i].fileKind, sizeof(CLSID));
        break;
      }
    }
  }
  return result;
}

// Does the given file purport to be an AAF file ?
//
static int isAnAAFFile(const wchar_t* fileName,
                       CLSID* fileKind,
                       bool* fileIsAAFFile)
{
  int result = 0;
  unsigned char* signature = new unsigned char[maxSignatureSize];
  FILE* f = wfopen(fileName, L"rb");
  if (f != 0) {
    int status = readSignature(f, signature, maxSignatureSize);
    if (status == 0) {
      if (isRecognizedSignature(signature, maxSignatureSize, fileKind)) {
        * fileIsAAFFile = true;
      }
    } else {
      result = 1; // Can't read signature
    }
    fclose(f);
  } else {
    result = 1; // Can't open file
  }
  delete [] signature;
  return result;
}

void usage(void)
{
  cerr << programName << ": Usage : " << programName
       << " [-x -r -p -a -c -s -z <pid> -m <n> -l <n> -v -h] <file...>"
       << endl;
  cerr << "-x       = hex dump"
       << " : for any file." << endl;
  cerr << "-r       = raw dump"
       << " : for any structured storage file (default)." << endl;
  cerr << "-p       = property dump"
       << " : for any file using the AAF stored object format." << endl;
  cerr << "-a       = AAF file dump"
       << " : for any AAF file." << endl;
  cerr << "-c       = print raw reference counts" << endl;
  cerr << "-s       = print statistics"
       << " : combine with -r, -p and -a." << endl;
  cerr << "-z <pid> = dump properties with pid <pid> (hex) as all zeros :"
       << endl
       << "             combine with -p and -a." << endl;
  cerr << "-m <n>   = dump only the first <n> bytes (dec) of media streams :"
       << endl
       << "             combine with -p and -a." << endl;
  cerr << "-l <n>   = dump only the first <n> bytes (dec) of the file :"
       << endl
       << "             combine with -x." << endl;
  cerr << "-v       = validate the structure of the file :"
       << endl
       << "             combine with -p and -a." << endl;
  cerr << "-h       = help"
       << " : print this message and exit." << endl;
}

bool printStats = false; // default

void printInteger(const size_t value, char* label)
{
    cout << label
         << setw(8)
         << value
         << endl;
}

void printFixed(const double value, char* label)
{
    IOS_FMT_FLAGS oldFlags = cout.flags(ios::right | ios::fixed);
    cout << label
         << setw(8)
         << setprecision(2)
         << value
         << endl;
    cout.flags(oldFlags);
}

void printFixedPercent(const double value, char* label)
{
    IOS_FMT_FLAGS oldFlags = cout.flags(ios::right | ios::fixed);
    cout << label
         << setw(8)
         << setprecision(2)
         << value
         << " %"
         << endl;
    cout.flags(oldFlags);
}

void resetStatistics(void)
{
  totalStorages = 0;
  totalStreams = 0;

  totalPropertyBytes = 0;
  totalObjects = 0;
  totalProperties = 0;

  totalStreamBytes = 0;
  totalFileBytes = 0;
}

void printStatistics(void)
{
  size_t totalMetadataBytes = totalStreamBytes - totalPropertyBytes;
  if (printStats) {
    cout << endl;
    printInteger(totalFileBytes,
      "Total number of bytes in file           (F) = ");
    if ((option == property) || (option == aaf)) {
      printInteger(totalPropertyBytes,
        "Total number of bytes in all properties (P) = ");
      printInteger(totalMetadataBytes,
        "Total number of bytes in AAF meta-data  (M) = ");
      cout << endl;
      printInteger(totalObjects,
        "Total number of objects                     = ");
      printInteger(totalProperties,
        "Total number of properties                  = ");
      cout << endl;
      double averageProperties = divide(totalProperties, totalObjects);
      printFixed(averageProperties,
        "Average number of properties per object     = ");
      double averageBytesPerObject = divide(totalPropertyBytes, totalObjects);
      printFixed(averageBytesPerObject,
        "Average number of bytes per object          = ");
      double averageBytesPerProperty = divide(totalPropertyBytes,
                                              totalProperties);
      printFixed(averageBytesPerProperty,
        "Average number of bytes per property        = ");
    } else if (option == raw) {
      printInteger(totalStreamBytes,
        "Total number of bytes in all streams    (S) = ");
      cout << endl;
      printInteger(totalStorages,
        "Total number of storages                    = ");
      printInteger(totalStreams,
        "Total number of streams                     = ");
      printInteger(totalStorages + totalStreams,
        "Total number of storage elements            = ");
      cout << endl;
      double averageBytesPerStream = divide(totalStreamBytes, totalStreams);
      printFixed(averageBytesPerStream,
        "Average number of bytes per stream          = ");
    }
    cout << endl;
    if ((option == property) || (option == aaf)) {
      double structuredStorageOverhead = percent(
                     totalFileBytes,
                     totalFileBytes - totalPropertyBytes - totalMetadataBytes);
      printFixedPercent(structuredStorageOverhead,
      "Structured storage overhead (F - P - M) / F = ");
      double aafOverhead = percent(totalPropertyBytes + totalMetadataBytes,
                                   totalMetadataBytes);
      printFixedPercent(aafOverhead,
        "AAF Overhead                 M / (P + M)    = ");
      double overallOverhead = percent(totalFileBytes,
                                       totalFileBytes - totalPropertyBytes);
      printFixedPercent(overallOverhead,
        "Overall overhead            (F - P) / F     = ");
    } else if (option == raw) {
      double structuredStorageOverhead = percent(
                                            totalFileBytes,
                                            totalFileBytes - totalStreamBytes);
      printFixedPercent(structuredStorageOverhead,
        "Structured storage overhead (F - S) / F     = ");
    }
  }
}

double divide(size_t dividend, size_t divisor)
{
  double result;
  double x = dividend;
  double y = divisor;

  if (y != 0) {
    result = x / y;
  } else {
    result = 0.0;
  }
  return result;
}

static double percent(size_t whole, size_t part)
{
  return divide(part, whole) * 100.0;
}

#define MAXIGNORE 64

OMUInt32 ignoredPids[MAXIGNORE];
int ignoreCount = 0;

bool ignoring(OMUInt32 pid)
{
  bool result = false;
  for (int i = 0; i < ignoreCount; i++) {
    if (ignoredPids[i] == pid) {
      result = true;
      break;
    }
  }
  return result;
}

void ignore(OMUInt32 pid)
{
  if (ignoreCount < MAXIGNORE) {
    ignoredPids[ignoreCount++] = pid;
  } else {
    cerr << programName
         << ": Error : Maximum number ("
         << MAXIGNORE
         << ") of property ids to ignore exceeded."
         << endl;
    usage();
    exit(EXIT_FAILURE);
  }
}

void initializeCOM(void)
{
#if !defined(OM_REFERENCE_SS)
  CoInitialize(0);
#endif
}

void finalizeCOM(void)
{
#if !defined(OM_REFERENCE_SS)
  CoUninitialize();
#endif
}


int main(int argumentCount, char* argumentVector[])
{
  checkSizes();

#if defined(OM_OS_MACOS)
#if defined(USECONSOLE)
  argumentCount = ccommand(&argumentVector); // console window for mac
#endif
#endif

  programName = baseName(argumentVector[0]);
  int fileCount = 0;
  int flagCount = 0;
  int i;

  for (i = 1; i < argumentCount; i++) {

    char* argument = argumentVector[i];
    char c = argument[0];

    if ((c == '-') && (strlen(argument) >= 2)) {
      char flag = argument[1];

      switch (flag) {
      case 'x':
        option = hexadecimal;
        break;
      case 'r':
        option = raw;
        break;
      case 'p':
        option = property;
        break;
      case 'a':
        option = aaf;
        break;
      case 'c':
        cFlag = true;
        break;
      case 's':
        printStats = true;
        break;
      case 'z':

        // Does a value follow -z ?
        //
        if ((i + 1 < argumentCount) && (*argumentVector[i + 1] != '-' )) {

          zFlag = true;

          // Consume value
          //
          flagCount = flagCount + 1;
          i = i + 1;

          // Convert value
          //
          char* pids = argumentVector[i];
          char* expectedEnd = &pids[strlen(pids)];
          char* end;
          int pid = strtoul(pids, &end, 16);

          if (end != expectedEnd) { // Some characters not consumed
            cerr << programName
                 << ": Error : \""
                 << pids
                 << "\" is not a valid property id."
                 << endl;
            usage();
            exit(EXIT_FAILURE);
          }
          if (!ignoring(pid)) {
            ignore(pid);
          } else {
            cerr << programName
                 << ": Warning : \""
                 << pid
                 << "\" is already being ignored."
                 << endl;
          }
        } else {
          cerr << programName
               << ": Error : -z must be followed by a property id."
               << endl;
          usage();
          exit(EXIT_FAILURE);
        }
        break;
      case 'm':

        // Does a value follow -m ?
        //
        if ((i + 1 < argumentCount) && (*argumentVector[i + 1] != '-' )) {

          mFlag = true;

          // Consume value
          //
          flagCount = flagCount + 1;
          i = i + 1;

          // Convert value
          //
          char* bytess = argumentVector[i];
          char* expectedEnd = &bytess[strlen(bytess)];
          char* end;
          int bytes = strtoul(bytess, &end, 10);

          if (end != expectedEnd) { // Some characters not consumed
            cerr << programName
                 << ": Error : \""
                 << bytess
                 << "\" is not a valid byte count."
                 << endl;
            usage();
            exit(EXIT_FAILURE);
          }
          mLimit = bytes;
        } else {
          cerr << programName
               << ": Error : -m must be followed by a byte count."
               << endl;
          usage();
          exit(EXIT_FAILURE);
        }
        break;
      case 'l':

        // Does a value follow -l ?
        //
        if ((i + 1 < argumentCount) && (*argumentVector[i + 1] != '-' )) {

          lFlag = true;

          // Consume value
          //
          flagCount = flagCount + 1;
          i = i + 1;

          // Convert value
          //
          char* bytess = argumentVector[i];
          char* expectedEnd = &bytess[strlen(bytess)];
          char* end;
          int bytes = strtoul(bytess, &end, 10);

          if (end != expectedEnd) { // Some characters not consumed
            cerr << programName
                 << ": Error : \""
                 << bytess
                 << "\" is not a valid byte count."
                 << endl;
            usage();
            exit(EXIT_FAILURE);
          }
          lLimit = bytes;
        } else {
          cerr << programName
               << ": Error : -l must be followed by a byte count."
               << endl;
          usage();
          exit(EXIT_FAILURE);
        }
        break;
      case 'v':
        vFlag = true;
        break;
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
        break;
      default:
        cerr << programName
             << ": Error : -"
             << flag
             << " is not a recognized option."
             << endl;
        usage();
        exit(EXIT_FAILURE);
        break;
      }
      flagCount = flagCount + 1;
    } else {
      fileCount = fileCount + 1;
    }
  }

  if (option == hexadecimal) {
    if (printStats) {
      cerr << programName
           << ": Error : -s not valid with -x."
           << endl;
      usage();
      exit(EXIT_FAILURE);
    }
    if (mFlag) {
      cerr << programName
           << ": Error : -m not valid with -x."
           << endl;
      usage();
      exit(EXIT_FAILURE);
    }
  }

  if ((option == hexadecimal) || (option == raw)) {
    if (zFlag) {
      cerr << programName
           << ": Error : -z not valid with -x or -r."
           << endl;
      usage();
      exit(EXIT_FAILURE);
    }
    if (vFlag) {
      cerr << programName
           << ": Error : -v not valid with -x or -r."
           << endl;
      usage();
      exit(EXIT_FAILURE);
    }
  }

  if (option != hexadecimal) {
    if (lFlag) {
      cerr << programName
           << ": Error : -l not valid with -r, -p or -a."
           << endl;
      usage();
      exit(EXIT_FAILURE);
    }
  }

  if (fileCount < 1) {
    cerr << programName
      << ": Error : Wrong number of arguments ("
      << fileCount << ")."
      << endl;
    usage();
    exit(EXIT_FAILURE);
  }
  
  switch (option) {
  case hexadecimal:
    for (i = flagCount + 1; i < argumentCount; i++) {
      dumpFileHex(argumentVector[i]);
    }
    break;

  case raw:
    for (i = flagCount + 1; i < argumentCount; i++) {
      dumpFile(argumentVector[i]);
    }
    break;

  case property:
    for (i = flagCount + 1; i < argumentCount; i++) {
      dumpFileProperties(argumentVector[i], "Property dump.");
    }
    break;

  case aaf:
    for (i = flagCount + 1; i < argumentCount; i++) {
      wchar_t wcFileName[FILENAME_MAX];
      size_t status = mbstowcs(wcFileName, argumentVector[i], FILENAME_MAX);
      ASSERT("Convert succeeded", status != (size_t)-1);
      CLSID x = {0};
      bool b = false;
      int s = isAnAAFFile(wcFileName, &x, &b);
      if (s == 0) {
        if (b) {
          if ((memcmp(&x, &aafFileKindAafSSBinary, sizeof(CLSID)) == 0) ||
              (memcmp(&x, &aafFileKindMxfSSBinary, sizeof(CLSID)) == 0)) {
            dumpFileProperties(argumentVector[i], "AAF property dump.");
          } else {
            if ((memcmp(&x, &aafFileKindAafXmlText, sizeof(CLSID)) == 0) ||
                (memcmp(&x, &aafFileKindMxfXmlText, sizeof(CLSID)) == 0)) {
              cerr << programName
                   << ": \""
                   << argumentVector[i]
                   << "\" is an XML format AAF file."
                   << endl;
            } else {
              cerr << programName
                   << ": Error : \""
                   << argumentVector[i]
                   << "\" is not a recognized kind of AAF file."
                  << endl;
            }
          }
        } else {
          cerr << programName
               << ": Error : \""
               << argumentVector[i]
               << "\" is not an AAF file."
               << endl;
        }
      } else {
        cerr << programName <<": Error: "
             << "File \"" << argumentVector[i] << "\" not found."
             << endl;
      }
    }
    break;

  }

  int result;
  if ((warningCount != 0) && (errorCount != 0)){
    cerr << programName << ": Dump completed with "
         << errorCount << " errors and "
         << warningCount << " warnings."
         << endl;
    result = EXIT_FAILURE;
  } else if (errorCount != 0) {
    cerr << programName << ": Dump completed with "
         << errorCount << " errors."
         << endl;
    result = EXIT_FAILURE;
  } else if (warningCount != 0) {
    cerr << programName << ": Dump completed with "
         << warningCount << " warnings."
         << endl;
    result = 2;
  } else {
    result = EXIT_SUCCESS;
  }
  return result;
}

