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
#include "AAFTypes.h"
#include <iostream.h>

//
// Structs, typedefs, and helper functions for use with the
// PersonnelResource exenstion example.
//



//********
//
// TypeID for extendable enumeration for ePosition
//
// {5F72AD68-703D-11d3-8449-00600832ACB8}
static const aafUID_t kTypeID_ePosition = 
{ 0x5f72ad68, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//
// Element values for ePosition
//
//    Producer, 
// {5F72AD69-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPosition_Producer = 
{ 0x5f72ad69, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//  	Editor, 
// {5F72AD6A-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPosition_Editor = 
{ 0x5f72ad6a, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//  	FloorManager, 
// {5F72AD6B-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPosition_FloorManager = 
{ 0x5f72ad6b, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

//		Actor
//  {F611B4AE-9070-11d3-A6A8-00609419FDAF}
static const aafUID_t kPosition_Actor  = 
{ 0xf611b4ae, 0x9070, 0x11d3, { 0xa6, 0xa8, 0x0, 0x60, 0x94, 0x19, 0xfd, 0xaf } };



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
static const aafUID_t kPropID_PersonnelResource_FamilyName = 
{ 0x5f72ad6e, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
// {E329CF70-9147-11d3-A6A9-00609419FDAF}
static const aafUID_t kPropID_PersonnelResource_GivenName = 
{ 0xe329cf70, 0x9147, 0x11d3, { 0xa6, 0xa9, 0x0, 0x60, 0x94, 0x19, 0xfd, 0xaf } };

//
// {5F72AD6F-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_PersonnelResource_Position = 
{ 0x5f72ad6f, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
// {5F72AD70-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_PersonnelResource_ContractID = 
{ 0x5f72ad70, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {E329CF71-9147-11d3-A6A9-00609419FDAF}
static const aafUID_t kPropID_PersonnelResource_Role = 
{ 0xe329cf71, 0x9147, 0x11d3, { 0xa6, 0xa9, 0x0, 0x60, 0x94, 0x19, 0xfd, 0xaf } };

//********
//
// ClassID for AdminMob class
//
// {5F72AD71-703D-11d3-8449-00600832ACB8}
static const aafUID_t kClassID_AdminMob = 
{ 0x5f72ad71, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };
//
//
// PropertyIDs for AdminMob class properties
//
// {5F72AD72-703D-11d3-8449-00600832ACB8}
static const aafUID_t kPropID_AdminMob_Personnel = 
{ 0x5f72ad72, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//********
//
// MobID for our AdminMob instance
//
// {5F72AD75-703D-11d3-8449-00600832ACB8}
static const aafUID_t kMobID_Personnel = 
{ 0x5f72ad75, 0x703d, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//
// macro for checking HRESULT return values
//
// argument 'r' will probably have side effects, so make sure it is
// evaluated once and only once.
//
#define check(r) {if (FAILED (r)) throw r; }



// For local in-memory use, we'll define the following typedef for
// handy manipulation of ePosition extensible enumerations.
//
typedef aafUID_t  ePosition;  

// Contract ID type
typedef aafUInt32 contractID_t;



static void SetStringProperty (IAAFObject * pObj,
						aafUID_constref propertyID,
						aafCharacter_constptr strValue);

bool classDefinitionIsA ( IAAFClassDef *pClassDefQuery,
							   aafUID_constref targetAUID);


// Main functions for PersonnelResource
void PersonnelResourceSetFamilyName(IAAFObject *pResource,
							   aafCharacter_constptr familyName);

void PersonnelResourceSetGivenName(IAAFObject *pResource,
							   aafCharacter_constptr givenName);
void PersonnelResourceSetPosition(IAAFObject *pResource,
							   const ePosition& position);
void PersonnelResourceSetActorRole (IAAFObject *pResource,
								  aafCharacter_constptr role);
void PersonnelResourceSetContractID (IAAFObject * pObj,
								   contractID_t cid);
void PersonnelResourceInitialize(IAAFObject *pResource,
							   aafCharacter_constptr familyName,
							   aafCharacter_constptr givenName,
							   const ePosition& position);
void PersonnelResourceGetGivenNameBufLen (IAAFObject * pObj, aafUInt32 *bufLen);


void PersonnelResourceGetGivenName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen);
void PersonnelResourceGetFamilyNameBufLen (IAAFObject * pObj, aafUInt32 *bufLen);


void PersonnelResourceGetFamilyName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen);

void PersonnelResourceGetPosition (IAAFObject * pResource,
							 ePosition *position);

HRESULT PersonnelResourceGetContractID (IAAFObject * pObj,
								   contractID_t *cid);
HRESULT PersonnelResourceGetActorNameBufLen (IAAFObject *pObj,
										   aafUInt32 *bufLen);
HRESULT PersonnelResourceGetActorName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen);

void AdminMobAppendResource (IAAFDictionary *pDict,
									IAAFMob *pAdminMob,
							        IAAFObject *pResource);
void AdminMobGetNumPersonnel (IAAFDictionary * pDict,
							  IAAFMob * pMob,
							  aafUInt32 *numPersonnel);
void AdminMobGetNthPersonnel (IAAFDictionary * pDict,
							  IAAFMob * pMob,
							  aafUInt32 index,
							  IAAFObject **personnelResource);


//
// Create and registration functions.
//
void CreateAndRegisterPositionEnum (IAAFDictionary * pDict);
void CreateAndRegisterPersonnelResource (IAAFDictionary * pDict);
void CreateAndRegisterPersonnelResourceReference(IAAFDictionary * pDict);
void CreateAndRegisterPersonnelResourceReferenceVector(IAAFDictionary * pDict);
void CreateAndRegisterAdminMob (IAAFDictionary * pDict);
void DefineResourceClassExtensions(IAAFDictionary *pDict);
void VerifyResourceClassExtensions(IAAFDictionary *pDict);



//
// Returns true if the positions match; returns false otherwise.
//
bool ArePositionsEqual (const ePosition & positionA, const ePosition & positionB);

//
// Prints the given ePosition.
//
void PrintPosition (IAAFDictionary * pDict,
				const ePosition & position);




//
// Prints the given string.
//
// On Irix CC, this operator conflicts with ostream::<<(const wchar_t*)
#if !defined( OS_IRIX )
ostream& operator<< (ostream& s,
					 aafCharacter_constptr wstring);
#endif

#endif // ! __extensionUtils_h__

