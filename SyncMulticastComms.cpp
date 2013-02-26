/* 
 * File:   CSyncMulticastComms.cpp
 * Author: Josh
 *
 * Created on December 3, 2012
 *
 * This class is a shell for the Synchronous Multicast Communication classes
 * Via this class, a developer can instantiate an Snyc Sender, an Sync Receiver, 
 * or Both, using the same or different Multicast Groups (IP/Port).
 * 
 * This is the class which is too be used for Sync Multicast Comms and derives from
 * CSyncConnectionlessComms<CSyncMulticastCommsImpl>, which will take care of all the 
 * underlying IO responsibilities
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#include "stdafx.h"
#include "SyncMulticastComms.h"

#include "SyncMulticastReceiver.h"
#include "SyncMulticastSender.h"

// Recv Only
//
// This constructor allows the developer to instantiate Sync Multicast comms which
// Receives Only
//
// If RecvLoopbackEnabled is true, then the receiver will listen to local messages,
// Typically, this is only desired if the Sender and Receiver are in different groups.
// or if there is no Sender
CSyncMulticastComms::CSyncMulticastComms(	const std::string& p_strMulticastAddress, 
											const unsigned short p_usMulticastPort,											
											const bool p_bRecvLoopbackEnabled )
:
CSyncConnectionlessComms<CSyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CSyncMulticastCommsImpl(  this->GetIOServiceRecv(),
													CommsUtilities::Multicast::s_strDEFAULT_LISTEN_ADDRESS,
													p_strMulticastAddress, 
													p_usMulticastPort, 														
													p_bRecvLoopbackEnabled ) );
}

// Send Only
//
// This constructor allows the developer to instantiate Sync Multicast comms which
// Sends Only
CSyncMulticastComms::CSyncMulticastComms(	const std::string& p_strMulticastAddress, 
											const unsigned short p_usMulticastPort )
:
CSyncConnectionlessComms<CSyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CSyncMulticastCommsImpl(	this->GetIOServiceSend(), 														
													p_strMulticastAddress, 
													p_usMulticastPort ) );
}

// Send and Recv on the Different Ports and/or Addresses
// 
// This constructor allows the developer to instantiate Sync Multicast comms which
// Sends and Receives to and from the different IP Addresses and/or Ports
//
// If RecvLoopbackEnabled is true, then the receiver will listen to local messages,
// Typically, this is only desired if the Sender and Receiver are in different groups.
CSyncMulticastComms::CSyncMulticastComms(	const std::string& p_strMulticastAddressRecv, 
											const unsigned short p_usMulticastPortRecv,
											const std::string& p_strMulticastAddressSend,
											const unsigned short p_usMulticastPortSend,
											const bool p_bRecvLoopbackEnabled )
:
CSyncConnectionlessComms<CSyncMulticastCommsImpl>( CommsUtilities::MULTICAST )
{
	this->ResetComms( new CSyncMulticastCommsImpl(  this->GetIOServiceRecv(),
													this->GetIOServiceSend(),
													CommsUtilities::Multicast::s_strDEFAULT_LISTEN_ADDRESS,
													p_strMulticastAddressRecv, 
													p_usMulticastPortRecv, 
													p_strMulticastAddressSend, 
													p_usMulticastPortSend,
													p_bRecvLoopbackEnabled ) );
}
