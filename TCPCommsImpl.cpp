#include "stdafx.h"
#include "TCPCommsImpl.h"


CTCPCommsImpl::CTCPCommsImpl(const CommsUtilities::eCommsSystemType p_eCommsSystemType, const std::string &p_strIP, const unsigned short p_usPort) : 
m_eCommsSystemType(p_eCommsSystemType), m_strIP( p_strIP ), m_usPort( p_usPort )
{	
}

CTCPCommsImpl::~CTCPCommsImpl()
{
	//If we have a comms object upon destruction, delete it. 
	if(this->m_pComms.get())
	{
		this->m_pComms.reset();
	}
}

//This InitComms method passes the ConnectionHandler from the parameter list, to the connection object.
//This allows all of the callback methods to be handled by the ConnectionHandler of the caller's choosing.
//This is only to be used with ASync implementations, the assert below will alert the developer if a sync
//comm type tries to use this.
void CTCPCommsImpl::InitComms(IClientConnectionHandler* p_pThis)
{
	ASSERT(m_eCommsSystemType == CommsUtilities::ASYNC);

	if( !this->m_pComms.get() )
	{
		//Reset the comms object with the new paramenters
		this->m_pComms.reset(new CTCPComms(m_eCommsSystemType, m_strIP, m_usPort));
	}

	ASSERT( this->m_pComms.get() );

	if(this->m_pComms.get())
	{
		this->m_pComms->Open(p_pThis);
	}

}
//This InitComms method will Open the connection object, but without a ConnectionHandler.
//This is solely for synchronous implementations, as the comm object will handle all callbacks. 
void CTCPCommsImpl::InitComms()
{
	if( !this->m_pComms.get() )
	{
		//Reset the comms object with the new paramenters
		this->m_pComms.reset(new CTCPComms(m_eCommsSystemType, m_strIP, m_usPort));
	}

	ASSERT( this->m_pComms.get() );

	if(this->m_pComms.get())
	{
		this->m_pComms->Open();
	}
}

//TerminateComms is used to destroy the connection object. This is necessary for the asynchronous implementation, due 
//to the async implementation handling callbacks outside of the comm object. 
void CTCPCommsImpl::TerminateComms()
{
	if(this->m_pComms.get())
	{
		this->m_pComms.reset();
	}
}

//ReadAsync here will return whatever is read to the Callback in the parameter list. Useful for synchronous implmentations.
void CTCPCommsImpl::ReadAsync(char *p_str, const int p_iMaxRecvSize, Callback_ConstError_RefSizeT handler)
{
	this->m_pComms->ReadAsync(p_str, p_iMaxRecvSize, handler);
}

//ReadAsync here will trigger a new read. 
void CTCPCommsImpl::ReadAsync()
{
	this->m_pComms->ReadAsync();
}

//Returns if the comm object exists. 
bool CTCPCommsImpl::HasComms() const
{
	return this->m_pComms.get() ? true : false;
}

//Cancels any remaining IO on the socket. 
void CTCPCommsImpl::CancelIO(boost::system::error_code &ec)
{
	this->m_pComms->Cancel(ec);
}

//Returns if the socket connection is open. 
bool CTCPCommsImpl::IsOpen() const
{
	return this->m_pComms->IsSocketOpen();
}

//WriteAsync here will return the error code and bytes written to the handler. Useful for Sync implementations.
void CTCPCommsImpl::WriteAsync(const std::string &p_strMessage, Callback_ConstError_RefSizeT handler)
{
	this->m_pComms->WriteAsync(p_strMessage, handler);
}

//WriteAsync here writes the string out and will call the callback once completed. 
void CTCPCommsImpl::WriteAsync(const std::string &p_strMessage)
{
	this->m_pComms->WriteAsync(p_strMessage);
}

//Returns if the socket is connected.
bool CTCPCommsImpl::IsConnected() const
{
	return this->m_pComms->IsSocketConnected();
}

//Returns the IO Service used for the comms object. 
boost::asio::io_service& CTCPCommsImpl::Get_IO_Service()
{
	return this->m_pComms->GetIOService();
}