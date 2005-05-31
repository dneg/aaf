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


#include "OMDiskRawStorageGroup.h"
#include "OMUtilities.h"
#include "wchar.h"


OMDiskRawStorageGroup* 
OMDiskRawStorageGroup::openExistingRead(const wchar_t* fileName)
{
    TRACE("OMDiskRawStorageGroup::openExistingRead");
    PRECONDITION("Valid file name", fileName != 0);

    return new OMDiskRawStorageGroup(OMDiskRawStorage::openExistingRead(fileName),
        fileName, READ_MODE);
}

OMDiskRawStorageGroup* 
OMDiskRawStorageGroup::openExistingModify(const wchar_t* fileName)
{
    TRACE("OMDiskRawStorageGroup::openExistingModify");
    PRECONDITION("Valid file name", fileName != 0);

    return new OMDiskRawStorageGroup(OMDiskRawStorage::openExistingModify(fileName),
        fileName, EXISTING_MODIFY_MODE);
}

OMDiskRawStorageGroup* 
OMDiskRawStorageGroup::openNewModify(const wchar_t* fileName)
{
    TRACE("OMDiskRawStorageGroup::openNewModify");
    PRECONDITION("Valid file name", fileName != 0);

    return new OMDiskRawStorageGroup(OMDiskRawStorage::openNewModify(fileName),
        fileName, NEW_MODIFY_MODE);
}


OMDiskRawStorageGroup::OMDiskRawStorageGroup(OMDiskRawStorage* storage, 
    const wchar_t* fileName, Mode mode)
: _storage(storage), _fileName(0), _mode(mode)
{
    TRACE("OMDiskRawStorageGroup::OMDiskRawStorageGroup");
    PRECONDITION("Valid storage", storage != 0);
    PRECONDITION("Valid file name", fileName != 0);

    _fileName = new wchar_t[wcslen(fileName) + 1];
    wcscpy(_fileName, fileName);
}

OMDiskRawStorageGroup::~OMDiskRawStorageGroup(void)
{
    TRACE("OMDiskRawStorageGroup::~OMDiskRawStorageGroup");

    if (_storage != 0)
    {
        delete _storage;
    }
    
    if (_fileName != 0)
    {
        delete [] _fileName;
    }
}

OMDiskRawStorage* 
OMDiskRawStorageGroup::openStorage(const wchar_t* fileName)
{
    TRACE("OMDiskRawStorageGroup::openStorage");
    PRECONDITION("Valid file name", fileName != 0);

    wchar_t* fullFileName;
    if (isRelative(fileName))
    {
        wchar_t* base = getBaseFilePath();
        fullFileName = new wchar_t[wcslen(base) + wcslen(fileName) + 1];
        wcscpy(fullFileName, base);
        wcscat(fullFileName, fileName);
        delete [] base;
    }
    else
    {
        fullFileName = new wchar_t[wcslen(fileName) + 1];
        wcscpy(fullFileName, fileName);
    }
        
    OMDiskRawStorage* storage = 0;    
    if (_mode == READ_MODE)
    {
        storage = OMDiskRawStorage::openExistingRead(fullFileName);
    }
    else
    {
        storage = OMDiskRawStorage::openExistingModify(fullFileName);
    }
    delete [] fullFileName;
    
    return storage;
}

OMDiskRawStorage* 
OMDiskRawStorageGroup::createStorage(const wchar_t* fileName)
{
    TRACE("OMDiskRawStorageGroup::createStorage");
    PRECONDITION("Valid file name", fileName != 0);

    wchar_t* fullFileName;
    if (isRelative(fileName))
    {
        wchar_t* base = getBaseFilePath();
        fullFileName = new wchar_t[wcslen(base) + wcslen(fileName) + 1];
        wcscpy(fullFileName, base);
        wcscat(fullFileName, fileName);
        delete [] base;
    }
    else
    {
        fullFileName = new wchar_t[wcslen(fileName) + 1];
        wcscpy(fullFileName, fileName);
    }
        
    wremove(fullFileName);

    OMDiskRawStorage* storage = OMDiskRawStorage::openNewModify(fullFileName);
    delete [] fullFileName;
    
    return storage;
}

const wchar_t* 
OMDiskRawStorageGroup::getFileName() const
{
    TRACE("OMDiskRawStorageGroup::getFileName");

    return _fileName;
}

bool 
OMDiskRawStorageGroup::isReadable(void) const
{
    TRACE("OMDiskRawStorageGroup::isReadable");
    
    return _storage->isReadable();
}

