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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDynamicLibrary

#include "OMDynamicLibrary.h"

#include "OMPortability.h"
#include "OMAssertions.h"
#include "OMUtilities.h"

#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
#include "OMOStream.h"
#endif

#if defined(OM_OS_WINDOWS)

#include <windows.h>

class OMWindowsDynamicLibrary : public OMDynamicLibrary {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWindowsDynamicLibrary(const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMWindowsDynamicLibrary(void);

    // @cmember The address of the symbol named <p symbolName> in this
    //          <c OMDynamicLibrary>.
  virtual void* findSymbol(const char* symbolName);

protected:
  // @access Protected members.

    // @cmember Load the library described by this <c OMDynamicLibrary>.
  virtual bool load(void);

    // @cmember Unload the library described by this <c OMDynamicLibrary>.
  virtual void unload(void);

private:
  // @access Private members.

  wchar_t* _name;
  HINSTANCE _library;

};

  // @mfunc Constructor.
OMWindowsDynamicLibrary::OMWindowsDynamicLibrary(const wchar_t* name)
  : _name(saveWideString(name)),
  _library(0)
{
  TRACE("OMWindowsDynamicLibrary::OMWindowsDynamicLibrary");
  PRECONDITION("Valid library name", validWideString(name));

}

  // @mfunc Destructor.
OMWindowsDynamicLibrary::~OMWindowsDynamicLibrary(void)
{
  TRACE("OMWindowsDynamicLibrary::~OMWindowsDynamicLibrary");
  PRECONDITION("Library not loaded", _library == 0);

  delete [] _name;
  _name = 0;
}

  // @mfunc The address of the symbol named <p symbolName> in this
  //        <c OMDynamicLibrary>.
  //   @parm The name of the symbol to find.
  //   @rdesc The address of the symbol.
  //          If symbol cannot be found then 0 is returned.
void* OMWindowsDynamicLibrary::findSymbol(const char* symbolName)
{
  TRACE("OMWindowsDynamicLibrary::findSymbol");
  PRECONDITION("Valid symbol name", validString(symbolName));

  void* result = 0;
  FARPROC address = GetProcAddress((HMODULE)_library, symbolName);
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
  if (address == 0) {
    OMUInt32 error = GetLastError();
    omlog << "GetProcAddress() failed to find symbol"
          << " \"" << symbolName << "\","
          << " GetLastError() returns"
          << " \"" << error << "\"." << endl;
  }
#endif
  result = address;
  return result;  
}

  // @mfunc Load the library described by this <c OMDynamicLibrary>.
  //   @rdesc True if the library was successfully loaded, false otherwise.
bool OMWindowsDynamicLibrary::load(void)
{
  TRACE("OMWindowsDynamicLibrary::load");
  PRECONDITION("Library not loaded", _library == 0);

  bool result;
  HINSTANCE library = LoadLibrary(_name);
  if (library != 0) {
    _library = library;
    result = true;
  } else {
    OMUInt32 error = GetLastError();
    if (error == ERROR_CALL_NOT_IMPLEMENTED) {
      // A version of Windows without UNICODE support
      char* cName = convertWideString(_name);
      library = LoadLibraryA(cName);
      delete [] cName;
      if (library != 0) {
        _library = library;
        result = true;
      } else {
        error = GetLastError();
        result = false;
      }
    } else {
      result = false;
    }
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    if (!result) {
      omlog << "LoadLibrary() failed - GetLastError() returns"
            << " \"" << error << "\"." << endl;
    }
#endif
  }
  return result;
}

  // @mfunc Unload the library described by this <c OMDynamicLibrary>.
void OMWindowsDynamicLibrary::unload(void)
{
  TRACE("OMWindowsDynamicLibrary::unload");

  if (_library != 0) {
    BOOL freed = FreeLibrary((HINSTANCE)_library);
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    if (!freed) {
      OMUInt32 error = GetLastError();
      omlog << "FreeLibrary() failed - GetLastError() returns"
            << " \"" << error << "\"." << endl;
    }
#endif
    _library = 0;
  }
}

#elif defined(OM_OS_MACOS)

#include <CodeFragments.h>

class OMMacOSDynamicLibrary : public OMDynamicLibrary {
public:
  // @access Public members.

