/* 
 * File:   CAsyncMulticastComms.cpp
 * Author: Josh
 *
 * Created on December 3, 2012
 *
 * This class is a shell for the Asynchronous Multicast Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different Multicast Groups (IP/Port).
 * 
 * This is the class which is too be used for Async Multicast Comms and derives from
 * CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>, which will take care of all the 
 * underlying IO responsibilities
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#include "stdafx.h"
#include "AsyncMulticastComms.h"

#include "AsyncMulticastReceiver.h"
#include "AsyncMulticastSender.h"

// Recv Only
//
// This constructor allows the developer to instantiate Async Multicast comms which
// Receives Only
//
// If RecvLoopbackEnabled is true, then the receiver will listen to local messages,
// Typically, this is only desired if the Sender and Receiver are in different groups.
// or if there is no Sender
//
// The Developer must define Receiver Callback during construction
CAsyncMulticastComms::CAsyncMulticastComms(	const std::string& p_strMulticastAddress, 
											const unsigned short p_usMulticastPort,
											const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,							
											const bool p_bRecvLoopbackEnabled )
:
CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CAsyncMulticastCommsImpl( this->GetIOServiceRecv(), 
													CommsUtilities::Multicast::s_strDEFAULT_LISTEN_ADDRESS,
													p_strMulticastAddress, 
													p_usMulticastPort, 
													p_bstPostRecvMessageCallback, 
													p_bRecvLoopbackEnabled ) );
}

// Send Only
//
// This constructor allows the developer to instantiate Async Multicast comms which
// Sends Only
//
// The Developer must define the Sender Callback during construction
CAsyncMulticastComms::CAsyncMulticastComms(	const std::string& p_strMulticastAddress, 
											const unsigned short p_usMulticastPort,
											const Callback_ConstRefString_Error p_bstPostSendMessageCallback )
:
CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CAsyncMulticastCommsImpl( this->GetIOServiceSend(),
													p_strMulticastAddress, 
													p_usMulticastPort, 
													p_bstPostSendMessageCallback ) );
}

// Send and Recv on the Same Port and Address
// 
// This constructor allows the developer to instantiate Async Multicast comms which
// Sends and Receives to and from the same group (IP/Port)
//
// If RecvLoopbackEnabled is true, then the receiver will listen to local messages,
// Typically, this is only desired if the Sender and Receiver are in different groups.
//
// The Developer must define Receiver and Sender Callbacks during construction
CAsyncMulticastComms::CAsyncMulticastComms(	const std::string& p_strMulticastAddress, 
											const unsigned short p_usMulticastPort,
											const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
											const Callback_ConstRefString_Error p_bstPostSendMessageCallback,
											const bool p_bRecvLoopbackEnabled )
:
CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CAsyncMulticastCommsImpl( this->GetIOServiceRecv(),
													this->GetIOServiceSend(),
													CommsUtilities::Multicast::s_strDEFAULT_LISTEN_ADDRESS,
													p_strMulticastAddress, 
													p_usMulticastPort, 
													p_bstPostRecvMessageCallback, 
													p_bstPostSendMessageCallback,
													p_bRecvLoopbackEnabled ) );
}

// Send and Recv on the Different Ports and/or Addresses
// 
// This constructor allows the developer to instantiate Async Multicast comms which
// Sends and Receives to and from the different IP Addresses and/or Ports
//
// If RecvLoopbackEnabled is true, then the receiver will listen to local messages,
// Typically, this is only desired if the Sender and Receiver are in different groups.
//
// The Developer must define Receiver and Sender Callbacks during construction
CAsyncMulticastComms::CAsyncMulticastComms(	const std::string& p_strMulticastAddressRecv, 
											const unsigned short p_usMulticastPortRecv,
											const std::string& p_strMulticastAddressSend,
											const unsigned short p_usMulticastPortSend,
											const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
											const Callback_ConstRefString_Error p_bstPostSendMessageCallback,
											const bool p_bRecvLoopbackEnabled )
:
CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CAsyncMulticastCommsImpl( this->GetIOServiceRecv(),
													this->GetIOServiceSend(),
													CommsUtilities::Multicast::s_strDEFAULT_LISTEN_ADDRESS,
													p_strMulticastAddressRecv, 
													p_usMulticastPortRecv, 
													p_strMulticastAddressSend, 
													p_usMulticastPortSend,
													p_bstPostRecvMessageCallback, 
													p_bstPostSendMessageCallback,
													p_bRecvLoopbackEnabled ) );
}
