#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMTypes.h"

class OMClassFactory;
class OMObjectDirectory;

class OMFile : public OMStorable {
public:

  OMFile(const char* name, const OMAccessMode mode);
  ~OMFile(void);

  static OMFile* openRead(const char* fileName);
  static OMFile* openModify(const char* fileName);
  static OMFile* createWrite(const char* fileName, const OMStorable* root);
  static OMFile* createModify(const char* fileName, const OMStorable* root);

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

private:

  void openRead(void);
  void openModify(void);
  void createModify(const OMStorable* root);

  char* _name;
  OMStorable* _root;
  OMStoredObject* _rootStoredObject;
  
  OMClassFactory* _classFactory;
  OMObjectDirectory* _objectDirectory;

  enum OMAccessMode _mode;

};

#endif
