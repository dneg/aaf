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

// @doc OMPRIVATE
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStackTrace

#include "OMStackTrace.h"

#if defined(OM_STACK_TRACE_ON_ASSERT)

#include "OMPortability.h"
#include "OMOStream.h"

#if defined(OM_OS_WINDOWS)

#include "OMUtilities.h"
#include "OMAssertions.h"

#include <windows.h>
#include <imagehlp.h>
#include <winbase.h>

#include <stdio.h>
#include <string.h>

// This Windows implementation taken from an "Under the Hood" article
// by Matt Pietrek also published on MSDN.

typedef BOOL   (__stdcall *SYMINITIALIZEPROC) (HANDLE, LPSTR, BOOL);
typedef BOOL   (__stdcall *SYMCLEANUPPROC) (HANDLE);
typedef BOOL   (__stdcall *STACKWALKPROC) (DWORD, HANDLE, HANDLE,
                                           LPSTACKFRAME, LPVOID,
                                           PREAD_PROCESS_MEMORY_ROUTINE,
                                           PFUNCTION_TABLE_ACCESS_ROUTINE,
                                           PGET_MODULE_BASE_ROUTINE,
                                           PTRANSLATE_ADDRESS_ROUTINE);
typedef LPVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC) (HANDLE, DWORD);
typedef DWORD  (__stdcall *SYMGETMODULEBASEPROC) (HANDLE, DWORD);
typedef BOOL   (__stdcall *SYMGETSYMFROMADDRPROC) (HANDLE, DWORD, PDWORD,
                                                   PIMAGEHLP_SYMBOL);
typedef BOOL   (__stdcall *UNDECORATESYMBOLNAME) (LPSTR, LPSTR, DWORD, DWORD);
typedef BOOL   (__stdcall *SYMLOADMODULE) (HANDLE, HANDLE, LPSTR, LPSTR,
                                           DWORD, DWORD);
typedef BOOL   (__stdcall *SYMGETSEARCHPATH) (HANDLE, LPSTR, DWORD);
typedef BOOL   (__stdcall *SYMSETSEARCHPATH) (HANDLE, LPSTR);

static SYMINITIALIZEPROC          _SymInitialize = 0;
static SYMCLEANUPPROC             _SymCleanup = 0;
static STACKWALKPROC              _StackWalk = 0;
static SYMFUNCTIONTABLEACCESSPROC _SymFunctionTableAccess = 0;
static SYMGETMODULEBASEPROC       _SymGetModuleBase = 0;
static SYMGETSYMFROMADDRPROC      _SymGetSymFromAddr = 0;
static UNDECORATESYMBOLNAME       _UnDecorateSymbolName = 0;
static SYMLOADMODULE              _SymLoadModule = 0;
static SYMGETSEARCHPATH           _SymGetSearchPath = 0;
static SYMSETSEARCHPATH           _SymSetSearchPath = 0;

static HMODULE hModImagehlp = 0;

static bool initialize(void)
{
  hModImagehlp = LoadLibrary(L"IMAGEHLP.DLL");
  if (!hModImagehlp) {
    return false;
  }

  _SymInitialize =
                  (SYMINITIALIZEPROC) GetProcAddress(hModImagehlp,
                                                     "SymInitialize");
  if (!_SymInitialize) {
    return false;
  }

  _SymCleanup =      (SYMCLEANUPPROC) GetProcAddress(hModImagehlp,
                                                     "SymCleanup");
  if (!_SymCleanup) {
    return false;
  }

  _StackWalk =        (STACKWALKPROC) GetProcAddress(hModImagehlp,
                                                     "StackWalk");
  if (!_StackWalk) {
    return false;
  }

  _SymFunctionTableAccess =
         (SYMFUNCTIONTABLEACCESSPROC) GetProcAddress(hModImagehlp,
                                                     "SymFunctionTableAccess");
  if (!_SymFunctionTableAccess) {
    return false;
  }

  _SymGetModuleBase =
               (SYMGETMODULEBASEPROC) GetProcAddress(hModImagehlp,
                                                     "SymGetModuleBase");
  if (!_SymGetModuleBase) {
    return false;
  }

  _SymGetSymFromAddr =
              (SYMGETSYMFROMADDRPROC) GetProcAddress(hModImagehlp,
                                                     "SymGetSymFromAddr");
  if (!_SymGetSymFromAddr) {
    return false;
  }

  _UnDecorateSymbolName  =
               (UNDECORATESYMBOLNAME) GetProcAddress(hModImagehlp,
                                                     "UnDecorateSymbolName");
  if (!_UnDecorateSymbolName) {
    return false;
  }

  _SymLoadModule =    (SYMLOADMODULE) GetProcAddress(hModImagehlp,
                                                     "SymLoadModule");
  if (!_SymLoadModule) {
    return false;
  }

  _SymGetSearchPath  =
                   (SYMGETSEARCHPATH) GetProcAddress(hModImagehlp,
                                                     "SymGetSearchPath");
  if (!_SymGetSearchPath) {
    return false;
  }

  _SymSetSearchPath  =
                   (SYMSETSEARCHPATH) GetProcAddress(hModImagehlp,
                                                     "SymSetSearchPath");
  if (!_SymSetSearchPath) {
    return false;
  }

  return true;
}

