// KLV/MXF file dumper.
//
// Tim Bingham - October 2002 - Tim_Bingham@avid.com

// This program is deliberately constructed as one file with no dependencies
// so that it can easily be ported to a new host. You'll be able to make some
// progress understanding an MXF file even if all you have is this source and
// a C++ compiler.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (_MSC_VER) && defined(_M_IX86) && defined(_WIN32)
#define MXF_COMPILER_MSC_INTEL_WINDOWS
#define MXF_OS_WINDOWS
#elif defined(__GNUC__) && defined(__i386__) && defined(__linux__)
#define MXF_COMPILER_GCC_INTEL_LINUX
#define MXF_OS_UNIX
#elif defined(__MWERKS__) && defined(__POWERPC__) && defined(macintosh)
#define MXF_COMPILER_MWERKS_PPC_MACOS
#define MXF_OS_MACOS
#elif defined(__MWERKS__) && defined(__MACH__)
#define MXF_COMPILER_MWERKS_PPC_MACOSX
#define MXF_OS_MACOSX
#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__)
#define MXF_COMPILER_GCC_PPC_MACOSX
#define MXF_OS_MACOSX
#else
#error "Unknown compiler"
#endif

#if defined(MXF_OS_MACOS)
#define MXF_USE_CONSOLE
#endif

#if defined(MXF_USE_CONSOLE)
#include <console.h>
#endif

#if defined(MXF_COMPILER_MSC_INTEL_WINDOWS)
typedef unsigned char          mxfUInt08;
typedef unsigned short int     mxfUInt16;
typedef unsigned int           mxfUInt32;
typedef unsigned _int64        mxfUInt64;
#elif defined(MXF_COMPILER_GCC_INTEL_LINUX)
typedef unsigned char          mxfUInt08;
typedef unsigned short int     mxfUInt16;
typedef unsigned long int      mxfUInt32;
typedef unsigned long long int mxfUInt64;
#elif defined(MXF_COMPILER_MWERKS_PPC_MACOS)
typedef unsigned char          mxfUInt08;
typedef unsigned short int     mxfUInt16;
typedef unsigned long int      mxfUInt32;
typedef unsigned long long int mxfUInt64;
#elif defined(MXF_COMPILER_MWERKS_PPC_MACOSX)
typedef unsigned char          mxfUInt08;
typedef unsigned short int     mxfUInt16;
typedef unsigned long int      mxfUInt32;
typedef unsigned long long int mxfUInt64;
#elif defined(MXF_COMPILER_GCC_PPC_MACOSX)
typedef unsigned char          mxfUInt08;
typedef unsigned short int     mxfUInt16;
typedef unsigned long int      mxfUInt32;
typedef unsigned long long int mxfUInt64;
#endif

typedef mxfUInt32 mxfLength;
typedef mxfUInt08 mxfByte;
typedef mxfByte mxfKey[16];
typedef mxfByte  mxfLocalKey[2];

mxfUInt08 hostByteOrder(void);

mxfUInt08 hostByteOrder(void)
{
  mxfUInt16 word = 0x1234;
  mxfUInt08  byte = *((mxfUInt08*)&word);
  mxfUInt08 result;

  if (byte == 0x12) {
    result = 'B';
  } else {
    result = 'L';
  }
  return result;
}

bool reorder(void);

bool reorder(void)
{
  bool result;
  if (hostByteOrder() == 'B') {
    result = false;
  } else {
    result = true;
  }
  return result;
}

void printField(FILE* f, mxfUInt32& i);

void printField(FILE* f, mxfUInt32& i)
{
#if defined(MXF_COMPILER_MSC_INTEL_WINDOWS)
  fprintf(f, "%10u", i);
#elif defined(MXF_COMPILER_GCC_INTEL_LINUX)
  fprintf(f, "%10lu", i);
#elif defined(MXF_COMPILER_MWERKS_PPC_MACOS)
  fprintf(f, "%10lu", i);
#elif defined(MXF_COMPILER_MWERKS_PPC_MACOSX)
  fprintf(f, "%10lu", i);
#elif defined(MXF_COMPILER_GCC_PPC_MACOSX)
  fprintf(f, "%10lu", i);
#endif
}

void printHexField(FILE* f, mxfUInt32& i);

