//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __OMDISKRAWSTORAGEGROUP_H__
#define __OMDISKRAWSTORAGEGROUP_H__

#include "OMDiskRawStorage.h"
#include "OMVector.h"

class OMRawStorage;

class OMDiskRawStorageGroup : public OMRawStorage 
{
public:

    static OMDiskRawStorageGroup* openExistingRead(const wchar_t* fileName);
    static OMDiskRawStorageGroup* openExistingModify(const wchar_t* fileName);
    static OMDiskRawStorageGroup* openNewModify(const wchar_t* fileName);

    
    OMDiskRawStorage* openStorage(const wchar_t* fileName);
    OMDiskRawStorage* createStorage(const wchar_t* fileName);
    const wchar_t* getFileName() const;
    
    
    virtual ~OMDiskRawStorageGroup(void);
    
    virtual bool isReadable(void) const;
    virtual void read(OMByte* bytes, OMUInt32 byteCount, OMUInt32& bytesRead) const;
    virtual void readAt(OMUInt64 position, OMByte* bytes, OMUInt32 byteCount,
        OMUInt32& bytesRead) const;
    virtual bool isWritable(void) const;
    virtual void write(const OMByte* bytes, OMUInt32 byteCount, OMUInt32& bytesWritten);
    virtual void writeAt(OMUInt64 position, const OMByte* bytes, OMUInt32 byteCount,
        OMUInt32& bytesWritten);
    virtual bool isExtendible(void) const;
    virtual OMUInt64 extent(void) const;
    virtual void extend(OMUInt64 newSize);
    virtual OMUInt64 size(void) const;
    virtual bool isPositionable(void) const;
    virtual OMUInt64 position(void) const;
    virtual void setPosition(OMUInt64 newPosition) const;
    virtual void synchronize(void);
    
    
protected:
    enum Mode {READ_MODE, EXISTING_MODIFY_MODE, NEW_MODIFY_MODE};
    
    OMDiskRawStorageGroup(OMDiskRawStorage* storage, const wchar_t* fileName, 
        Mode mode);
    
private:
    bool isRelative(const wchar_t* fileName);
    wchar_t* getBaseFilePath() const;

    OMDiskRawStorage*   _storage;
    wchar_t*            _fileName;
    Mode                _mode;
};


#endif


