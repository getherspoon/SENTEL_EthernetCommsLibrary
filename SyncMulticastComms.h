/* 
 * File:   CSyncMulticastComms.h
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
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#ifndef CSYNCMULTICASTCOMMS_H
#define	CSYNCMULTICASTCOMMS_H

#include "SyncMulticastCommsImpl.h"
#include "SyncConnectionlessComms.h"
#include "EthernetCommsAPI.h"

class ETHERNETCOMMS_API CSyncMulticastComms : public CSyncConnectionlessComms<CSyncMulticastCommsImpl>
{
public:	
	// Recv Only
	CSyncMulticastComms(	const std::string& p_strMulticastAddress, 
							const unsigned short p_usMulticastPort,													
							const bool p_bRecvLoopbackEnabled );

	// Send Only
	CSyncMulticastComms(	const std::string& p_strMulticastAddress, 
							const unsigned short p_usMulticastPort );

	// Send and Recv (IP and Ports for each may be different or the same)
	CSyncMulticastComms(	const std::string& p_strMulticastAddressRecv, 
							const unsigned short p_usMulticastPortRecv,							
							const std::string& p_strMulticastAddressSend, 
							const unsigned short p_usMulticastPortSend,
							const bool p_bRecvLoopbackEnabled );

	// Destructor
	virtual ~CSyncMulticastComms() {}

private:
	// Reset the Comms Object
	virtual void ResetComms( CSyncMulticastCommsImpl* p_pComms )
	{
		CSyncConnectionlessComms<CSyncMulticastCommsImpl>::ResetComms( p_pComms );
	}

	// Get the Comms Object
	virtual CSyncMulticastCommsImpl* GetComms() const
	{
		return CSyncConnectionlessComms<CSyncMulticastCommsImpl>::GetComms();
	}

	// Get the Recv IO Service
	virtual boost::asio::io_service& GetIOServiceRecv()
	{
		return CSyncConnectionlessComms<CSyncMulticastCommsImpl>::GetIOServiceRecv();
	}

	// Get the Send IO Service
	virtual boost::asio::io_service& GetIOServiceSend()
	{
		return CSyncConnectionlessComms<CSyncMulticastCommsImpl>::GetIOServiceSend();
	}
};

#endif // CSYNCMULTICASTCOMMS_H