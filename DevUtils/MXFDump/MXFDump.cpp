
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long int mxfLength;
typedef unsigned char mxfKey[16];

const char* programName;

void checkSizes(void)
{
  if (sizeof(mxfLength) != 4) {
    cerr << programName
      << ": Error : Wrong sizeof(mxfLength)."
      << endl;
  }
    if (sizeof(mxfKey) != 16) {
    cerr << programName
      << ": Error : Wrong sizeof(mxfKey)."
      << endl;
  }
}

void printUsage(void)
{
  cerr << programName << ": Usage : "
       << programName << " OPTIONS <file>"
                      << endl;
  cerr << "OPTIONS :" << endl;
  cerr << "--raw-dump      = ";
  cerr << "dump raw KLV (-r)" << endl;
  cerr << "--mxf-dump      = ";
  cerr << "dump MXF (-m)" << endl;
  cerr << "--help          = ";
  cerr << "print this messsage and exit (-h)" << endl;
  cerr << "--verbose       = ";
  cerr << "print more detailed information (-v)" << endl;
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

void mxfDumpFile(char* fileName);

void mxfDumpFile(char* /* fileName */)
{
  cout << "MXF dump not yet implemented." << endl;
}

void rawDumpFile(char* fileName);

void rawDumpFile(char* fileName)
{ 
  FILE* infile;

  infile = fopen(fileName, "rb");
  if (infile != NULL) {
//
    unsigned char buffer[76];
    fread(buffer, sizeof(buffer), 1, infile);
    
    while (!feof(infile)) {
      mxfKey k;
      fread(&k, sizeof(mxfKey), 1, infile);
      mxfLength len;
      fread(&len, sizeof(mxfLength), 1, infile);
      for (mxfLength i = 0; i < len; i++) {
        unsigned char ch;
        fread(&ch, sizeof(unsigned char), 1, infile);
      }
    }
//
    fclose(infile);
  } else {
    cerr << programName <<": Error: "
         << "File \"" << fileName << "\" not found."
         << endl;
    exit(EXIT_FAILURE);
  }
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
    cerr << programName
         << " : Error : Multiple modes specified"
         << endl;
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
      cerr << programName
           << " : Error : Invalid option \""
           << p << "\""
           << endl;
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
    cerr << programName
         << " : Error : Wrong number of arguments ("
         << argumentCount
         << ")"
         << endl;
    printUsage();
    exit(EXIT_FAILURE);
  }
  char* fileName = argumentVector[fileArg];
  if (verbose) {
    cout << "file = " << fileName << endl;
  }
  if (mode == mxfMode) {
    mxfDumpFile(fileName);
  } else {
    rawDumpFile(fileName);
  }
  return 0;  
}
