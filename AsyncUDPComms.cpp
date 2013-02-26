/* 
 * File:   CAsyncUDPComms.cpp
 * Author: Josh
 *
 * Created on December 3, 2012
 *
 * This class is a shell for the Asynchronous UDP Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different IP Addresses and/or Ports
 * 
 * This is the class which is too be used for Async UDP Comms and derives from
 * CAsyncConnectionlessComms<CAsyncUDPCommsImpl>, which will take care of all the 
 * underlying IO responsibilities
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#include "stdafx.h"
#include "AsyncUDPComms.h"

#include "AsyncUDPCommsImpl.h"
#include "AsyncUDPReceiver.h"
#include "AsyncUDPSender.h"

// Recv Only
//
// This constructor allows the developer to instantiate Async UDP comms which
// Receives Only
//
// The Developer must define Receiver Callback during construction
CAsyncUDPComms::CAsyncUDPComms(	const unsigned short p_usUDPPort,
								const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback )
:
CAsyncConnectionlessComms<CAsyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CAsyncUDPCommsImpl(	this->GetIOServiceRecv(),
												p_usUDPPort, 
												p_bstPostRecvMessageCallback ) );
}

// Send Only
//
// This constructor allows the developer to instantiate Async UDP comms which
// Sends Only
//
// The Developer must define the Sender Callback during construction
CAsyncUDPComms::CAsyncUDPComms(	const std::string& p_strUDPAddress, 
								const unsigned short p_usUDPPort,
								const Callback_ConstRefString_Error p_bstPostSendMessageCallback )
:
CAsyncConnectionlessComms<CAsyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CAsyncUDPCommsImpl(	this->GetIOServiceSend(), 														
												p_strUDPAddress, 
												p_usUDPPort, 
												p_bstPostSendMessageCallback ) );
}

// Send and Recv on the Same Port and Address
// 
// This constructor allows the developer to instantiate Async UDP comms which
// Sends and Receives to and from the same group (IP/Port)
//
// The Developer must define Receiver and Sender Callbacks during construction
CAsyncUDPComms::CAsyncUDPComms(	const std::string& p_strUDPAddress, 
								const unsigned short p_usUDPPort,
								const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
								const Callback_ConstRefString_Error p_bstPostSendMessageCallback )
:
CAsyncConnectionlessComms<CAsyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CAsyncUDPCommsImpl(	this->GetIOServiceRecv(),
												this->GetIOServiceSend(),
												p_strUDPAddress, 
												p_usUDPPort, 
												p_bstPostRecvMessageCallback, 
												p_bstPostSendMessageCallback ) );
}

// Send and Recv on the Different Ports and/or Addresses
// 
// This constructor allows the developer to instantiate Async UDP comms which
// Sends and Receives to and from the different IP Addresses and/or Ports
//
// The Developer must define Receiver and Sender Callbacks during construction
CAsyncUDPComms::CAsyncUDPComms(	const unsigned short p_usUDPPortRecv,
								const std::string& p_strUDPAddressSend,
								const unsigned short p_usUDPPortSend,
								const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
								const Callback_ConstRefString_Error p_bstPostSendMessageCallback )
:
CAsyncConnectionlessComms<CAsyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CAsyncUDPCommsImpl(	this->GetIOServiceRecv(),
												this->GetIOServiceSend(),													
												p_usUDPPortRecv, 
												p_strUDPAddressSend, 
												p_usUDPPortSend,
												p_bstPostRecvMessageCallback, 
												p_bstPostSendMessageCallback ) );
}
