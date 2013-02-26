///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BoomerangPropertiesDialog.h
//-----------------------------------------------------------------------------
// MODULE NAME:			EthernetCommsModule
// FILE NAME:			AsyncTCPCommsSendRecv.h
//
// PURPOSE:				Implements the Send/Recv methods for the Async TCP Comms class. 
//						Also handles the Callbacks from Client Connection for the Async Comms. 
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer			Date			Modification
//	0		Rob McGuire			01/03/2013		Creation
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TCPCommsImpl.h"
#include "IClientConnectionHandler.h"

class ETHERNETCOMMS_API CAsyncTCPCommsSendRecv :	public CTCPCommsImpl,
													public IClientConnectionHandler
{
public:
	//Writes string out to the socket
	void Send(const std::string& p_strMsg);
	
	//Initiates a new Async Read off of the socket.
	void BeginNewAsyncRead();

protected:
	//protected constructor
	CAsyncTCPCommsSendRecv(const std::string &p_strIP, const unsigned short p_usPort, Callback_ConstRefString_RefError p_bstReadCallback, Callback_Error_RefSizeT p_bstSendCallback);
	~CAsyncTCPCommsSendRecv();

	//IConnectionHandler Methods
	virtual void OnConnect( const boost::asio::ip::tcp::endpoint& endpoint ){};
	virtual void OnConnectFailed( const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code &ec ){};
	virtual void OnNewConnection( const boost::asio::ip::tcp::endpoint& endpoint, ConnectionBase& connection ){};
	virtual void OnDisconnect( const boost::asio::ip::tcp::endpoint& endpoint ){};
	virtual void OnTerminate( const boost::asio::ip::tcp::endpoint& endpoint ){};

	//ICCH Method that always returns false
	virtual bool OnIdleTimeout( const boost::asio::ip::tcp::endpoint& endpoint, time_t tCurIdleTime ){return false;}

	//IConnectionHandler Methods [the following two are implemented]
	virtual bool OnRead( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& buf, std::size_t& nBytesAvailable, std::size_t& nBytesConsumed );
	virtual bool OnWrite( const boost::system::error_code &bstError, std::size_t& nBytesSent);
	
	//Returns the max message size for basic connection to size the socket read buffer. 
	virtual std::size_t GetMaxMessageSize( const boost::asio::ip::tcp::endpoint& endpoint ) const	{	return m_s_nMAX_MESSAGE_SIZE;	}

private:
	
	Callback_ConstRefString_RefError	m_bstReadCallback;
	Callback_Error_RefSizeT				m_bstSendCallback;

};
