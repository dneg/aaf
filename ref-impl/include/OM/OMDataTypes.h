#ifndef OMDATATYPES_H
#define OMDATATYPES_H

// Figure out the host architecture
//
#if defined(_M_IX86)
#define HOST_IX86
#elif defined(_M_PPC) || defined(powerc)
#define HOST_PPC
#elif defined(__sgi)
#define HOST_SGI
#else
#error "Unknown host machine architecture"
#endif

#if defined(HOST_IX86)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef _int64                 OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned _int64        OMUInt64;

#elif defined(HOST_PPC)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#elif defined(HOST_SGI)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;
#endif

typedef OMUInt8                OMByte;

typedef struct _OMClassId
{
    OMUInt32 Data1;
    OMUInt16 Data2;
    OMUInt16 Data3;
    OMUInt8  Data4[8];
} OMClassId;

const OMClassId nullOMClassId = {0};

typedef OMUInt32 OMPropertyId;

typedef OMUInt16 OMByteOrder;

const OMByteOrder littleEndian      = 0x4949; // 'II' for Intel
const OMByteOrder bigEndian         = 0x4d4d; // 'MM' for Motorola

#endif
