/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

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
