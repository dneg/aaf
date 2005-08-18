#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long int mxfLength;
typedef unsigned char mxfKey[16];

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
}

void printUsage(void);

void printUsage(void)
{
  fprintf(stderr, "% : Usage : %s OPTIONS <file>\n", programName, programName);
  fprintf(stderr, "OPTIONS :\n");
  fprintf(stderr, "--raw-dump      = ");
  fprintf(stderr, "dump raw KLV (-r)");
  fprintf(stderr, "--mxf-dump      = ");
  fprintf(stderr, "dump MXF (-m)\n");
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
  fprintf(stdout, "MXF dump not yet implemented\n.");
}

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

    for (mxfLength i = 0; i < len; i++) {
      unsigned char ch;
      int c = fread(&ch, sizeof(unsigned char), 1, infile);
      if (c != 1) {
        fprintf(stderr, "%s : Error : Failed to read key.\n", programName);
        exit(EXIT_FAILURE);
      }
    }
  }
  fclose(infile);
}

bool verbose = false;

typedef enum ModeTag {
  unspecifiedMode,
  rawMode,
  mxfMode} Mode;
Mode mode = unspecifiedMode;

void setMode(Mode m);

void setMode(Mode m)
{
  if (mode != unspecifiedMode) {
    fprintf(stderr, "%s : Error : Multiple modes specified.\n", programName);
    printUsage();
    exit(EXIT_FAILURE);
  }
  mode = m;
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
  if (fileCount > expectedFiles) {
    fprintf(stderr,
            "%s : Error : Wrong number of arguments (%d).\n",
            programName,
            argumentCount);
    printUsage();
    exit(EXIT_FAILURE);
  }
  char* fileName = argumentVector[fileArg];
  if (verbose) {
    fprintf(stdout, "file = %s\n", fileName);
  }
  if (mode == mxfMode) {
    mxfDumpFile(fileName);
  } else {
    rawDumpFile(fileName);
  }
  return 0;  
}
