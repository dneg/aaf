#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMTypes.h"

class OMClassFactory;
class OMObjectDirectory;

class OMFile : public OMStorable {
public:
  OMFile(const char* name);
  ~OMFile(void);

  static OMFile* open(const char* fileName);
  static OMFile* create(const char* fileName);

  void close(void);

  OMStoredObject* root(void);

  OMFile& operator << (const OMStorable& o);

  OMClassFactory* classFactory(void);
  OMObjectDirectory* objectDirectory(void);
  
  virtual const OMClassId& classId(void) const;

  virtual OMFile* file(void) const;

private:

  void create(void);
  void open(void);

  const char* _name;
  OMStoredObject* _root;
  
  OMClassFactory* _classFactory;
  OMObjectDirectory* _objectDirectory;
};

#endif
