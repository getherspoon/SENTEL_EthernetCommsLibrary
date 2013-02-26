/* 
 * File:   CSyncMulticastCommsImpl.h
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is a shell for the Synchronous Multicast Communication classes
 * Via this class, a developer can instantiate an Snyc Sender, an Sync Receiver, 
 * or Both, using the same or different Multicast Groups (IP/Port).
 * 
 * Typically, this class would be accessed via the CSyncMulticastComms class, which allows
 * the instantiation of Sync comms and handles all of the underlying details
 * 
 * While it is allowed for this class to be used on its own, it is not advised, as it then requires
 * the developer to handle the ASIO IO Services and threads. Using the CSyncMulticastComms class will 
 * save time, frustration, and lower the learning curve.
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */

#ifndef CSYNCMULTICASTCOMMSIMPL_H
#define	CSYNCMULTICASTCOMMSIMPL_H

#include "SyncConnectionlessCommsImpl.h"
#include "EthernetCommsAPI.h"

#include <string>

#include <boost/system/system_error.hpp>

class CSyncMulticastReceiver;
class CSyncMulticastSender;

class ETHERNETCOMMS_API CSyncMulticastCommsImpl : public CSyncConnectionlessCommsImpl<CSyncMulticastReceiver,CSyncMulticastSender>
{
public:	
	// Receiver Only
	CSyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
							 const std::string& p_strListenAddress, 
							 const std::string& p_strMulticastAddress, 
							 const unsigned short p_usMulticastPort,                         
							 const bool p_bRecvLoopbackEnabled ); 

	// Sender Only
	CSyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceSend,                                                    
							 const std::string& p_strMulticastAddress, 
							 const unsigned short p_usMulticastPort ); 

	// Sender and Receiver on Same Port & Address
    CSyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
							 boost::asio::io_service& p_bstIOServiceSend,
							 const std::string& p_strListenAddress, 
							 const std::string& p_strMulticastAddress, 
							 const unsigned short p_usMulticastPort,
							 const bool p_bRecvLoopbackEnabled );    

	// Sender and Receiver on Different Ports and Addresses
    CSyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
							 boost::asio::io_service& p_bstIOServiceSend,							 
							 const std::string& p_strListenAddress,
							 const std::string& p_strMulticastAddressRecv, 
							 const unsigned short p_usMulticastPortRecv,
							 const std::string& p_strMulticastAddressSend, 
							 const unsigned short p_usMulticastPortSend,							
							 const bool p_bRecvLoopbackEnabled );  

	virtual ~CSyncMulticastCommsImpl() {}

private:
	// Reset the Recv Object
	virtual void ResetRecv( CSyncMulticastReceiver* p_pRecv )
	{
		CSyncConnectionlessCommsImpl<CSyncMulticastReceiver,CSyncMulticastSender>::ResetRecv( p_pRecv );
	}

	// Rest the Send Object
	virtual void ResetSend( CSyncMulticastSender* p_pSend )
	{
		CSyncConnectionlessCommsImpl<CSyncMulticastReceiver,CSyncMulticastSender>::ResetSend( p_pSend );
	}

	// Get the Recv Object
	virtual CSyncMulticastReceiver* GetRecv() const
	{
		return CSyncConnectionlessCommsImpl<CSyncMulticastReceiver,CSyncMulticastSender>::GetRecv();
	}

	// Get the Send Object
	virtual CSyncMulticastSender* GetSend() const
	{
		return CSyncConnectionlessCommsImpl<CSyncMulticastReceiver,CSyncMulticastSender>::GetSend();
	}
};

#endif	/* CSYNCMULTICASTCOMMSIMPL_H */

