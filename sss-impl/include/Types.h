/************************************************************************
* Types.h
*
* Contains typedefs for commonly-used types
* 
* If there are compiler errors or warnings when compiling the structured
* storage library, most likely the problem is here.  Just change the 
* definitions appropriately for your environment.
*
* (c) Schema Software Inc., 2001-2003
* 
*************************************************************************

$Revision$
$Date$
*/
#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef UINT8
#undef UINT8
#endif /* UINT8 */

#ifdef UINT4
#undef UINT4
#endif /* UINT4 */

#ifdef UINT2
#undef UINT2
#endif /* UINT2 */

#ifdef BYTE
#undef BYTE
#endif /* BYTE */

#ifdef SINT8
#undef SINT8
#endif /* SINT8 */

#ifdef SINT4
#undef SINT4
#endif /* SINT4 */

#ifdef SINT2
#undef SINT2
#endif /* SINT2 */

#ifdef CHAR
#undef CHAR
#endif /* CHAR */

#ifdef CHAR2
#undef CHAR2
#endif /* CHAR2 */

#ifdef FLT8
#undef FLT8
#endif /* FLT8 */

#ifdef FLT4
#undef FLT4
#endif /* FLT4 */

#ifdef IA_64
typedef unsigned long UINT8;
typedef unsigned int  UINT4;
typedef unsigned short UINT2;
typedef unsigned char BYTE;


#else /* IA_64 */

#ifdef _MSC_VER /* If this is Visual C++ */
typedef unsigned __int64 UINT8;
#else /* _MSC_VER */
typedef unsigned long long UINT8;
#endif /* _MSC_VER */
typedef unsigned long UINT4;
typedef unsigned short UINT2;
typedef unsigned char BYTE;

#endif /* IA_64 */


#ifdef IA_64
typedef long SINT8;
typedef int  SINT4;
typedef short SINT2;

#else /* IA_64 */

#ifdef _MSC_VER /* If this is Visual C++ */
typedef signed __int64 SINT8;
#else /* _MSC_VER */
typedef signed long long SINT8;
#endif /* _MSC_VER */
typedef signed long SINT4;
typedef signed short SINT2;


#endif /* IA_64 */

typedef signed char CHAR;
typedef UINT2 CHAR2;


typedef double FLT8;
typedef float FLT4;



#endif /* __TYPES_H__ */

