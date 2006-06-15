#ifndef __AAFResult_h__
#define __AAFResult_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#define AAFRESULT_FAILED(Status) ((AAFRESULT)(Status)<0)
#define AAFRESULT_SUCCEEDED(Status) (!(AAFRESULT_FAILED(Status)))

#define _FACILITY_AAF 0x12
#define MAKE_AAFHRESULT( code ) \
    ((HRESULT) (((aafUInt32)(1)             <<31) | \
                ((aafUInt32)(_FACILITY_AAF) <<16) | \
                ((aafUInt32)(code))) )

/* Non-AAF codes */
#define AAFRESULT_NOT_IMPLEMENTED                 ((HRESULT)0x80004001L)

/* Success codes */
#define AAFRESULT_SUCCESS                                   0

/* SESSION/FILE Error Codes */
#define AAFRESULT_BAD_SESSION                     ((HRESULT)0x80120010)
#define AAFRESULT_BADSESSIONOPEN                  ((HRESULT)0x80120011)
#define AAFRESULT_BADSESSIONMETA                  ((HRESULT)0x80120012)
#define AAFRESULT_BADSESSIONCLOSE                 ((HRESULT)0x80120013)
#define AAFRESULT_BADCONTAINER                    ((HRESULT)0x80120014)
#define AAFRESULT_FILEREV_NOT_SUPP                ((HRESULT)0x80120015)
#define AAFRESULT_FILEREV_DIFF                    ((HRESULT)0x80120016)
#define AAFRESULT_OPEN_FAILURE                    ((HRESULT)0x80120017)
#define AAFRESULT_BADCLOSE                        ((HRESULT)0x80120018)
#define AAFRESULT_BAD_FHDL                        ((HRESULT)0x80120019)
#define AAFRESULT_BADHEAD                         ((HRESULT)0x8012001A)
#define AAFRESULT_NOBYTEORDER                     ((HRESULT)0x8012001B)
#define AAFRESULT_INVALID_BYTEORDER               ((HRESULT)0x8012001C)
#define AAFRESULT_NOT_AAF_FILE                    ((HRESULT)0x8012001D)
#define AAFRESULT_WRONG_FILETYPE                  ((HRESULT)0x8012001E)
#define AAFRESULT_WRONG_OPENMODE                  ((HRESULT)0x8012001F)
#define AAFRESULT_CONTAINERWRITE                  ((HRESULT)0x80120020)
#define AAFRESULT_FILE_NOT_FOUND                  ((HRESULT)0x80120021)
#define AAFRESULT_CANNOT_SAVE_CLSD                ((HRESULT)0x80120022)
#define AAFRESULT_CANNOT_LOAD_CLSD                ((HRESULT)0x80120023)
#define AAFRESULT_FILE_REV_200                    ((HRESULT)0x80120024)
#define AAFRESULT_NEED_PRODUCT_IDENT              ((HRESULT)0x80120025)
#define AAFRESULT_NOT_WRITEABLE                   ((HRESULT)0x80120026)
#define AAFRESULT_NOT_READABLE                    ((HRESULT)0x80120027)
#define AAFRESULT_FILE_EXISTS                     ((HRESULT)0x80120028)
#define AAFRESULT_NOT_OPEN                        ((HRESULT)0x80120029)
#define AAFRESULT_ALREADY_OPEN                    ((HRESULT)0x8012002A)
#define AAFRESULT_BAD_FLAGS                       ((HRESULT)0x8012002B)
#define AAFRESULT_BAD_FLAG_COMBINATION            ((HRESULT)0x8012002C)
#define AAFRESULT_UNSAVED_CHANGES                 ((HRESULT)0x8012002D)
#define AAFRESULT_NOT_REVERTABLE                  ((HRESULT)0x8012002E)
#define AAFRESULT_MEDIA_NOT_REVERTABLE            ((HRESULT)0x8012002F)
#define AAFRESULT_OPERATION_NOT_PERMITTED         ((HRESULT)0x80120030)

