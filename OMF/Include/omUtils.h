/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

#ifndef _OMF_UTIL_API_
#define _OMF_UTIL_API_ 1

#include <stdio.h>

#include "omErr.h"
#include "omTypes.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

typedef enum
{
	kSwabIfNeeded, kNeverSwab
}               omfSwabCheck_t;

typedef omfBool(*omfDataValidityTest_t) (omfHdl_t file, 
										 const void *data,
										 omfErr_t *omfError);

#if OMFI_ENABLE_SEMCHECK
/************************************************************************
 *
 * Semantic Checking
 *
 ************************************************************************/

OMF_EXPORT omfErr_t omfsAddPropertyCheck(omfSessionHdl_t session, 
							  omfProperty_t prop, 
							  omfValidRev_t rev, 
							  omfDataValidityTest_t test, 
							  char *testName);
OMF_EXPORT omfErr_t omfsAddPropertyClass(	omfSessionHdl_t			session, 
							omfProperty_t			prop, 
							omfValidRev_t			rev, 
							omfClassIDPtr_t			dataClass);
OMF_EXPORT omfErr_t omfsCheckObjectType(omfHdl_t file, 
							 omfObject_t obj, 
							 omfProperty_t prop,
							 omfType_t type);
OMF_EXPORT omfErr_t omfsCheckDataValidity(omfHdl_t file, 
							   omfProperty_t prop, 
							   const void *data, 
							   omfAccessorType_t type);
OMF_EXPORT omfErr_t omfsCheckObjRefValidity(omfHdl_t file, 
								 omfProperty_t prop, 
								 omfObject_t refObj, 
								 omfAccessorType_t type);
OMF_EXPORT omfErr_t omfsGetSemanticTestName(omfSessionHdl_t session, 
								 omfInt16 buflen, 
								 char *buffer);
OMF_EXPORT omfErr_t omfsGetAccessorType(omfSessionHdl_t session, 
							 omfAccessorType_t * type);

OMF_EXPORT omfErr_t omfsSemanticCheckOn(omfHdl_t file);
OMF_EXPORT omfErr_t omfsSemanticCheckOff(omfHdl_t file);
OMF_EXPORT omfBool omfsIsSemanticCheckOn(omfHdl_t file);

OMF_EXPORT omfErr_t omfsFileCheckInit(omfHdl_t file, omfFileCheckHdl_t *hdl);
OMF_EXPORT omfErr_t omfsFileCheckCleanup(omfFileCheckHdl_t hdl);

OMF_EXPORT omfErr_t omfsFileCheckObjectIntegrity(
			omfFileCheckHdl_t hdl,
			omfCheckVerbose_t verbose,
			omfCheckWarnings_t warn, 
			omfInt32		*numErrors,
			omfInt32		*numWarnings,
			FILE *textOut);
			
OMF_EXPORT omfErr_t omfsFileCheckMobData(
			omfFileCheckHdl_t hdl,
			omfCheckVerbose_t verbose,
			omfCheckWarnings_t warn, 
			omfInt32		*numErrors,
			omfInt32		*numWarnings,
			FILE *textOut);

OMF_EXPORT omfErr_t omfsFileCheckMediaIntegrity(
			omfFileCheckHdl_t handle,
			omfCheckVerbose_t verbose,
			omfCheckWarnings_t warn, 
			omfInt32		*numErrors,
			omfInt32		*numWarnings,
			FILE *textOut);
#endif

/************************************************************************
 *
 * Class, type, and property registration and Information
 *
 ************************************************************************/
OMF_EXPORT omfErr_t omfsNewClass(
			omfSessionHdl_t	session,			/* IN - For this session */
			omfClassType_t		type,				/* IN - add a class of this type */
			omfValidRev_t		revs,				/* IN - valid for these revisions */
			omfClassIDPtr_t	aClass,			/* IN - with this classID */
			omfClassIDPtr_t	itsSuperclass);	/* IN - derived from this superclass */

OMF_EXPORT omfErr_t omfsRegisterType(
			omfSessionHdl_t	session,			/* IN - For this session */
			omfType_t			aType,			/* IN - Register this typecode */
			omfValidRev_t		revs,			/* IN - calid for these revisions */
			char				*typeName,		/* IN - With this name */
			omfSwabCheck_t		swabNeeded);	/* IN - Does this type depend on "endian-ness" */

