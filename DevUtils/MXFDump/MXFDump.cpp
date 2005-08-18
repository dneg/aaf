#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long int mxfLength;
typedef unsigned char mxfKey[16];
typedef unsigned char mxfByte;

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
size_t address;

void init(void)
{
  bufferIndex = 0;
  address = 0;
}

void flush(void)
{
  fprintf(stdout, "%8d", address);
  fprintf(stdout, "    ");
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
  fprintf(stderr, "--raw-dump      = ");
  fprintf(stderr, "dump raw KLV (-r)\n");
  fprintf(stderr, "  --limit <n>   = ");
  fprintf(stderr, "dump only the first <n> bytes of each value (-l)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "--mxf-dump      = ");
  fprintf(stderr, "dump MXF (-m)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "--help          = ");
  fprintf(stderr, "print this messsage and exit (-h)\n");
  fprintf(stderr, "--verbose       = ");
  fprintf(stderr, "print more detailed information (-v)\n");
}

const char* baseName(char* pathName);

const char* baseName(char* pathName)
{
  const char* result;
  const int delimiter = '\\';
  result = strrchr(pathName, delimiter);
  if (result == 0) {
    result = pathName;
  } else if (strlen(result) == 0) {
    result = pathName;
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
  fprintf(f, "%12d (%8x)", l, l);
}

void readMxfKey(mxfKey& k, FILE* f);

void readMxfKey(mxfKey& k, FILE* f)
{
  int c = fread(&k, sizeof(mxfKey), 1, f);
  if (c != 1) {
    fprintf(stderr, "%s : Error : Failed to read key.\n", programName);
    exit(EXIT_FAILURE);
  }
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

void mxfDumpFile(char* fileName);

void mxfDumpFile(char* /* fileName */)
{
  fprintf(stderr,
          "%s : Error : --mxf-dump not yet implemented.\n",
          programName);
  printUsage();
  exit(EXIT_FAILURE);
}

bool verbose = false;
bool lFlag;
mxfLength limit = 0;

void rawDumpFile(char* fileName);

void rawDumpFile(char* fileName)
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

  while (!feof(infile)) {
    mxfKey k;
    readMxfKey(k, infile);
    printMxfKey(k, stdout);
    fprintf(stdout, "  ");
    mxfLength len;
    readMxfLength(len, infile);
    printMxfLength(len, stdout);
    fprintf(stdout, "\n");

    mxfLength count = len;
    if (lFlag) {
      count = limit;
    }
    init();
    for (mxfLength i = 0; i < count; i++) {
      mxfByte b;
      readMxfByte(b, infile);
      dumpByte(b);
    }
    flush();
    if (count < len) {
      for (mxfLength i = count; i < len; i++) {
        mxfByte b;
        readMxfByte(b, infile);
      }
    }
  }
  fclose(infile);
}

typedef enum ModeTag {
  unspecifiedMode,
  rawMode,
  mxfMode} Mode;
Mode mode = unspecifiedMode;

void setMode(Mode m);

void setMode(Mode m)
{
  if (mode != unspecifiedMode) {
    fprintf(stderr,
            "%s : Error : Specify only one of --raw-dump, --mxf-dump.\n",
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
  programName = baseName(argumentVector[0]);
  checkSizes();
  int fileCount = 0;
  int fileArg = 0;
  char* p = 0;
  for (int i = 1; i < argumentCount; i++) {
    p = argumentVector[i];
    if ((strcmp(p, "-r") == 0) || (strcmp(p, "--raw-dump") == 0)) {
      setMode(rawMode);
    } else if ((strcmp(p, "-m") == 0) || (strcmp(p, "--mxf-dump") == 0)) {
      setMode(mxfMode);
    } else if ((strcmp(p, "-v") == 0) || (strcmp(p, "--verbose") == 0)) {
      verbose = true;
    } else if ((strcmp(p, "-l") == 0) ||(strcmp(p, "--limit") == 0)) {
      if ((i + 1 < argumentCount) && (*argumentVector[i + 1] != '-' )) {
        lFlag = true;
        i = i + 1;
        char* bytess = argumentVector[i];
        int bytes;
        if (!getInteger(bytes, bytess)) {
          fprintf(stderr, 
                  ": Error : \"%s\" is not a valid byte count.",
                  programName,
                  bytess);
          printUsage();
          exit(EXIT_FAILURE);
        }
        limit = bytes;
      } else {
        fprintf(stderr,
                "%s : Error : \"%s\" must be followed by a byte count.",
                programName,
                p);
        printUsage();
        exit(EXIT_FAILURE);
      }
    } else if ((strcmp(p, "-h") == 0) || (strcmp(p, "--help") == 0)) {
      printUsage();
      exit(EXIT_SUCCESS);
    } else if (*p == '-') {
      fprintf(stderr, "%s : Error : Invalid option \"%s\".\n", programName, p);
      printUsage();
      exit(EXIT_FAILURE);
    } else {
      fileCount = fileCount + 1;
      fileArg = i;
    }
  }
  if (mode == unspecifiedMode) {
    mode = mxfMode;
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
              "dumping only the first %d bytes of values.\n",
              limit);
    }
  }
  if (mode == mxfMode) {
    mxfDumpFile(fileName);
  } else {
    rawDumpFile(fileName);
  }
  return 0;  
}