/* Errors returned from raw storage interfaces */
#define AAFRESULT_READ_FAILURE                    ((HRESULT)0x80120031)
#define AAFRESULT_WRITE_FAILURE                   ((HRESULT)0x80120032)
#define AAFRESULT_SYNCHRONIZE_FAILURE             ((HRESULT)0x80120033)
#define AAFRESULT_SET_EXTENT_FAILURE              ((HRESULT)0x80120034)
#define AAFRESULT_NOT_CREATABLE                   ((HRESULT)0x80120035)
#define AAFRESULT_NOT_MODIFIABLE                  ((HRESULT)0x80120036)
#define AAFRESULT_GETSIZE_FAILURE                 ((HRESULT)0x80120037)
#define AAFRESULT_GETEXTENT_FAILURE               ((HRESULT)0x80120038)
#define AAFRESULT_SETEXTENT_FAILURE               ((HRESULT)0x80120039)

/* Read-while-modify errors */
#define AAFRESULT_FILE_BEING_MODIFIED             ((HRESULT)0x8012003A)

/* MEDIA Error Codes */
#define AAFRESULT_DESCSAMPRATE                    ((HRESULT)0x80120064)
#define AAFRESULT_SOURCEMOBLIST                   ((HRESULT)0x80120065)
#define AAFRESULT_DESCLENGTH                      ((HRESULT)0x80120066)
#define AAFRESULT_INTERNAL_MDO                    ((HRESULT)0x80120067)
#define AAFRESULT_3COMPONENT                      ((HRESULT)0x80120068)
#define AAFRESULT_INVALID_PARM_SIZE               ((HRESULT)0x80120069)
#define AAFRESULT_BADSAMPLEOFFSET                 ((HRESULT)0x8012006A)
#define AAFRESULT_ONESAMPLEREAD                   ((HRESULT)0x8012006B)
#define AAFRESULT_ONESAMPLEWRITE                  ((HRESULT)0x8012006C)
#define AAFRESULT_DECOMPRESS                      ((HRESULT)0x8012006D)
#define AAFRESULT_NODATA                          ((HRESULT)0x8012006E)
        /* Sample too large to fit in given buffer */