    // @cmember Constructor.
  OMMacOSDynamicLibrary(const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMMacOSDynamicLibrary(void);

    // @cmember The address of the symbol named <p symbolName> in this
    //          <c OMDynamicLibrary>.
  virtual void* findSymbol(const char* symbolName);

protected:
  // @access Protected members.

    // @cmember Load the library described by this <c OMDynamicLibrary>.
  virtual bool load(void);

    // @cmember Unload the library described by this <c OMDynamicLibrary>.
  virtual void unload(void);

private:
  // @access Private members.

  char* _name;
  CFragConnectionID _library;

};

  // @mfunc Constructor.
OMMacOSDynamicLibrary::OMMacOSDynamicLibrary(const wchar_t* name)
  : _name(convertWideString(name)),
  _library(0)
{
  TRACE("OMMacOSDynamicLibrary::OMMacOSDynamicLibrary");
  PRECONDITION("Valid library name", validWideString(name));

}

  // @mfunc Destructor.
OMMacOSDynamicLibrary::~OMMacOSDynamicLibrary(void)
{
  TRACE("OMMacOSDynamicLibrary::~OMMacOSDynamicLibrary");
  PRECONDITION("Library not loaded", _library == 0);

  delete [] _name;
  _name = 0;
}

  // @mfunc The address of the symbol named <p symbolName> in this
  //        <c OMDynamicLibrary>.
  //   @parm The name of the symbol to find.
  //   @rdesc The address of the symbol.
  //          If symbol cannot be found then 0 is returned.
void* OMMacOSDynamicLibrary::findSymbol(const char* symbolName)
{
  TRACE("OMMacOSDynamicLibrary::findSymbol");
  PRECONDITION("Valid symbol name", validString(symbolName));

  void* result = 0;
  Str255 pSymbolName;
  Ptr symbol = 0;
  CFragSymbolClass symbolClass;
  CFragConnectionID connectionID = _library;

  copyCToPString(pSymbolName, sizeof(Str255), symbolName);
  OSErr err = FindSymbol(connectionID, pSymbolName, &symbol, &symbolClass);
  if (err == noErr) {
    result = symbol;
  } else {
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    omlog << "FindSymbol() failed to find symbol"
          << " \"" << symbolName << "\""
          << " - error ="
          << " \"" << err << "\"." << endl;
#endif
    result = 0;
  } 
  
  return result;
}

  // @mfunc Load the library described by this <c OMDynamicLibrary>.
  //   @rdesc True if the library was successfully loaded, false otherwise.
bool OMMacOSDynamicLibrary::load(void)
{
  TRACE("OMMacOSDynamicLibrary::load");
  PRECONDITION("Library not loaded", _library == 0);

  bool result; 
  CFragArchitecture architectureType = kCompiledCFragArch;
  CFragLoadOptions loadFlags = kPrivateCFragCopy;
  CFragConnectionID connectionID = 0;
  Ptr mainAddress = 0;
  Str255 pErrorName;
  Str63 pLibraryName;

  copyCToPString(pLibraryName, sizeof(Str63), _name);
  OSErr err = GetSharedLibrary(pLibraryName,
                               architectureType,
                               loadFlags,
                               &connectionID,
                               &mainAddress,
                               pErrorName);
  if (err == noErr) {
    result = true;
    _library = connectionID;
  } else {
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    omlog << "GetSharedLibrary() failed - error ="
          << " \"" << err << "\"." << endl;
    char errorName[256];
    copyPToCString(errorName, sizeof(errorName), pErrorName);
    omlog << "LoadLibrary() failed to load"
          << " \"" << errorName << "\"." << endl;
#endif
    result = false;
    _library = 0;
  }
  return result;
}

  // @mfunc Unload the library described by this <c OMDynamicLibrary>.
void OMMacOSDynamicLibrary::unload(void)
{
  TRACE("OMMacOSDynamicLibrary::unload");

  if (_library != 0) {
    OSErr err = CloseConnection(&_library);
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    if (err != noErr) {
      omlog << "GetSharedLibrary() failed - error ="
            << " \"" << err << "\"." << endl;
    }
#endif
    _library = 0;
  }
}

#elif defined(OM_OS_UNIX)

#include <dlfcn.h>

class OMUnixDynamicLibrary : public OMDynamicLibrary {
public:
  // @access Public members.

    // @cmember Constructor.
  OMUnixDynamicLibrary(const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMUnixDynamicLibrary(void);

    // @cmember The address of the symbol named <p symbolName> in this
    //          <c OMDynamicLibrary>.
  virtual void* findSymbol(const char* symbolName);

protected:
  // @access Protected members.

    // @cmember Load the library described by this <c OMDynamicLibrary>.
  virtual bool load(void);

    // @cmember Unload the library described by this <c OMDynamicLibrary>.
  virtual void unload(void);

private:
  // @access Private members.

  char* _name;
  void* _library;

};

