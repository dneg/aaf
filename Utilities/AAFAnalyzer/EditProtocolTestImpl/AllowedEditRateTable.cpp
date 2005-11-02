//=---------------------------------------------------------------------=
//
// $Id$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

//Edit Protocol Test files
#include "AllowedEditRateTable.h"

//Standard C++ files
#include <cmath>

namespace {

using namespace aafanalyzer;

} // end of namespace

namespace aafanalyzer {

using namespace boost;

AllowedEditRateTable::AllowedEditRateTable()
{
    this->InitializeTable();
}

AllowedEditRateTable::~AllowedEditRateTable()
{
}

void AllowedEditRateTable::InitializeTable()
{
    //Create entries for the 59.94i Pull-Down Rational column.
    shared_ptr<NumeratorMap> sp5994iPullDown( new NumeratorMap );
    (*sp5994iPullDown)[24000]     =     24;
    (*sp5994iPullDown)[25000]     =     25;
    (*sp5994iPullDown)[30000]     =     30;
    (*sp5994iPullDown)[50000]     =     50;
    (*sp5994iPullDown)[60000]     =     60;
    (*sp5994iPullDown)[44100000]  =  44100;
    (*sp5994iPullDown)[48000000]  =  48000;
    (*sp5994iPullDown)[88200000]  =  88200;
    (*sp5994iPullDown)[96000000]  =  96000;
    (*sp5994iPullDown)[176400000] = 176400;
    (*sp5994iPullDown)[192000000] = 192000;
    
    //Store the column in the denominator map.
    _editRateTable[1001] = sp5994iPullDown;
    
    //Create entries for the 59.94i Pull-Up Rational column.
    shared_ptr<NumeratorMap> sp5994iPullUp( new NumeratorMap );
    (*sp5994iPullUp)[24024]     =     24;
    (*sp5994iPullUp)[25025]     =     25;
    (*sp5994iPullUp)[30030]     =     30;
    (*sp5994iPullUp)[50050]     =     50;
    (*sp5994iPullUp)[60060]     =     60;
    (*sp5994iPullUp)[44144100]  =  44100;
    (*sp5994iPullUp)[48048000]  =  48000;
    (*sp5994iPullUp)[88288200]  =  88200;
    (*sp5994iPullUp)[96096000]  =  96000;
    (*sp5994iPullUp)[176576400] = 176400;
    (*sp5994iPullUp)[192192000] = 192000;
    
    //Store the column in the denominator map.
    _editRateTable[1000] = sp5994iPullUp;
    
    //Create entries for the 50i Pull-Down Rational column.
    shared_ptr<NumeratorMap> sp50iPullDown( new NumeratorMap );
    (*sp50iPullDown)[576]     =     24;
    (*sp50iPullDown)[600]     =     25;
    (*sp50iPullDown)[720]     =     30;
    (*sp50iPullDown)[1200]    =     50;
    (*sp50iPullDown)[1440]    =     60;
    (*sp50iPullDown)[1058400] =  44100;
    (*sp50iPullDown)[1152000] =  48000;
    (*sp50iPullDown)[2116800] =  88200;
    (*sp50iPullDown)[2304000] =  96000;
    (*sp50iPullDown)[4233600] = 176400;
    (*sp50iPullDown)[4608000] = 192000;
    
    //Store the column in the denominator map.
    _editRateTable[25] = sp50iPullDown;
    
    //Create entries for the 50i Pull-Up Rational column.
    shared_ptr<NumeratorMap> sp50iPullUp( new NumeratorMap );
    (*sp50iPullUp)[600]     =     24;
    (*sp50iPullUp)[625]     =     25;
    (*sp50iPullUp)[750]     =     30;
    (*sp50iPullUp)[1250]    =     50;
    (*sp50iPullUp)[1500]    =     60;
    (*sp50iPullUp)[1102500] =  44100;
    (*sp50iPullUp)[1200000] =  48000;
    (*sp50iPullUp)[2205000] =  88200;
    (*sp50iPullUp)[2400000] =  96000;
    (*sp50iPullUp)[4410000] = 176400;
    (*sp50iPullUp)[4800000] = 192000;
    
    //Store the column in the denominator map.
    _editRateTable[24] = sp50iPullUp;
    
    //Add rounded columns - call Round() here to ensure the same floating point
    //representation as when numbers are divided.
    _roundedEditRates[Round(     23.976 )] =     24;
    _roundedEditRates[Round(     24.975 )] =     25;
    _roundedEditRates[Round(     29.970 )] =     30;
    _roundedEditRates[Round(     49.950 )] =     50;
    _roundedEditRates[Round(     59.940 )] =     60;
    _roundedEditRates[Round(  44056     )] =  44100;
    _roundedEditRates[Round(  47952     )] =  48000;
    _roundedEditRates[Round(  88112     )] =  88200;
    _roundedEditRates[Round(  95904     )] =  96000;
    _roundedEditRates[Round( 176224     )] = 176400;
    _roundedEditRates[Round( 191808     )] = 192000;

    _roundedEditRates[Round(     24.024 )] =     24;
    _roundedEditRates[Round(     25.025 )] =     25;
    _roundedEditRates[Round(     30.030 )] =     30;
    _roundedEditRates[Round(     50.050 )] =     50;
    _roundedEditRates[Round(     60.060 )] =     60;
    _roundedEditRates[Round(  44144     )] =  44100;
    _roundedEditRates[Round(  48048     )] =  48000;
    _roundedEditRates[Round(  88288     )] =  88200;
    _roundedEditRates[Round(  96096     )] =  96000;
    _roundedEditRates[Round( 176576     )] = 176400;
    _roundedEditRates[Round( 192192     )] = 192000;

    _roundedEditRates[Round(     23.040 )] =     24;
    _roundedEditRates[Round(     24.000 )] =     25;
    _roundedEditRates[Round(     28.800 )] =     30;
    _roundedEditRates[Round(     48.000 )] =     50;
    _roundedEditRates[Round(     57.600 )] =     60;
    _roundedEditRates[Round(  42336     )] =  44100;
    _roundedEditRates[Round(  46080     )] =  48000;
    _roundedEditRates[Round(  84672     )] =  88200;
    _roundedEditRates[Round(  92160     )] =  96000;
    _roundedEditRates[Round( 169344     )] = 176400;
    _roundedEditRates[Round( 184320     )] = 192000;

    _roundedEditRates[Round(     25.000 )] =     24;
    _roundedEditRates[Round(     26.042 )] =     25;
    _roundedEditRates[Round(     31.250 )] =     30;
    _roundedEditRates[Round(     52.083 )] =     50;
    _roundedEditRates[Round(     62.500 )] =     60;
    _roundedEditRates[Round(  45938     )] =  44100;
    _roundedEditRates[Round(  50000     )] =  48000;
    _roundedEditRates[Round(  91875     )] =  88200;
    _roundedEditRates[Round( 100000     )] =  96000;
    _roundedEditRates[Round( 183750     )] = 176400;
    _roundedEditRates[Round( 200000     )] = 192000;

    //Set up the video rate table.
    _nominalVideoRates.insert( 24 );
    _nominalVideoRates.insert( 25 );
    _nominalVideoRates.insert( 30 );
    _nominalVideoRates.insert( 50 );
    _nominalVideoRates.insert( 60 );
    
}

bool AllowedEditRateTable::IsInTable( aafRational_t editRate, bool isVideo )
{
    aafInt32 numerator = editRate.numerator;
    aafInt32 denominator = editRate.denominator;

    //If the denominator is zero, the edit rate is not in the table.  Return
    //false now to prevent division by zero errors.
    if ( denominator == 0 )
    {
        return false;
    }
    
    //Determine if this rational may be stored in the table:
    DenominatorMap::const_iterator dm_iter;
    for ( dm_iter = _editRateTable.begin(); dm_iter != _editRateTable.end(); dm_iter++ )
    {
        /* 
         * Let d = denominator/dm_iter->first
         * 
         * If the numerator/d is int dm_iter->second then the edit rate is
         * in the map
         * 
         */
        double d = (double)denominator / (double)(dm_iter->first);
         
        double adjustedNumerator = (double)numerator / d;
        shared_ptr<NumeratorMap> spNumMap( dm_iter->second );

        //If the adjusted numerator is not an integer, it will not be in the
        //map

        if ( adjustedNumerator == ceil(adjustedNumerator) )
        {
            NumeratorMap::const_iterator nm_iter = spNumMap->find( (aafInt32)adjustedNumerator );
            if ( nm_iter != spNumMap->end() )
            {
                //If this is a video track, make sure the found edit rate
                //is a video edit rate.
                if ( !isVideo || 
                     _nominalVideoRates.find( nm_iter->second ) != _nominalVideoRates.end()
                   )
                {
                    return true;
                }
            }
        }
    }
    
    //The edit rate was not found in rational form in rational form in the
    //table, so convert the edit rate to a rounded number and search the rounded
    //columns.
    
    double roundedEditRate = (double)numerator/(double)denominator;
    roundedEditRate = Round( roundedEditRate );

    RoundedMap::const_iterator rm_iter = _roundedEditRates.find( roundedEditRate );
    if ( rm_iter != _roundedEditRates.end() )
    {
        //If this is a video track, make sure the found edit rate
        //is a video edit rate.
        if ( !isVideo || 
             _nominalVideoRates.find( rm_iter->second ) != _nominalVideoRates.end()
           )
        {
            return true;
        }
    }
    
    return false;
    
}

//Round positive numbers <= 99999 to 5 digits, Round numbers >= 99999 to integers.
double AllowedEditRateTable::Round( double num )
{
    double digits = ceil( log10( num ) );
    
    double multiplier;
    
    if ( digits < 5 )
    {
        multiplier = pow( 10.0, 5.0 - digits );
    }
    else
    {
        multiplier = 1;
    }
    
    double allDigitsNum = num * multiplier;
    double extraDigitNum = allDigitsNum * 10;
    double roundAllDigitsNum = floor( allDigitsNum );
    double extraRoundAllDigitsNum = roundAllDigitsNum * 10;
    double roundExtraDigitNum = floor( extraDigitNum );
    if ( roundExtraDigitNum - extraRoundAllDigitsNum >= 5 )
    {
        roundAllDigitsNum++;
    }
    
    double roundedNum = roundAllDigitsNum/multiplier;
    
    return roundedNum;
    

}
    
} // end of namespace aafanalyzer