OMF_EXPORT omfErr_t omfsRegisterProp(
			omfSessionHdl_t	session,		/* IN - For this session */
			omfProperty_t		aProp,		/* IN - Register this property code */
			omfValidRev_t		revs,			/* IN - valid for these revisions */
			char					*propName,	/* IN - with this name */
			omfClassIDPtr_t	classTag,	/* IN - attached to this class */
			omfType_t			type,			/* IN - with this type */
			omfPropertyOpt_t	opt);			/* IN - may be optional property */

OMF_EXPORT omfErr_t omfsRegisterDynamicType(
			omfSessionHdl_t	session,			/* IN - For this session, register a */
			omfValidRev_t		revs,			/* IN - type for these revisions */
			char				*typeName,		/* IN - with this name */
			omfSwabCheck_t		swabNeeded,		/* IN - Does this type depend on "endian-ness" */
			omfType_t			*resultPtr);	/* OUT - Return the type code */

OMF_EXPORT omfErr_t omfsRegisterDynamicProp(
			omfSessionHdl_t	session,		/* IN - For this session, register a */
			omfValidRev_t		revs,			/* IN - property for these revisions */
			char					*propName,	/* IN - with this name */
			omfClassIDPtr_t	classTag,	/* IN - attached to this class */
			omfType_t			type,			/* IN - with this type */
			omfPropertyOpt_t	opt,			/* IN - may be optional property */
			omfProperty_t		*resultPtr);	/* OUT - return the property code */

OMF_EXPORT omfErr_t omfsCreateDatakindDef(omfHdl_t file, omfUniqueName_t name, 
						   omfDDefObj_t *def);

OMF_EXPORT omfErr_t omfiDatakindRegister(
	omfHdl_t file,            /* IN - File Handle */
    omfUniqueNamePtr_t name,  /* IN - Datakind Unique Name */
	omfDDefObj_t defObject);  /* IN - Datakind Definition Object */

OMF_EXPORT omfErr_t omfiEffectDefRegister(
	omfHdl_t file,           /* IN - File Handle */
    omfUniqueNamePtr_t name, /* IN - Effect Definition Unique Name */
	omfDDefObj_t defObject); /* IN - Effect Definition object */

/************************************************************************
 *
 * General Object and Property Information Calls
 *
 ************************************************************************/

OMF_EXPORT void omfsGetDateTime(omfTimeStamp_t *ptime);

OMF_EXPORT omfErr_t omfiGetNextProperty(
	omfIterHdl_t iterHdl,    /* IN - Iterator Handle */
	omfObject_t obj,         /* IN - Object to iterator over */
	omfProperty_t *property, /* OUT - Property ID of "next" property */
	omfType_t *type);        /* OUT - Type ID of "next" type */

OMF_EXPORT omfErr_t omfiGetPropertyName(
	omfHdl_t file,               /* IN - File Handle */
	omfProperty_t property,      /* IN - Property ID */
	omfInt32 nameSize,           /* IN - Size of name buffer */
	omfUniqueNamePtr_t propName);/* IN/OUT - Property Unique Name */

OMF_EXPORT omfErr_t omfiGetPropertyTypename(
	omfHdl_t file,               /* IN - File Handle */
	omfType_t type,              /* IN - Type ID */
	omfInt32 nameSize,           /* IN - Size of name buffer */
	omfUniqueNamePtr_t typeName);/* IN/OUT - Type Unique Name */

OMF_EXPORT omfErr_t omfiGetNextObject(
	omfIterHdl_t iterHdl,    /* IN - Iterator Handle */
	omfObject_t *obj);       /* OUT - Next object */

OMF_EXPORT omfErr_t omfsClassFindSuperClass(omfHdl_t file, 
						omfClassIDPtr_t aClass, 
						omfClassIDPtr_t aSuperclass, 
						omfBool * found);

OMF_EXPORT omfBool omfsIsTypeOf(omfHdl_t file, 
				 omfObject_t anObject, 
				 omfClassIDPtr_t aClass, 
				 omfErr_t * errRtn);

/************************************************************************
 *
 * Container Functions
 *
 ************************************************************************/

