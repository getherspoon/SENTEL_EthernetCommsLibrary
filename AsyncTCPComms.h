///////////////////////////////////////////////////////////////////////////////
//						UNCLASSIFIED
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BoomerangPropertiesDialog.h
//-----------------------------------------------------------------------------
// MODULE NAME:			EthernetCommsModule
// FILE NAME:			AsyncTCPComms.h
//
// PURPOSE:				Implements the Async TCP Comms class. 
//						
//
// Revision History:
//-----------------------------------------------------------------------------
// Rev#		Developer			Date			Modification
//	0		Rob McGuire			01/03/2013		Creation
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "AsyncTCPCommsSendRecv.h"

class ETHERNETCOMMS_API CAsyncTCPComms : public CAsyncTCPCommsSendRecv
{
public:
	//receive and send callback required
	CAsyncTCPComms(const std::string &p_strIP, const unsigned short p_usPort, Callback_ConstRefString_RefError p_bstReadCallback, Callback_Error_RefSizeT p_bstSendCallback );

	~CAsyncTCPComms();

};