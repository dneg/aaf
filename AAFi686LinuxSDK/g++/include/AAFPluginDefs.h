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

// Following are the UIDs of the well known constants used in AAFPluginDescriptors.
//

//******************
// Which platforms is the code known to run on?
//
const aafUID_t kAAFPlatformIndependant = { 0x3D1DD891, 0xE793, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

//******************
// Which media engines does the code require?
//
const aafUID_t kAAFNoEngine = { 0x9FDEF8C1, 0xE847, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

//******************
// Which API does this plugin support?
//
const aafUID_t kAAFEssencePluginAPI = { 0x69C870A1, 0xE793, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

//******************
// Used for "don't care" when enumerating loaded plugins
//
const aafUID_t kAAFPluginNoCategory = { 0x56905e0b, 0x537d, 0x11d4, { 0xa3, 0x6c, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