OMF_EXPORT omfErr_t omfsFileGetValueAlignment(
			omfHdl_t		file,					/* IN - For this file */
			omfUInt32	*valueAlignment);	/* OUT - return the alignment factor */

OMF_EXPORT omfErr_t omfsFileSetValueAlignment(
			omfHdl_t		file,					/* IN - For this file */
			omfUInt32	valueAlignment);	/* IN - set the alignment factor */

OMF_EXPORT omfErr_t omfsFileGetDefaultByteOrder(
			omfHdl_t	file,				/* IN -- For this omf file */
			omfInt16	*order);			/* OUT -- return the default byte order. */

OMF_EXPORT omfErr_t omfsFileSetDefaultByteOrder(
			omfHdl_t file,				/* IN -- For this omf file */
			omfInt16 order);			/* OUT -- set the default byte order. */

OMF_EXPORT omfBool omfsIsPropPresent(
			omfHdl_t			file,		/* IN -- For this omf file */
			omfObject_t		obj,		/* IN -- in this object */
			omfProperty_t	prop,		/* IN -- read this property */
			omfType_t		dataType);/* IN -- check the type */

/************************************************************************
 *
 * Object Creation / Deletion Functions
 *
 ************************************************************************/

OMF_EXPORT omfErr_t omfsObjectNew(
			omfHdl_t				file,		/* IN - In this file */
			omfClassIDPtr_t	classID,	/* IN - create an object of this class */
			omfObject_t			*result);	/* OUT - and return the result here. */

OMF_EXPORT omfErr_t omfsObjectDelete(
			omfHdl_t		file,		/* IN - From this file */
			omfObject_t obj);		/* IN - delete this object */

/************************************************************************
 *
 * Basic Accessor Functions
 *
 ************************************************************************/