  // @mfunc Constructor.
OMUnixDynamicLibrary::OMUnixDynamicLibrary(const wchar_t* name)
  : _name(convertWideString(name)),
  _library(0)
{
  TRACE("OMUnixDynamicLibrary::OMUnixDynamicLibrary");
  PRECONDITION("Valid library name", validWideString(name));

}

  // @mfunc Destructor.
OMUnixDynamicLibrary::~OMUnixDynamicLibrary(void)
{
  TRACE("OMUnixDynamicLibrary::~OMUnixDynamicLibrary");
  PRECONDITION("Library not loaded", _library == 0);

  delete [] _name;
  _name = 0;
}

  // @mfunc The address of the symbol named <p symbolName> in this
  //        <c OMDynamicLibrary>.
  //   @parm The name of the symbol to find.
  //   @rdesc The address of the symbol.
  //          If symbol cannot be found then 0 is returned.
void* OMUnixDynamicLibrary::findSymbol(const char* symbolName)
{
  TRACE("OMUnixDynamicLibrary::findSymbol");
  PRECONDITION("Valid symbol name", validString(symbolName));

  void* result = 0;
  void* address = dlsym(_library, symbolName);
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
  if (address == 0) {
    char* error = dlerror();
    omlog << "dlsym() failed - dlerror() returns"
          << " \"" << error << "\"." << endl;
  }
#endif
  result = address;
  return result;  
}

  // @mfunc Load the library described by this <c OMDynamicLibrary>.
  //   @rdesc True if the library was successfully loaded, false otherwise.
bool OMUnixDynamicLibrary::load(void)
{
  TRACE("OMUnixDynamicLibrary::load");
  PRECONDITION("Library not loaded", _library == 0);

  bool result;
  void* library = dlopen(_name, RTLD_LAZY);
  if (library != 0) {
    _library = library;
    result = true;
  } else {
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    char* error = dlerror();
    omlog << "dlopen() failed - dlerror() returns"
          << " \"" << error << "\"." << endl;
#endif
    result = false;
  }
  return result;
}

  // @mfunc Unload the library described by this <c OMDynamicLibrary>.
void OMUnixDynamicLibrary::unload(void)
{
  TRACE("OMUnixDynamicLibrary::unload");

  if (_library != 0) {
    int closed = dlclose(_library);
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    if (closed != 0) {
      char* error = dlerror();
      omlog << "dlclose() failed - dlerror() returns"
            << " \"" << error << "\"." << endl;
    }
#endif
    closed = 0; // For GCC
    _library = 0;
  }
}

#elif defined(OM_OS_MACOSX)

#include <CFBundle.h>

class OMMacOSXDynamicLibrary : public OMDynamicLibrary {
public:
  // @access Public members.

    // @cmember Constructor.
  OMMacOSXDynamicLibrary(const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMMacOSXDynamicLibrary(void);

    // @cmember The address of the symbol named <p symbolName> in this
    //          <c OMDynamicLibrary>.
  virtual void* findSymbol(const char* symbolName);

protected:
  // @access Protected members.

    // @cmember Load the library described by this <c OMDynamicLibrary>.
  virtual bool load(void);

    // @cmember Unload the library described by this <c OMDynamicLibrary>.
  virtual void unload(void);

private:
  // @access Private members.

  char* _name;
  CFBundleRef _library;

};

  // @mfunc Constructor.
OMMacOSXDynamicLibrary::OMMacOSXDynamicLibrary(const wchar_t* name)
  : _name(convertWideString(name)),
  _library(0)
{
  TRACE("OMMacOSXDynamicLibrary::OMMacOSXDynamicLibrary");
  PRECONDITION("Valid library name", validWideString(name));

}

  // @mfunc Destructor.
OMMacOSXDynamicLibrary::~OMMacOSXDynamicLibrary(void)
{
  TRACE("OMMacOSXDynamicLibrary::~OMMacOSXDynamicLibrary");
  PRECONDITION("Library not loaded", _library == 0);

  delete [] _name;
  _name = 0;
}

