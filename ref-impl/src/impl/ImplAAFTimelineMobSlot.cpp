

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafBool,
// aafRational_t,
// aafPosition_t,
// aafTrackID_t,




#ifndef __ImplAAFTimelineMobSlot_h__
#include "ImplAAFTimelineMobSlot.h"
#endif

#include <assert.h>
#include <AAFResult.h>
#include "aafCvt.h" 


ImplAAFTimelineMobSlot::ImplAAFTimelineMobSlot ():
	_editRate(	PID_TIMELINEMOBSLOT_EDITRATE,	"EditRate")
{
	_persistentProperties.put( _editRate.address());

}


ImplAAFTimelineMobSlot::~ImplAAFTimelineMobSlot ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetEditRate (aafRational_t *editRate)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	*editRate = _editRate;
	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetOrigin (aafPosition_t *origin)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	ImplAAFSegment * tmpSegment = (ImplAAFSegment*) NULL;

	assert(origin != NULL);

	XPROTECT( )
	  {
		CHECK(GetTrackDesc(0, (aafString_t*) NULL, origin, (unsigned long*) NULL));
	  }

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetTrackID (aafTrackID_t *  /*result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


  // Override from AAFMobSlot
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::IsATrack (/*[out,retval]*/ aafBool *  /*retval*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

/*************************************************************************
 * Private Function: MobSlotGetTrackDesc()
 *
 * 		This function returns the requested information (non-NULL
 *      parameters) identifying a track.  For 1.x, the information is
 *      taken from the TRAK object.  For 2.x, it is taken from the
 *      TRKD object associated with the input mob slot object.
 *
 * Argument Notes:
 *      NULL should be passed for any argument that is not requested.
 *      The name buffer must be a preallocated buffer of size nameSize.
 *      If the name is longer than the buffer provided, it will truncate
 *      the string to fit into the buffer.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT  ImplAAFTimelineMobSlot::
			GetTrackDesc(aafInt32 nameSize,		// IN - Size of name buffer
						aafString_t *name,		// IN/OUT preallocated buffer to return name
						aafPosition_t *origin,	// OUT - Origin property value
						aafTrackID_t *trackID)	// OUT - ttrack id property value

{
//    ImplAAFObject *tmp1xTrackCpnt = NULL;
	aafInt16 tmp1xTrackID = 0;
	aafInt16 tmp1xTrackType = 0;
	aafInt32 tmp1xStartPosition = 0;
	AAFRESULT aafError = AAFRESULT_SUCCESS;

//	XPROTECT( )
	{
		/* Initialize values if not found (mob slot doesn't have descriptor */
		if (trackID)
		  *trackID = 0;
		if (origin)
		  CvtInt32toPosition(0, (*origin));
		  
		{
			if (name)
			{
				GetName( name );
			}
			if (origin)
			{
				GetOrigin(origin);
			 }
			if (trackID)
			{	
				GetTrackID(trackID);
			}
		} /* OmfRev2x */
	} /* XPROTECT */

//	XEXCEPT
	{
//		return(XCODE());
	}
//	XEND;
	return aafError;
}



