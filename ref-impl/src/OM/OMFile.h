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
  static OMFile* createModify(const char* fileName);

  void close(void);

  OMStoredObject* root(void);

  OMFile& operator << (const OMStorable& o);

  OMClassFactory* classFactory(void);
  OMObjectDirectory* objectDirectory(void);
  
  virtual const OMClassId& classId(void) const;

  virtual OMFile* file(void) const;

private:

  void openRead(void);
  void openModify(void);
  void createModify(void);

  char* _name;
  OMStoredObject* _root;
  
  OMClassFactory* _classFactory;
  OMObjectDirectory* _objectDirectory;

  enum OMAccessMode _mode;

};

#endif