#define AAFRESULT_SMALLBUF                        ((HRESULT)0x8012006F)
#define AAFRESULT_BADCOMPR                        ((HRESULT)0x80120070)
#define AAFRESULT_BADPIXFORM                      ((HRESULT)0x80120071)
#define AAFRESULT_BADLAYOUT                       ((HRESULT)0x80120072)
#define AAFRESULT_COMPRLINESWR                    ((HRESULT)0x80120073)
#define AAFRESULT_COMPRLINESRD                    ((HRESULT)0x80120074)
#define AAFRESULT_BADMEDIATYPE                    ((HRESULT)0x80120075)
#define AAFRESULT_BADDATAADDRESS                  ((HRESULT)0x80120076)
#define AAFRESULT_BAD_MDHDL                       ((HRESULT)0x80120077)
#define AAFRESULT_MEDIA_NOT_FOUND                 ((HRESULT)0x80120078)
#define AAFRESULT_ILLEGAL_MEMFMT                  ((HRESULT)0x80120079)
#define AAFRESULT_ILLEGAL_FILEFMT                 ((HRESULT)0x8012007A)
#define AAFRESULT_SWABBUFSIZE                     ((HRESULT)0x8012007B)
#define AAFRESULT_MISSING_SWAP_PROC               ((HRESULT)0x8012007C)
#define AAFRESULT_NULL_STREAMPROC                 ((HRESULT)0x8012007D)
#define AAFRESULT_NULLBUF                         ((HRESULT)0x8012007E)
#define AAFRESULT_SWAP_SETUP                      ((HRESULT)0x8012007F)
#define AAFRESULT_INVALID_FILE_MOB                ((HRESULT)0x80120080)
#define AAFRESULT_SINGLE_CHANNEL_OP               ((HRESULT)0x80120081)
#define AAFRESULT_INVALID_CACHE_SIZE              ((HRESULT)0x80120082)
#define AAFRESULT_NOT_FILEMOB                     ((HRESULT)0x80120083)
#define AAFRESULT_TRANSLATE_SAMPLE_SIZE           ((HRESULT)0x80120084)
#define AAFRESULT_TRANSLATE_NON_INTEGRAL_RATE     ((HRESULT)0x80120085)
#define AAFRESULT_MISSING_MEDIA_REP               ((HRESULT)0x80120086)
#define AAFRESULT_NOT_LONGWORD                    ((HRESULT)0x80120087)
#define AAFRESULT_XFER_DUPCH                      ((HRESULT)0x80120088)
#define AAFRESULT_MEDIA_NOT_INIT                  ((HRESULT)0x80120089)
#define AAFRESULT_BLOCKING_SIZE                   ((HRESULT)0x8012008A)
#define AAFRESULT_WRONG_MEDIATYPE                 ((HRESULT)0x8012008B)
#define AAFRESULT_MULTI_WRITELEN                  ((HRESULT)0x8012008C)
#define AAFRESULT_STREAM_REOPEN                   ((HRESULT)0x8012008D)
#define AAFRESULT_TOO_MANY_FMT_OPS                ((HRESULT)0x8012008E)
#define AAFRESULT_MEDIASTREAM_NOTALLOWED          ((HRESULT)0x8012008F)
#define AAFRESULT_STILLFRAME_BADLENGTH            ((HRESULT)0x80120090)
#define AAFRESULT_DATA_NONCONTIG                  ((HRESULT)0x80120091)
#define AAFRESULT_OPLIST_OVERFLOW                 ((HRESULT)0x80120092)
#define AAFRESULT_STREAM_CLOSED                   ((HRESULT)0x80120093)
#define AAFRESULT_USE_MULTI_CREATE                ((HRESULT)0x80120094)
#define AAFRESULT_MEDIA_OPENMODE                  ((HRESULT)0x80120095)
#define AAFRESULT_MEDIA_CANNOT_CLOSE              ((HRESULT)0x80120096)
#define AAFRESULT_CODEC_INVALID                   ((HRESULT)0x80120097)
#define AAFRESULT_INVALID_OP_CODEC                ((HRESULT)0x80120098)
#define AAFRESULT_BAD_CODEC_REV                   ((HRESULT)0x80120099)
#define AAFRESULT_CODEC_CHANNELS                  ((HRESULT)0x8012009A)
#define AAFRESULT_INTERN_TOO_SMALL                ((HRESULT)0x8012009B)
#define AAFRESULT_INTERNAL_UNKNOWN_LOC            ((HRESULT)0x8012009C)
#define AAFRESULT_TRANSLATE                       ((HRESULT)0x8012009D)
#define AAFRESULT_EOF                             ((HRESULT)0x8012009E)
#define AAFRESULT_TIFFVERSION                     ((HRESULT)0x8012009F)
#define AAFRESULT_BADTIFFCOUNT                    ((HRESULT)0x801200A0)
#define AAFRESULT_24BITVIDEO                      ((HRESULT)0x801200A1)
#define AAFRESULT_JPEGBASELINE                    ((HRESULT)0x801200A2)
#define AAFRESULT_BADJPEGINFO                     ((HRESULT)0x801200A3)
#define AAFRESULT_BADQTABLE                       ((HRESULT)0x801200A4)
#define AAFRESULT_BADACTABLE                      ((HRESULT)0x801200A5)
#define AAFRESULT_BADDCTABLE                      ((HRESULT)0x801200A6)
#define AAFRESULT_NOFRAMEINDEX                    ((HRESULT)0x801200A7)
#define AAFRESULT_BADFRAMEOFFSET                  ((HRESULT)0x801200A8)
#define AAFRESULT_JPEGPCM                         ((HRESULT)0x801200A9)
#define AAFRESULT_JPEGDISABLED                    ((HRESULT)0x801200AA)
#define AAFRESULT_JPEGPROBLEM                     ((HRESULT)0x801200AB)
#define AAFRESULT_BADEXPORTPIXFORM                ((HRESULT)0x801200AC)
#define AAFRESULT_BADEXPORTLAYOUT                 ((HRESULT)0x801200AD)
#define AAFRESULT_BADRWLINES                      ((HRESULT)0x801200AE)
#define AAFRESULT_BADAIFCDATA                     ((HRESULT)0x801200AF)
#define AAFRESULT_BADWAVEDATA                     ((HRESULT)0x801200B0)
#define AAFRESULT_NOAUDIOCONV                     ((HRESULT)0x801200B1)
#define AAFRESULT_XFER_NOT_BYTES                  ((HRESULT)0x801200B2)
#define AAFRESULT_CODEC_NAME_SIZE                 ((HRESULT)0x801200B3)
#define AAFRESULT_ZERO_SAMPLESIZE                 ((HRESULT)0x801200B4)
#define AAFRESULT_ZERO_PIXELSIZE                  ((HRESULT)0x801200B5)
#define AAFRESULT_BAD_VARIETY                     ((HRESULT)0x801200B6)
#define AAFRESULT_FORMAT_BOUNDS                   ((HRESULT)0x801200B7)
#define AAFRESULT_FORMAT_NOT_FOUND                ((HRESULT)0x801200B8)
#define AAFRESULT_UNKNOWN_CONTAINER               ((HRESULT)0x801200B9)
#define AAFRESULT_NO_MORE_FLAVOURS                ((HRESULT)0x801200BA)

