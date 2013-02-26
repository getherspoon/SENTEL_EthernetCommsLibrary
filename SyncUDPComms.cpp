/* 
 * File:   CSyncUDPComms.cpp
 * Author: Josh
 *
 * Created on December 3, 2012
 *
 * This class is a shell for the Synchronous UDP Communication classes
 * Via this class, a developer can instantiate an Snyc Sender, an Sync Receiver, 
 * or Both, using the same or different IP Addresses and/or Ports
 * 
 * This is the class which is too be used for Sync UDP Comms and derives from
 * CSyncConnectionlessComms<CSyncUDPCommsImpl>, which will take care of all the 
 * underlying IO responsibilities
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#include "stdafx.h"
#include "SyncUDPComms.h"

#include "SyncUDPCommsImpl.h"
#include "SyncUDPReceiver.h"
#include "SyncUDPSender.h"

// Recv Only
//
// This constructor allows the developer to instantiate Sync UDP comms which
// Receives Only
CSyncUDPComms::CSyncUDPComms(	const unsigned short p_usUDPPort )
:
CSyncConnectionlessComms<CSyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CSyncUDPCommsImpl(	this->GetIOServiceRecv(),
												p_usUDPPort ) );
}

// Send Only
//
// This constructor allows the developer to instantiate Sync UDP comms which
// Sends Only
CSyncUDPComms::CSyncUDPComms(	const std::string& p_strUDPAddress, 
								const unsigned short p_usUDPPort )
:
CSyncConnectionlessComms<CSyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CSyncUDPCommsImpl( this->GetIOServiceSend(), 														
											p_strUDPAddress, 
											p_usUDPPort ) );
}

// Send and Recv on the Different Ports and/or Addresses
// 
// This constructor allows the developer to instantiate Sync UDP comms which
// Sends and Receives to and from the different IP Addresses and/or Ports
CSyncUDPComms::CSyncUDPComms(	const unsigned short p_usUDPPortRecv,
								const std::string& p_strUDPAddressSend,
								const unsigned short p_usUDPPortSend )
:
CSyncConnectionlessComms<CSyncUDPCommsImpl>( CommsUtilities::UDP )
{
	this->ResetComms( new CSyncUDPCommsImpl(this->GetIOServiceRecv(),
											this->GetIOServiceSend(),
											p_usUDPPortRecv, 
											p_strUDPAddressSend, 
											p_usUDPPortSend ) );
}