void 
OMDiskRawStorageGroup::read(OMByte* bytes, OMUInt32 byteCount, OMUInt32& bytesRead) const
{
    TRACE("OMDiskRawStorageGroup::read");

    _storage->read(bytes, byteCount, bytesRead);
}

void 
OMDiskRawStorageGroup::readAt(OMUInt64 position, OMByte* bytes, OMUInt32 byteCount,
    OMUInt32& bytesRead) const
{
    TRACE("OMDiskRawStorageGroup::readAt");

    _storage->readAt(position, bytes, byteCount, bytesRead);
}

bool 
OMDiskRawStorageGroup::isWritable(void) const
{
    TRACE("OMDiskRawStorageGroup::isWritable");

    return _storage->isWritable();
}

void 
OMDiskRawStorageGroup::write(const OMByte* bytes, OMUInt32 byteCount, OMUInt32& bytesWritten)
{
    TRACE("OMDiskRawStorageGroup::write");

    _storage->write(bytes, byteCount, bytesWritten);
}

void 
OMDiskRawStorageGroup::writeAt(OMUInt64 position, const OMByte* bytes, OMUInt32 byteCount,
    OMUInt32& bytesWritten)
{
    TRACE("OMDiskRawStorageGroup::writeAt");
    
    _storage->writeAt(position, bytes, byteCount, bytesWritten);
}

bool 
OMDiskRawStorageGroup::isExtendible(void) const
{
    TRACE("OMDiskRawStorageGroup::isExtendible");

    return _storage->isExtendible();
}

OMUInt64 
OMDiskRawStorageGroup::extent(void) const
{
    TRACE("OMDiskRawStorageGroup::extent");

    return _storage->extent();
}

void 
OMDiskRawStorageGroup::extend(OMUInt64 newSize)
{
    TRACE("OMDiskRawStorageGroup::extend");

    _storage->isReadable();
}

OMUInt64 
OMDiskRawStorageGroup::size(void) const
{
    TRACE("OMDiskRawStorageGroup::size");

    return _storage->size();
}

bool 
OMDiskRawStorageGroup::isPositionable(void) const
{
    TRACE("OMDiskRawStorageGroup::isPositionable");

    return _storage->isPositionable();
}

OMUInt64 
OMDiskRawStorageGroup::position(void) const
{
    TRACE("OMDiskRawStorageGroup::position");

    return _storage->position();
}

void 
OMDiskRawStorageGroup::setPosition(OMUInt64 newPosition) const
{
    TRACE("OMDiskRawStorageGroup::setPosition");

    _storage->setPosition(newPosition);
}

void 
OMDiskRawStorageGroup::synchronize(void)
{
    TRACE("OMDiskRawStorageGroup::synchronize");

    _storage->synchronize();
}
   
    
bool 
OMDiskRawStorageGroup::isRelative(const wchar_t* fileName)
{
    TRACE("OMDiskRawStorageGroup::isRelative");

#ifdef _WIN32
    // check if the path starts with a drive letter, i.e. xxx:
    const wchar_t* pathPtr = fileName;
    while (*pathPtr != 0 && *pathPtr != L'\\' && *pathPtr != L':')
    {
        pathPtr++;
    }
    if (*pathPtr != L':')
    {
        return true;
    }
    return false;
#else
    // check if path starts with root
    if (*fileName != L'/')
    {
        return true;
    }
    return false;
#endif
}

wchar_t* 
OMDiskRawStorageGroup::getBaseFilePath() const
{
    TRACE("OMDiskRawStorageGroup::getBaseFilePath");

    const wchar_t* endPtr = _fileName;
    const wchar_t* ptr = _fileName;
    while (*ptr != L'\0')
    {
#ifdef _WIN32
        if (*ptr == L'\\' || *ptr == L':')
#else
        if (*ptr == L'/')
#endif
        {
            endPtr = ptr + 1;
        }
        ptr++;
    }
#ifdef _WIN32
    ASSERT("Valid end pointer", endPtr == _fileName || *(endPtr - 1) == L'\\' 
        || *(endPtr - 1) == L':');
#else
    ASSERT("Valid end pointer", endPtr == _fileName || *(endPtr - 1) == L'/');
#endif
    
    size_t len = endPtr - _fileName;
    wchar_t* result = new wchar_t[len + 1];
    wcsncpy(result, _fileName, len);
    result[len] = L'\0';
    
    return result;
}