static void finalize(void)
{
  if (hModImagehlp) {
    FreeLibrary(hModImagehlp);
  }
}

void printStackTrace(OMOStream& s)
{
  s << "Symbolic stack trace." << endl;

  if (initialize()) {

    BOOL status;

    status = _SymInitialize(GetCurrentProcess(), 0, TRUE);
    if (!status) {
      s << "SymInitialize() failed." << endl;
    }

    CONTEXT context;
    memset(&context, 0, sizeof(CONTEXT));
    context.ContextFlags = CONTEXT_FULL;

    status = GetThreadContext(GetCurrentThread(), &context);
    if (!status) {
     s << "GetThreadContext() failed." << endl;
    }

    STACKFRAME stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME));
    stackFrame.AddrPC.Offset = context.Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;

    stackFrame.AddrStack.Offset = context.Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;

    stackFrame.AddrFrame.Offset = context.Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;

	// The declarations of PREAD_PROCESS_MEMORY_ROUTINE (a typedef)
	// and ReadProcessMemory (a function) differ slightly in the
	// type of their lpBaseAddress arguments.  The former uses
	// DWORD, the latter LPCVOID.  To work around this problem
	// ReadProcessMemory is cast to PREAD_PROCESS_MEMORY_ROUTINE.
	// jptrainor, 30 Jan 2001

    do {
      status = _StackWalk(
        IMAGE_FILE_MACHINE_I386,
        GetCurrentProcess(),
        GetCurrentThread(),
        &stackFrame,
        0,
		(PREAD_PROCESS_MEMORY_ROUTINE)ReadProcessMemory,
        _SymFunctionTableAccess,
        _SymGetModuleBase,
        0);

      if (!status) {
        break;
      }

      if (stackFrame.AddrPC.Offset) {
        s << (void*)stackFrame.AddrPC.Offset << " ";

        char buffer[sizeof(IMAGEHLP_SYMBOL) + 1024];
        memset(buffer, 0, sizeof(buffer));
        PIMAGEHLP_SYMBOL sym = (PIMAGEHLP_SYMBOL) buffer;
        sym->SizeOfStruct = sizeof(buffer);
        sym->MaxNameLength = 1024;
        DWORD disp;
        DWORD address = stackFrame.AddrPC.Offset;
        BOOL symStatus = _SymGetSymFromAddr(
          GetCurrentProcess(),
          address,
          &disp,
          sym);

        if (symStatus) {
          if (*(sym->Name) == '?') {
            char buffer[512];
            DWORD size = 512;
            DWORD flags = UNDNAME_COMPLETE | UNDNAME_32_BIT_DECODE;
            symStatus = _UnDecorateSymbolName(sym->Name, buffer, size, flags);
            if (symStatus) {
              s << buffer;
            } else {
              DWORD e = GetLastError();
              s << sym->Name;
            }
          } else {
            s << sym->Name;
          }
          if (disp != 0) {
            s << " + " << disp;
          }
        } else {
          s << "<unknown routine>";
        }

        wchar_t moduleName[MAX_PATH];
        MEMORY_BASIC_INFORMATION mbi;

        BOOL modStatus = VirtualQuery((void*)address, &mbi, sizeof(mbi));

        DWORD moduleHandle = (DWORD)mbi.AllocationBase;

        modStatus = GetModuleFileName((HMODULE)moduleHandle,
                                      moduleName,
                                      sizeof(moduleName));

        if (modStatus) {
          char* fullName = convertWideString(moduleName);
          char* name = strrchr(fullName, '\\');
          if (name != 0) {
            name = name + 1;
          } else {
            name = fullName;
          }
          s << " in " << name;
          delete [] fullName;
        }
        s << endl;
      }

    } while (status);

    status = _SymCleanup(GetCurrentProcess());
    if (!status) {
      s << "symCleanup() failed." << endl;
    }

  } else {
    s << "Failed to load IMAGEHLP.DLL" << endl;
  }

  finalize();

  s << "End of symbolic stack trace." << endl;
}

#elif defined(OM_OS_UNIX) && defined(__GLIBC__)

#include <execinfo.h>
#include <stdlib.h>

void printStackTrace(OMOStream& s)
{
  s << "Symbolic stack trace." << endl;
  void* addresses[64];

  size_t depth = backtrace(addresses, sizeof(addresses)/sizeof(addresses[0]));

  char** names = backtrace_symbols(addresses, depth);

  for (size_t i = 0; i < depth; i++) {
    s << names[i] << endl;
  }
  free(names);
  s << "End of symbolic stack trace." << endl;
}

#else

void printStackTrace(OMOStream& s)
{
  s << "Stack trace not available." << endl;
}

#endif

#else

void printStackTrace(OMOStream& /* s */)
{
}

#endif
