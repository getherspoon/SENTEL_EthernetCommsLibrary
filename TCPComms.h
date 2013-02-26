///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BoomerangPropertiesDialog.h
//-----------------------------------------------------------------------------
// MODULE NAME:			EthernetCommsModule
// FILE NAME:			TCPComms.h
//
// PURPOSE:				Wrapper of the ClientConnection class for TCP Communications. 
//						Also implements IClientConnectionHandler for Sync TCP Comms.
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer			Date			Modification
//	0		Rob McGuire			01/03/2013		Creation
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "EthernetCommsAPI.h"
#include "ClientConnection.h"
#include "CommsUtilities.h"

class ETHERNETCOMMS_API CTCPComms : private ClientConnection,
									public IClientConnectionHandler

{
public:
	CTCPComms(const CommsUtilities::eCommsSystemType p_eCommsSystemType, const std::string &p_strIP, const unsigned short p_usPort);
	virtual ~CTCPComms();
	
	//IConnectionHandler Methods will not be implemented here.
	//This will avoid have to be concerned with these for Sync connections
	//For Async, the AsyncTCPCommsSendRecv class will inherit from ICCH and be passed to ClientConnection
	//to call when necessary.
	virtual void OnConnect( const boost::asio::ip::tcp::endpoint& endpoint ) {};
	virtual void OnConnectFailed( const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code &ec ){};
	virtual void OnNewConnection( const boost::asio::ip::tcp::endpoint& endpoint, ConnectionBase& connection ){};
	virtual void OnDisconnect( const boost::asio::ip::tcp::endpoint& endpoint ){};
	virtual void OnTerminate( const boost::asio::ip::tcp::endpoint& endpoint ){};
	
	//Methods Implemented from ICH
	//These methods return false to ensure the socket buffer is cleared after each packet is read, this avoids any incomplete data remaining in the buffer.
	//See ClientConnection for more details on this issue. 
	virtual bool OnRead( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& buf, std::size_t& nBytesAvailable, std::size_t& nBytesConsumed ){return false;}
	virtual bool OnIdleTimeout( const boost::asio::ip::tcp::endpoint& endpoint, time_t tCurIdleTime ){return false;}
	virtual bool OnWrite( const boost::system::error_code &ec, std::size_t& nBytesSent){return false;}
	virtual std::size_t GetMaxMessageSize( const boost::asio::ip::tcp::endpoint& endpoint )	const {	return m_s_nMAX_MESSAGE_SIZE;	}
	
	//Max message size
	static const std::size_t	m_s_nMAX_MESSAGE_SIZE = 65536;	

	//Wrapping ClientConnection Methods here to only partially expose the socket to classes composed of this object.
	bool IsSocketOpen() const;
	bool IsSocketConnected();
	void Cancel(boost::system::error_code &ec);
	void Open(IClientConnectionHandler *p_handler);
	void Open();
	void WriteAsync(const std::string &p_str);
	void WriteAsync(const std::string &p_str, Callback_ConstError_RefSizeT handler);
	void ReadAsync();
	void ReadAsync(char *p_str, const int p_iMaxRecvSize, Callback_ConstError_RefSizeT handler);
	boost::asio::io_service&	GetIOService();

	boost::asio::ip::tcp::socket& Socket()	{ return this->GetSocket(); }

private:
	CommsUtilities::eCommsSystemType		m_eCommsSystemType;

	// ASIO Worker Object used to keep the ASIO IO Service alive even with no IO is occuring
	std::auto_ptr<boost::asio::io_service::work>    m_bstWork;

};