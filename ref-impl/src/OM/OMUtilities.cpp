#include <iostream.h>

#include "OMUtilities.h"
#include "OMAssertions.h"

#include <windows.h>

const char* programName;

void setProgramName(const char* name)
{
  programName = name;
}

void convert(wchar_t* wcName, size_t length, const char* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validString(name));
  PRECONDITION("Valid output buffer", wcName != 0);
  PRECONDITION("Valid output buffer size", length > 0);
  
  ASSERT("Valid program name", validString(programName));

  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    cerr << programName
      << "Error : Failed to convert \""
      << name
      << "\" to a wide character string."
      << endl;
    exit(FAILURE);  
  }
}

void convert(char* cName, size_t length, wchar_t* name)
{
  ASSERT("Valid program name", validString(programName));

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    cerr << programName
      << ": Error : Conversion failed."
      << endl;
    exit(FAILURE);  
  }
}

void formatError(DWORD errorCode)
{
  wchar_t buffer[256];

  int status = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errorCode,
    LANG_SYSTEM_DEFAULT,
    buffer, 256,
    NULL);

  char message[256];

  convert(message, 256, buffer);

  if (status != 0) {
    int length = strlen(message);
    // zap cr/lf
    if (length >= 2) {
      message[length - 2] = '\0';
    }
    cerr << message << endl;
  } else {
    cerr << "Error code = " << hex << errorCode << dec << endl;
  }

}

void printError(const char* prefix, const char* type)
{
  cerr << prefix << ": " << type << ": ";
}

void printName(const char* name)
{
  cerr << "\"" << name << "\": ";
}

int check(HRESULT resultCode)
{
  TRACE("check");

  ASSERT("Valid program name", validString(programName));

  if (FAILED(resultCode)) {
    printError(programName, "Error");
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }
}

int checkFile(HRESULT resultCode, const char* fileName)
{
  TRACE("checkFile");
  PRECONDITION("Valid file name", validString(fileName));

  ASSERT("Valid program name", validString(programName));

  if (FAILED(resultCode)) {
    printError(programName, "File error");
    printName(fileName);
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }
}

int checkStream(HRESULT resultCode, const char* streamName)
{
  TRACE("checkStream");
  PRECONDITION("Valid stream name", validString(streamName));

  ASSERT("Valid program name", validString(programName));

  if (FAILED(resultCode)) {
    printError(programName, "Stream error");
    printName(streamName);
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }

}

int checkStorage(HRESULT resultCode, const char* storageName)
{
  TRACE("checkStorage");
  PRECONDITION("Valid storage name", validString(storageName));

  ASSERT("Valid program name", validString(programName));

  if (FAILED(resultCode)) {
    printError(programName, "Storage error");
    printName(storageName);
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }

}
