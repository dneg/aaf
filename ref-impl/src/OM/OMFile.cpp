#include "OMFile.h"

#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"

#include <string.h>

#include <windows.h>

OMFile::OMFile(const char* name)
: _name(name), _root(0)
{
  TRACE("OMFile::OMFile");

  setName("/");
}

OMFile::~OMFile(void)
{
  TRACE("OMFile::~OMFile");
}

void OMFile::create(void)
{
  TRACE("File::create");

  _root = createFile(_name);
  _root.create();
}

void OMFile::open(void)
{
  TRACE("OMFile::open");

  _root = openFile(_name);
  _root.open();
}

void OMFile::close(void)
{
  TRACE("OMFile::close");

  _root.close();
}

OMStoredObject& OMFile::root(void)
{
  TRACE("OMFile::root");

  return _root;
}

OMStoredObject& OMFile::openStoragePath(const char* storagePathName)
{
  TRACE("OMFile::openStoragePath");
  PRECONDITION("Valid root storage", root != 0);
  PRECONDITION("Valid stream path name", validString(storagePathName));
  PRECONDITION("Path name is absolute", storagePathName[0] == '/');

  char* path = new char[strlen(storagePathName) + 1];
  strcpy(path, storagePathName);
  
  char* element = path;
  element++; // skip first '/'

  OMStoredObject* storage = &root();
  OMStoredObject* result = 0;

  char* end = strchr(element, '/');
  
  while (end != 0) {
    *end = 0;
    storage = storage->openSubStorage(element);
    ASSERT("Valid storage pointer", storage != 0);
    element = ++end;
    end = strchr(element, '/');
  }

  if ((element != 0) && (strlen(element) > 0)) {
    result = storage->openSubStorage(element);
  } else {
    result = storage;
  }
  
  return *result;
}

OMFile& OMFile::operator << (const OMStorable& o)
{
  TRACE("OMFile::operator <<");

  const_cast<OMStorable&>(o).setContainingObject(this);
  const_cast<OMStorable&>(o).setName("head");
  o.saveTo(root());
  return *this;
}

OMClassFactory* OMFile::_classFactory = 0;

OMClassFactory* OMFile::classFactory(void)
{
  TRACE("OMFile::classFactory");

  if (_classFactory == 0) {
    _classFactory = new OMClassFactory(100);
  }

  return _classFactory;
}

OMObjectDirectory* OMFile::_objectDirectory = 0;

OMObjectDirectory* OMFile::objectDirectory(void)
{
  TRACE("OMFile::objectDirectory");

  if (_objectDirectory == 0) {
    _objectDirectory = new OMObjectDirectory(100);
  }
  return _objectDirectory;
}

OMStoredObject& OMFile::createFile(const char* fileName)
{
  TRACE("createFile");
  PRECONDITION("Valid file name", validString(fileName));

  wchar_t wcFileName[256];
  convert(wcFileName, 256, fileName);

  HRESULT result;
  IStorage* storage;

  result = StgCreateDocfile(
    wcFileName,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    0,
    &storage);
  if (!checkFile(result, fileName)) {
    exit(FAILURE);
  }

  return OMStoredObject(storage);
}

OMStoredObject& OMFile::openFile(const char* fileName)
{
  TRACE("openFile");
  PRECONDITION("Valid file name", validString(fileName));

  wchar_t wcFileName[256];
  convert(wcFileName, 256, fileName);

  HRESULT result;
  IStorage* storage;

  result = StgOpenStorage(
    wcFileName,
    0,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0,
    0,
    &storage);
  if (!checkFile(result, fileName)) {
    exit(FAILURE);
  }

  return OMStoredObject(storage);
}

int OMFile::classId(void) const
{
  TRACE("OMFile::classId");

  return 63;
}

OMFile* OMFile::file(void) const
{
  TRACE("OMFile::file");

  return const_cast<OMFile*>(this);
}
