//
// An example program that dumps out the contents of
// an AAF file.
//

// Tim Bingham 05-May-1998 Tim_Bingham@avid.com
//             19-June-1998
//             12-August-1998
//
// Tom Ransdell 28-Sept-1998 Tom_Ransdell@avid.com
//              Adapt to building on the Macintosh with CodeWarrior Pro3
//

//
// Usage:
//
//  $ dump [-r -p -a -s -z <pid> -m <n> -h] files...
//
//    -r       = raw dump, works for any structured storage file.
//    -p       = property dump, works for files using the AAF stored
//                 object format.
//    -a       = AAF file dump, works only for AAF files.
//    -s       = print statistics.
//    -z <pid> = dump properties with pid <pid> (hex) as all zeroes.
//    -m <n>   = dump only the first <n> bytes (dec) of media streams. 
//    -h       = print help.
//
//  Notes:
//
//    1) -r, -p and -a are mutually exclusive.
//    2) -s is valid with -r, -p and -a. When combined with either -p or -a
//       statistics on objects and properties are displayed, when combined
//       with -r statistics on IStorages, IStreams and bytes are displayed.
//    3) -z is not valid with -r. Multiple -z flags may be supplied.
//    4) <pid> must be specified in hex. Examples are 1a00 0x1a00 0x1A00 
//    5) -m is not valid with -r.
//

#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <ctype.h>
#include <string.h>

#if defined(_MAC) || defined(macintosh)
#include <console.h>

#include "wintypes.h"
#include "compobj.h"
#include "storage.h"

// define standard guids
#include <initguid.h>
#include <coguid.h>
#elif defined(__sgi)
#include "storage.h"
#else
#include <objbase.h>
#endif

#if defined(_WIN32) && defined(UNICODE)
typedef wchar_t OMCHAR;
#else
typedef char OMCHAR;
#endif

#if defined (_MSC_VER)
#if _MSC_VER <= 1000
#define typename class
typedef int bool;
const bool false = 0;
const bool true = 1;
#endif
#endif

#define SUCCESS (1)
#define FAILURE (2)

// The following should be provided in
// a header file shared between this dumper and the
// toolkit. That header doesn't yet exist.
// A higher level dumper should obtaing this information
// from the AAFDictionary.

// Type ids.
// Note that these are structural types. These types convey the
// information needed to understand the structure of a file.
//
// The actual type of a property can be derived from the stored PID by
// an association PID -> type in the dictionary.
//

// version 0.8 and above
const int TID_DATA                           = 0;
const int TID_DATA_STREAM                    = 1;
const int TID_STRONG_OBJECT_REFERENCE        = 2;
const int TID_STRONG_OBJECT_REFERENCE_VECTOR = 3;
const int TID_STRONG_OBJECT_REFERENCE_SET    = 4;
const int TID_WEAK_OBJECT_REFERENCE          = 5;
const int TID_WEAK_OBJECT_REFERENCE_VECTOR   = 6;
const int TID_WEAK_OBJECT_REFERENCE_SET      = 7;

// version 0.7 and below
const int OLD_TID_DATA                           = 0;
const int OLD_TID_STRONG_OBJECT_REFERENCE        = 1;
const int OLD_TID_STRONG_OBJECT_REFERENCE_VECTOR = 2;
const int OLD_TID_WEAK_OBJECT_REFERENCE          = 3;
const int OLD_TID_WEAK_OBJECT_REFERENCE_VECTOR   = 5;
const int OLD_TID_DATA_STREAM                    = 4;

// Integral types
//
typedef signed char        OMInt8;
typedef signed short int   OMInt16;
typedef signed int         OMInt32;

typedef unsigned char      OMUInt8;
typedef unsigned short int OMUInt16;
typedef unsigned int       OMUInt32;

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

// Byte ordering
//
typedef OMUInt16 ByteOrder;
const ByteOrder unspecifiedEndian = 0;
const ByteOrder littleEndian      = 0x4949;
const ByteOrder bigEndian         = 0x4d4d;

