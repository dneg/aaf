/*
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aaf2dot.
 *
 *  aaf2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  aaf2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with aaf2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#pragma warning (disable:4786)

#include <iostream>
#include <assert.h>
using namespace std;

#include <AxMetaDef.h>
#include <AxUtil.h>
#include <AAFTypeDefUIDs.h>

#include <Utilities.h>


//-----------------------------------------------------------------------------
aafUInt8
GetUInt8( AxPropertyValue axPropertyValue )
{
   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefInt axTypeDefInt(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefInt > (
	 axTypeDef ) );

   aafUInt8 i;
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );
   axTypeDefInt.GetInteger( propValue, &i );

   return i;
}


//-----------------------------------------------------------------------------
aafUInt8*
GetUInt8Array( AxPropertyValue axPropertyValue, aafUInt32 &size )
{
   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefFixedArray axTypeDefFixedArray(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefFixedArray > (
	 axTypeDef ) );
   size = axTypeDefFixedArray.GetCount();
   aafUInt8 *array = new aafUInt8[ size ];
   for ( aafUInt32 i = 0; i<size; i++ )
   {
      IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );
      AxPropertyValue m( axTypeDefFixedArray.GetElementValue( propValue, i ) );
      array[ i ] = GetUInt8( m );
   }
   return array;
}


//-----------------------------------------------------------------------------
aafUInt16
GetUInt16( AxPropertyValue axPropertyValue )
{
   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefInt axTypeDefInt(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefInt > (
	 axTypeDef ) );

   aafUInt16 i;
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );
   axTypeDefInt.GetInteger( propValue, &i );

   return i;
}


//-----------------------------------------------------------------------------
aafUInt32
GetUInt32( AxPropertyValue axPropertyValue )
{
   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefInt axTypeDefInt(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefInt > (
	 axTypeDef ) );

   aafUInt32 i;
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );
   axTypeDefInt.GetInteger( propValue, &i );

   return i;
}


//-----------------------------------------------------------------------------
aafInt32
GetInt32( AxPropertyValue axPropertyValue )
{
   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefInt axTypeDefInt(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefInt > (
	 axTypeDef ) );

   aafInt32 i;
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );
   axTypeDefInt.GetInteger( propValue, &i );

   return i;
}


//-----------------------------------------------------------------------------
aafInt64
GetEnumValue( AxPropertyValue axPropertyValue )
{
   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefEnum axTypeDefEnum(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefEnum > (
	 axTypeDef ) );

   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );
   aafInt64 i = axTypeDefEnum.GetIntegerValue( propValue );

   return i;
}


//-----------------------------------------------------------------------------
/*
  typedef struct  _aafUID_t
  {
  aafUInt32 Data1;
  aafUInt16 Data2;
  aafUInt16 Data3;
  aafUInt8 Data4[ 8 ];
  }	aafUID_t;

*/
_aafUID_t
GetUID( AxPropertyValue axPropertyValue )
{
   _aafUID_t retUID;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue data1 = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue data2 = axTypeDefRecord.GetValue( propValue, 1 );
   AxPropertyValue data3 = axTypeDefRecord.GetValue( propValue, 2 );
   AxPropertyValue data4 = axTypeDefRecord.GetValue( propValue, 3 );

   retUID.Data1 = GetUInt32( data1 );
   retUID.Data2 = GetUInt16( data2 );
   retUID.Data3 = GetUInt16( data3 );

   aafUInt32 size;
   aafUInt8* data4UInt8Array = GetUInt8Array( data4 , size );
   for ( aafUInt32 i=0; i<size; i++ )
   {
      retUID.Data4[ i ] = data4UInt8Array[ i ];
   }

   delete [] data4UInt8Array;

   return retUID;
}