/* OBJECT Error Codes */
        /* NULL object not allowed */
#define AAFRESULT_NULLOBJECT                      ((HRESULT)0x801200C8)
        /* Array index out of range */
#define AAFRESULT_BADINDEX                        ((HRESULT)0x801200C9)
#define AAFRESULT_INVALID_LINKAGE                 ((HRESULT)0x801200CA)
        /* Property code out of range */
#define AAFRESULT_BAD_PROP                        ((HRESULT)0x801200CB)
        /* Type code out of range */
#define AAFRESULT_BAD_TYPE                        ((HRESULT)0x801200CC)
#define AAFRESULT_SWAB                            ((HRESULT)0x801200CD)
#define AAFRESULT_END_OF_DATA                     ((HRESULT)0x801200CE)
        /* Property missing from the file */
#define AAFRESULT_PROP_NOT_PRESENT                ((HRESULT)0x801200CF)
        /* Invalid data definition */
#define AAFRESULT_INVALID_DATADEF                 ((HRESULT)0x801200D0)
#define AAFRESULT_DATADEF_EXIST                   ((HRESULT)0x801200D1)
#define AAFRESULT_TOO_MANY_TYPES                  ((HRESULT)0x801200D2)
#define AAFRESULT_BAD_TYPE_CATEGORY               ((HRESULT)0x801200D5)
        /* Object not found */
#define AAFRESULT_OBJECT_NOT_FOUND                ((HRESULT)0x801200D6)
#define AAFRESULT_IS_ROOT_CLASS                   ((HRESULT)0x801200D7)
        /* Type not found */
#define AAFRESULT_TYPE_NOT_FOUND                  ((HRESULT)0x801200D8)
#define AAFRESULT_PROPERTY_NOT_FOUND              ((HRESULT)0x801200D9)
#define AAFRESULT_CLASS_NOT_FOUND                 ((HRESULT)0x801200DA)
#define AAFRESULT_PROPERTY_DUPLICATE              ((HRESULT)0x801200DB)
#define AAFRESULT_ELEMENT_NOT_PRESENT             ((HRESULT)0x801200DC)
#define AAFRESULT_ELEMENT_NOT_OBJECT              ((HRESULT)0x801200DD)
#define AAFRESULT_PROP_ALREADY_PRESENT            ((HRESULT)0x801200DE)

/* MOB Error Codes */
#define AAFRESULT_NOT_SOURCE_CLIP                 ((HRESULT)0x801200FA)
#define AAFRESULT_FILL_FOUND                      ((HRESULT)0x801200FB)
#define AAFRESULT_BAD_LENGTH                      ((HRESULT)0x801200FC)
#define AAFRESULT_BADRATE                         ((HRESULT)0x801200FD)
#define AAFRESULT_INVALID_ROUNDING                ((HRESULT)0x801200FE)
#define AAFRESULT_TIMECODE_NOT_FOUND              ((HRESULT)0x801200FF)
#define AAFRESULT_NO_TIMECODE                     ((HRESULT)0x80120100)
#define AAFRESULT_INVALID_TIMECODE                ((HRESULT)0x80120101)
#define AAFRESULT_SLOT_NOT_FOUND                  ((HRESULT)0x80120102)
#define AAFRESULT_BAD_SLOTLENGTH                  ((HRESULT)0x80120103)
#define AAFRESULT_MISSING_TRACKID                 ((HRESULT)0x80120104)
#define AAFRESULT_SLOT_EXISTS                     ((HRESULT)0x80120105)
#define AAFRESULT_MOB_NOT_FOUND                   ((HRESULT)0x80120106)
#define AAFRESULT_NO_MORE_MOBS                    ((HRESULT)0x80120107)
#define AAFRESULT_DUPLICATE_MOBID                 ((HRESULT)0x80120108)
#define AAFRESULT_MISSING_MOBID                   ((HRESULT)0x80120109)
#define AAFRESULT_EFFECTDEF_EXIST                 ((HRESULT)0x8012010A)
#define AAFRESULT_INVALID_EFFECTDEF               ((HRESULT)0x8012010B)
#define AAFRESULT_INVALID_EFFECT                  ((HRESULT)0x8012010C)
#define AAFRESULT_INVALID_EFFECTARG               ((HRESULT)0x8012010D)
#define AAFRESULT_INVALID_CVAL                    ((HRESULT)0x8012010E)
#define AAFRESULT_RENDER_NOT_FOUND                ((HRESULT)0x8012010F)
#define AAFRESULT_BAD_ITHDL                       ((HRESULT)0x80120110)
        /* No More Objects */