  // @mfunc The address of the symbol named <p symbolName> in this
  //        <c OMDynamicLibrary>.
  //   @parm The name of the symbol to find.
  //   @rdesc The address of the symbol.
  //          If symbol cannot be found then 0 is returned.
void* OMMacOSXDynamicLibrary::findSymbol(const char* symbolName)
{
  TRACE("OMMacOSXDynamicLibrary::findSymbol");
  PRECONDITION("Valid symbol name", validString(symbolName));

  void* result = 0;
  CFBundleRef bundle = _library;
  CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault,
                                               symbolName,
                                               CFStringGetSystemEncoding());
  ASSERT("Valid string", name != 0);
  void* address = CFBundleGetFunctionPointerForName(bundle, name);

#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
  if (address == 0) {
    omlog << "CFGetFunctionPointerForName() failed to find symbol"
          << " \"" << symbolName << "\"."
          << endl;
  }
#endif
  CFRelease(name);
  result = address;
  return result;
}

  // @mfunc Load the library described by this <c OMDynamicLibrary>.
  //   @rdesc True if the library was successfully loaded, false otherwise.
bool OMMacOSXDynamicLibrary::load(void)
{
  TRACE("OMMacOSXDynamicLibrary::load");
  PRECONDITION("Library not loaded", _library == 0);

  bool result = false;

  CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault,
                                               _name,
                                               CFStringGetSystemEncoding());
  ASSERT("Valid string", name != 0);

  CFURLRef bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, 
                                                     name,
                                                     kCFURLPOSIXPathStyle,
                                                     true);
  ASSERT("Valid URL", bundleURL != 0);

#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
  unsigned char uName[256];
  Boolean status = CFURLGetFileSystemRepresentation(bundleURL,
                                                    true,
                                                    uName,
                                                    sizeof(uName));
  char* path;
  if (status) {
    path = reinterpret_cast<char*>(uName);
  } else {
    path = "Unknown";
  }
  omlog << "path = \"" << (char*)uName << "\"." << endl;
#endif

  CFBundleRef bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
  _library = bundle;

  CFRelease(bundleURL);
  CFRelease(name);

  if (bundle != 0) {
    Boolean loaded = CFBundleLoadExecutable(bundle);
    if (loaded) {
      result = true;
    } else {
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
      omlog << "CFBundleLoadExecutable() failed." << endl;
#endif
      CFRelease(bundle);
      result = false;
    }
  } else {
#if defined(OM_DYNAMIC_LIBRARY_DEBUG)
    omlog << "CFBundleCreate() failed." << endl;
#endif
    result = false;
  }
  return result;
}

  // @mfunc Unload the library described by this <c OMDynamicLibrary>.
void OMMacOSXDynamicLibrary::unload(void)
{
  TRACE("OMMacOSXDynamicLibrary::unload");

  if (_library != 0) {
    CFBundleRef bundle = _library;
    CFBundleUnloadExecutable(bundle);
    CFRelease(bundle);
    _library = 0;
  }
}

#endif

  // @mfunc Find and load the given library.
  //   @parm The name of the library to load.
  //   @rdesc A pointer to a newly created <c OMDynamicLibrary>.
  //          If the library cannot be found or loaded then 0 is returned. 
OMDynamicLibrary* OMDynamicLibrary::loadLibrary(const wchar_t* libraryName)
{
  TRACE("OMDynamicLibrary::loadLibrary");
  PRECONDITION("Valid library name", validWideString(libraryName));

  OMDynamicLibrary* result = 0;
#if defined(OM_OS_WINDOWS)
  result = new OMWindowsDynamicLibrary(libraryName);
#elif defined(OM_OS_MACOS)
  result = new OMMacOSDynamicLibrary(libraryName);
#elif defined(OM_OS_UNIX)
  result = new OMUnixDynamicLibrary(libraryName);
#elif defined(OM_OS_MACOSX)
  result = new OMMacOSXDynamicLibrary(libraryName);
#endif
  ASSERT("Valid heap pointer", result != 0);
  bool loaded = result->load();
  if (!loaded) {
    delete result;
    result = 0;
  }
  return result;
}

  // @mfunc Unload the given library.
  //   @parm The <c OMdynamicLibrary> to unload.
void OMDynamicLibrary::unloadLibrary(OMDynamicLibrary* library)
{
  TRACE("OMDynamicLibrary::unloadLibrary");
  PRECONDITION("Valid library", library != 0);

  library->unload();
  delete library;
}

  // @cmember Destructor.
OMDynamicLibrary::~OMDynamicLibrary(void)
{
}

