#ifndef __extensionUtils_h__
#define __extensionUtils_h__
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include "AAF.h"
#include <iostream.h>

//
// Structs, typedefs, and helper functions for use with the
// PersonnelResource exenstion example.
//



//********
//
// TypeID for extendable enumeration for eRole
//
// {5F72AD68-703D-11d3-8449-00600832ACB8}
static const aafUID_t kTypeID_eRole = 
{ 0x5f72ad68, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//
// Element values for eRole
//
//    Producer, 
// {5F72AD69-703D-11d3-8449-00600832ACB8}
static const aafUID_t kRole_Producer = 
{ 0x5f72ad69, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//  	Editor, 
// {5F72AD6A-703D-11d3-8449-00600832ACB8}
static const aafUID_t kRole_Editor = 
{ 0x5f72ad6a, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//  	FloorManager, 
// {5F72AD6B-703D-11d3-8449-00600832ACB8}
static const aafUID_t kRole_FloorManager = 
{ 0x5f72ad6b, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//********
//
// TypeID for Strong Reference to PersonnelResource
// {5F72AD73-703D-11d3-8449-00600832ACB8}
static const aafUID_t kTypeID_PersonnelResourceStrongReference = 
{ 0x5f72ad73, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//********
//
// TypeID for Vector of Strong References to PersonnelResource
// {5F72AD74-703D-11d3-8449-00600832ACB8}
static const aafUID_t kTypeID_PersonnelResourceStrongReferenceVector = 
{ 0x5f72ad74, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//********
//
// ClassID for PersonnelResource class
//
// {5F72AD6C-703D-11d3-8449-00600832ACB8}
static const aafUID_t kClassID_PersonnelResource = 
{ 0x5f72ad6c, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//
// PropertyIDs for PersonnelResource class properties
//
// {5F72AD6E-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_PersonnelResource_Name = 
{ 0x5f72ad6e, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
// {5F72AD6F-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_PersonnelResource_Role = 
{ 0x5f72ad6f, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
// {5F72AD70-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_PersonnelResource_ContractID = 
{ 0x5f72ad70, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//********
//
// ClassID for PersonnelMob class
//
// {5F72AD71-703D-11d3-8449-00600832ACB8}
static const aafUID_t kClassID_PersonnelMob = 
{ 0x5f72ad71, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//
// PropertyIDs for PersonnelMob class properties
//
// {5F72AD72-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_PersonnelMob_Personnel = 
{ 0x5f72ad72, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//********
//
// MobID for our PersonnelMob instance
//
// {5F72AD75-703D-11d3-8449-00600832ACB8}
static const aafUID_t kMobID_Personnel = 
{ 0x5f72ad75, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//
// macro for checking HRESULT return values
//
// argument 'r' will probably have side effects, so make sure it is
// evaluated once and only once, and outside of the assert() macro
// (since assertions may be turned off).
//
#define check(r) {if (FAILED (r)) assert(0); }



// For local in-memory use, we'll define the following typedef for
// handy manipulation of eRole extensible enumerations.
//
typedef aafUID_t  eJobFunction, eRole;  // eRole being replaced by eJobFunction

// Contract ID type
typedef aafUInt32 contractID_t;

// For local in-memory use, we'll define the following struct for
// handy manipulation of PersonnelResources.
//
static const size_t kMaxNameLen = 100;
struct PersonnelResource
{
  aafCharacter name[kMaxNameLen+1];
  eRole        role;

  // contract ID is optional
  bool         cid_present;
  contractID_t cid;
};

bool classDefinitionIsA ( IAAFClassDef *pClassDefQuery,
							   const aafUID_t targetAUID);

//
// Returns true if the roles match; returns false otherwise.
//
bool AreRolesEqual (const eRole & roleA, const eRole & roleB);

//
// Prints the given eRole.
//
void PrintRole (IAAFDictionary * pDict,
				const eRole role);

//
// Simple utility to  create PersonnelResource from name and role
// info.  No contract ID info will be present.
// 
PersonnelResource FormatResource (aafCharacter * name,
								  const eRole &  role);


//
// Simple utility to  create PersonnelResource from name, role, and
// contract ID info.
// 
PersonnelResource FormatResource (aafCharacter * name,
								  const eRole &  role,
								  contractID_t   cid);


//
// Prints the given PersonnelResource.
//
void PrintPersonnelResource (IAAFDictionary * pDict,
							 const PersonnelResource & resource);

//
// Prints all personnel resources in the given PersonnelMob.
//
void PrintPersonnelResources (IAAFDictionary * pDict,
							  IAAFMob * pMob);

//
// Set/Get name on a PersonnelResource object.  GetName requires that
// buflen be large enough to hold the name.  It can be determined from
// PersonnelRecordGetNameBufLen().
//
void PersonnelRecordSetName (IAAFObject * pObj,
							 const aafCharacter * name);
aafUInt32 PersonnelRecordGetNameBufLen (IAAFObject * pObj);
void PersonnelRecordGetName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen);

//
// Set/Get role from a PersonnelResource object.
//
void PersonnelRecordSetRole (IAAFObject * pObj,
							 eRole role);
eRole PersonnelRecordGetRole (IAAFObject * pObj);

//
// Set/Get contract ID on a PersonnelResource object.  Get requires
// that the contract ID property be present (that is, has already been
// set).  That can be determined from
// PersonnelRecordContractIDIsPresent().
//
void PersonnelRecordSetContractID (IAAFObject * pObj,
								   contractID_t cid);
bool PersonnelRecordContractIDIsPresent (IAAFObject * pObj);
contractID_t PersonnelRecordGetContractID (IAAFObject * pObj);


//
// Set/Get the personnel resource info in this object.
//
void PersonnelRecordSetInfo (IAAFObject * pObj,
							 const PersonnelResource & info);
PersonnelResource  PersonnelRecordGetInfo (IAAFObject * pObj);


//
// Prints the given string.
//
ostream& operator<< (ostream& s,
					 const aafCharacter * wstring);


#endif // ! __extensionUtils_h__
