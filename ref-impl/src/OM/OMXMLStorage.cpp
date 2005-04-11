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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMXMLStorage.h"
#include "OMSetIterator.h"
#include "OMListIterator.h"
#include "OMUtilities.h"
#include "OMAssertions.h"


OMXMLStorage::OMXMLStorage(OMRawStorage* storage, bool isRead)
: _objectSetId(0), _baselineSymbolspace(0), _defaultExtSymbolspace(0),
    _dataStreamNotationNameIndex(0), _dataStreamEntityNameIndex(0),
    _dataStreamEntityValueIndex(0)
{
    TRACE("OMXMLStorage::OMXMLStorage");
    
    _isRead = isRead;
    
    if (isRead)
    {
        _xmlWriter = 0;
        _xmlReader = new OMXMLReader(storage);;
    }
    else
    {
        _xmlWriter = new OMXMLWriter(storage);
        _xmlReader = 0;
    }
    
    _baselineSymbolspace = OMSymbolspace::createV11Symbolspace(this);
    _symbolspaces.insert(_baselineSymbolspace->getURI(), _baselineSymbolspace);

    loadStringIds();
}

OMXMLStorage::~OMXMLStorage()
{
    TRACE("OMXMLStorage::~OMXMLStorage");

    if (_xmlReader != 0)
    {
        delete _xmlReader;
    }
    if (_xmlWriter != 0)
    {
        delete _xmlWriter;
    }

    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (++iter) 
    {
        delete iter.value();
    }
}

bool 
OMXMLStorage::isRead()
{
    return _isRead;
}

OMXMLWriter* 
OMXMLStorage::getWriter()
{
    TRACE("OMXMLStorage::getWriter");
    PRECONDITION("Is writer", !_isRead);

    return _xmlWriter;
}

OMXMLReader* 
OMXMLStorage::getReader()
{
    TRACE("OMXMLStorage::getReader");
    PRECONDITION("Is reader", _isRead);

    return _xmlReader;
}
    
OMSymbolspace* 
OMXMLStorage::getBaselineSymbolspace() const
{
    TRACE("OMXMLStorage::getBaselineSymbolspace");
    
    return _baselineSymbolspace;
}

OMSymbolspace* 
OMXMLStorage::getDefaultExtSymbolspace() const
{
    TRACE("OMXMLStorage::getBaselineSymbolspace");
    
    return _defaultExtSymbolspace;
}

OMSymbolspace*
OMXMLStorage::createDefaultExtSymbolspace(OMClassId id)
{
    TRACE("OMXMLStorage::createDefaultExtSymbolspace");
    PRECONDITION("Default symbolspace does not already exist", _defaultExtSymbolspace == 0);
    
    _defaultExtSymbolspace = OMSymbolspace::createDefaultExtSymbolspace(this, id);
    _symbolspaces.insert(_defaultExtSymbolspace->getURI(), _defaultExtSymbolspace);
    
    return _defaultExtSymbolspace;
}

bool 
OMXMLStorage::getSymbol(OMClassId id, const wchar_t** symbolspaceURI, const wchar_t** symbol) const
{
    TRACE("OMXMLStorage::getSymbol");
    
    const wchar_t* sym = 0;
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (sym == 0 && ++iter) 
    {
        if ((sym = iter.value()->getSymbol(id)) != 0)
        {
            *symbol = sym;
            *symbolspaceURI = iter.value()->getURI();
        }
    }
    
    return sym != 0;
}

const wchar_t* 
OMXMLStorage::getDefinitionSymbol(OMClassId id)
{
    TRACE("OMXMLStorage::getDefinitionSymbol");
    
    OMWString* defSymbol;
    if (_idToDefSymbol.find(id, &defSymbol))
    {
        return (*defSymbol).c_str();
    }
    return 0;
}

OMClassId 
OMXMLStorage::getId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getId");
    
    OMClassId result = nullOMUniqueObjectIdentification;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getId(symbol); 
    }
    
    return result;
}

OMPropertyId 
OMXMLStorage::getPropertyId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getPropertyId");
    
    OMPropertyId result = 0x0000;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getPropertyId(symbol); 
    }
    
    return result;
}

OMClassId 
OMXMLStorage::getId(const wchar_t* definitionSymbol) const
{
    OMClassId id;
    if (_defSymbolToId.find(definitionSymbol, id))
    {
        return id;
    }
    return nullOMUniqueObjectIdentification;
}

