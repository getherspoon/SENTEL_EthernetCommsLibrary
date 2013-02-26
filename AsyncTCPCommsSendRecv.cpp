#include "stdafx.h"
#include "AsyncTCPCommsSendRecv.h"

//Read and Send Callbacks are required. 
CAsyncTCPCommsSendRecv::CAsyncTCPCommsSendRecv(const std::string &p_strIP, const unsigned short p_usPort, Callback_ConstRefString_RefError p_bstReadCallback, Callback_Error_RefSizeT p_bstSendCallback) :
CTCPCommsImpl(CommsUtilities::ASYNC, p_strIP, p_usPort)
{
	this->m_bstReadCallback = p_bstReadCallback;
	this->m_bstSendCallback = p_bstSendCallback;
	this->InitComms(this);
}

CAsyncTCPCommsSendRecv::~CAsyncTCPCommsSendRecv()
{
	//Because we are using this as the IClientConnectionHandler object, we need to destroy the ClientConnection object first, so the handlers still exist for
	//ClientConnection/BasicConnection to call on disconnect/terminate.
	this->TerminateComms();
}

void CAsyncTCPCommsSendRecv::BeginNewAsyncRead()
{
	this->ReadAsync();
}

bool CAsyncTCPCommsSendRecv::OnRead(const boost::asio::ip::tcp::endpoint &endpoint, const std::string &buf, std::size_t &nBytesAvailable, std::size_t &nBytesConsumed)
{
	boost::system::error_code bstError;
	bool bResult = false;
	
	if(this->m_bstReadCallback)
	{
		//Call the user defined callback function with the parameters from BasicConnection
		this->m_bstReadCallback(buf, bstError);
	}
	else
	{
		//Somehow the callback was not set!
		ASSERT(false);
	}

	//If we do not have an error, the ReadCallback is assumed to have consumed all the available data.
	//If we do have an error, the ReadCallback is assumed to not be able to process the message. 
	//In either of these cases, the buffer will be cleared by BasicConnection. If the BytesConsumed is less
	//than the BytesAvailable, BasicConnection will shift the message and remove the portion that was read. 
	if(!bstError)
	{
		bResult = true;
		nBytesConsumed = nBytesAvailable;
	}
	else
	{
		bResult = false;
	}

	return bResult;
}

void CAsyncTCPCommsSendRecv::Send(const std::string &p_strMsg)
{
	this->WriteAsync(p_strMsg);
}

bool CAsyncTCPCommsSendRecv::OnWrite(const boost::system::error_code &bstError, std::size_t &nBytesSent)
{
	boost::system::error_code p_bstError;
	
	if(this->m_bstSendCallback)
	{
		//Call the user defined callback function with the parameters from BasicConnection
		this->m_bstSendCallback(p_bstError, nBytesSent);
	}
	else
	{
		//Somehow the write callback was not set!	
		ASSERT(false);
	}

	return !p_bstError;
}