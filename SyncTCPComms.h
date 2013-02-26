///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BoomerangPropertiesDialog.h
//-----------------------------------------------------------------------------
// MODULE NAME:			EthernetCommsModule
// FILE NAME:			SyncTCPComms.h
//
// PURPOSE:				Implements the Sync TCP Comms class. 
//						
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer			Date			Modification
//	0		Rob McGuire			01/03/2013		Creation
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CommsUtilities.h"
#include "EthernetCommsAPI.h"
#include "SyncTCPCommsSendRecv.h"

class ETHERNETCOMMS_API CSyncTCPComms : public CSyncTCPCommsSendRecv
{
public:
	//The only sync constructor
	CSyncTCPComms(const std::string &p_strIP, const unsigned short p_usPort);
	~CSyncTCPComms();

	//Will cancel all IO on socket
	void CancelIO(boost::system::error_code &ec);
	//Checks if the socket is open
	bool IsOpen() const;
};