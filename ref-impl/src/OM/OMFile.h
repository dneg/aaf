#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMStoredObject.h"

class OMClassFactory;
class OMObjectDirectory;

class OMFile : public OMStorable {
public:
  OMFile(const char* name);
  ~OMFile(void);
  void create(void);
  void open(void);
  void close(void);
  OMStoredObject& root(void);
  OMStoredObject& openStoragePath(const char* storagePathName);

  OMFile& operator << (const OMStorable& o);

  static OMClassFactory* classFactory(void);
  static OMObjectDirectory* objectDirectory(void);
  
  virtual int classId(void) const;

  virtual OMFile* file(void) const;

private:

  OMStoredObject& createFile(const char* fileName);
  OMStoredObject& openFile(const char* fileName);

  const char* _name;
  OMStoredObject _root;
  
  static OMClassFactory* _classFactory;
  static OMObjectDirectory* _objectDirectory;
};

#endif
