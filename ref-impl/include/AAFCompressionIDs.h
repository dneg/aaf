#ifndef __AAFCompressionIDs_h__
#define __AAFCompressionIDs_h__

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
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// AAF CompressionIDs

const aafUID_t AAF_CMPR_FULL_JPEG = { 0xedb35383, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t AAF_CMPR_AUNC422 = { 0xedb35391, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFCompression_IEC_DV_625_50 = { 0x04010202, 0x0201, 0x0200, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
const aafUID_t kAAFCompression_IEC_DV_525_60 = { 0x04010202, 0x0201, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
const aafUID_t kAAFCompression_LegacyDV = { 0xedb35390, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };

#endif // ! __AAFCompressionIDs_h__