#define AAFRESULT_NO_MORE_OBJECTS                 ((HRESULT)0x80120111)
#define AAFRESULT_ITER_WRONG_TYPE                 ((HRESULT)0x80120112)
#define AAFRESULT_INVALID_SEARCH_CRIT             ((HRESULT)0x80120113)
#define AAFRESULT_INTERNAL_ITERATOR               ((HRESULT)0x80120114)
#define AAFRESULT_NULL_MATCHFUNC                  ((HRESULT)0x80120115)
#define AAFRESULT_NULL_CALLBACKFUNC               ((HRESULT)0x80120116)
        /* Mob traversal failed */
#define AAFRESULT_TRAVERSAL_NOT_POSS              ((HRESULT)0x80120117)
#define AAFRESULT_INVALID_TRAN_EFFECT             ((HRESULT)0x80120118)
#define AAFRESULT_ADJACENT_TRAN                   ((HRESULT)0x80120119)
#define AAFRESULT_LEADING_TRAN                    ((HRESULT)0x8012011A)
#define AAFRESULT_INSUFF_TRAN_MATERIAL            ((HRESULT)0x8012011B)
#define AAFRESULT_PULLDOWN_DIRECTION              ((HRESULT)0x8012011C)
#define AAFRESULT_PULLDOWN_FUNC                   ((HRESULT)0x8012011D)
#define AAFRESULT_PULLDOWN_KIND                   ((HRESULT)0x8012011E)
#define AAFRESULT_BAD_SRCH_ITER                   ((HRESULT)0x8012011F)
#define AAFRESULT_NOT_COMPOSITION                 ((HRESULT)0x80120120)
#define AAFRESULT_NOT_A_TRACK                     ((HRESULT)0x80120121)
#define AAFRESULT_PARSE_EFFECT_AMBIGUOUS          ((HRESULT)0x80120122)
#define AAFRESULT_NO_ESSENCE_DESC                 ((HRESULT)0x80120123)
#define AAFRESULT_TAPE_DESC_ONLY                  ((HRESULT)0x80120124)
#define AAFRESULT_FILM_DESC_ONLY                  ((HRESULT)0x80120125)
#define AAFRESULT_UNKNOWN_PARAMETER_CLASS         ((HRESULT)0x80120126)
#define AAFRESULT_PARAMETER_NOT_FOUND             ((HRESULT)0x80120127)
#define AAFRESULT_SEGMENT_NOT_FOUND               ((HRESULT)0x80120128)
#define AAFRESULT_ESSENCE_NOT_FOUND               ((HRESULT)0x80120129)
#define AAFRESULT_EVENT_SEMANTICS                 ((HRESULT)0x8012012A)

/* SIMPLE COMPOSITION Error Codes */
#define AAFRESULT_BAD_STRACKHDL                   ((HRESULT)0x8012012C)
#define AAFRESULT_STRACK_APPEND_ILLEGAL           ((HRESULT)0x8012012D)

/* Object Management Related Error Codes */
#define AAFRESULT_OBJECT_ALREADY_IN_FILE          ((HRESULT)0x80120130)
#define AAFRESULT_OBJECT_NOT_IN_FILE              ((HRESULT)0x80120131)
        /* Object already attached */
#define AAFRESULT_OBJECT_ALREADY_ATTACHED         ((HRESULT)0x80120132)
        /* Object not attached */
#define AAFRESULT_OBJECT_NOT_ATTACHED             ((HRESULT)0x80120133)
#define AAFRESULT_OBJECT_ALREADY_PERSISTENT       ((HRESULT)0x80120134)
        /* Object not persistent */