// Stream names and punctuation
//
const char* const propertyIndexStreamName = "property index";
const char* const propertyValueStreamName = "property values";
const char* const openArrayKeySymbol = "{";
const char* const closeArrayKeySymbol = "}";

// CLSID for AAFHeader 
// Up to and including version 0.3
// {B1A21383-1A7D-11D2-BF78-00104BC9156D}
const CLSID OldCLSID_AAFHeader =
  { 0xB1A21383,
    0x1A7D, 0x11D2,
  { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

// Version 0.4 and up - SMPTE style stored class id
//{06480000-0000-0000-060E-2B3401010104}
const CLSID CLSID_AAFHeader =
  { 0x06480000,
    0x0000, 0x0000,
  { 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04 } };

// End of stuff that should be in a shared header

// A note on file format versions.
//
// History
//
// Version : Changes
//
//  0.1    : initial version
//  0.2    : add byte order flag to the property index, change stream names
//  0.3    : change property ids to be unique within a file (this
//           includes using automatically generated PIDs and changes to
//           property names)
//  0.4    : change to the use of SMPTE conformant stored class ids
//  0.5    : change the value of many pids because of the addition of the
//           new plugin related classes
//  0.6    : change property names to match those in the dictionary
//  0.7    : change AAFSourceMob::MediaDescription to
//           AAFSourceMob::EssenceDescription.
//  0.8    : remove AAFPluggableDef. Renumbered TID_* values.
//  0.9    : Effect -> OperationGroup, EffectDefinition -> OperationDefinition,
//           PlugInDescriptors -> PluginDescriptors.
//
//

// The following may change at run time depending on the file format
// version.
//
char* _propertyValueStreamName = (char*)propertyValueStreamName;
char* _openArrayKeySymbol = (char*)openArrayKeySymbol;
char* _closeArrayKeySymbol = (char*)closeArrayKeySymbol;

// Highest version of file/index format recognized by this dumper
//
const OMUInt32 HIGHVERSION = 9;

// Output format requested
//
enum optionType {raw, property, aaf};
enum optionType option = raw; // default
bool zFlag = false;
bool mFlag = false;
unsigned long int mLimit = 0;

// Statistics gathering
//
size_t totalStorages;
size_t totalStreams;

size_t totalPropertyBytes;
size_t totalObjects;
size_t totalProperties;

size_t totalStreamBytes;
size_t totalFileBytes;

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
static void warning(char* routineName, char* message);
static void printError(const char* prefix,
                       const char* fileName,
                       DWORD errorCode);
static int check(const char* fileName, DWORD resultCode);
static int checks(DWORD resultCode);
#if defined(_WIN32) || defined(UNICODE)
static void convert(wchar_t* wcName, size_t length, const char* name);
static void convert(char* cName, size_t length, const wchar_t* name);
#endif
static void convert(char* cName, size_t length, const char* name);
static void convertName(char* cName,
                        size_t length,
                        OMCHAR* wideName,
                        char** tag);
static void indent(int level);
static void printClsid(REFCLSID clsid);
static void openStream(IStorage* storage,
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
#if !defined (__GNUC__)
static void readUInt8(IStream* stream, OMUInt8* value);
static void readUInt16(IStream* stream, OMUInt16* value, bool swapNeeded);
#endif
static void readUInt32(IStream* stream, OMUInt32* value, bool swapNeeded);
#if !defined (__GNUC__)
static void swapUInt16(OMUInt16* value);
#endif
static void swapUInt32(OMUInt32* value);
static void dumpIndexEntry(OMUInt32 i, IndexEntry* indexEntry);
static void printIndex(IndexEntry* index, OMUInt32 entries);
static void readIndexEntry(IStream* stream,
                           IndexEntry* entry,
                           bool swapNeeded);
static IndexEntry* readIndex(IStream* stream, OMUInt32 count, bool swapNeeded);
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
                             OMUInt32 highWaterMark);
static void readVectorIndexEntry(IStream* stream,
                           VectorIndexEntry* entry,
                           bool swapNeeded);
static VectorIndexEntry* readVectorIndex(IStream* stream,
                                         OMUInt32 count,
                                         bool swapNeeded);
static ByteOrder readByteOrder(IStream* stream);
static void dumpObject(IStorage* storage, char* pathName, int isRoot);
static OMUInt32 typeOf(IndexEntry* entry, OMUInt32 version);
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
                           const char* streamName);
static void dumpProperties(IStorage* storage,
                           IndexEntry* index,
                           OMUInt32 entries,
                           OMUInt32 version,
                           char* pathName,
                           int isRoot,
                           bool swapNeeded);
static void openStorage(char* fileName, IStorage** storage);
static void dumpFile(char* fileName);
static void dumpFileProperties(char* fileName);
static bool isAnAAFFile(const char* fileName);
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
};

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
  
  long int savedFlags = cout.setf(ios::basefield);
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
    if (isprint(_buffer[i])) {
      cout << (char)_buffer[i];
    } else {
      cout << '.';
    }
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