//-----------------------------------------------------------------------------
/*
  typedef struct  _aafMobID_t
  {
  aafUInt8 SMPTELabel[ 12 ];
  aafUInt8 length;
  aafUInt8 instanceHigh;
  aafUInt8 instanceMid;
  aafUInt8 instanceLow;
  struct _aafUID_t material;
  }	aafMobID_t;
*/
_aafMobID_t 
GetMobID( AxPropertyValue axPropertyValue )
{
   _aafMobID_t retMobID;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue SMPTELabel = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue length = axTypeDefRecord.GetValue( propValue, 1 );
   AxPropertyValue instanceHigh = axTypeDefRecord.GetValue( propValue, 2 );
   AxPropertyValue instanceMid = axTypeDefRecord.GetValue( propValue, 3 );
   AxPropertyValue instanceLow = axTypeDefRecord.GetValue( propValue, 4 );
   AxPropertyValue material = axTypeDefRecord.GetValue( propValue, 5 );

   aafUInt32 size;
   aafUInt8* labelUInt8Array = GetUInt8Array( SMPTELabel , size );
   for ( aafUInt32 i=0; i<size; i++ )
   {
      retMobID.SMPTELabel[ i ] = labelUInt8Array[ i ];
   }
   delete [] labelUInt8Array;

   retMobID.length = GetUInt8( length );
   retMobID.instanceHigh = GetUInt8( instanceHigh );
   retMobID.instanceMid = GetUInt8( instanceMid );
   retMobID.instanceLow = GetUInt8( instanceLow );
   retMobID.material = GetUID( material );

   return retMobID;
}


//-----------------------------------------------------------------------------
/*
  typedef struct  _aafDateStruct_t
  {
  aafInt16 year;
  aafUInt8 month;
  aafUInt8 day;
  }	aafDateStruct_t;

*/
_aafDateStruct_t 
GetDate( AxPropertyValue axPropertyValue )
{
   _aafDateStruct_t retDate;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue year = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue month = axTypeDefRecord.GetValue( propValue, 1 );
   AxPropertyValue day = axTypeDefRecord.GetValue( propValue, 2 );

   retDate.year = GetUInt16( year );
   retDate.month = GetUInt8( month );
   retDate.day = GetUInt8( day );

   return retDate;
}



//-----------------------------------------------------------------------------
/*
  typedef struct  _aafTimeStruct_t
  {
  aafUInt8 hour;
  aafUInt8 minute;
  aafUInt8 second;
  aafUInt8 fraction;
  }	aafTimeStruct_t;

*/
_aafTimeStruct_t 
GetTime( AxPropertyValue axPropertyValue )
{
   _aafTimeStruct_t retTime;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue hour = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue minute = axTypeDefRecord.GetValue( propValue, 1 );
   AxPropertyValue second = axTypeDefRecord.GetValue( propValue, 2 );
   AxPropertyValue fraction = axTypeDefRecord.GetValue( propValue, 3 );

   retTime.hour = GetUInt16( hour );
   retTime.minute = GetUInt8( minute );
   retTime.second = GetUInt8( second );
   retTime.fraction = GetUInt8( fraction );

   return retTime;
}


//-----------------------------------------------------------------------------
/*
  typedef struct  _aafTimeStamp_t
  {
  aafDateStruct_t date;
  aafTimeStruct_t time;
  }	aafTimeStamp_t;
*/
_aafTimeStamp_t 
GetTimeStamp( AxPropertyValue axPropertyValue )
{
   _aafTimeStamp_t retTimeStamp;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue date = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue time = axTypeDefRecord.GetValue( propValue, 1 );

   retTimeStamp.date = GetDate( date );
   retTimeStamp.time = GetTime( time );

   return retTimeStamp;
}


/*
  typedef struct  _aafVersionType_t
  {
  aafInt8 major;
  aafInt8 minor;
  }	aafVersionType_t;

*/
_aafVersionType_t 
GetVersion( AxPropertyValue axPropertyValue )
{
   _aafVersionType_t retVersion;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue major = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue minor = axTypeDefRecord.GetValue( propValue, 1 );

   retVersion.major = GetUInt8( major );
   retVersion.minor = GetUInt8( minor );

   return retVersion;
}


