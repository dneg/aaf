/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/

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
