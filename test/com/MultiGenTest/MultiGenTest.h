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
//=---------------------------------------------------------------------=

#include <AAFTypes.h>
#include <AAF.h>
#include <AAFSmartPointer.h>
#include <AAFResult.h>
#include <AAFCOMPlatformTypes.h>

#include <vector>
#include <string>

// iostream only required by the THROW_STRING macro
#include <iostream>

using namespace std;

// Class that stores state info that must be passed between
// tests.  i.e. One bit of test code opens the file and sets
// it.  The next requireds that file so gets it.  Others added
// as required.

class CmdState {
public:
  CmdState();
  ~CmdState();
  
  void SetFile( IAAFSmartPointer<IAAFFile> iaafFile );
  IAAFSmartPointer<IAAFFile> GetFile();
  void ClearPriorToUnload();

private:
  bool _isFileSet;
  IAAFSmartPointer<IAAFFile> _iaafFile;
};

// MultiGenTest - Base class from which test implementations are derived.

class MultiGenTest {
 public:

  MultiGenTest( const char* name,
		const char *desc,
		const char *usage,
		const char *notes );

  virtual ~MultiGenTest();

  const char* GetName() const;
  const char* GetDesc() const;
  const char* GetUsage() const;
  const char* GetNotes() const;

  virtual void RunTest( CmdState& state, int sub_argc, char** sub_argv ) = 0;

 private:
  const char* _name;
  const char* _desc;
  const char* _usage;
  const char* _notes;
};

//=---------------------------------------------------------------------=

// MultiGenTestFactory - Factory that creates instances of tests

class MultiGenTestFactory {
 public:
  MultiGenTestFactory( const char* name,
		       const char* desc,
		       const char* usage,
		       const char* notes );

  virtual ~MultiGenTestFactory();

  const char* GetName() const;
  const char* GetDesc() const;
  const char* GetUsage() const;
  const char* GetNotes() const;

  // Caller takes ownership of the returned pointer.
  virtual MultiGenTest* Create() = 0;

 private:
  const char* _name;
  const char* _desc;
  const char* _usage;
  const char* _notes;
};

// Common implementation in the form of a macro.  This macro assumes a
// class exists which has the same name as the test (i.e. TESTNAME).
#define MULTIGEN_TEST_FACTORY( FCTRYNAME, TESTNAME, DESC, USAGE, NOTES ) \
MULTIGEN_TEST_FACTORY_LONG( FCTRYNAME, TESTNAME, TESTNAME, DESC, USAGE, NOTES )

#define MULTIGEN_TEST_FACTORY_LONG( FCTRYNAME, TESTNAME, TESTCLASS, DESC, USAGE, NOTES ) \
class FCTRYNAME : public MultiGenTestFactory {                    \
public:                                                           \
  FCTRYNAME ()                                                    \
    : MultiGenTestFactory( #TESTNAME, DESC, USAGE, NOTES )        \
  {                                                               \
    MultiGenTestRegistry::GetInstance().Register( this );         \
  }                                                               \
  virtual ~FCTRYNAME () {}                                        \
  virtual MultiGenTest* Create()                                  \
  {                                                               \
    return new TESTCLASS( #TESTNAME, DESC, USAGE, NOTES );        \
  }                                                               \
};

//=---------------------------------------------------------------------=

// MultiGenTestRegistry - A singleton registry of test factories.

class MultiGenTestRegistry {
 public:
  static MultiGenTestRegistry& GetInstance();

  ~MultiGenTestRegistry();

  // Registry takes ownership of factory.
  void Register( MultiGenTestFactory* factory );

  // An iterator to with what you wish.  The registry
  // retains ownership of the returned pointer.
  MultiGenTestFactory* Next();
  void ResetIter() ;

  MultiGenTestFactory& GetFactory( const char* name );

  bool IsKnown( const char* name );

  private:
    MultiGenTestRegistry();
    void IncreaseVectorSize();

    static MultiGenTestRegistry* _singleInstance;

    std::vector<MultiGenTestFactory*> _vec;
    std::vector<MultiGenTestFactory*>::const_iterator _vecIter;
};

//=---------------------------------------------------------------------=

// Misc. stuff.
inline void checkResult( HRESULT hr )
{
  if ( AAFRESULT_FAILED(hr) ) throw hr;
}

// Usage errors will be common.  Best to have an exception so that
// they can be clearly reported as such
// Ctor takes a string because it is expect that the error may be
// allocated on the heap because it will be built on the fly.  In
// that case, use a string to copy it.
class UsageEx {
public:
  UsageEx( const char* msg )
    : _msg(msg) {}

  UsageEx( string& msg )
    : _msg(msg) {}

  const string& GetMsg() const
    { return _msg; };
private:
  string _msg;
};

// HRESULT errors will be common.  Best to have an exception so that
// they can be clearly reported as such
class HResultEx {
public:
  HResultEx( HRESULT hr, const char *file, int line )
    : _hr(hr), _file(file), _line(line) {}

  HRESULT GetHResult() const
  { return _hr; };

private:
  const HRESULT _hr;
  const char* _file;
  const int _line;
};

#define CHECK_HRESULT( expr )			       \
{						       \
  HRESULT _hr = (expr);				       \
  if ( !SUCCEEDED( _hr ) ) {			       \
     throw HResultEx( _hr, __FILE__, __LINE__ );       \
  }						       \
}

template <class T>
inline IUnknown** ToIUnknown( T** p )
{
  return reinterpret_cast<IUnknown**>(p);
}

template <class T>
inline void** ToVoid( T** p )
{
  return reinterpret_cast<void**>(p);
}

// Caller takes ownership of returned pointer.
wchar_t* ToWideString( const char* str );

// wide string compare
bool wstrcmp( wchar_t* a, wchar_t* b );

//=---------------------------------------------------------------------=