/*
  typedef struct  _aafProductVersion_t
  {
  aafUInt16 major;
  aafUInt16 minor;
  aafUInt16 tertiary;
  aafUInt16 patchLevel;
  aafProductReleaseType_t type;
  }	aafProductVersion_t;
*/
_aafProductVersion_t 
GetProductVersion( AxPropertyValue axPropertyValue )
{
   _aafProductVersion_t retProductVersion;

   AxTypeDef axTypeDef( axPropertyValue.GetType() );
   AxTypeDefRecord axTypeDefRecord(
      AxQueryInterface< IAAFTypeDef,IAAFTypeDefRecord > (
	 axTypeDef ) );
   IAAFPropertyValueSP propValue( axPropertyValue.GetValue() );

   AxPropertyValue major = axTypeDefRecord.GetValue( propValue, 0 );
   AxPropertyValue minor = axTypeDefRecord.GetValue( propValue, 1 );
   AxPropertyValue tertiary = axTypeDefRecord.GetValue( propValue, 2 );
   AxPropertyValue patchLevel = axTypeDefRecord.GetValue( propValue, 3 );
   AxPropertyValue type = axTypeDefRecord.GetValue( propValue, 4 );

   retProductVersion.major = GetUInt16( major );
   retProductVersion.minor = GetUInt16( minor );
   retProductVersion.tertiary = GetUInt16( tertiary );
   retProductVersion.patchLevel = GetUInt16( patchLevel );
   retProductVersion.type = GetEnumValue( type );

   return retProductVersion;
}


//-----------------------------------------------------------------------------
string 
MobIDToString( aafMobID_t &mobID )
{
   return AxStringUtil::wctomb( AxStringUtil::mobid2Str( mobID ) );
}


//-----------------------------------------------------------------------------
string 
UIDToString( aafUID_t &uid )
{
   return AxStringUtil::wctomb( AxStringUtil::uid2Str( uid ) );
}


//-----------------------------------------------------------------------------
string
DateToString( _aafDateStruct_t &date )
{
   char buf[11];

   sprintf( buf, "%02d-%02d-%04d", date.day, date.month, date.year );

   return buf;
}


//-----------------------------------------------------------------------------
string
TimeToString( _aafTimeStruct_t &time )
{
   char buf[12];

   sprintf( buf, "%02d:%02d:%02d.%02d", time.hour, time.minute, time.second, time.fraction );

   return buf;
}


//-----------------------------------------------------------------------------
string
TimeStampToString( _aafTimeStamp_t &timeStamp )
{
   char buf[22];

   strcpy( buf, DateToString( timeStamp.date ).c_str() );
   strcat( buf, " " );
   strcat( buf, TimeToString( timeStamp.time ).c_str() );

   return buf;
}



//-----------------------------------------------------------------------------
std::string VersionToString( _aafVersionType_t version )
{
   char buf[8];

   sprintf( buf, "%d.%d", version.major, version.minor );

   return buf;
}


//-----------------------------------------------------------------------------
std::string ProductVersionToString( _aafProductVersion_t productVersion )
{
   char buf[48];

   string typeStr;
   switch ( productVersion.type )
   {
      case kAAFVersionReleased:
	 typeStr = "released";
	 break;
      case kAAFVersionDebug:
	 typeStr = "debug";
	 break;
      case kAAFVersionPatched:
	 typeStr = "patched";
	 break;
      case kAAFVersionBeta:
	 typeStr = "beta";
	 break;
      case kAAFVersionPrivateBuild:
	 typeStr = "private build";
	 break;
      default:
	 typeStr = "unknown";
   }

   sprintf( buf, "%d.%d.%d.%d %s", productVersion.major, productVersion.minor, productVersion.tertiary,
	    productVersion.patchLevel, typeStr.c_str() );

   return buf;
}


//-----------------------------------------------------------------------------
string
AxStringToString( AxString axString )
{
   string name;
	
   AxString::iterator iter;
   for ( iter=axString.begin(); iter!=axString.end(); iter++ )
   {
      name += *iter;
   }

   return name;
}


//-----------------------------------------------------------------------------
string 
ProcessStringForQuoting( string s )
{
   string ret = s;
   int index = 0;
   while ( index < ret.size() )
   {
      // escape newlines
      if ( ret[ index ] == '\n' )
      {
	 ret.erase( index, 1 );
	 ret.insert( index, "\\n" );
      }
      index++;
   }

   return ret;
}


//-----------------------------------------------------------------------------
string 
LimitAttributeSize( string attribute, int maxLength, int maxWidth )
{
   string retAttribute = attribute;
   if ( maxLength == 0 )
   {
      return "";
   }
   else if ( attribute.size() > maxLength )
   {
      retAttribute.resize( maxLength - 1 );
      retAttribute.resize( maxLength, '~' );
   }

   // spread value over lines.
   int newLineIndex = maxWidth;
   while ( newLineIndex < retAttribute.size() )
   {
      retAttribute.insert( newLineIndex, "\\n" );
      newLineIndex += maxWidth + 2;
   }

   return retAttribute;
}


