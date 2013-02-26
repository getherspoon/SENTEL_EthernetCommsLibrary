///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BoomerangPropertiesDialog.h
//-----------------------------------------------------------------------------
// MODULE NAME:			EthernetCommsModule
// FILE NAME:			TCPCommsImpl.h
//
// PURPOSE:				Wrapper of the TCPComms class for both Sync/Async implementations.
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer			Date			Modification
//	0		Rob McGuire			01/03/2013		Creation
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "EthernetCommsAPI.h"
#include "CommsUtilities.h"
#include "TCPComms.h"

class ETHERNETCOMMS_API CTCPCommsImpl
{
public:


protected:
	//Max message size
	static const std::size_t	m_s_nMAX_MESSAGE_SIZE = 1024 ;
	
	//protected methods, this class should not be instantiated directly.
	CTCPCommsImpl(const CommsUtilities::eCommsSystemType p_eCommsSystemType, const std::string &p_strIP, const unsigned short p_usPort);
	virtual ~CTCPCommsImpl();

	//Synchronous Init [All callbacks return to this class and are handled without requiring the synchronous class to do anything]
	void InitComms();
	//Asyncchronous Init [Async class derives from IClientConnectionHandler so callbacks will go directly to that class]
	void InitComms(IClientConnectionHandler* p_pThis);
	
	//Used to clear the m_pComms object.
	void TerminateComms();
	
	//Used for Synchronous Read [This is needed to implement synchronous methods using asynchronous methods]
	void ReadAsync(char* p_str, const int p_iMaxRecvSize, Callback_ConstError_RefSizeT handler);
	//Used for initiating a new async read
	void ReadAsync();
	
	//Used for Synchronous Write Implementation [As with the SyncRead, we are using asynchronous methods to implement SyncWrite]
	void WriteAsync(const std::string &p_str, Callback_ConstError_RefSizeT handler);
	//Used for Async Write
	void WriteAsync(const std::string &p_strMessage);
	
	//Used for determining if we've got a Comms object
	bool HasComms() const;
	//Will cancel all IO on socket
	void CancelIO(boost::system::error_code &ec);
	//Checks if the socket is open
	bool IsOpen() const;
	//Checks if the socket is connected
	bool IsConnected() const;
	
	//Return the IO service, this is required for the Synchronous Ops timers
	boost::asio::io_service& Get_IO_Service();

// private:
	std::auto_ptr<CTCPComms>			m_pComms;
	CommsUtilities::eCommsSystemType	m_eCommsSystemType;
	std::string							m_strIP;
	unsigned short						m_usPort;
};
