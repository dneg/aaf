#include "OMCommandLine.h"
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//
typedef unsigned long int length;
typedef unsigned char key[16];
key prefaceKey = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53, 0x01, 0x01,
                 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

key headerKey  = {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
                  0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00};

//

char* programName;

void usage(void)
{
  cerr << programName << ": Usage : "
       << programName << " <file>"
                      << endl;
}

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

void dumpFile(char* fileName)
{ 
  FILE* infile;

  infile = fopen(fileName, "rb");
  if (infile != NULL) {
//
    unsigned char buffer[76];
    fread(buffer, sizeof(buffer), 1, infile);
    
    while (!feof(infile)) {
      key k;
      fread(&k, sizeof(key), 1, infile);
      length len;
      fread(&len, sizeof(length), 1, infile);
      for (length i = 0; i < len; i++) {
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

int main(int argumentCount, char* argumentVector[])
{
  if (sizeof(length) != 4) {
    cerr << programName
      << ": Error : Wrong sizeof(length)."
      << endl;
  }
    if (sizeof(key) != 16) {
    cerr << programName
      << ": Error : Wrong sizeof(key)."
      << endl;
  }
  programName = baseName(argumentVector[0]);
  
  int fileCount = argumentCount - 1;
  
  if (fileCount != 1) {
    cerr << programName
      << ": Error : Wrong number of arguments ("
      << fileCount << ")."
      << endl;
    usage();
    exit(EXIT_FAILURE);
  }
  char* fileName = argumentVector[1];
  dumpFile(fileName);

  return 0;  
}
