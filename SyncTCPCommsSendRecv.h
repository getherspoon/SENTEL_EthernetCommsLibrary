///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BoomerangPropertiesDialog.h
//-----------------------------------------------------------------------------
// MODULE NAME:			EthernetCommsModule
// FILE NAME:			SyncTCPCommsSendRecv.h
//
// PURPOSE:				Implements the Send/Recv methods for the Sync TCP Comms class. 
//						
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer			Date			Modification
//	0		Rob McGuire			01/03/2013		Creation
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "EthernetCommsAPI.h"
#include "CommsUtilities.h"
#include "TCPCommsImpl.h"
#include "ClientConnection.h"

class ETHERNETCOMMS_API CSyncTCPCommsSendRecv : public CTCPCommsImpl
{
public:
	virtual ~CSyncTCPCommsSendRecv();
	//Read and Send Methods, which will return true/false if the operation was successful. Will return 
	//additional information via parameters. 
	bool Read(std::string& p_strMsg, boost::system::error_code& p_bstError, const long p_lTimeoutInMs);
	bool Send(const std::string& p_strMsg, boost::system::error_code& p_bstError, const long p_lTimeoutInMs);
	

protected:
	CSyncTCPCommsSendRecv(const std::string &p_strIP, const unsigned short p_usPort);
	//Handlers for the Read/Write operations.
	void ReadHandler(const boost::system::error_code &p_bstError, const std::size_t sz);
	void WriteHandler(const boost::system::error_code &p_bstError, const std::size_t sz);
	void HandleTimeout(const boost::system::error_code& p_bstError); 

private:
	//Receive Buffer
	char											m_strRecvBuffer[m_s_nMAX_MESSAGE_SIZE];
	//Error codes for read/write errors
	std::auto_ptr<boost::system::error_code>		m_bstReadError;
	std::auto_ptr<boost::system::error_code>		m_bstWriteError;

	// Size of the data Read/Write in Bytes
	std::size_t										m_sizeReadSize;
	std::size_t										m_sizeWriteSize;

	// Boost Error object to hold any error that may occur when the timer times out.
	std::auto_ptr<boost::system::error_code>		m_bstTimeoutError;

	// Timer to determine when a Send/Receive operation will time out 
	// (our way of faking Sync using Async, we do this so that we're not stuck at a call when trying 
	// to terminate the class or the program)
	std::auto_ptr<boost::asio::deadline_timer>		m_bstReadTimer;
	std::auto_ptr<boost::asio::deadline_timer>		m_bstWriteTimer;

	// Boolean, TRUE when trying to write/receive.
	bool											m_bReceiving;
	bool											m_bWriting;
	
	//Critical section for timers
	boost::mutex									m_bstTimerLock;

};