OMF_EXPORT omfErr_t omfsReadBoolean(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfBool 			*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteBoolean(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfBool			data);	/* IN - with this value */
			
OMF_EXPORT omfErr_t omfsReadInt8(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
		   char	        	*data);/* OUT - and return the result here */
		   
OMF_EXPORT omfErr_t omfsWriteInt8(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			char				data);	/* IN - with this value */
			
OMF_EXPORT omfErr_t omfsReadInt16(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t 	obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfInt16			*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteInt16(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfInt16			data);	/* IN - with this value */
			
OMF_EXPORT omfInt32 omfsLengthString(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop);		/* IN - Return the length */
			
OMF_EXPORT omfErr_t omfsReadString(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */
			char				*data,	/* IN - into a buffer */
			omfInt32			maxsize);	/* IN - of this size */
			
OMF_EXPORT omfErr_t omfsWriteString(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			const char		*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsReadUInt8(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfUInt8			*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteUInt8(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfUInt8			data);	/* IN - with this value */
			
OMF_EXPORT omfErr_t omfsReadUInt32(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfUInt32		*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteUInt32(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfUInt32		data);	/* IN - with this value */
			
OMF_EXPORT omfErr_t omfsReadUInt16(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfUInt16		*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteUInt16(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfUInt16		data);	/* IN - with this value */
			
OMF_EXPORT omfErr_t omfsReadVarLenBytes(
			omfHdl_t			file,			/* IN - From this file */
			omfObject_t		obj,			/* IN - and this object */
			omfProperty_t	prop,			/* IN - read this property */
			omfPosition_t	offset,		/* IN - from this Bento offset */
			omfInt32			maxsize,		/* IN - for this many bytes */
			void				*data,		/* IN/OUT - into this buffer */
			omfUInt32		*bytesRead);	/* OUT - and return the count here */

OMF_EXPORT omfErr_t omfsWriteVarLenBytes(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - write this property */
			omfPosition_t	offset,	/* IN - at this bento offset */
			omfInt32			count,	/* IN - for this many bytes */
			const void		*data);	/* IN - with this data */

OMF_EXPORT omfLength_t omfsLengthVarLenBytes(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop);	/* IN - get the length of this property */
			
OMF_EXPORT omfErr_t omfsReadInt32(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfInt32			*data);	/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteInt32(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfInt32			data);	/* IN - with this value */
			

OMF_EXPORT omfErr_t omfsGetByteOrder(
			omfHdl_t 	file,		/* IN -- For this omf file */
			omfObject_t obj,		/* IN -- in this object */
			omfInt16		*data);	/* OUT -- return the byte order. */

OMF_EXPORT omfErr_t omfsPutByteOrder(
			omfHdl_t 	file,	/* IN -- For this omf file */
			omfObject_t obj,	/* IN -- in this object */
		   omfInt16		data);	/* OUT -- set the byte order to "data". */

/************************************************************************
 *
 * OMFI-Specific Accessor Functions
 *
 ************************************************************************/
OMF_EXPORT omfErr_t omfsReadCharSetType(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - read this property */
			omfCharSetType_t	*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteCharSetType(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - Write this property */
			omfCharSetType_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadAttrKind(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - read this property */
			omfAttributeKind_t	*data);      /* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteAttrKind(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - Write this property */
			omfAttributeKind_t	data);	      /* IN - with this value */

OMF_EXPORT omfErr_t omfsReadEdgeType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfEdgeType_t	*data);	/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteEdgeType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t 	obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - Write this property */ 
			omfEdgeType_t	data);		/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadRational(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */
			omfRational_t	*data);	/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteRational(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfRational_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadFilmType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfFilmType_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteFilmType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */ 
			omfFilmType_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadDirectionCode(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfDirectionCode_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteDirectionCode(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */ 
			omfDirectionCode_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadColorSpace(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfColorSpace_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteColorSpace(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */ 
			omfColorSpace_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadTapeCaseType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfTapeCaseType_t	*data);/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteTapeCaseType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */ 
			omfTapeCaseType_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadTapeFormatType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfTapeFormatType_t	*data);/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteTapeFormatType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */ 
			omfTapeFormatType_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadVideoSignalType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfVideoSignalType_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteVideoSignalType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */ 
			omfVideoSignalType_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadJPEGTableIDType(
			omfHdl_t				file,		/* IN - From this file */
			omfObject_t			obj,		/* IN - and this object */
			omfProperty_t		prop,		/* IN - read this property */
			omfJPEGTableID_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteJPEGTableIDType(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - write this property */ 
			omfJPEGTableID_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadPhysicalMobType(
		   omfHdl_t					file,	/* IN - From this file */
			omfObject_t				obj,	/* IN - and this object */
			omfProperty_t			prop,	/* IN - read this property */
			omfPhysicalMobType_t	*data);/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWritePhysicalMobType(
			omfHdl_t					file,	/* IN - From this file */
			omfObject_t				obj,	/* IN - and this object */
			omfProperty_t			prop,	/* IN - write this property */ 
			omfPhysicalMobType_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsGetNthObjIndex(
			omfHdl_t					file,		/* IN - From this file */
			omfObject_t				obj,		/* IN - and this object */
			omfProperty_t			prop,		/* IN - and this property */
			omfObjIndexElement_t	*data,	/* OUT - read index element into here */
			omfInt32					i);			/* IN - from this index */

OMF_EXPORT omfErr_t omfsPutNthObjIndex(
			omfHdl_t							file,	/* IN - From this file */
			omfObject_t						obj,	/* IN - and this object */
			omfProperty_t					prop,	/* IN - and this property */
			const omfObjIndexElement_t	*data,/* IN = write this data */
			omfInt32							i);		/* IN = at this array index */

OMF_EXPORT omfErr_t omfsRemoveNthObjIndex(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - and this property */
			omfInt32			index);	/* IN - Remove this element index */

OMF_EXPORT omfErr_t omfsAppendObjIndex(
			omfHdl_t							file,	/* IN - From this file */
			omfObject_t						obj,	/* IN - and this object */
			omfProperty_t					prop,	/* IN - and this property */
			const omfObjIndexElement_t	*data);/* IN - Append this data */
			
OMF_EXPORT omfInt32 omfsLengthObjIndex(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object, return the */
			omfProperty_t	prop);	/* IN - number of elements in this array */
			
OMF_EXPORT omfErr_t omfsReadObjectTag(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t 	prop,	/* IN = read this property */
			omfTagPtr_t		data);	/* IN = into this array space */
			
OMF_EXPORT omfErr_t omfsWriteObjectTag(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN = write this property */
			const omfTagPtr_t	data);	/* IN - with this value */
			
OMF_EXPORT omfErr_t omfsReadObjRef(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfObject_t		*data);/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteObjRef(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - write this property */
			const omfObject_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsGetNthObjRefArray(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - and this property */
			omfObject_t		*data,	/* OUT - Get the object reference */
			omfInt32			i);			/* IN - at this index. */
			
OMF_EXPORT omfErr_t omfsPutNthObjRefArray(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - and this property */
			const omfObject_t	data,	/* IN - Write the object reference */
			omfInt32					i);	/* IN - at this index. */

OMF_EXPORT omfErr_t omfsRemoveNthObjRefArray(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - and this property */
			omfInt32			i);			/* IN - Remove this element index */

OMF_EXPORT omfErr_t omfsAppendObjRefArray(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - and this property */
			const omfObject_t	data);	/* IN - append this value */

OMF_EXPORT omfErr_t omfsInsertObjRefArray(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - inset in this property */
			omfObject_t		data,	/* IN - this data */
			omfInt32			pos);	/* IN - at this position */

OMF_EXPORT omfInt32 omfsLengthObjRefArray(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object, return the  */
			omfProperty_t	prop);	/* IN - # of of elements in this array */

OMF_EXPORT omfErr_t omfsReadTimeStamp(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */
			omfTimeStamp_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteTimeStamp(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfTimeStamp_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadUID(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfUID_t			*data);/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteUID(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfUID_t			data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadUsageCodeType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfUsageCode_t	*data);/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteUsageCodeType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfUsageCode_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadVersionType(
			omfHdl_t				file,		/* IN - From this file */
			omfObject_t			obj,		/* IN - and this object */
			omfProperty_t		prop,		/* IN - read this property */
			omfVersionType_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t  omfsWriteVersionType(
			omfHdl_t				file,		/* IN - From this file */
			omfObject_t			obj,		/* IN - and this object */
			omfProperty_t		prop,		/* IN - write this property */
			omfVersionType_t	data);		/* IN - with this value */

OMF_EXPORT omfErr_t omfsWriteArgIDType(
			omfHdl_t			file,	/* IN - Into this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfArgIDType_t	data);	/* IN - with this data */

OMF_EXPORT omfErr_t omfsReadArgIDType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfArgIDType_t	*data);/* OUT - and return the data here */

OMF_EXPORT omfErr_t omfsReadTrackType(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */
			omfInt16			*data);	/* OUT - and return the result here */
			
OMF_EXPORT omfErr_t omfsWriteTrackType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfInt16			data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadPosition(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */
			omfPosition_t	*data);   /* OUT -- The data returned */

OMF_EXPORT omfErr_t omfsWritePosition(
			omfHdl_t			file,	/* IN - Into this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfPosition_t	data);	/* IN -- with this data */

OMF_EXPORT omfErr_t omfsReadLength(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfLength_t		*data);/* OUT -- and return the data here */

OMF_EXPORT omfErr_t omfsWriteLength(
			omfHdl_t			file,	/* IN - Into this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfLength_t		data);	/* IN -- with this data */

OMF_EXPORT omfErr_t omfsReadUniqueName(
			omfHdl_t					file,		/* IN - From this file */
			omfObject_t				obj,		/* IN - and this object */
			omfProperty_t			prop,		/* IN - read this property */
			omfUniqueNamePtr_t 	data,		/* IN - and return the data into */
			omfInt32					bufsize);	/* IN - a buffer of this size */

OMF_EXPORT omfErr_t omfsWriteUniqueName(
			omfHdl_t							file,	/* IN - Into this file */
			omfObject_t						obj,	/* IN - and this object */
			omfProperty_t					prop,	/* IN - write this property */
			const omfUniqueNamePtr_t	data);	/* IN - with this data */

OMF_EXPORT omfErr_t omfsReadFadeType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop, /* IN - read this property */
			omfFadeType_t	*data);/* OUT -- and return the data here */

OMF_EXPORT omfErr_t omfsWriteFadeType(
			omfHdl_t			file,	/* IN - Into this file */
			omfObject_t		obj,	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfFadeType_t	data);	/* IN -- with this data */

OMF_EXPORT omfErr_t omfsReadInterpKind(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj, 	/* IN - and this object */
			omfProperty_t		prop,	/* IN - read this property */
			omfInterpKind_t	*data);/* OUT -- and return the data here */

OMF_EXPORT omfErr_t omfsWriteInterpKind(
			omfHdl_t				file,	/* IN - Into this file */
			omfObject_t			obj, 	/* IN - and this object */
			omfProperty_t		prop,	/* IN - write this property */
			omfInterpKind_t	data);	/* IN -- with this data */

OMF_EXPORT omfErr_t omfsReadEditHint(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj, 	/* IN - and this object */
			omfProperty_t	prop,	/* IN - read this property */
			omfEditHint_t	*data);/* OUT - and return the data here */

OMF_EXPORT omfErr_t omfsWriteEditHint(
			omfHdl_t			file,	/* IN - Into this file */
			omfObject_t		obj, 	/* IN - and this object */
			omfProperty_t	prop,	/* IN - write this property */
			omfEditHint_t	data);	/* IN - with this data */

OMF_EXPORT omfErr_t omfsReadDataValue(
			omfHdl_t			file,			/* IN - From this file */
			omfObject_t		obj, 			/* IN - and this object */
			omfProperty_t	prop, 		/* IN - read this property */
			omfDDefObj_t   datakind,   /* IN - datakind object */
			omfDataValue_t	data,			/* IN -- into a buffer. */
			omfPosition_t	offset,		/* IN - Read from this offset */
			omfUInt32		maxsize,		/* IN - into a buffer of this size */
			omfUInt32		*bytesRead);	/* OUT - and return the bytes read */

OMF_EXPORT omfErr_t omfsWriteDataValue(
			omfHdl_t					file,		/* IN - Into this file */
			omfObject_t				obj, 		/* IN - and this object */
			omfProperty_t			prop, 	/* IN - write this property */
			omfDDefObj_t   datakind,  		 /* IN - datakind object */
			omfDataValue_t data,		       /* IN - with this data */
			omfPosition_t			offset,	/* IN - at this byte offset */
			omfUInt32				count);	/* IN - for this many bytes */

OMF_EXPORT omfLength_t omfsLengthDataValue(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object, return the */
			omfProperty_t	prop); /* IN - length of this property. */

OMF_EXPORT omfErr_t omfsRemovePropData(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop, 	/* IN -  with this property */
			omfPosition_t		offset,	/* IN - at this byte offset */
			omfLength_t			count);	/* IN - for this many bytes */

OMF_EXPORT omfErr_t omfsReadClassID(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t 		obj,	/* IN - and this object */
			omfProperty_t				prop,
			omfClassIDPtr_t	data);	/* IN - read the classID */
			 
OMF_EXPORT omfErr_t omfsWriteClassID(
			omfHdl_t						file,	/* IN - Into this file */
			omfObject_t					obj, 	/* IN - and this object */
			omfProperty_t				prop,
			const omfClassIDPtr_t	data);	/* IN - write the classID */

OMF_EXPORT omfInt32 omfsNumAttributes(
			omfHdl_t		file,	/* IN - From this file */
			omfObject_t		obj,	/* IN - and this object, return the  */
			omfProperty_t	prop);	/* IN - # of of elements in this array */

OMF_EXPORT omfErr_t omfsGetNthAttribute(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - and this property */
			omfObject_t		*data,	/* OUT - Get the object reference */
			omfInt32			i);			/* IN - at this index (1-based) */

OMF_EXPORT omfErr_t omfsAppendAttribute(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - and this property */
			const omfObject_t	data);	/* IN - append this value */

OMF_EXPORT omfErr_t omfsReadProductVersionType(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t				obj,	/* IN - and this object */
			omfProperty_t			prop,	/* IN - read this property */
			omfProductVersion_t	*data);		/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteProductVersionType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - write this property */
			omfProductVersion_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadPulldownKindType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfPulldownKind_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWritePulldownKindType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - write this property */ 
			omfPulldownKind_t	data);	/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadPhaseFrameType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfPhaseFrame_t	*data);		/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWritePhaseFrameType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - write this property */ 
			omfPhaseFrame_t	data);		/* IN - with this value */

OMF_EXPORT omfErr_t omfsReadPulldownDirectionType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */ 
			omfPulldownDir_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWritePulldownDirectionType(
			omfHdl_t			file,	/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - write this property */ 
			omfPulldownDir_t	data);	/* IN - with this value */

/************************************************************************
 *
 * 1.5 File Compatability Functions (Don't call for 2.0 files)
 *
 ************************************************************************/
OMF_EXPORT omfErr_t omfsReadExactEditRate(
			omfHdl_t			file,		/* IN - From this file */
			omfObject_t		obj,		/* IN - and this object */
			omfProperty_t	prop,		/* IN - read this property */
			omfRational_t	*data);	/* OUT - and return the result here */

OMF_EXPORT omfErr_t omfsWriteExactEditRate(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj,	/* IN - and this object */
			omfProperty_t		prop,	/* IN - write this property */
			omfRational_t		data);	/* IN - with this value */

/************************************************************************
 *
 * Utility Functions
 *
 ************************************************************************/

OMF_EXPORT void *omfsMalloc(
			size_t size);	/* Allocate this many bytes */

OMF_EXPORT void omfsFree(
			void *ptr);	/* Free up this buffer */

OMF_EXPORT omfErr_t omfsGetHeadObject(
			omfHdl_t		file,		/* IN -- For this omf file */
			omfObject_t	*head);	/* OUT -- return the head object */

OMF_EXPORT omfErr_t        omfsFindSourceMobByID(omfHdl_t file, 
									  omfUID_t mobID, 
									  omfObject_t * result);

OMF_EXPORT omfErr_t omfsSetProgressCallback(
			omfHdl_t				file,		/* IN - For this file */
			omfProgressProc_t aProc);	/* IN - Set this progress callback */

OMF_EXPORT void omfsFixShort(
			omfInt16 * wp);	/* IN/OUT -- Byte swap this value */
			
OMF_EXPORT void omfsFixLong(
			omfInt32 *lp);	/* IN/OUT -- Byte swap this value */

OMF_EXPORT void omfsFixLong64(
			omfInt64 *lp);	/* IN/OUT -- Byte swap this value */

#define PROGRESS_UNKNOWN	-1
	
/************************************************************************
 *
 * Low level Bento wrapper functions, which should only be needed by those
 * developers adding new types or properties.
 *
 ************************************************************************/
OMF_EXPORT omfErr_t OMReadProp(
			omfHdl_t			file,			/* IN - For this omf file */
			omfObject_t		obj,			/* IN - in this object */
			omfProperty_t	prop,			/* IN - read this property */
			omfPosition_t	offset,		/* IN - at this Bento Offset. */
			omfSwabCheck_t		swabType,	/* IN - Swab the data (optional) */
			omfType_t		dataType,	/* IN - check the type */
			omfInt32			dataSize,	/* IN --and size */
			void				*data);	/* IN/OUT - and put the result here. */

OMF_EXPORT omfErr_t OMGetPropertyFileOffset(
			omfHdl_t			file,			/* IN - For this omf file */
			omfObject_t		obj,			/* IN - in this object */
			omfProperty_t	prop,			/* IN - find the file offset of this property */
			omfPosition_t		offset,			/* IN - at this Bento Offset. */
			omfType_t		dataType,		/* IN - check the type */
			omfLength_t		*result);	/* OUT - and put the result here. */

OMF_EXPORT omfErr_t OMWriteProp(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- read this property */
			omfPosition_t	offset,		/* IN -- at this Bento Offset. */
			omfType_t		dataType,	/* IN -- check the type */
			omfInt32			dataSize,	/* IN -- and size. */
			const void		*data);		/* IN -- This buffer points to the data. */

OMF_EXPORT omfErr_t OMGetNumPropertySegments(
			omfHdl_t		file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- is property */
			omfType_t		dataType,		/* IN -- of this type */
			omfInt32		*numSegments);	/* OUT -- how many segments are there? */

OMF_EXPORT omfErr_t OMGetPropertySegmentInfo(
			omfHdl_t		file,		/* IN -- For this omf file */
			omfObject_t		obj,		/* IN -- in this object */
			omfProperty_t	prop,		/* IN -- is property */
			omfType_t		dataType,	/* IN -- of this type */
			omfInt32		index,		/* IN -- for this segment */
			omfPosition_t	*startPos,	/* OUT -- where does the segment begin, */
			omfLength_t		*length);	/* OUT -- and how long is it? */

OMF_EXPORT omfErr_t OMReadBaseProp(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- read this property */
			omfType_t		dataType,	/* IN -- Check the type */
			omfInt32			dataSize,	/* IN -- and size */
			void				*data);		/* IN/OUT -- and put the result here. */

OMF_EXPORT omfErr_t OMWriteBaseProp(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- write this property */
			omfType_t		dataType,	/* IN -- Check the type */
			omfInt32			dataSize,	/* IN -- and size */
			const void		*data);		/* IN -- This buffer points to the data. */

OMF_EXPORT omfErr_t OMIsPropertyContiguous(
			omfHdl_t			file,				/* IN -- For this omf file */
			omfObject_t		obj,				/* IN -- in this object */
			omfProperty_t	prop,				/* IN -- is property */
			omfType_t		dataType,		/* IN -- of this type */
			omfBool			*isContig);		/* OUT -- contiguous? */

OMF_EXPORT omfErr_t OMLengthProp(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- find the length of this properties value */
			omfType_t		dataType,	/* IN -- Check the type */
			omfInt32			defaultVal,	/* IN -- and pass back this if property is missing */
			omfLength_t		*length);	/* OUT -- Return the length through here. */

OMF_EXPORT omfErr_t OMRemovePropData(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- find the length of this properties value */
			omfType_t		dataType,	/* IN -- Check the type */
			omfPosition_t	offset,	/* IN - at this byte offset */
			omfLength_t		length);	/* IN - for this many bytes */

OMF_EXPORT omfErr_t OMPutNthPropHdr(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- and this property */
			omfInt32			index,		/* IN -- Set this array element */
			omfType_t		dataType,	/* IN - array element type */
			omfInt32			dataSize,	/* IN - array element size in bytes */
			omfPosition_t	*outOffset);	/* OUT - Here is where to write the data */

OMF_EXPORT omfErr_t OMGetNthPropHdr(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- and this property */
			omfInt32			index,		/* IN -- Set this array element header */
			omfType_t		dataType,	/* IN - array element type */
			omfInt32			dataSize,	/* IN - array element size in bytes */
			omfPosition_t	*outOffset);	/* OUT - Here is where to read the data */

OMF_EXPORT omfErr_t omfsGetArrayLength(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- and this property */
			omfType_t		dataType,	/* IN -- of this data type */
			omfInt32			dataSize,	/* IN -- with entries of this size */
			omfInt32			*length);		/* OUT - return the number of elements */
OMF_EXPORT omfErr_t OMRemoveNthArrayProp(
			omfHdl_t			file,			/* IN -- For this omf file */
			omfObject_t		obj,			/* IN -- in this object */
			omfProperty_t	prop,			/* IN -- delete this array index */
			omfType_t		dataType,	/* IN -- Check the type */
			omfInt32			dataSize,	/* IN -- and size */
			const omfInt32	index);		/* IN - The element index */

OMF_EXPORT omfErr_t omfsReadLayoutType(
			omfHdl_t				file,	/* IN - From this file */
			omfObject_t			obj, 	/* IN - and this object */
			omfProperty_t		prop,	/* IN - read this property */
			omfFrameLayout_t	*data);/* OUT - and return the data here */

OMF_EXPORT omfErr_t omfsWriteLayoutType(
			omfHdl_t				file,	/* IN - Into this file */
			omfObject_t			obj, 	/* IN - and this object */
			omfProperty_t		prop,	/* IN - write this property */
			omfFrameLayout_t	data);	/* IN - with this data */

OMF_EXPORT omfInt32 omfsGetBentoID(
								omfHdl_t file,		/* IN -- For this omf file */
								omfObject_t obj);	/* IN -- objID for this object */

#ifdef VIRTUAL_BENTO_OBJECTS
OMF_EXPORT omfErr_t omfsObjectPurge(
			omfHdl_t				file,				/* IN -- For this omf file */
			omfObject_t			anObject);		/* IN -- see if this object */
#endif

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_UTIL_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
