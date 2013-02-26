/* 
 * File:   CSyncUDPComms.h
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
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#ifndef CSYNCUDPCOMMS_H
#define	CSYNCUDPCOMMS_H

#include "SyncConnectionlessComms.h"
#include "EthernetCommsAPI.h"

class CSyncUDPCommsImpl;

class ETHERNETCOMMS_API CSyncUDPComms : public CSyncConnectionlessComms<CSyncUDPCommsImpl>
{
public:	
	// Recv Only
	CSyncUDPComms(	const unsigned short p_usUDPPort );

	// Send Only
	CSyncUDPComms(	const std::string& p_strUDPAddress, 
					const unsigned short p_usUDPPort );

	// Send and Recv (IP and Ports for each may be different or the same)
	CSyncUDPComms(	const unsigned short p_usUDPPortRecv,							
					const std::string& p_strUDPAddressSend, 
					const unsigned short p_usUDPPortSend );

	// Destructor
	virtual ~CSyncUDPComms() {}

private:
	// Reset the Comms Object
	virtual void ResetComms( CSyncUDPCommsImpl* p_pComms )
	{
		CSyncConnectionlessComms<CSyncUDPCommsImpl>::ResetComms( p_pComms );
	}

	// Get the Comms Object
	virtual CSyncUDPCommsImpl* GetComms() const
	{
		return CSyncConnectionlessComms<CSyncUDPCommsImpl>::GetComms();
	}

	// Get the Recv IO Service
	virtual boost::asio::io_service& GetIOServiceRecv()
	{
		return CSyncConnectionlessComms<CSyncUDPCommsImpl>::GetIOServiceRecv();
	}

	// Get the Send IO Service
	virtual boost::asio::io_service& GetIOServiceSend()
	{
		return CSyncConnectionlessComms<CSyncUDPCommsImpl>::GetIOServiceSend();
	}
};

#endif // CSYNCUDPCOMMS_H