#define AAFRESULT_OBJECT_NOT_PERSISTENT           ((HRESULT)0x80120135)

/* File kind/file encoding Error Codes */
#define AAFRESULT_FILEKIND_NOT_REGISTERED         ((HRESULT)0x80120140)

/* GENERIC Error Codes */
        /* Memory allocation failed, no more heap memory */
#define AAFRESULT_NOMEMORY                        ((HRESULT)0x8012015E)
#define AAFRESULT_OFFSET_SIZE                     ((HRESULT)0x8012015F)
#define AAFRESULT_INTERNAL_NEG64                  ((HRESULT)0x80120160)
#define AAFRESULT_OVERFLOW64                      ((HRESULT)0x80120161)
        /* Not implemented in the current version */
#define AAFRESULT_NOT_IN_CURRENT_VERSION          ((HRESULT)0x80120162)
        /* NULL actual parameter to function call */
#define AAFRESULT_NULL_PARAM                      ((HRESULT)0x80120164)
#define AAFRESULT_ZERO_DIVIDE                     ((HRESULT)0x80120165)
        /* Object already initialized */
#define AAFRESULT_ALREADY_INITIALIZED             ((HRESULT)0x80120166)
        /* Object not initialized */
#define AAFRESULT_NOT_INITIALIZED                 ((HRESULT)0x80120167)
        /* Internal error */
#define AAFRESULT_INTERNAL_ERROR                  ((HRESULT)0x80120168)
#define AAFRESULT_DATA_SIZE                       ((HRESULT)0x80120169)
        /* Illegal value */
#define AAFRESULT_ILLEGAL_VALUE                   ((HRESULT)0x8012016A)
#define AAFRESULT_INVALID_TRANSPARENCY            ((HRESULT)0x8012016B)
        /* Invalid parameter */
#define AAFRESULT_INVALID_PARAM                   ((HRESULT)0x8012016C)
#define AAFRESULT_INVALID_ENUM_VALUE              ((HRESULT)0x8012016D)

/* SEMANTIC CHECKING Error Codes */
#define AAFRESULT_REQUIRED_POSITIVE               ((HRESULT)0x80120190)
#define AAFRESULT_INVALID_TRACKKIND               ((HRESULT)0x80120191)
#define AAFRESULT_INVALID_EDGETYPE                ((HRESULT)0x80120192)
#define AAFRESULT_INVALID_FILMTYPE                ((HRESULT)0x80120193)
#define AAFRESULT_INVALID_MOBTYPE                 ((HRESULT)0x80120194)
#define AAFRESULT_INVALID_TAPECASETYPE            ((HRESULT)0x80120195)
#define AAFRESULT_INVALID_VIDEOSIGNALTYPE         ((HRESULT)0x80120196)
#define AAFRESULT_INVALID_TAPEFORMATTYPE          ((HRESULT)0x80120197)
#define AAFRESULT_INVALID_EDITHINT                ((HRESULT)0x80120198)
#define AAFRESULT_INVALID_INTERPKIND              ((HRESULT)0x80120199)
#define AAFRESULT_INVALID_TRACK_REF               ((HRESULT)0x8012019A)
        /* Invalid object for this operation */
#define AAFRESULT_INVALID_OBJ                     ((HRESULT)0x8012019B)
#define AAFRESULT_BAD_VIRTUAL_CREATE              ((HRESULT)0x8012019C)
#define AAFRESULT_INVALID_CLASS_ID                ((HRESULT)0x8012019D)
#define AAFRESULT_OBJECT_SEMANTIC                 ((HRESULT)0x8012019E)
#define AAFRESULT_DATA_IN_SEMANTIC                ((HRESULT)0x8012019F)
#define AAFRESULT_DATA_OUT_SEMANTIC               ((HRESULT)0x801201A0)
#define AAFRESULT_TYPE_SEMANTIC                   ((HRESULT)0x801201A1)
#define AAFRESULT_INVALID_ATTRIBUTEKIND           ((HRESULT)0x801201A2)
#define AAFRESULT_DATA_MDES_DISAGREE              ((HRESULT)0x801201A3)
#define AAFRESULT_CODEC_SEMANTIC_WARN             ((HRESULT)0x801201A4)
#define AAFRESULT_INVALID_BOOLTYPE                ((HRESULT)0x801201A5)

