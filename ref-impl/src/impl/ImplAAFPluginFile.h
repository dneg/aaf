/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#ifndef __ImplAAFPluginFile_h__
#define __ImplAAFPluginFile_h__


#include "AAFTypes.h"

//***********************************************************
// Define function prototypes in a manner consistent with the 
// ActiveX and OLE SDK's.

#if !defined(_MSC_VER)

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCANUNLOADNOW)();

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETCLASSOBJECT)(
    REFCLSID rclsid, REFIID riid, LPVOID* ppv);

typedef STDAPICALLTYPE ULONG (* LPFNAAFGETCLASSCOUNT)();

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETCLASSOBJECTID)(
    ULONG index, CLSID* pclsid);



#else

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCANUNLOADNOW)();

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETCLASSOBJECT)(
    REFCLSID rclsid, REFIID riid, LPVOID* ppv);

typedef ULONG (STDAPICALLTYPE * LPFNAAFGETCLASSCOUNT)();

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETCLASSOBJECTID)(
    ULONG index, CLSID* pclsid);

#endif




class ImplAAFPluginFile
{
protected:
  ImplAAFPluginFile(const char *name);

  ~ImplAAFPluginFile();

  
public:
  // Factory method. Objects are created in a "loaded" (ie IsLoaded() will return true)
  // A file is successfully loaded if ALL of the expected entry points could
  // found.
  static HRESULT CreatePluginFile(const char* name, ImplAAFPluginFile** ppPluginFile);

  // Reference Counting interface
  virtual aafUInt32 AcquireReference() const;
  virtual aafUInt32 ReleaseReference();
  virtual aafUInt32 ReferenceCount() const;
  
  //
  // Wrapper methods for exported dll functions:
  //
  HRESULT CanUnloadNow();
  HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

  ULONG GetClassCount();
  HRESULT GetClassObjectID(ULONG index, CLSID* pclsid);

  //
  // Utility methods
  //
  bool IsLoaded() const;
  HRESULT Load();
  HRESULT Unload();

protected:
  void ClearEntryPoints();


private:
  //
  // Reference count for this instance:
  aafUInt32 _refCount;
  //
  // Name of the loaded plugin.
  //
  const char* _name;

	//
	// Platform independent (opaque) handle to the loaded dynamic library.
	//
	void * _libHandle;

  //
  // Callback function member data
  //

  // COM DLL callbacks
  LPFNAAFCANUNLOADNOW _pfnCanUnloadNow;
  LPFNAAFGETCLASSOBJECT _pfnGetClassObject;

  // AAF Plugins must implement and export these callbacks
  LPFNAAFGETCLASSCOUNT _pfnGetClassCount;
  LPFNAAFGETCLASSOBJECTID _pfnGetClassObjectID;
};


#endif /* __ImplAAFPluginFile_h__ */
