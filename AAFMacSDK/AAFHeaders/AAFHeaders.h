/*
 *	AAFHeaders.h
 *
 *	Processor independant interface to the AAFHeaders<xxx> files ...
 */

#ifdef __cplusplus

#if __POWERPC__
	#include <AAFHeadersPPC++>
#elif __CFM68K__
	#include <AAFHeadersCFM68K++>
#else
	#include <AAFHeaders68K++>
#endif

#else

#if __POWERPC__
	#include <AAFHeadersPPC>
#elif __CFM68K__
	#include <AAFHeadersCFM68K>
#else
	#include <AAFHeaders68K>
#endif

#endif
