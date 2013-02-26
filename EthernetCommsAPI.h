///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// SentelUtilities.h
//-----------------------------------------------------------------------------
// MODULE NAME:			SentelUtilities
// FILE NAME:			SentelUtilitiesAPI.h
//
// PURPOSE:				Define exports/imports for SentelUtilities DLL
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer		 Date		Modification
//  0		Josh Armentrout         2012            Creation
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////

#ifndef _ETHERNETCOMMSAPI_H_
#define _ETHERNETCOMMSAPI_H_

#ifdef _WINDOWS_

#ifdef ETHERNETCOMMS_EXPORTS
	#define ETHERNETCOMMS_API __declspec(dllexport)
#else
	#define ETHERNETCOMMS_API __declspec(dllimport)
#endif

#else 
        #define ETHERNETCOMMS_API 
#endif

#endif //_ETHERNETCOMMSAPI_H_
