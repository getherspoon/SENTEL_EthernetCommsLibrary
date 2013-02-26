#include "stdafx.h"
#include "TCPComms.h"

//Logger for BasicConnection. 
#ifndef LOGGER
#define LOGGER
Templogger logger;
#endif

CTCPComms::CTCPComms(const CommsUtilities::eCommsSystemType p_eCommsSystemType, const std::string &p_strIP, const unsigned short p_usPort) : 
m_eCommsSystemType(p_eCommsSystemType), 
ClientConnection(p_strIP, p_usPort, 2/*2 second reconnect delay*/ , p_eCommsSystemType )
{
	this->m_bstWork.reset( new boost::asio::io_service::work( this->GetIOService() ) ); 
}

//Forces ClientConnection to terminate all connection activities. 
CTCPComms::~CTCPComms()
{
	this->Stop();
}
//Check if we have a open socket.
bool CTCPComms::IsSocketOpen() const
{
	return this->GetSocket().is_open();
}

//Cancel all IO on the socket.
void CTCPComms::Cancel(boost::system::error_code &ec)
{
	this->GetSocket().cancel(ec);
}

//Synchronous clients will use this to avoid dealing with callbacks. 
void CTCPComms::Open()
{
	ASSERT(this->m_eCommsSystemType == CommsUtilities::SYNC);
	this->Start(this);
}

//Asynchronous clients will use this Open because they will handle the callbacks directly.
void CTCPComms::Open(IClientConnectionHandler *p_handler)
{
	ASSERT(this->m_eCommsSystemType == CommsUtilities::ASYNC);
	this->Start(p_handler);
}

//Expanded to directly write to the socket and pass error/size info back to the handler
//this is for our syncwrite implementation.
void CTCPComms::WriteAsync(const std::string &p_str, Callback_ConstError_RefSizeT handler)
{
	this->GetSocket().async_write_some(boost::asio::buffer(p_str.c_str(), p_str.size()), 
		boost::bind(handler, _1, _2));
}

//Async writing over the socket using the basic connection method.
void CTCPComms::WriteAsync(const std::string &p_str)
{
	this->AsyncWrite(p_str);
}

//Expanded to directly read from the socket and pass message back to the handler.
//This is for our SyncRead implementation.
void CTCPComms::ReadAsync(char *p_str, const int p_iMaxRecvSize, Callback_ConstError_RefSizeT handler)
{
	this->GetSocket().async_read_some(boost::asio::buffer(p_str, p_iMaxRecvSize), 
		boost::bind(handler, _1, _2)); 
}

//Depends on callbacks being set appropriately. Will trigger a new read.
void CTCPComms::ReadAsync()
{
	ASSERT(this->m_eCommsSystemType == CommsUtilities::ASYNC);
	this->BeginNewAsyncRead();
}

//Returns if the Socket is Connected.
bool CTCPComms::IsSocketConnected()
{
	return this->IsConnected();
}

//Returns the IO Service. Useful for Sync Implementation Timers. 
boost::asio::io_service&	CTCPComms::GetIOService()	
{ 
	return this->Get_io_service();
}