const wchar_t* 
OMXMLStorage::getDataStreamNotationName(OMClassId typeId)
{
    TRACE("OMXMLStorage::getDataStreamNotationName");
    
    OMWString* ret;
    if (_dataStreamNotationNames.find(typeId, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[19];
    sprintf(buffer, "DataStream%x", _dataStreamNotationNameIndex);
    wchar_t* name = convertString(buffer);
    _dataStreamNotationNames.insert(typeId, name); 
    delete [] name;
    _dataStreamNotationNameIndex++;

    if (!_dataStreamNotationNames.find(typeId, &ret))
    {
        ASSERT("Name correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityName(void* ref)
{
    TRACE("OMXMLStorage::getDataStreamEntityName");
    
    OMWString* ret;
    if (_dataStreamEntityNames.find(ref, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[15];
    sprintf(buffer, "stream%x", _dataStreamEntityNameIndex);
    wchar_t* name = convertString(buffer);
    _dataStreamEntityNames.insert(ref, name); 
    delete [] name;
    _dataStreamEntityNameIndex++;
    
    if (!_dataStreamEntityNames.find(ref, &ret))
    {
        ASSERT("Name correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityValue(void* ref)
{
    TRACE("OMXMLStorage::getDataStreamEntityValue");
    
    OMWString* ret;
    if (_dataStreamEntityValues.find(ref, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[19];
    sprintf(buffer, "stream%x.raw", _dataStreamEntityValueIndex);
    wchar_t* value = convertString(buffer);
    _dataStreamEntityValues.insert(ref, value); 
    delete [] value;
    _dataStreamEntityValueIndex++;
    
    if (!_dataStreamEntityValues.find(ref, &ret))
    {
        ASSERT("Value correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}
    
void 
OMXMLStorage::forwardObjectSetId(const wchar_t* id)
{
    TRACE("OMXMLStorage::forwardObjectSetId");
    PRECONDITION("Valid object set id", id != 0);
    
    if (_objectSetId != 0)
    {
        delete [] _objectSetId;
    }
    
    _objectSetId = new wchar_t[lengthOfWideString(id) + 1];
    copyWideString(_objectSetId, id);
}

bool 
OMXMLStorage::haveForwardedObjectSetId()
{
    TRACE("OMXMLStorage::haveForwardedObjectSetId");
    
    return _objectSetId != 0;
}

wchar_t* 
OMXMLStorage::getForwardedObjectSetId()
{
    TRACE("OMXMLStorage::getForwardedObjectSetId");
    PRECONDITION("Have forwarded object set id", _objectSetId != 0);
    
    wchar_t* copy = new wchar_t[lengthOfWideString(_objectSetId) + 1];
    copyWideString(copy, _objectSetId);
    
    delete [] _objectSetId;
    _objectSetId = 0;
    
    return copy;
}

#define LITERAL_AUID(l, w1, w2,  b1, b2, b3, b4, b5, b6, b7, b8) \
    {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}

#define ADD_STRING_ID(ID, STRING) \
{ \
    const OMClassId id = ID; \
    _idToDefSymbol.insert(id, STRING); \
    _defSymbolToId.insert(STRING, id); \
}

void 
OMXMLStorage::loadStringIds()
{
    ADD_STRING_ID(
        LITERAL_AUID(0x01030202, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Picture");
    ADD_STRING_ID(
        LITERAL_AUID(0x6f3c8ce1, 0x6cef, 0x11d2, 0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_LegacyPicture");
    ADD_STRING_ID(
        LITERAL_AUID(0x05cba731, 0x1daa, 0x11d3, 0x80, 0xad, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_Matte");
    ADD_STRING_ID(
        LITERAL_AUID(0x05cba732, 0x1daa, 0x11d3, 0x80, 0xad, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_PictureWithMatte");
    ADD_STRING_ID(
        LITERAL_AUID(0x01030202, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Sound");
    ADD_STRING_ID(
        LITERAL_AUID(0x78e1ebe1, 0x6cef, 0x11d2, 0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_LegacySound");
    ADD_STRING_ID(
        LITERAL_AUID(0x01030201, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Timecode");
    ADD_STRING_ID(
        LITERAL_AUID(0x7f275e81, 0x77e5, 0x11d2, 0x80, 0x7f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_LegacyTimecode");
    ADD_STRING_ID(
        LITERAL_AUID(0xd2bb2af0, 0xd234, 0x11d2, 0x89, 0xee, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12),
        L"DataDef_Edgecode");
    ADD_STRING_ID(
        LITERAL_AUID(0x01030201, 0x1000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_DescriptiveMetadata");
    ADD_STRING_ID(
        LITERAL_AUID(0x01030203, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x05),
        L"DataDef_Auxiliary");
    ADD_STRING_ID(
        LITERAL_AUID(0x851419d0, 0x2e4f, 0x11d3, 0x8a, 0x5b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"DataDef_Unknown");
    ADD_STRING_ID(
        LITERAL_AUID(0x568fb761, 0x9458, 0x11d2, 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecDef_None");
    ADD_STRING_ID(
        LITERAL_AUID(0x90ac17c8, 0xe3e2, 0x4596, 0x9e, 0x9e, 0xa6, 0xdd, 0x1c, 0x70, 0xc8, 0x92),
        L"CodecDef_PCM");
    ADD_STRING_ID(
        LITERAL_AUID(0x820f09b1, 0xeb9b, 0x11d2, 0x80, 0x9f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecDef_WAVE");
    ADD_STRING_ID(
        LITERAL_AUID(0x4b1c1a45, 0x03f2, 0x11d4, 0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecDef_AIFC");
    ADD_STRING_ID(
        LITERAL_AUID(0x18634f8c, 0x3bab, 0x11d3, 0xbf, 0xd6, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d),
        L"CodecDef_JPEG");
    ADD_STRING_ID(
        LITERAL_AUID(0x4e84045e, 0x0f29, 0x11d4, 0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a),
        L"CodecDef_CDCI");
    ADD_STRING_ID(
        LITERAL_AUID(0x4e84045f, 0x0f29, 0x11d4, 0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a),
        L"CodecDef_RGBA");
    ADD_STRING_ID(
        LITERAL_AUID(0x1b31f3b1, 0x9450, 0x11d2, 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecFlavour_None");
    ADD_STRING_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7c, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_LegacyDV_625_50");
    ADD_STRING_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7d, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_LegacyDV_525_60");
    ADD_STRING_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7e, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_IEC_DV_625_50");
    ADD_STRING_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7f, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_IEC_DV_525_60");
    ADD_STRING_ID(
        LITERAL_AUID(0x4313b572, 0xd8ba, 0x11d2, 0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"ContainerDef_External");
    ADD_STRING_ID(
        LITERAL_AUID(0x4b1c1a46, 0x03f2, 0x11d4, 0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"ContainerDef_OMF");
    ADD_STRING_ID(
        LITERAL_AUID(0x4313b571, 0xd8ba, 0x11d2, 0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"ContainerDef_AAF");
    ADD_STRING_ID(
        LITERAL_AUID(0x42464141, 0x000d, 0x4d4f, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff),
        L"ContainerDef_AAFMSS");
    ADD_STRING_ID(
        LITERAL_AUID(0x4b464141, 0x000d, 0x4d4f, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff),
        L"ContainerDef_AAFKLV");
    ADD_STRING_ID(
        LITERAL_AUID(0x58464141, 0x000d, 0x4d4f, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff),
        L"ContainerDef_AAFXML");
    ADD_STRING_ID(
        LITERAL_AUID(0x0d011301, 0x0101, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06),
        L"ContainerDef_RIFFWAVE");
    ADD_STRING_ID(
        LITERAL_AUID(0x0d011301, 0x0102, 0x0200, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07),
        L"ContainerDef_JFIF");
    ADD_STRING_ID(
        LITERAL_AUID(0x0d011301, 0x0104, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06),
        L"ContainerDef_AIFFAIFC");
    ADD_STRING_ID(
        LITERAL_AUID(0x5b6c85a3, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_None");
    ADD_STRING_ID(
        LITERAL_AUID(0x5b6c85a4, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_Linear");
    ADD_STRING_ID(
        LITERAL_AUID(0x5b6c85a5, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_Constant");
    ADD_STRING_ID(
        LITERAL_AUID(0x5b6c85a6, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_BSpline");
    ADD_STRING_ID(
        LITERAL_AUID(0x15829ec3, 0x1f24, 0x458a, 0x96, 0x0d, 0xc6, 0x5b, 0xb2, 0x3c, 0x2a, 0xa1),
        L"InterpolationDef_Log");
    ADD_STRING_ID(
        LITERAL_AUID(0xc09153f7, 0xbd18, 0x4e5a, 0xad, 0x09, 0xcb, 0xdd, 0x65, 0x4f, 0xa0, 0x01),
        L"InterpolationDef_Power");
    ADD_STRING_ID(
        LITERAL_AUID(0x0c3bea40, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoDissolve");
    ADD_STRING_ID(
        LITERAL_AUID(0x0c3bea44, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_SMPTEVideoWipe");
    ADD_STRING_ID(
        LITERAL_AUID(0x9d2ea890, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoSpeedControl");
    ADD_STRING_ID(
        LITERAL_AUID(0x9d2ea891, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoRepeat");
    ADD_STRING_ID(
        LITERAL_AUID(0xf1db0f32, 0x8d64, 0x11d3, 0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"OperationDef_Flip");
    ADD_STRING_ID(
        LITERAL_AUID(0xf1db0f34, 0x8d64, 0x11d3, 0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"OperationDef_Flop");
    ADD_STRING_ID(
        LITERAL_AUID(0xf1db0f33, 0x8d64, 0x11d3, 0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"OperationDef_FlipFlop");
    ADD_STRING_ID(
        LITERAL_AUID(0x86f5711e, 0xee72, 0x450c, 0xa1, 0x18, 0x17, 0xcf, 0x3b, 0x17, 0x5d, 0xff),
        L"OperationDef_VideoPosition");
    ADD_STRING_ID(
        LITERAL_AUID(0xf5826680, 0x26c5, 0x4149, 0x85, 0x54, 0x43, 0xd3, 0xc7, 0xa3, 0xbc, 0x09),
        L"OperationDef_VideoCrop");
    ADD_STRING_ID(
        LITERAL_AUID(0x2e0a119d, 0xe6f7, 0x4bee, 0xb5, 0xdc, 0x6d, 0xd4, 0x29, 0x88, 0x68, 0x7e),
        L"OperationDef_VideoScale");
    ADD_STRING_ID(
        LITERAL_AUID(0xf2ca330d, 0x8d45, 0x4db4, 0xb1, 0xb5, 0x13, 0x6a, 0xb0, 0x55, 0x58, 0x6f),
        L"OperationDef_VideoRotate");
    ADD_STRING_ID(
        LITERAL_AUID(0x21d5c51a, 0x8acb, 0x46d5, 0x93, 0x92, 0x5c, 0xae, 0x64, 0x0c, 0x88, 0x36),
        L"OperationDef_VideoCornerPinning");
    ADD_STRING_ID(
        LITERAL_AUID(0x14db900e, 0xd537, 0x49f6, 0x88, 0x9b, 0x01, 0x25, 0x68, 0xfc, 0xc2, 0x34),
        L"OperationDef_VideoAlphaWithinVideoKey");
    ADD_STRING_ID(
        LITERAL_AUID(0xe599cb0f, 0xba5f, 0x4192, 0x93, 0x56, 0x51, 0xeb, 0x19, 0xc0, 0x85, 0x89),
        L"OperationDef_VideoSeparateAlphaKey");
    ADD_STRING_ID(
        LITERAL_AUID(0x38ff7903, 0x69e5, 0x476b, 0xbe, 0x5a, 0xea, 0xfc, 0x20, 0x00, 0xf0, 0x11),
        L"OperationDef_VideoLuminanceKey");
    ADD_STRING_ID(
        LITERAL_AUID(0x30a315c2, 0x71e5, 0x4e82, 0xa4, 0xef, 0x05, 0x13, 0xee, 0x05, 0x6b, 0x65),
        L"OperationDef_VideoChromaKey");
    ADD_STRING_ID(
        LITERAL_AUID(0x9d2ea894, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioGain");
    ADD_STRING_ID(
        LITERAL_AUID(0x9d2ea893, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioPan");
    ADD_STRING_ID(
        LITERAL_AUID(0x0c3bea41, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioDissolve");
    ADD_STRING_ID(
        LITERAL_AUID(0x2311bd90, 0xb5da, 0x4285, 0xaa, 0x3a, 0x85, 0x52, 0x84, 0x87, 0x79, 0xb3),
        L"OperationDef_TwoParameterMonoAudioDissolve");
    ADD_STRING_ID(
        LITERAL_AUID(0x1575e350, 0xfca3, 0x11d2, 0x8a, 0x2a, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_Unknown");
    ADD_STRING_ID(
        LITERAL_AUID(0x0c3bea43, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoFadeToBlack");
    ADD_STRING_ID(
        LITERAL_AUID(0x0a3c75e0, 0xfd82, 0x11d2, 0x8a, 0x2b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_PictureWithMate");
    ADD_STRING_ID(
        LITERAL_AUID(0x9d2ea892, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoFrameToMask");
    ADD_STRING_ID(
        LITERAL_AUID(0x0c3bea42, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_StereoAudioDissolve");
    ADD_STRING_ID(
        LITERAL_AUID(0x9d2ea895, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_StereoAudioGain");
    ADD_STRING_ID(
        LITERAL_AUID(0x8d896ad0, 0x2261, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioMixdown");
    ADD_STRING_ID(
        LITERAL_AUID(0xe4962320, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_Level");
    ADD_STRING_ID(
        LITERAL_AUID(0xe4962323, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEWipeNumber");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba0, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEReverse");
    ADD_STRING_ID(
        LITERAL_AUID(0x72559a80, 0x24d7, 0x11d3, 0x8a, 0x50, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SpeedRatio");
    ADD_STRING_ID(
        LITERAL_AUID(0xc573a510, 0x071a, 0x454f, 0xb6, 0x17, 0xad, 0x6a, 0xe6, 0x90, 0x54, 0xc2),
        L"ParameterDef_PositionOffsetX");
    ADD_STRING_ID(
        LITERAL_AUID(0x82e27478, 0x1336, 0x4ea3, 0xbc, 0xb9, 0x6b, 0x8f, 0x17, 0x86, 0x4c, 0x42),
        L"ParameterDef_PositionOffsetY");
    ADD_STRING_ID(
        LITERAL_AUID(0xd47b3377, 0x318c, 0x4657, 0xa9, 0xd8, 0x75, 0x81, 0x1b, 0x6d, 0xc3, 0xd1),
        L"ParameterDef_CropLeft");
    ADD_STRING_ID(
        LITERAL_AUID(0x5ecc9dd5, 0x21c1, 0x462b, 0x9f, 0xec, 0xc2, 0xbd, 0x85, 0xf1, 0x40, 0x33),
        L"ParameterDef_CropRight");
    ADD_STRING_ID(
        LITERAL_AUID(0x8170a539, 0x9b55, 0x4051, 0x9d, 0x4e, 0x46, 0x59, 0x8d, 0x01, 0xb9, 0x14),
        L"ParameterDef_CropTop");
    ADD_STRING_ID(
        LITERAL_AUID(0x154ba82b, 0x990a, 0x4c80, 0x91, 0x01, 0x30, 0x37, 0xe2, 0x88, 0x39, 0xa1),
        L"ParameterDef_CropBottom");
    ADD_STRING_ID(
        LITERAL_AUID(0x8d568129, 0x847e, 0x11d5, 0x93, 0x5a, 0x50, 0xf8, 0x57, 0xc1, 0x00, 0x00),
        L"ParameterDef_ScaleX");
    ADD_STRING_ID(
        LITERAL_AUID(0x8d56812a, 0x847e, 0x11d5, 0x93, 0x5a, 0x50, 0xf8, 0x57, 0xc1, 0x00, 0x00),
        L"ParameterDef_ScaleY");
    ADD_STRING_ID(
        LITERAL_AUID(0x062cfbd8, 0xf4b1, 0x4a50, 0xb9, 0x44, 0xf3, 0x9e, 0x2f, 0xc7, 0x3c, 0x17),
        L"ParameterDef_Rotation");
    ADD_STRING_ID(
        LITERAL_AUID(0x72a3b4a2, 0x873d, 0x4733, 0x90, 0x52, 0x9f, 0x83, 0xa7, 0x06, 0xca, 0x5b),
        L"ParameterDef_PinTopLeftX");
    ADD_STRING_ID(
        LITERAL_AUID(0x29e4d78f, 0xa502, 0x4ebb, 0x8c, 0x07, 0xed, 0x5a, 0x03, 0x20, 0xc1, 0xb0),
        L"ParameterDef_PinTopLeftY");
    ADD_STRING_ID(
        LITERAL_AUID(0xa95296c0, 0x1ed9, 0x4925, 0x84, 0x81, 0x20, 0x96, 0xc7, 0x2e, 0x81, 0x8d),
        L"ParameterDef_PinTopRightX");
    ADD_STRING_ID(
        LITERAL_AUID(0xce1757ae, 0x7a0b, 0x45d9, 0xb3, 0xf3, 0x36, 0x86, 0xad, 0xff, 0x1e, 0x2d),
        L"ParameterDef_PinTopRightY");
    ADD_STRING_ID(
        LITERAL_AUID(0x08b2bc81, 0x9b1b, 0x4c01, 0xba, 0x73, 0xbb, 0xa3, 0x55, 0x4e, 0xd0, 0x29),
        L"ParameterDef_PinBottomLeftX");
    ADD_STRING_ID(
        LITERAL_AUID(0xc163f2ff, 0xcd83, 0x4655, 0x82, 0x6e, 0x37, 0x24, 0xab, 0x7f, 0xa0, 0x92),
        L"ParameterDef_PinBottomLeftY");
    ADD_STRING_ID(
        LITERAL_AUID(0x53bc5884, 0x897f, 0x479e, 0xb8, 0x33, 0x19, 0x1f, 0x86, 0x92, 0x10, 0x0d),
        L"ParameterDef_PinBottomRightX");
    ADD_STRING_ID(
        LITERAL_AUID(0x812fb15b, 0x0b95, 0x4406, 0x87, 0x8d, 0xef, 0xaa, 0x1c, 0xff, 0xc1, 0x29),
        L"ParameterDef_PinBottomRightY");
    ADD_STRING_ID(
        LITERAL_AUID(0xa2667f65, 0x65d8, 0x4abf, 0xa1, 0x79, 0x0b, 0x9b, 0x93, 0x41, 0x39, 0x49),
        L"ParameterDef_AlphaKeyInvertAlpha");
    ADD_STRING_ID(
        LITERAL_AUID(0x21ed5b0f, 0xb7a0, 0x43bc, 0xb7, 0x79, 0xc4, 0x7f, 0x85, 0xbf, 0x6c, 0x4d),
        L"ParameterDef_LumKeyLevel");
    ADD_STRING_ID(
        LITERAL_AUID(0xcbd39b25, 0x3ece, 0x441e, 0xba, 0x2c, 0xda, 0x47, 0x3a, 0xb5, 0xcc, 0x7c),
        L"ParameterDef_LumKeyClip");
    ADD_STRING_ID(
        LITERAL_AUID(0xe4962321, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_Amplitude");
    ADD_STRING_ID(
        LITERAL_AUID(0xe4962322, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_Pan");
    ADD_STRING_ID(
        LITERAL_AUID(0x9e610007, 0x1be2, 0x41e1, 0xbb, 0x11, 0xc9, 0x5d, 0xe9, 0x96, 0x4d, 0x03),
        L"ParameterDef_OutgoingLevel");
    ADD_STRING_ID(
        LITERAL_AUID(0x48cea642, 0xa8f9, 0x455b, 0x82, 0xb3, 0x86, 0xc8, 0x14, 0xb7, 0x97, 0xc7),
        L"ParameterDef_IncomingLevel");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba1, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTESoft");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba2, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEBorder");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba3, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEPosition");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba4, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEModulator");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba5, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEShadow");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba6, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTETumble");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba7, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTESpotlight");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba8, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEReplicationH");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894ba9, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEReplicationV");
    ADD_STRING_ID(
        LITERAL_AUID(0x9c894baa, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTECheckerboard");
    ADD_STRING_ID(
        LITERAL_AUID(0x5f1c2560, 0x2415, 0x11d3, 0x8a, 0x4f, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_PhaseOffset");
    ADD_STRING_ID(
        LITERAL_AUID(0x3d1dd891, 0xe793, 0x11d2, 0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"Platform_Independent");
    ADD_STRING_ID(
        LITERAL_AUID(0x9fdef8c1, 0xe847, 0x11d2, 0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"Engine_None");
    ADD_STRING_ID(
        LITERAL_AUID(0x69c870a1, 0xe793, 0x11d2, 0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"PluginAPI_EssenceAccess");
    ADD_STRING_ID(
        LITERAL_AUID(0x56905e0b, 0x537d, 0x11d4, 0xa3, 0x6c, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a),
        L"PluginCategory_Codec");

}



