#include "stdafx.h"
#include "SyncTCPComms.h"

//Constructor which instantiates the Sync Send/Recv Class.
CSyncTCPComms::CSyncTCPComms(const std::string &p_strIP, const unsigned short p_usPort) :
CSyncTCPCommsSendRecv(p_strIP, p_usPort)
{
}

CSyncTCPComms::~CSyncTCPComms()
{
}

void CSyncTCPComms::CancelIO(boost::system::error_code &ec)
{
	CTCPCommsImpl::CancelIO(ec);
}

bool CSyncTCPComms::IsOpen() const
{
	return CTCPCommsImpl::IsOpen();
}