#ifndef __AAFDataDefs_h__
#define __AAFDataDefs_h__
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Following are the UIDs of the well-known data definition objects in AAF.
//
// When an API call asks for a DDEF UID, store this into a local UID variable, and then pass
// in the address of the variable, as in:
//		aafUID_t	uid = DDEF_Picture;
//
// When you need an IAAFDataDef *, (ex: when you want to do DoesDataDefConvertTo()),
// then do the following (where pDictionary is a pointer to the
// dictionary object in the current file.)
//		aafUID_t	uid = DDEF_Picture;
//		IDataDef	*definition = NULL;
//		hr = pDictionary->LookupDataDefintion(&uid, &definition);
//		... Use the definition (ex: hr = pDefinition->DoesDataDefConvertTo (&uid, &converts); )
//		definition->Release()
//		definition = NULL;;
//
const aafUID_t DDEF_Picture = { 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_Matte = { 0x05CBA731, 0x1DAA, 0x11d3, { 0x80, 0xAD, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_PictureWithMatte = { 0x05CBA732, 0x1DAA, 0x11d3, { 0x80, 0xAD, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_Sound = { 0x78E1EBE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_Timecode = { 0x7F275E81, 0x77E5, 0x11D2, { 0x80, 0x7F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_Edgecode = { 0xd2bb2af0, 0xd234, 0x11d2, { 0x89, 0xee, 0x0, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const aafUID_t DDEF_Unknown = { 0x851419d0, 0x2e4f, 0x11d3, { 0x8a, 0x5b, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };


#endif // !  __AAFDataDefs_h__
