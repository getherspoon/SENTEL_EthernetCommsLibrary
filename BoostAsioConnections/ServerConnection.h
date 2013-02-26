#ifndef SERVER_CONNECTION_INCLUDED_
#define SERVER_CONNECTION_INCLUDED_ 1

#include "BasicConnection.h"

class IConnectionManager
{
public: 
   // used by ServerConnection to notify some outside "manager" or server object that the connection has ended.
   virtual void OnTerminateConnection( const boost::asio::ip::tcp::endpoint& endpoint ) = 0;
};

// The intended usage life-cycle is for some outside "manager", presumably a server process, to accept (or otherwise create) a socket and
// io_service, and to use those to create a ServerConnection. Then call Start() to begin the boost::io_service event
// processing loop. When the socket gets closed or an error occurs on the socket the event processing loop will stop. 
//
// Generally a handler object that implements IClientConnectionHandler must be provided when calling Start(). 
// However, if you derive a class from both ServerConnection and IConnectionHandler then call Start with no arguments 
// then that object will be used as both the connection and the connection handler.
//
// The callback methods defined in IConnectionHandler will run on the comm thread, so the developer must 
// consider thread safety for any objects they access.
class ServerConnection : public BasicConnection<IConnectionHandler>
{
public:

   // Ctor parameters:
   // sock - an auto_ptr to a socket that has been accepted
   // client_io_service - an auto_ptr to the io_service that sock uses
   // endpoint - the remote endpoint for the socket (note - has to be passed b/c if sock is closed by the time we get it we
   //            can't call sock->remote_endpoint().
   // manager - a pointer to the some object (e.g., a server) that accepted and/or created this socket so we can tell it when the socket closes
   ServerConnection( std::auto_ptr<boost::asio::ip::tcp::socket> sock, std::auto_ptr<boost::asio::io_service> client_io_service,
      const boost::asio::ip::tcp::endpoint& endpoint, IConnectionManager* manager )
      : m_manager(manager)
   {
      logger.Log( "ServerConnection::ServerConnection-" );

      // set the endpoint 
      SetRemoteEndpoint( endpoint );
      Set_io_service( client_io_service );
      SetSocket( sock );

      if ( GetSocket().is_open() )
      {
         SetLocalEndpoint();

         // we are connected so call UpdateLastSuccessTime()
         UpdateLastSuccessTime();


#ifdef _DEBUG
         // just a stupidity check
         // if the socket is still open (i.e., no error on GetSocket().remote_endpoint()) then get its remote endpoint and 
         // compare it to the endpoint we were told 
         boost::system::error_code ec;
         boost::asio::ip::tcp::endpoint socketEndpoint = GetSocket().remote_endpoint( ec );
         assert( ec || socketEndpoint == endpoint );
#endif
      }

      logger.Log( "ServerConnection::ServerConnection+" );
   }

   // Call this exactly once after the ctor or in the ctor of a derived class.
   void Start( IConnectionHandler* handler = 0 )
   {
      logger.Log( "ServerConnection::Start-" );

      if ( handler )
      {
         SetConnectionHandler(handler);
      }

      BasicConnection::Start();

      logger.Log( "ServerConnection::Start+" );
   }


private:

   // not implemented - copying is not allowed
   ServerConnection( const ServerConnection& );

   // override of the virtual function - runs on the main thread
   void ProcessNewConnection()
   {
      logger.Log( "ServerConnection::ProcessNewConnection-" );

      // will make an async call to new_connection_handler() 
      BasicConnection::ProcessNewConnection();

      // will make an async call to begin reading
      BeginNewAsyncRead();

      logger.Log( "ServerConnection::ProcessNewConnection+" );
   }

   // override of the virtual function
   bool ProcessDisconnect(const boost::system::error_code& ec)
   {
      logger.Log( "ServerConnection::ProcessDisconnect-" );

      // base call will make sure the socket is closed and call the handler's OnDisconnect() function
      // and tell us if we should do anything or it's already known that we're disconnected
      bool bDoDisconnect = BasicConnection::ProcessDisconnect(ec);

      if ( bDoDisconnect )
      {
         // No way we can reconnect with a client. Have the connection stop and then exit normally
         AsyncStop();
      }

      logger.Log( "ServerConnection::ProcessDisconnect+" );

      return bDoDisconnect;
   }

   // override of the virtual function
   void ProcessTerminate()
   {
      logger.Log( "ServerConnection::ProcessTerminate-" );

      // will make sure the socket is closed and call the handler's OnTerminate() function
      BasicConnection::ProcessTerminate();

      // tell the manager this connection has ended
      m_manager->OnTerminateConnection( GetRemoteEndpoint() );

      logger.Log( "ServerConnection::ProcessTerminate+" );
   }

   private:
      IConnectionManager* m_manager;
};

#endif // SERVER_CONNECTION_INCLUDED_