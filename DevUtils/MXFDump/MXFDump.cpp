
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//
typedef unsigned long int mxfLength;
typedef unsigned char mxfKey[16];

mxfKey prefaceKey = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53, 0x01, 0x01,
                     0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

mxfKey headerKey  = {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
                     0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00};

//

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
       << programName << " <file>"
                      << endl;
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

int main(int argumentCount, char* argumentVector[])
{
  programName = argumentVector[0];
  checkSizes();
  int fileCount = argumentCount - 1;
  
  if (fileCount != 1) {
    cerr << programName
      << ": Error : Wrong number of arguments ("
      << fileCount << ")."
      << endl;
    printUsage();
    exit(EXIT_FAILURE);
  }
  char* fileName = argumentVector[1];
  dumpFile(fileName);

  return 0;  
}
