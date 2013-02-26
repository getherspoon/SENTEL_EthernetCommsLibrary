#include "stdafx.h"
#include "AsyncTCPComms.h"

//Both send and receive callbacks are required to instantiate. 
CAsyncTCPComms::CAsyncTCPComms(const std::string &p_strIP, const unsigned short p_usPort, Callback_ConstRefString_RefError p_bstReadCallback, Callback_Error_RefSizeT p_bstSendCallback) :
CAsyncTCPCommsSendRecv(p_strIP, p_usPort, p_bstReadCallback, p_bstSendCallback)
{
}

CAsyncTCPComms::~CAsyncTCPComms()
{
}