void printHexField(FILE* f, mxfUInt32& i)
{
#if defined(MXF_COMPILER_MSC_INTEL_WINDOWS)
  fprintf(f, "%08x", i);
#elif defined(MXF_COMPILER_GCC_INTEL_LINUX)
  fprintf(f, "%08lx", i);
#elif defined(MXF_COMPILER_MWERKS_PPC_MACOS)
  fprintf(f, "%08lx", i);
#elif defined(MXF_COMPILER_MWERKS_PPC_MACOSX)
  fprintf(f, "%08lx", i);
#elif defined(MXF_COMPILER_GCC_PPC_MACOSX)
  fprintf(f, "%08lx", i);
#endif
}

#define MXF_LABEL(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) \
                 {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p}

#define MXF_DEFINE_PACK_KEY(n, k)     {#n, k},
#define MXF_DEFINE_SET_KEY(n, k)      {#n, k},
#define MXF_DEFINE_SEGEMENT_KEY(n, k) {#n, k},
#define MXF_DEFINE_KEY(n, k)          {#n, k},

struct {
  const char* _name;
  mxfKey _key;
} keyTable [] = {
#include "MXFMetaDictionary.h"
{"bogus", {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
};

size_t keyTableSize = (sizeof(keyTable)/sizeof(keyTable[0])) - 1;

const mxfLocalKey nullMxfLocalKey = {0, 0};

const mxfKey Primer = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
                       0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00};
const mxfKey Filler = {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
                       0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

const char* programName;

void checkSizes(void);

void checkSizes(void)
{
  if (sizeof(mxfLength) != 4) {
    fprintf(stderr, "%s : Error : Wrong sizeof(mxfLength).\n", programName);
    exit(EXIT_FAILURE);
  }
  if (sizeof(mxfKey) != 16) {
    fprintf(stderr, "%s : Error : Wrong sizeof(mxfKey).\n", programName);
    exit(EXIT_FAILURE);
  }
  if (sizeof(mxfByte) != 1) {
    fprintf(stderr, "%s : Error : Wrong sizeof(mxfByte).\n", programName);
    exit(EXIT_FAILURE);
  }
}

char map(int c);
void init(void);
void flush(void);
void dumpByte(mxfByte byte);

// Interpret values 0x00 - 0x7f as ASCII characters.
//
static const char table[128] = {
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

char map(int c)
{
  char result;
  if (c < 0x80) {
    result = table[c & 0x7f];
  } else {
    result = '.';
  }
  return result;
}

unsigned char buffer[16];
size_t bufferIndex;
mxfUInt32 address;

void init(void)
{
  bufferIndex = 0;
  address = 0;
}

void flush(void)
{
  printField(stdout, address);
  fprintf(stdout, "  ");
  for (size_t i = 0; i < bufferIndex; i++) {
    fprintf(stdout, "%02x ", buffer[i]);
  }
  fprintf(stdout, "   ");
  for (size_t j = 0; j < 16 - bufferIndex; j++) {
    fprintf(stdout, "   ");
  }
  for (size_t k = 0; k < bufferIndex; k++) {
    char c = map(buffer[k]);
    fprintf(stdout, "%c", c);
  }
  fprintf(stdout, "\n");
}

void dumpByte(mxfByte byte)
{
  if (bufferIndex == 16) {
    flush();
    bufferIndex = 0;
    address = address + 16;
  }
  buffer[bufferIndex++] = byte;
}

void printUsage(void);

void printUsage(void)
{
  fprintf(stderr,
          "%s : Usage : %s OPTIONS <file>\n",
          programName,
          programName);
  fprintf(stderr, "OPTIONS :\n");
  fprintf(stderr, "--klv-dump      = ");
  fprintf(stderr, "dump raw KLV (-k)\n");
  fprintf(stderr, "  --limit <n>   = ");
  fprintf(stderr, "dump only the first <n> bytes of each value (-l)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "--mxf-dump      = ");
  fprintf(stderr, "dump MXF (-m)\n");
  fprintf(stderr, "  --filler      = ");
  fprintf(stderr, "dump filler bytes (-f)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "--symbolic      = ");
  fprintf(stderr, "dump the names of keys if known (-s)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "--help          = ");
  fprintf(stderr, "print this message and exit (-h)\n");
  fprintf(stderr, "--verbose       = ");
  fprintf(stderr, "print more detailed information (-v)\n");
  fprintf(stderr, "--debug         = ");
  fprintf(stderr, "print information useful in debugging this program (-d)\n");
}

const char* baseName(char* fullName);

const char* baseName(char* fullName)
{
  char* result;
#if defined(MXF_OS_WINDOWS)
  const int delimiter = '\\';
#elif defined(MXF_OS_MACOS)
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

void decodeMxfLength(mxfLength& l);

void decodeMxfLength(mxfLength& l)
{
  mxfLength x;
  unsigned char* p = reinterpret_cast<unsigned char*>(&l);
  x = (p[1] << 16) + (p[2] << 8) + p[3];
  l = x;
}

void readMxfByte(mxfByte& b, FILE* f);

void readMxfByte(mxfByte& b, FILE* f)
{
  int c = fread(&b, sizeof(mxfByte), 1, f);
  if (c != 1) {
    fprintf(stderr, "%s : Error : Failed to read byte.\n", programName);
    exit(EXIT_FAILURE);
  }
}

void reorder(mxfUInt16& i);

void reorder(mxfUInt16& i)
{
  mxfUInt08* p = (mxfUInt08*)&i;
  mxfUInt08 temp;

  temp = p[0];
  p[0] = p[1];
  p[1] = temp;
}

void readMxfUInt16(mxfUInt16& i, FILE* f);

void readMxfUInt16(mxfUInt16& i, FILE* f)
{
  int c = fread(&i, sizeof(mxfUInt16), 1, f);
  if (c != 1) {
    fprintf(stderr, "%s : Error : Failed to read mxfUInt16.\n", programName);
    exit(EXIT_FAILURE);
  }
  if (reorder()) {
    reorder(i);
  }
}

void reorder(mxfUInt32& i);

void reorder(mxfUInt32& i)
{
  mxfUInt08* p = (mxfUInt08*)&i;
  mxfUInt08 temp;

  temp = p[0];
  p[0] = p[3];
  p[3] = temp;

  temp = p[1];
  p[1] = p[2];
  p[2] = temp;
}

void readMxfUInt32(mxfUInt32& i, FILE* f);

void readMxfUInt32(mxfUInt32& i, FILE* f)
{
  int c = fread(&i, sizeof(mxfUInt32), 1, f);
  if (c != 1) {
    fprintf(stderr, "%s : Error : Failed to read mxfUInt32.\n", programName);
    exit(EXIT_FAILURE);
  }
  if (reorder()) {
    reorder(i);
  }
}

void readMxfLength(mxfLength& l, FILE* f);

void readMxfLength(mxfLength& l, FILE* f)
{
  int c = fread(&l, sizeof(mxfLength), 1, f);
  if (c != 1) {
    fprintf(stderr, "%s : Error : Failed to read length.\n", programName);
    exit(EXIT_FAILURE);
  }
  decodeMxfLength(l);
}

void printMxfLength(mxfLength& l, FILE* f);

void printMxfLength(mxfLength& l, FILE* f)
{
  printField(f, l);
  fprintf(f, " ");
  fprintf(f, "(");
  printHexField(f, l);
  fprintf(f, ")");
}

bool readMxfKey(mxfKey& k, FILE* f);

bool readMxfKey(mxfKey& k, FILE* f)
{
  bool result = true;
  int c = fread(&k, sizeof(mxfKey), 1, f);
  if (c != 1) {
    if (!feof(f)) {
      fprintf(stderr, "%s : Error : Failed to read key.\n", programName);
      exit(EXIT_FAILURE);
    } else {
      result = false;
    }
  }
  return result;
}

void printMxfKey(mxfKey& k, FILE* f);

void printMxfKey(mxfKey& k, FILE* f)
{
  for (size_t i = 0; i < sizeof(mxfKey); i++) {
    unsigned int b = k[i];
    fprintf(f, "%02x", b);
    if (i < (sizeof(mxfKey) - 1)) {
      fprintf(f, ".");
    }
  }
}

#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
{#name, tag},

struct {
  const char* _name;
  mxfUInt16 _key;
} localKeyTable [] = {
#include "AAFMetaDictionary.h"
{"bogus", 0x00}
};

size_t
localKeyTableSize = (sizeof(localKeyTable)/sizeof(localKeyTable[0])) - 1;

bool lookupLocalKey(mxfLocalKey& k, size_t& index);

bool lookupLocalKey(mxfLocalKey& k, size_t& index)
{
  bool result = false;
  mxfUInt16 ik = reinterpret_cast<mxfUInt16&>(k);
  if (reorder()) {
    reorder(ik);
  }
  for (size_t i = 0; i < localKeyTableSize; i++) {
    if (localKeyTable[i]._key == ik) {
      index = i;
      result = true;
      break;
    }
  }
  return result;
}

void readMxfLocalKey(mxfLocalKey& k, FILE* f);

void readMxfLocalKey(mxfLocalKey& k, FILE* f)
{
  int c = fread(&k, sizeof(mxfLocalKey), 1, f);
  if (c != 1) {
    fprintf(stderr, "%s : Error : Failed to read local key.\n", programName);
    exit(EXIT_FAILURE);
  }
}

void printMxfLocalKey(mxfLocalKey& k, FILE* f);

void printMxfLocalKey(mxfLocalKey& k, FILE* f)
{
  for (size_t i = 0; i < sizeof(mxfLocalKey); i++) {
    unsigned int b = k[i];
    fprintf(f, "%02x", b);
    if (i < (sizeof(mxfLocalKey) - 1)) {
      fprintf(f, ".");
    }
  }
}

bool symbolic = false;

void printMxfLocalKeySymbol(mxfLocalKey& k, FILE* f);

void printMxfLocalKeySymbol(mxfLocalKey& k, FILE* f)
{
  if (symbolic) {
    size_t i;
    bool found = lookupLocalKey(k, i);
    if (found) {
      fprintf(stdout, "%s\n", localKeyTable[i]._name);
    } else {
      fprintf(stdout, "Unknown\n");
    }
  } else {
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "  ");
  printMxfLocalKey(k, f);
}

bool lookupKey(mxfKey& k, size_t& index);

bool lookupKey(mxfKey& k, size_t& index)
{
  bool result = false;
  for (size_t i = 0; i < keyTableSize; i++) {
    if (memcmp(k, keyTable[i]._key, sizeof(mxfKey)) == 0) {
      index = i;
      result = true;
      break;
    }
  }
  return result;
}

void printMxfKeySymbol(mxfKey& k, FILE* f);

void printMxfKeySymbol(mxfKey& k, FILE* f)
{
  if (symbolic) {
    size_t i;
    bool found = lookupKey(k, i);
    if (found) {
      fprintf(stdout, "%s\n", keyTable[i]._name);
    } else {
      fprintf(stdout, "Unknown\n");
    }
  } else {
    fprintf(stdout, "\n");
  }
  printMxfKey(k, f);
}

void printKL(mxfKey& k, mxfLength& l);

void printKL(mxfKey& k, mxfLength& l)
{
  fprintf(stdout, "\n");
  fprintf(stdout, "[ K = ");
  printMxfKeySymbol(k, stdout);
  fprintf(stdout, ", ");
  fprintf(stdout, "L = ");
  printMxfLength(l, stdout);
  fprintf(stdout, " ]\n");
}

void printV(mxfLength& length, bool lFlag, mxfUInt32 limit, FILE* f);

void printV(mxfLength& length, bool lFlag, mxfUInt32 limit, FILE* f)
{
  init();
  for (mxfLength i = 0; i < length; i++) {
    mxfByte b;
    readMxfByte(b, f);
    if ((i < limit) || !lFlag) {
      dumpByte(b);
    }
  }
  flush();
}

void skipV(mxfLength& length, FILE* f);

void skipV(mxfLength& length, FILE* f)
{
  for (mxfLength i = 0; i < length; i++) {
    mxfByte b;
    readMxfByte(b, f);
  }
}

void printLocalKL(mxfLocalKey& k, mxfUInt16& l);

void printLocalKL(mxfLocalKey& k, mxfUInt16& l)
{
  fprintf(stdout, "  [ k = ");
  printMxfLocalKeySymbol(k, stdout);
  fprintf(stdout,
          ", l = %5d (%04x) ]\n",
          l,
          l);
}

void dumpLocalSet(mxfLength& len, FILE* infile);

void dumpLocalSet(mxfLength& len, FILE* infile)
{
  mxfLength setLength = 0;
  while (setLength < len) {
    mxfLocalKey identifier;
    readMxfLocalKey(identifier, infile);
    mxfUInt16 length;
    readMxfUInt16(length, infile);
    setLength = setLength + 4;
    printLocalKL(identifier, length);
    if (memcmp(identifier, nullMxfLocalKey, sizeof(mxfLocalKey))== 0) {
      fprintf(stdout,
              "%s : Error : Illegal local key (",
              programName);
      printMxfLocalKey(identifier, stdout);
      fprintf(stdout, ").\n");
    }
    mxfLength vLength;
    if ((setLength + length) < len) {
      vLength = length;
    } else {
      vLength = len - setLength;
    }
    printV(vLength, false, 0, infile);
    setLength = setLength + length;
    if (setLength > len) {
        mxfUInt16 remain = length - (setLength - len);
        fprintf(stdout,
                "%s : Error : Wrong size (k = ",
                programName);
        printMxfLocalKey(identifier, stdout);
        fprintf(stdout,
                ") size = %d (should be %d).\n",
                length,
                remain);
    }
  }
}

void dumpPrimer(FILE* infile);

void dumpPrimer(FILE* infile)
{
  mxfUInt32 elementCount;
  readMxfUInt32(elementCount, infile);
  mxfUInt32 elementSize;
  readMxfUInt32(elementSize, infile);
  for (mxfUInt32 j = 0; j < elementCount; j++) {
    mxfLocalKey identifier;
    readMxfLocalKey(identifier, infile);
    mxfKey longIdentifier;
    readMxfKey(longIdentifier, infile);
    fprintf(stdout, "  ");
    printMxfLocalKey(identifier, stdout);
    fprintf(stdout, " : ");
    printMxfKey(longIdentifier, stdout);
    fprintf(stdout, "\n");
  }
}

bool dumpFiller = false;

void mxfDumpFile(char* fileName);

void mxfDumpFile(char* fileName)
{
  FILE* infile;

  infile = fopen(fileName, "rb");
  if (infile == NULL) {
    fprintf(stderr,
            "%s : Error : File \"%s\" not found.\n",
            programName,
            fileName);
    exit(EXIT_FAILURE);
  }

  mxfKey k;
  while (readMxfKey(k, infile)) {
    mxfLength len;
    readMxfLength(len, infile);
    printKL(k, len);

    if (k[5] == 0x53) {
      dumpLocalSet(len, infile);
    } else if (memcmp(&Primer, &k, sizeof(mxfKey)) == 0) {
      dumpPrimer(infile);
    } else if ((memcmp(&Filler, &k, sizeof(mxfKey)) == 0) && !dumpFiller) {
      skipV(len, infile);
    } else {
      printV(len, false, 0, infile);
    }
  }
  fclose(infile);
}

bool verbose = false;
bool debug = false;
bool lFlag;
mxfUInt32 limit = 0;

void klvDumpFile(char* fileName);

void klvDumpFile(char* fileName)
{ 
  FILE* infile;

  infile = fopen(fileName, "rb");
  if (infile == NULL) {
    fprintf(stderr,
            "%s : Error : File \"%s\" not found.\n",
            programName,
            fileName);
    exit(EXIT_FAILURE);
  }

  mxfKey k;
  while (readMxfKey(k, infile)) {
    mxfLength len;
    readMxfLength(len, infile);
    printKL(k, len);
    printV(len, lFlag, limit, infile);
  }
  fclose(infile);
}

typedef enum ModeTag {
  unspecifiedMode,
  klvMode,
  mxfMode} Mode;
Mode mode = unspecifiedMode;

void setMode(Mode m);

void setMode(Mode m)
{
  if (mode != unspecifiedMode) {
    fprintf(stderr,
            "%s : Error : Specify only one of --klv-dump, --mxf-dump.\n",
            programName);
    printUsage();
    exit(EXIT_FAILURE);
  }
  mode = m;
}

bool getInteger(int& i, char* s)
{
  bool result;
  char* expectedEnd = &s[strlen(s)];
  char* end;
  int b = strtoul(s, &end, 10);
  if (end != expectedEnd) {
    result = false;
  } else {
    i = b;
    result = true;
  }
  return result;
}

int main(int argumentCount, char* argumentVector[])
{
#if defined(MXF_USE_CONSOLE)
  argumentCount = ccommand(&argumentVector);
#endif

  programName = baseName(argumentVector[0]);
  checkSizes();
  int fileCount = 0;
  int fileArg = 0;
  char* p = 0;
  for (int i = 1; i < argumentCount; i++) {
    p = argumentVector[i];
    if ((strcmp(p, "-k") == 0) || (strcmp(p, "--klv-dump") == 0)) {
      setMode(klvMode);
    } else if ((strcmp(p, "-m") == 0) || (strcmp(p, "--mxf-dump") == 0)) {
      setMode(mxfMode);
    } else if ((strcmp(p, "-v") == 0) || (strcmp(p, "--verbose") == 0)) {
      verbose = true;
    } else if ((strcmp(p, "-f") == 0) || (strcmp(p, "--filler") == 0)) {
      dumpFiller = true;
    } else if ((strcmp(p, "-l") == 0) || (strcmp(p, "--limit") == 0)) {
      if ((i + 1 < argumentCount) && (*argumentVector[i + 1] != '-' )) {
        lFlag = true;
        i = i + 1;
        char* bytess = argumentVector[i];
        int bytes;
        if (!getInteger(bytes, bytess)) {
          fprintf(stderr, 
                  "%s : Error : \"%s\" is not a valid byte count.\n",
                  programName,
                  bytess);
          printUsage();
          exit(EXIT_FAILURE);
        }
        limit = bytes;
      } else {
        fprintf(stderr,
                "%s : Error : \"%s\" must be followed by a byte count.\n",
                programName,
                p);
        printUsage();
        exit(EXIT_FAILURE);
      }
    } else if ((strcmp(p, "-s") == 0) || (strcmp(p, "--symbolic") == 0)) {
      symbolic = true;
    } else if ((strcmp(p, "-h") == 0) || (strcmp(p, "--help") == 0)) {
      printUsage();
      exit(EXIT_SUCCESS);
    } else if ((strcmp(p, "-d") == 0) || (strcmp(p, "--debug") == 0)) {
      debug = true;
    } else if (*p == '-') {
      fprintf(stderr, "%s : Error : Invalid option \"%s\".\n", programName, p);
      printUsage();
      exit(EXIT_FAILURE);
    } else {
      fileCount = fileCount + 1;
      fileArg = i;
    }
  }
  if (debug) {
    for (size_t i = 0; i < keyTableSize; i++) {
      fprintf(stdout, "%s\n", keyTable[i]._name);
      fprintf(stdout, "  ");
      printMxfKey(keyTable[i]._key, stdout);
      fprintf(stdout, "\n");
    }
  }
  if (mode == unspecifiedMode) {
    mode = mxfMode;
  }
  if (mode == mxfMode) {
    if (lFlag) {
      fprintf(stderr,
              "%s : Error : --limit not valid with --mxf-dump.\n",
              programName);
      printUsage();
      exit(EXIT_FAILURE);
    }
  } else { // mode == klvMode
    if (dumpFiller) {
      fprintf(stderr,
              "%s : Error : --filler not valid with --klv-dump.\n",
              programName);
      printUsage();
      exit(EXIT_FAILURE);
    }
  }
  int expectedFiles = 1;
  if (fileCount != expectedFiles) {
    fprintf(stderr,
            "%s : Error : Wrong number of arguments (%d).\n",
            programName,
            fileCount);
    printUsage();
    exit(EXIT_FAILURE);
  }
  char* fileName = argumentVector[fileArg];
  if (verbose) {
    fprintf(stdout, "file = %s\n", fileName);
    if (lFlag) {
      fprintf(stdout,
              "dumping only the first ");
      printField(stdout, limit);
      fprintf(stdout,
              " bytes of values.\n");
    }
  }
  if (mode == mxfMode) {
    mxfDumpFile(fileName);
  } else {
    klvDumpFile(fileName);
  }
  return 0;  
}