/* INTERNAL Error Codes */
#define AAFRESULT_TABLE_DUP_KEY                   ((HRESULT)0x801201C2)
#define AAFRESULT_TABLE_MISSING_COMPARE           ((HRESULT)0x801201C3)
        /* Bad table handle */
#define AAFRESULT_TABLE_BAD_HDL                   ((HRESULT)0x801201C4)
#define AAFRESULT_TABLE_BAD_ITER                  ((HRESULT)0x801201C5)
#define AAFRESULT_PROPID_MATCH                    ((HRESULT)0x801201C8)
#define AAFRESULT_INTERNAL_DIVIDE                 ((HRESULT)0x801201C9)
        /* Abstract class */
#define AAFRESULT_ABSTRACT_CLASS                  ((HRESULT)0x801201CA)
#define AAFRESULT_WRONG_SIZE                      ((HRESULT)0x801201CB)
#define AAFRESULT_INCONSISTENCY                   ((HRESULT)0x801201CC)

/* INTERNAL Error Codes - programming errors */
        /* Reference implementation assertion violation (programming error). */
#define AAFRESULT_ASSERTION_VIOLATION             ((HRESULT)0x801201D0)
        /* Reference implementation unexpected C++ exception (programming error) */
#define AAFRESULT_UNEXPECTED_EXCEPTION            ((HRESULT)0x801201D1)
        /* Reference implementation unhandled exception (programming error) */
#define AAFRESULT_UNHANDLED_EXCEPTION             ((HRESULT)0x801201D2)

/* Testing Error Codes */
#define AAFRESULT_TEST_FAILED                     ((HRESULT)0x801201F4)
#define AAFRESULT_TEST_PARTIAL_SUCCESS            ((HRESULT)0x801201F5)

/* Property access error codes */
        /* Bad size */
#define AAFRESULT_BAD_SIZE                        ((HRESULT)0x80120200)
        /* Not registered */
#define AAFRESULT_NOT_REGISTERED                  ((HRESULT)0x80120201)
#define AAFRESULT_NOT_EXTENDABLE                  ((HRESULT)0x80120202)
#define AAFRESULT_ALREADY_UNIQUELY_IDENTIFIED     ((HRESULT)0x80120203)
#define AAFRESULT_DEFAULT_ALREADY_USED            ((HRESULT)0x80120204)

/* Object extension error codes */
#define AAFRESULT_EXTENSION_NOT_FOUND             ((HRESULT)0x80120300)
#define AAFRESULT_EXTENSION_ALREADY_INITIALIZED   ((HRESULT)0x80120301)
#define AAFRESULT_PLUGIN_NOT_REGISTERED           ((HRESULT)0x80120302)
#define AAFRESULT_PLUGIN_ALREADY_REGISTERED       ((HRESULT)0x80120303)
#define AAFRESULT_PLUGIN_CIRCULAR_REFERENCE       ((HRESULT)0x80120304)
#define AAFRESULT_PLUGIN_INVALID_REFERENCE_COUNT  ((HRESULT)0x80120305)

/* DLL/Shared Library runtime error codes */
        /* Either the DLL or a dependent DLL could not be found and loaded */
#define AAFRESULT_DLL_LOAD_FAILED                 ((HRESULT)0x80120400)
        /* Expected symbol not exported from DLL. Need newer DLL */
#define AAFRESULT_DLL_SYMBOL_NOT_FOUND            ((HRESULT)0x80120401)

/* Result code -> text error codes */
        /* The specified result code is not an AAF result code */
#define AAFRESULT_RESULT_NOT_AAF                  ((HRESULT)0x80120500)
        /* The specified AAF result code is not recognized */
#define AAFRESULT_RESULT_NOT_RECOGNIZED           ((HRESULT)0x80120501)

/* For compatibility */
#define AAF_RESULT_USE_MULTI_CREATE               AAFRESULT_USE_MULTI_CREATE
#define AAFRESULT_INCONSISTANCY                   AAFRESULT_INCONSISTENCY
#define AAFRESULT_BADOPEN                         AAFRESULT_OPEN_FAILURE
#define HRESULT_NOT_IMPLEMENTED                   AAFRESULT_NOT_IMPLEMENTED
#define HRESULT_SUCCESS                           AAFRESULT_SUCCESS

#endif // ! __AAFResult_h__