Dumper dumper;

char* baseName(char* fullName)
{
  char* result;
#if defined(WIN32)
  const int delimiter = '\\';
#elif defined(_MAC) || defined(macintosh)
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

  exit(FAILURE);
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
#if defined(_WIN32) || defined(WIN32)
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

  exit(FAILURE);
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

#if defined(_WIN32) || defined(UNICODE)

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

#if defined(__sgi)

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

void printClsid(REFCLSID clsid)
{
  char cs[256];

  if (!IsEqualCLSID(CLSID_NULL, clsid)) {
    OMCHAR s[256];
    int result = StringFromGUID2(clsid, s, 256);
    if (result <= 0) {
      strcpy(cs, "unknown");
    } else {
      convert(cs, 256, s);
    }
  } else {
    strcpy(cs, "null");
  }
  cout << cs << endl;
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

size_t sizeOfStream(IStream* stream, const char* streamName)
{
  STATSTG statstg;
  HRESULT result = stream->Stat(&statstg, STATFLAG_NONAME);
  if (!check(streamName, result)) {
    fatalError("sizeOfStream", "Falied to Stat() stream.");
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

#if !defined(__GNUC__)

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

#endif

void readUInt32(IStream* stream, OMUInt32* value, bool swapNeeded)
{
  read(stream, value, sizeof(OMUInt32));
  if (swapNeeded) {
    swapUInt32(value);
  }
}

#if !defined(__GNUC__)

void swapUInt16(OMUInt16* value)
{
  OMUInt8* p = (OMUInt8*)value;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[1];
  p[1] = temp;

}

#endif

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

void dumpIndexEntry(OMUInt32 i, IndexEntry* indexEntry)
{
  ASSERT("Valid index entry", indexEntry != 0);
  cout << setw(12) << i;
  cout << setw(12) << hex
                  << indexEntry->_pid
                  << dec;
  cout << setw(12) << indexEntry->_type;
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
    cout << setw(12) << "type";
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

void readIndexEntry(IStream* stream, IndexEntry* entry, bool swapNeeded)
{
  // Reading native index entries not yet supported.
  // Instead read the entire index.
  //
  ASSERT("Swap needed", swapNeeded);
  if (!swapNeeded) {
    // NYI
  } else {
    IndexEntry newEntry;
    readUInt32(stream, &newEntry._pid, swapNeeded);
    readUInt32(stream, &newEntry._type, swapNeeded);
    readUInt32(stream, &newEntry._offset, swapNeeded);
    readUInt32(stream, &newEntry._length, swapNeeded);
    memcpy(entry, &newEntry, sizeof(IndexEntry));
  }
}

IndexEntry* readIndex(IStream* stream, OMUInt32 count, bool swapNeeded)
{
  IndexEntry* result = new IndexEntry[count];
  ASSERT("Successfully allocated index array", result != 0);
  if (!swapNeeded) {
    read(stream, result, sizeof(IndexEntry) * count);
  } else {
    for (OMUInt32 i = 0; i < count; i++) {
      readIndexEntry(stream, &result[i], swapNeeded);
    }
  }
  return result;
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
      fatalError("isValid", "Property set index entry has zero length.");
      result = false;
      break;
    }
    if (i == 0) {
      // First entry
      previousOffset = currentOffset;
      position = currentOffset + currentLength;
    } else {
      // Subsequent entries
      if (currentOffset < previousOffset) {
        warning("isValid", "Property set index entries out of order.");
        result = false;
        break;
      } else if (position > currentOffset) {
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
  size_t result;

  if (entries != 0) {
    size_t last = entries - 1;
    result = index[last]._offset + index[last]._length;
  } else {
    result = 0;
  }
  return result;
}

char* typeName(OMUInt32 type)
{
  char * result;
  
  switch (type) {
    
  case TID_DATA:
    result = "data";
    break;
    
  case TID_DATA_STREAM:
    result = "data stream";
    break;

  case TID_STRONG_OBJECT_REFERENCE:
    result = "strong object reference";
    break;
    
  case TID_STRONG_OBJECT_REFERENCE_VECTOR:
    result = "strong object reference vector";
    break;

  case TID_STRONG_OBJECT_REFERENCE_SET:
    result = "strong object reference set";
    break;
    
  case TID_WEAK_OBJECT_REFERENCE:
    result = "weak object reference";
    break;

  case TID_WEAK_OBJECT_REFERENCE_VECTOR:
    result = "weak object reference vector";
    break;

  case TID_WEAK_OBJECT_REFERENCE_SET:
    result = "weak object reference set";
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
       << setw(8) << vectorIndexEntry->_elementName << endl;
}

void printVectorIndex(VectorIndexEntry* vectorIndex,
                      OMUInt32 count,
                      OMUInt32 highWaterMark)
{
  cout << "Dump of vector index" << endl;
  cout << "( High water mark = " << highWaterMark
       << ", Number of entries = " << count << " )" << endl;

  if (count > 0) {
    cout << setw(8) << "index"
         << "   "
         << setw(8) << "key" << endl;

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

OMUInt32 typeOf(IndexEntry* entry, OMUInt32 version)
{
  OMUInt32 result;

  if (version > 7) {
    result = entry->_type;
  } else {
    switch (entry->_type) {
      case OLD_TID_DATA:
        result = TID_DATA; 
        break;
      case OLD_TID_STRONG_OBJECT_REFERENCE:
        result = TID_STRONG_OBJECT_REFERENCE;
        break;
      case OLD_TID_STRONG_OBJECT_REFERENCE_VECTOR:
        result = TID_STRONG_OBJECT_REFERENCE_VECTOR;
        break;
      case OLD_TID_WEAK_OBJECT_REFERENCE:
        result = TID_WEAK_OBJECT_REFERENCE;
        break;
      case OLD_TID_WEAK_OBJECT_REFERENCE_VECTOR:
        result = TID_WEAK_OBJECT_REFERENCE_VECTOR;
        break;
      case OLD_TID_DATA_STREAM:
        result = TID_DATA_STREAM;
        break;
      default:
        break;
    }
  }
  return result;
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
  for (OMUInt32 i = 0; i < entries; i++) {

    switch (typeOf(&index[i], version)) {
      
    case TID_DATA:
      // value is dumped when the property value stream is dumped
      break;

    case TID_DATA_STREAM: {
      char* subStreamName = new char[index[i]._length];
      read(propertiesStream,
           index[i]._offset,
           subStreamName,
           index[i]._length);
      
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
      dumpDataStream(stream, thisPathName, subStreamName);

      stream->Release();
      stream = 0;

      delete [] subStreamName;
      subStreamName = 0;
    }
    break;

    case TID_STRONG_OBJECT_REFERENCE: {
      // get name of sub-storage
      //
      char* subStorageName = new char[index[i]._length];
      read(propertiesStream,
           index[i]._offset,
           subStorageName,
           index[i]._length);
      
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
      dumpObject(subStorage, thisPathName, 0);

      delete [] subStorageName;
      subStorageName = 0;
    }
    break;

    case TID_STRONG_OBJECT_REFERENCE_VECTOR:
    case TID_STRONG_OBJECT_REFERENCE_SET : {
      // get name of vector index
      //
      char* suffix = " index";
      char* vectorName = new char[index[i]._length];
      read(propertiesStream, index[i]._offset, vectorName, index[i]._length);

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

      OMUInt32 _highWaterMark;
      readUInt32(subStream, &_highWaterMark, swapNeeded);

      OMUInt32 _count;
      readUInt32(subStream, &_count, swapNeeded);
      
      // Read the vector index.
      //
      VectorIndexEntry* vectorIndex = readVectorIndex(subStream,
                                                      _count,
                                                      swapNeeded);

      // dump the vector index
      //
      cout << endl;
      cout << thisPathName << endl;
      printVectorIndex(vectorIndex, _count, _highWaterMark);

      // for each vector index entry
      //
      for (OMUInt32 entry = 0; entry < _count; entry++) {
        //   compute storage name
        char* elementName = new char[strlen(vectorName) + 1 + 1];
        strcpy(elementName, vectorName);
        strcat(elementName, _openArrayKeySymbol);
        
        char number[256];
        sprintf(number, "%x", vectorIndex[entry]._elementName);
        
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
        dumpObject(subStorage, thisPathName, 0);

        delete [] elementName;
        elementName = 0;

        delete [] subStorageName;
        subStorageName = 0;
      }

      delete [] vectorName;
      vectorName = 0;

      delete [] vectorIndexName;
      vectorIndexName = 0;

      delete [] vectorIndex;
      vectorIndex = 0;
    }
    break;

    case TID_WEAK_OBJECT_REFERENCE:
      // value is dumped when the property value stream is dumped
      break;

    case TID_WEAK_OBJECT_REFERENCE_VECTOR:
    case TID_WEAK_OBJECT_REFERENCE_SET:
      // value is dumped when the property value stream is dumped
      break;

    default:
      break;
    }

  }

}

void dumpDataStream(IStream* stream,
                    const char* pathName,
                    const char* streamName)
{
  cout << endl;
  cout << pathName << endl;
  cout << "Dump of data stream" << endl;

  STATSTG statstg;
  HRESULT result = stream->Stat(&statstg, STATFLAG_NONAME);
  if (!check(streamName, result)) {
    fatalError("dumpdataStream", "Falied to Stat() stream.");
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
           << mLimit
           << " )"
           << endl;
    } else {
      cout << "( Size = "
           << streamBytes
           << ", smaller than output limit of "
           << mLimit
           << " )"
           << endl;
    }
  } else {
    cout << "( Size = "
         << streamBytes
         << " )"
         << endl;
  }

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
                    IndexEntry* index,
                    OMUInt32 entries,
                    OMUInt32 version,
                    char* pathName,
                    int isRoot,
                    bool swapNeeded)
{
  IStream* stream = 0;
  openStream(storage, _propertyValueStreamName, &stream);

  if (stream == 0) {
    fatalError("dumpProperties", "openStream() failed.");
  }

  // Check that the property value stream is the correct size for the
  // given index.
  //
  size_t actualStreamSize = sizeOfStream(stream, _propertyValueStreamName);
  size_t expectedStreamSize = valueStreamSize(index, entries);

  if (actualStreamSize < expectedStreamSize) {
    fatalError("dumpProperties", "Property value stream too small.");
  }
  totalStreamBytes = totalStreamBytes + actualStreamSize;

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
    cout << "property " << i << " "
         << "( "
         << typeName(typeOf(&index[i], version))
         << " )" << endl;

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

  stream->Release();
  stream = 0;

}

ByteOrder fileByteOrder = unspecifiedEndian;

ByteOrder readByteOrder(IStream* stream)
{
  // Reset format version dependent parameters.
  //
  _propertyValueStreamName = (char*)propertyValueStreamName;
  _openArrayKeySymbol = (char*)openArrayKeySymbol;
  _closeArrayKeySymbol = (char*)closeArrayKeySymbol;

  OMUInt16 byteOrder;
  read(stream, &byteOrder, sizeof(byteOrder));

  ByteOrder result;

  if (byteOrder == littleEndian) {              // explicitly little endian
    result = littleEndian;
  } else if (byteOrder == bigEndian) {          // explicitly big endian
    result = bigEndian;
  } else if ((byteOrder == 0x0001) || (byteOrder == 0x0000)) {

    // Handle old (version 1) prototype files with no byte order flag.
    // Here we use the version number as the byte order flag.
    //

    // Read version number from start of stream.
    //
    OMUInt32 version;
    read(stream, 0, &version, sizeof(OMUInt32)); 

    ByteOrder hostOrder = hostByteOrder();
    if (version == 0x00000001) {                // native version number 1 
      result = hostOrder;
    } else if (version == 0x01000000) {         // foreign version number 1
      if (hostOrder == littleEndian) {
        result = bigEndian;
      } else {
        result = littleEndian;
      }
    } else {
      fatalError("readByteOrder", "Can't determine byte order.");
    }

    // Set format version dependent parameters.
    //
    _propertyValueStreamName = "properties";
    _openArrayKeySymbol = "[";
    _closeArrayKeySymbol = "]";

    // Seek back to start of stream.
    //
    LARGE_INTEGER newPosition = {0, 0};
    ULARGE_INTEGER oldPosition;
    HRESULT status = stream->Seek(newPosition, STREAM_SEEK_SET, &oldPosition);
    if (!checks(status)) {
      fatalError("readByteOrder", "IStream::Seek() failed.");
    }

  } else {
    fatalError("readByteOrder", "Can't determine byte order.");
  }

  ASSERT("Valid result", (result == littleEndian) || (result == bigEndian));
  return result;
}

void dumpObject(IStorage* storage, char* pathName, int isRoot)
{
  totalObjects = totalObjects + 1; // Count this object

  IStream* stream = 0;
  openStream(storage, propertyIndexStreamName, &stream);

  if (stream == 0) {
    fatalError("dumpObject", "Property index stream not found.");
  }

  // Check that the stream is not empty.
  //
  size_t indexStreamSize = sizeOfStream(stream, propertyIndexStreamName);
  if (indexStreamSize == 0){
    fatalError("dumpObject", "Property index stream empty.");
  }
  totalStreamBytes = totalStreamBytes + indexStreamSize;

  OMUInt16 _byteOrder = readByteOrder(stream);

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

  OMUInt32 _formatVersion;
  readUInt32(stream, &_formatVersion, swapNeeded);

  OMUInt32 _entryCount;
  readUInt32(stream, &_entryCount, swapNeeded);

  // Compute the header size which is as follows ...
  //
  //   _byteOrder     2 (_formatVersion >= 2)
  //   _formatVersion 4
  //   _entryCount    4
  //
  size_t headSize;
  if (_formatVersion < 2) {
    headSize = 8;
  } else {
    headSize = 10;
  }
  
  // Check that the stream size is consistent with the entry count
  //
  size_t expectedSize = headSize + (_entryCount * sizeof(IndexEntry));
  if (indexStreamSize < expectedSize) {
    fatalError("dumpObject", "Property index stream too small.");
  } else if (indexStreamSize > expectedSize) {
    warning("dumpObject", "Property index stream too large.");
  }

  if ((isRoot) && (_formatVersion < 3)) {
    if (zFlag) {
      cout << programName
           << ": Warning : Specifying -x with format version "
           << _formatVersion
           << " is not supported." << endl;
    }
  }

  if (_formatVersion > HIGHVERSION) {
    cout << programName
         << ": Error : Unrecognized format version ("
         << _formatVersion
         << "). Highest recognized version is " << HIGHVERSION << "." << endl;
    exit(FAILURE);
  }

  // Should compare the format version for the file (the version of
  // the root object) with that for this object. Are files containing
  // mixed versions allowed ?

  cout << endl;
  cout << pathName << endl;

  char* endianity;
  if (_byteOrder == hostByteOrder()) {
    endianity = "native";
  } else {
    endianity = "foreign";
  }

  cout << "Dump of property index" << endl
       << "( Byte order = " << byteOrder(_byteOrder)
       << " (" << endianity << ")"
       << ", Version = " << _formatVersion
       << ", Number of entries = " << _entryCount << " )" << endl;

  IndexEntry* index = readIndex(stream, _entryCount, swapNeeded);
  printIndex(index, _entryCount);

  if (!isValid(index, _entryCount)) {
    fatalError("dumpObject", "Invalid property set index.");
  }

  cout << "Dump of properties" << endl;
  dumpProperties(storage,
                 index,
                 _entryCount,
                 _formatVersion,
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
    exit(FAILURE);
    break;
    
  case STG_E_FILENOTFOUND:
    cerr << programName <<": Error: "
         << "File \"" << fileName << "\" not found."
         << endl;
    exit(FAILURE);
    break;
  case STG_E_PATHNOTFOUND:
    cerr << programName <<": Error: "
         << "Path \"" << fileName << "\" not found."
         << endl;
    exit(FAILURE);
    break;
    
  }
  
  if (!check(fileName, result)) {
    fatalError("openStorage", "StgIsStorageFile() failed.");
  }
  
  if (result != S_OK) {
    cout << "\"" << fileName << "\" is NOT a structured storage file." << endl;
    exit(FAILURE);
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

void dumpFile(char* fileName)
{
  resetStatistics();

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
  
  dumpStorage(storage, &statstg, "/", 1);
  
  // Having called Stat() specifying STATFLAG_DEFAULT
  // we are responsible for freeing statstg.pwcsName.
  //
  CoTaskMemFree(statstg.pwcsName);
    
  // Releasing the last reference to the root storage closes the file.
  storage->Release();

  totalFileBytes = fileSize(fileName);
  printStatistics();
}

void dumpFileProperties(char* fileName)
{
  resetStatistics();

  IStorage* storage = 0;
  openStorage(fileName, &storage);

  if (storage == 0) {
    fatalError("dumpFileProperties", "openStorage() failed.");
  }

  dumpObject(storage, "/", 1);
    
  // Releasing the last reference to the root storage closes the file.
  storage->Release();
  storage = 0;

  totalFileBytes = fileSize(fileName);
  printStatistics();

}

static bool isAnAAFFile(const char* fileName)
{
  bool result = false;
  IStorage* storage = 0;
  STATSTG statstg;
  HRESULT status;
  OMCHAR wcFileName[256];

  convert(wcFileName, 256, fileName);

  status = StgOpenStorage(
    wcFileName,
    NULL,
    STGM_READ | STGM_SHARE_DENY_WRITE,
    NULL,
    0,
    &storage);

  if (!check(fileName, status)) {
    result = false;
  } else {
    status = storage->Stat(&statstg, STATFLAG_NONAME);
    if (!check(fileName, status)) {
      result = false;
    } else {
      if (IsEqualCLSID(statstg.clsid, OldCLSID_AAFHeader)) {
        result = true;
      } else if (IsEqualCLSID(statstg.clsid, CLSID_AAFHeader)) {
        result = true;
      } else {
        result = false;
      }
    }
  }
  return result;
}

void usage(void)
{
  cerr << programName << ": Usage : "
       << programName << " [-r -p -a -s -z <pid> -m <n> -h] <file...>" << endl;
  cerr << "-r       = raw dump"
       << " : for any structured storage file (default)." << endl;
  cerr << "-p       = property dump"
       << " : for any file using the AAF stored object format." << endl;
  cerr << "-a       = AAF file dump"
       << " : for any AAF file." << endl;
  cerr << "-s       = print statistics"
       << " : combine with -r, -p and -a." << endl;
  cerr << "-z <pid> = dump properties with pid <pid> (hex) as all zeros :"
       << endl
       << "             combine with -p and -a." << endl;
  cerr << "-m <n>   = dump only the first <n> bytes (dec) of media streams :"
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
    long oldFlags = cout.flags(ios::right | ios::fixed);
    cout << label
         << setw(8)
         << setprecision(2)
         << value
         << endl;
    cout.flags(oldFlags);
}

void printFixedPercent(const double value, char* label)
{
    long oldFlags = cout.flags(ios::right | ios::fixed);
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
    exit(FAILURE);
  }
}

#if defined(__sgi)
// stubs
void CoInitialize(void *)
{
}

void CoUninitialize(void)
{
}

#endif

// helper class
struct CComInitialize
{
	CComInitialize() { CoInitialize(NULL); }
	~CComInitialize() { CoUninitialize(); }
};

int main(int argumentCount, char* argumentVector[])
{
  checkSizes();

#if defined(_MAC) || defined(macintosh)
  argumentCount = ccommand(&argumentVector); // console window for mac
#endif


  // Initialize com library for this process.
  CComInitialize comInit;


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
      case 'r':
        option = raw;
        break;
      case 'p':
        option = property;
        break;
      case 'a':
        option = aaf;
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
            exit(FAILURE);
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
          exit(FAILURE);
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
            exit(FAILURE);
          }
          mLimit = bytes;
        } else {
          cerr << programName
               << ": Error : -m must be followed by a byte count."
               << endl;
          usage();
          exit(FAILURE);
        }
        break;
      case 'h':
        usage();
        exit(SUCCESS);
        break;
      default:
        cerr << programName
             << ": Error : -"
             << flag
             << " is not a recognized option."
             << endl;
        usage();
        exit(FAILURE);
        break;
      }
      flagCount = flagCount + 1;
    } else {
      fileCount = fileCount + 1;
    }
  }

  if (option == raw) {
    if (zFlag) {
      cerr << programName
           << ": Error : -z not valid with -r."
           << endl;
      usage();
      exit(FAILURE);
    }
  }

  if (fileCount < 1) {
    cerr << programName
      << ": Error : Wrong number of arguments ("
      << fileCount << ")."
      << endl;
    usage();
    exit(FAILURE);
  }
  
  switch (option) {
  case raw:
    for (i = flagCount + 1; i < argumentCount; i++) {
      cout << "Dump of structured storage file"
           << " \"" << argumentVector[i] << "\""
           << " (raw)." << endl;
      dumpFile(argumentVector[i]);
    }
    break;

  case property:
    for (i = flagCount + 1; i < argumentCount; i++) {
      cout << "Dump of structured storage file"
           << " \"" << argumentVector[i] << "\""
           << " (properties)." << endl;
      dumpFileProperties(argumentVector[i]);
    }
    break;

  case aaf:
    for (i = flagCount + 1; i < argumentCount; i++) {
      cout << "Dump of structured storage file"
           << " \"" << argumentVector[i] << "\""
           << " (AAF properties)." << endl;
      if (isAnAAFFile(argumentVector[i])) {
        dumpFileProperties(argumentVector[i]);
      } else {
        cerr << programName
             << ": Error : \""
             << argumentVector[i]
             << "\" is not an AAF file."
             << endl;
      }
    }
    break;

  }

  return (SUCCESS);
}

