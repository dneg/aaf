//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMByteArray.h"
#include "OMAssertions.h"


OMByteArray::OMByteArray()
: _bytes(0), _bytesSize(0), _increment(256), _initialBufferSize(256), _bufferSize(0)
{}

OMByteArray::OMByteArray(OMUInt32 size)
: _bytes(0), _bytesSize(0), _increment(256), _initialBufferSize(size), _bufferSize(0)
{}

OMByteArray::~OMByteArray()
{
    TRACE("OMByteArray::~OMByteArray");
    
    if (_bytes != 0)
    {
        delete [] _bytes;
    }
}


void OMByteArray::append(const OMByte* bytes, OMUInt32 size)
{
    TRACE("OMByteArray::append");
    
    if (_bufferSize == 0 && size < _initialBufferSize)
    {
        _bytes = new OMByte[_initialBufferSize];
        _bufferSize = _initialBufferSize;
    }
    else if (_bytesSize + size > _bufferSize)
    {
        if (_bytesSize + size - _bufferSize > _increment)
        {
            _bufferSize += _bytesSize + size - _bufferSize;
        }
        _bufferSize += _increment;

        OMByte* newBytes = new OMByte[_bufferSize];
        memcpy(newBytes, _bytes, _bytesSize);
        delete [] _bytes;
        _bytes = newBytes;

    }

    memcpy(_bytes + _bytesSize, bytes, size);
    _bytesSize += size;
}

void OMByteArray::setSize(OMUInt32 size)
{
    TRACE("OMByteArray::setSize");
    ASSERT("New size in buffer bounds", size <= _bufferSize);

    _bytesSize = size;
}

void OMByteArray::grow(OMUInt32 size)
{
    TRACE("OMByteArray::grow");

    if (_bytesSize + size > _bufferSize)
    {
        _bufferSize = _bytesSize + size;
        OMByte* newBytes = new OMByte[_bufferSize];
        memcpy(newBytes, _bytes, _bytesSize);
        delete [] _bytes;
        _bytes = newBytes;
    }
}

void OMByteArray::setBufferIncrement(OMUInt32 increment)
{
    TRACE("OMByteArray::setBufferIncrement");

    _increment = increment;
}

void OMByteArray::clear()
{
    TRACE("OMByteArray::clear");

    _bytesSize = 0;
}

void OMByteArray::reset()
{
    TRACE("OMByteArray::setSize");

    if (_bufferSize != _initialBufferSize)
    {
        _bufferSize = _initialBufferSize;
        delete [] _bytes;
        _bytes = 0;
    }
    _bytesSize = 0;
}
