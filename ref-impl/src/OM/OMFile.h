#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMTypes.h"

#include <stddef.h>

class OMClassFactory;
class OMObjectDirectory;

class OMFile : public OMStorable {
public:

  OMFile(const OMAccessMode mode, OMStoredObject* store);
  OMFile(const OMAccessMode mode, OMStoredObject* store, OMStorable* root);
  ~OMFile(void);

  static OMFile* openRead(const wchar_t* fileName);
  static OMFile* openModify(const wchar_t* fileName);
  static OMFile* createWrite(const wchar_t* fileName, OMStorable* root);
  static OMFile* createModify(const wchar_t* fileName, OMStorable* root);

  void save(void);

  void revert(void);

  OMStorable* restore(void);

  void close(void);

  OMStorable* root(void);

  OMStoredObject* rootStoredObject(void);

  OMClassFactory* classFactory(void);
  OMObjectDirectory* objectDirectory(void);
  
  virtual const OMClassId& classId(void) const;

  virtual OMFile* file(void) const;

  virtual bool attached(void);

private:

  OMStorable* _root;
  OMStoredObject* _rootStoredObject;
  
  OMClassFactory* _classFactory;
  OMObjectDirectory* _objectDirectory;

  enum OMAccessMode _mode;

};

#endif
