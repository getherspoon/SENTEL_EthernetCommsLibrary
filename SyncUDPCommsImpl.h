/*
 * File:   CSyncUDPCommsImpl.h
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is a shell for the Synchronous UDP Communication classes
 * Via this class, a developer can instantiate an Snyc Sender, an Sync Receiver, 
 * or Both, using the same or different UDP Groups (IP/Port).
 * 
 * Typically, this class would be accessed via the CUDPComms class, which allows
 * the instantiation of either Sync or Async comms and handles all of the underlying details
 * 
 * While it is allowed for this class to be used on its own, it is not advised, as it then requires
 * the developer to handle the ASIO IO Services and threads. Using the CUDPComms class will 
 * save time, frustration, and lower the learning curve.
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#ifndef CSYNCUDPCOMMSIMPL_H
#define	CSYNCUDPCOMMSIMPL_H

#include "SyncConnectionlessCommsImpl.h"
#include "EthernetCommsAPI.h"

#include <memory>

#include <boost/system/error_code.hpp>

class CSyncUDPReceiver;
class CSyncUDPSender;

class ETHERNETCOMMS_API CSyncUDPCommsImpl : public CSyncConnectionlessCommsImpl<CSyncUDPReceiver, CSyncUDPSender>
{
public:
	// Recv Only
	CSyncUDPCommsImpl(boost::asio::io_service& p_bstIOServiceRecv,                          
				  const unsigned short p_usUDPPort );   

	// Send Only
	CSyncUDPCommsImpl(boost::asio::io_service& p_bstIOServiceSend,                          
                  const std::string& p_strUDPAddress, 
                  const unsigned short p_usUDPPort );

	// Send and Recv on Same Port
    CSyncUDPCommsImpl(boost::asio::io_service& p_bstIOServiceRecv,
					  boost::asio::io_service& p_bstIOServiceSend,					  
					  const std::string& p_strUDPAddress, 
					  const unsigned short p_usUDPPort );

	// Send and Recv on Different Ports
    CSyncUDPCommsImpl(boost::asio::io_service& p_bstIOServiceRecv,                          
					  boost::asio::io_service& p_bstIOServiceSend,
					  const unsigned short p_usUDPPortRecv,
					  const std::string& p_strUDPAddressSend, 
					  const unsigned short p_usUDPPortSend );  

	virtual ~CSyncUDPCommsImpl() {}   

private:
	// Reset the Recv Object
	virtual void ResetRecv( CSyncUDPReceiver* p_pRecv )
	{
		CSyncConnectionlessCommsImpl<CSyncUDPReceiver,CSyncUDPSender>::ResetRecv( p_pRecv );
	}

	// Rest the Send Object
	virtual void ResetSend( CSyncUDPSender* p_pSend )
	{
		CSyncConnectionlessCommsImpl<CSyncUDPReceiver,CSyncUDPSender>::ResetSend( p_pSend );
	}

	// Get the Recv Object
	virtual CSyncUDPReceiver* GetRecv() const
	{
		return CSyncConnectionlessCommsImpl<CSyncUDPReceiver,CSyncUDPSender>::GetRecv();
	}

	// Get the Send Object
	virtual CSyncUDPSender* GetSend() const
	{
		return CSyncConnectionlessCommsImpl<CSyncUDPReceiver,CSyncUDPSender>::GetSend();
	}
};

#endif	/* CSYNCUDPCOMMSIMPL_H */