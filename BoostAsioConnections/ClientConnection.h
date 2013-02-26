#ifndef CLIENT_CONNECTION_INCLUDED_
#define CLIENT_CONNECTION_INCLUDED_ 1

#include "BasicConnection.h"
#include "IClientConnectionHandler.h"
#include "CommsUtilities.h"

#include <boost/lexical_cast.hpp>

// See the comments for BasicConnection.
//
// The intended usage life-cycle is to create a ClientConnection, call Start to begin the boost::io_service event
// processing loop, then to delete the object when event processing should stop. 
//
// Generally a handler object that implements IClientConnectionHandler must be provided when calling Start(). 
// However, if you derive a class from both ClientConnection and IClientConnectionHandler then call Start with no arguments 
// then that object will be used as both the connection and the connection handler.
//
// ClientConnection will attempt to establish a socket level connection. If the socket level connection is closed 
// then ClientConnection will attempt to establish it unless it is passed nReconnectDelaySec=0.
class ClientConnection : public BasicConnection<IClientConnectionHandler>
{
public:

   // Ctor parameters:
   // strIp - the remote IP to connect to
   // nPort - the remote port number to connect to
   // nReconnectDelaySec - the time delay between reconnect attempts if the socket is disconnected. 
   //    If nReconnectDelaySec is 0 then ClientConnection won't attempt to automatically reconnect.
   //    There has to be at least a 1 second delay to avoid sucking up the whole CPU if a connection can't be made.
	ClientConnection( const std::string& strIp, unsigned short nPort, unsigned short nReconnectDelaySec = 2, const CommsUtilities::eCommsSystemType eCommsType = CommsUtilities::ASYNC )
      : m_nReconnectDelaySec(nReconnectDelaySec), m_eCommsType( eCommsType )
   {
      logger.Log( "ClientConnection::ClientConnection-" );      

	  // create an endpoint for this IP and port
	  boost::system::error_code ec;
      boost::asio::ip::address addr = boost::asio::ip::address::from_string(strIp, ec);
	  std::auto_ptr<boost::asio::ip::tcp::endpoint>	endpoint;
	  // If we have an error, then perhaps we have been passed a hostname, we'll try to resolve that
	  if( ec )
	  {
			boost::asio::io_service						io_service;
			boost::asio::ip::tcp::resolver				bstResolver( io_service );
			boost::asio::ip::tcp::resolver::query		bstQuery( strIp, boost::lexical_cast<std::string>( nPort ) );
			boost::asio::ip::tcp::resolver::iterator	bstEndpointIterator = bstResolver.resolve( bstQuery );	
			endpoint.reset( new boost::asio::ip::tcp::endpoint( *bstEndpointIterator ) );
	  }	  
	  else
	  {
		  endpoint.reset( new boost::asio::ip::tcp::endpoint(addr, nPort) );
	  }
      SetRemoteEndpoint( *endpoint );

      // create an io_service
      std::auto_ptr<boost::asio::io_service> io_service( new boost::asio::io_service );
      Set_io_service( io_service );	  

      // create a socket to use for comms
      std::auto_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(Get_io_service()) );
      SetSocket( sock );

      logger.Log( "ClientConnection::ClientConnection+" );
   }

   // Call this exactly once after the ctor or in the ctor of a derived class.
   void Start( IClientConnectionHandler* handler = 0 )
   {
      logger.Log( "ClientConnection::Start-" );

      if ( handler )
      {
         SetConnectionHandler(handler);
      }

      BasicConnection<IClientConnectionHandler>::Start();

      logger.Log( "ClientConnection::Start+" );
   }


private:

   // not implemented - copying is not allowed
   ClientConnection( const ClientConnection& );

   // override of the virtual function
   bool ProcessDisconnect(const boost::system::error_code& ec)
   {
      std::string strLog = "ClientConnection::Disconnect-";
      strLog += GetRemoteEndpoint().address().to_string();
      logger.Log( strLog, &ec );

      // base call will make sure the socket is closed and call the handler's OnDisconnect() function
      // and tell us if we should do anything or it's already known that we're disconnected
      bool bDoDisconnect = BasicConnection<IClientConnectionHandler>::ProcessDisconnect(ec);

      if ( bDoDisconnect && m_nReconnectDelaySec )
      {
         logger.Log( "ClientConnection::ProcessDisconnect REC" );

         // got disconnected for some reason, so do the delay then try again 
         boost::asio::deadline_timer timer(Get_io_service(), boost::posix_time::seconds(m_nReconnectDelaySec));
         boost::system::error_code ec;
         timer.wait( ec );
         GetSocket().async_connect( GetRemoteEndpoint(), boost::bind( &ClientConnection::connect_handler, this, _1 ) );      
      }
      logger.Log( "ClientConnection::ProcessDisconnect+" );

      return bDoDisconnect;
   }

   // override of the virtual function - runs on the main thread
   void ProcessNewConnection()
   {
      std::string strLog = "ClientConnection::ProcessNewConnection- ";
      strLog += GetRemoteEndpoint().address().to_string();
      logger.Log( strLog );
      // will make an async call to new_connection_handler() 
      BasicConnection<IClientConnectionHandler>::ProcessNewConnection();

      // make an async call to connect the socket
      GetSocket().async_connect( GetRemoteEndpoint(), boost::bind( &ClientConnection::connect_handler, this, _1 ) );
      logger.Log( "ClientConnection::ProcessNewConnection+" );
   }

   // called asynchronously after an attempt is made to connect to the server
   void connect_handler(const boost::system::error_code &ec)
   {
      std::string strLog = "ClientConnection::connect_handler- ";
      strLog += GetRemoteEndpoint().address().to_string();
      logger.Log( strLog, &ec );

      // Although it shouldn't be possible, logging showed that two consecutive connection
      // attempts returned 10061 (refused by target) and 10056 (connection made on an already
      // connected socket), respectively. So if the socket says it's connected, go with it.
      if ( !ec || ec.value() == 10056 )      
      {
         SetLocalEndpoint();

         // we connected, so the connection is up...
         UpdateLastSuccessTime();

         // call the OnConnect() callback to allow m_handler to do any processing
         GetConnectionHandler().OnConnect(GetRemoteEndpoint());

		 // Only perform a Read right away if this is ASYNC
		 if( m_eCommsType == CommsUtilities::ASYNC )
		 {
			// start a new async read cycle which will clear anything left in the buffer
			BeginNewAsyncRead();
		}
      }
      else
      {
         // debugging only - was trying to make sure what boost does. Found that after closing the socket and then trying to reconnect,
         // if ec == 10061 (target machine actively refused connection, which occured because no app was running on the target) that
         // the assertion failed. Makes no sense to me.
         // assert(!GetSocket().is_open());

         // call the OnConnectFailed() callback to allow m_handler to do any processing
         GetConnectionHandler().OnConnectFailed(GetRemoteEndpoint(), ec);

         if ( m_nReconnectDelaySec )
         {
            // didn't connect for some reason, so do the delay then try again 
            boost::asio::deadline_timer timer(Get_io_service(), boost::posix_time::seconds(m_nReconnectDelaySec));
            boost::system::error_code ec;
            timer.wait( ec );

            logger.Log( "ClientConnection::connect_handler REC" );
            GetSocket().async_connect( GetRemoteEndpoint(), boost::bind( &ClientConnection::connect_handler, this, _1 ) );      
         }
      }
   }

   // private data members
   const unsigned short m_nReconnectDelaySec;
   const CommsUtilities::eCommsSystemType m_eCommsType;
};

#endif // CLIENT_CONNECTION_INCLUDED_