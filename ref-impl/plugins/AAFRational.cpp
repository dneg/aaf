#include "AAFRational.h"

AAFRational::AAFRational(AAFRational& s)
{
	num = s.num;
	denom = s.denom;
}

AAFRational& AAFRational::operator=(AAFRational& s)
{
	if(this != &s)
	{
		num = s.num;
		denom = s.denom;
	}
	return(*this);
}

AAFRational AAFRational::operator+(AAFRational& s)
{
	AAFRational	result;
	result.num = (num * s.denom) + (s.num * denom);
	result.denom = s.denom * denom;
	return result;
}

AAFRational AAFRational::operator-(AAFRational& s)
{
	AAFRational	result;
	result.num = (num * s.denom) - (s.num * denom);
	result.denom = s.denom * denom;
	return result;
}

AAFRational AAFRational::operator*(AAFRational& s)
{
	AAFRational	result;
	result.num = num * s.num;
	result.denom = denom * s.denom;
	return(result);
}

AAFRational AAFRational::operator/(AAFRational& s)
{
	AAFRational	result;
	result.num = num * s.denom;
	result.denom = denom * s.num;
	return(result);
}

AAFRational::operator aafRational_t()
{ 
	aafRational_t result;
	result.numerator = num;
	result.denominator = denom;
	return(result);
}