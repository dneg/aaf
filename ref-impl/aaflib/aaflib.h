/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

#ifndef __aaflib_h__
#define __aaflib_h__

#include "AAF.h"
#include "aafrdli.h"




//***********************************************************
// Define function prototypes in a manner consistent with the 
// ActiveX and OLE SDK's.

#if !defined(_MSC_VER)

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENEXISTINGREAD)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENEXISTINGMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENNEWMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENTRANSIENT)(
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETPLUGINMANAGER)(
    IAAFPluginManager ** ppPluginManager);

#else

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENEXISTINGREAD)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENEXISTINGMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENNEWMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENTRANSIENT)(
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETPLUGINMANAGER)(
    IAAFPluginManager ** ppPluginManager);

#endif


// Forward declarations
class AAFDLL;


//***********************************************************
// Abstract helper class to hide the platform dependent
// details for handling dll entrypoints.
//
class AAFDLL
{
protected:
  // Constructor and destructor for base class.
  AAFDLL();
  
public:
  virtual ~AAFDLL();

	// Factory method.
	static AAFDLL * MakeAAFDLL();


  // Return the singleton instance.
  static AAFDLL * GetAAFDLL();


  // Implement platform specific initialization of dll and 
	// entry points. NOTE: ALL ENTRY POINTS ARE REQUIRED! 
	// If any entry point cannot be found then the override 
	// of of Load MUST FAIL.
  HRESULT Load(const char *dllname);
  
  // Implement platform specific cleanup of dll and entry points.
  // NOTE: Implementations must be prepared to be called
  // even if their implementation of Load has failed.
  HRESULT Unload();
  
  // Resets all entry point function pointers to NULL.
  void ClearEntrypoints();


  //
  // Wrapper functions for calling member entry points.
  //
  HRESULT OpenExistingRead (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);
  
  HRESULT OpenExistingModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenNewModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenTransient (
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT GetPluginManager (
    IAAFPluginManager ** ppPluginManager);
  
  
protected:
  //
  // The single instance of the dll wrapper.
  //
  static AAFDLL *_singleton;

	//
	// Platform independent (opaque) handle to the loaded dynamic library.
	//
	AAFLibraryHandle _libHandle;

  //
  // Callback function member data loaded by overridden versions
  // of the Load() method:
  //
  LPFNAAFFILEOPENEXISTINGREAD   _pfnOpenExistingRead;
  LPFNAAFFILEOPENEXISTINGMODIFY _pfnOpenExistingModify;
  LPFNAAFFILEOPENNEWMODIFY      _pfnOpenNewModify;
  LPFNAAFFILEOPENTRANSIENT      _pfnOpenTransient;
  LPFNAAFGETPLUGINMANAGER       _pfnGetPluginManager;
};



#ifdef __cplusplus
extern "C"{
#endif 




//
// Assertion code copied from OM...
//

#ifdef _DEBUG

void reportAssertionFailure(char* kind,
                            char* name,
                            char* expressionString,
                            char* routine,
                            char* fileName,
                            size_t lineNumber);


#define ASSERT(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionFailure("Assertion",    name, #expression, \
                             currentRoutineName, __FILE__, __LINE__)

#define TRACE(routine) char* currentRoutineName = routine;

#else

#define ASSERT(name, expression)

#define TRACE(routine)

#endif /* _DEBUG */



#ifdef __cplusplus
}
#endif




#endif /* __aaflib_h__ */