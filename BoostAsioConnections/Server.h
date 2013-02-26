#ifndef SERVER_INCLUDED_
#define SERVER_INCLUDED_ 1

#include "ServerConnection.h"

// A templated class that works as a TCP server. The server listens on a specified local endpoint
// for connection attempts.  When it accepts a socket connection it creates an object of type
// SERVER_CONNECTION, which must be ServerConnection or a class derived from ServerConnection.
// It passes the socket to that connection and starts the connection, which then does whatever
// it is that those connections are designed to do.
//
// The Server keeps a map of all connections, using the remote endpoint as key. When an entry is deleted
// from the map that causes the connection's destructor to be called, which closes its socket and 
// ends its thread.
template < typename SERVER_CONNECTION >
class Server : IConnectionManager
{
public:

   // Run the server on the specified IP and port number. To use IP_ANY for the IP address
// leave strIP empty or specify it as "0.0.0.0"
   Server( std::string strIp, unsigned short nPort, IConnectionHandler* handler = NULL );

   virtual ~Server()
   {
      Stop();
   }

   // Call this exactly once after the ctor to start the thread for the socket acceptor
   void Start();

   // Generally this should only be called by the dtor. However, it might be needed in some special
   // circumstances. Once Stop() is called you can't use this object anymore.
   void Stop();

   // Called to asynchronously write the string to the specified endpoint. If the endpoint is
   // not in m_connections it won't be sent.
   void Write( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& strOut );

   // Get a pointer to the connection to the specified endpoint (if it exists)
   boost::shared_ptr<SERVER_CONNECTION> GetConnection( const boost::asio::ip::tcp::endpoint& endpoint );

   // delete an existing connection
   void DeleteConnection( const boost::asio::ip::tcp::endpoint& endpoint );

   // override of the IConnectionManager pure virtual function
   void OnTerminateConnection( const boost::asio::ip::tcp::endpoint& endpoint )
   {
      DeleteConnection( endpoint );
   }

   /////////////////////////////////////////
   // public accessor functions
   /////////////////////////////////////////
   // NOTE: If this server was started without specifying an IP address then the IP address returned here will be 
   // "0.0.0.0".  To find the IP address used by a particular connection, call that connection's GetLocalEndpoint() function.
   const boost::asio::ip::tcp::endpoint& GetListenEndpoint() const { return m_localEndpoint; }
   std::string GetListenIP() const { return m_localEndpoint.address().to_string(); }
   unsigned short GetListenPort() const { return m_localEndpoint.port(); }

protected:
	// typedef and member declaration for our list of connections.
	typedef std::map< boost::asio::ip::tcp::endpoint, boost::shared_ptr<SERVER_CONNECTION> > ConnectionList;

private:

   // not implemented - copying is not allowed
   Server( const Server& );

   void accept_handler(const boost::system::error_code &ec);

   // Called asynchronously to delete a connection to the specified endpoint. If a connection doesn't exist nothing happens.
   void delete_connection_handler( const boost::asio::ip::tcp::endpoint& endpoint );

   void invoke_write_handler( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& strOut );

   // Run_io_service will be running on m_thread
   void Run_io_service();


   // data members
   std::auto_ptr<boost::thread> m_thread;
   std::auto_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
   const boost::asio::ip::tcp::endpoint m_localEndpoint;
   boost::asio::io_service m_io_service;

   // This can be null or not, we don't care. If not null then all connections will use it for their
   // handler, otherwise they must get their handler another way.
   IConnectionHandler* const m_handler;

   // things we create and then pass to a new connection when we accept it
   std::auto_ptr<boost::asio::ip::tcp::endpoint> m_clientEndpoint;
   std::auto_ptr<boost::asio::ip::tcp::socket> m_clientSock;
   std::auto_ptr<boost::asio::io_service> m_client_io_service;
   
   // VERY IMPORTANT! All access to m_connections MUST be done on m_thread if it is running EXCEPT for when the
   // list is cleared after m_thread has exited. (The connections themselves can be accessed on any thread, just the
   // container is at issue here.) If for some reason another thread needs access to m_connections then a lock
   // must be added and used everywhere.   
   ConnectionList m_connections;
};


// Run the server on the specified IP and port number. To use IP_ANY for the IP address
// leave strIP empty or specify it as "0.0.0.0"
template < typename SERVER_CONNECTION >
Server<SERVER_CONNECTION>::Server( std::string strIp, unsigned short nPort, IConnectionHandler* handler )
: m_handler(handler)
{
   logger.Log( "Server::Server" );
   if ( strIp.empty() )
      strIp = "0.0.0.0";

   // if you hit a compile error here then it means SERVER_CONNECTION isn't ServerConnection
   // or a class derived from ServerConnection
   SERVER_CONNECTION* compileCheckDerived = 0;
   ServerConnection* compileCheckBase = compileCheckDerived;

   // create an endpoint for the specified IP and port
   boost::asio::ip::address addr = boost::asio::ip::address::from_string( strIp );
   const_cast<boost::asio::ip::tcp::endpoint&>( m_localEndpoint )= boost::asio::ip::tcp::endpoint(addr, nPort);
}

// Call this exactly once after the ctor to start the thread for the socket acceptor
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::Start()
{
   logger.Log( "Server::Start");
   // create a thread to run io_service on.
   m_thread.reset( new boost::thread(&Server::Run_io_service, this) );
}


// Generally this should be called by the dtor. However, it might be needed in some 
// circumstances. Once Stop() is called you can't use this object anymore.
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::Stop()
{
   logger.Log( "Server::Stop");

   // stop the io_service and wait for the thread to end
   m_io_service.stop();
   m_thread->join();

   // Get rid of all the connections, causing them to close sockets, end their threads, then be deleted.
   // Since m_thread has already ended thread safety isn't an issue.
   m_connections.clear();
}


// Called to asynchronously write the string to the specified endpoint. If the endpoint is
// not in m_connections it won't be sent.
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::Write( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& strOut )
{
   logger.Log( "Server::Write" );

   m_io_service.dispatch( boost::bind( &Server::invoke_write_handler, this, endpoint, strOut ) );
}

template < typename SERVER_CONNECTION >
boost::shared_ptr<SERVER_CONNECTION> Server<SERVER_CONNECTION>::GetConnection( const boost::asio::ip::tcp::endpoint& endpoint )
{
	boost::shared_ptr<SERVER_CONNECTION> connection;        
	ConnectionList::iterator it = m_connections.find( endpoint );
	if( it != m_connections.end() )
	{
		connection = it->second;
	}
	return connection;
}

// delete an existing connection
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::DeleteConnection( const boost::asio::ip::tcp::endpoint& endpoint )
{
   // Post the command to the server's own thread, instead of the thread DeleteConnection was called on.
   // (Note that delete_connection_handler() gets a copy of endpoint even though this function and 
   // delete_connection_handler() pass by reference - important since it is accessed on another thread.)
   m_io_service.post( boost::bind( &Server::delete_connection_handler, this, endpoint ) );
}


// private member
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::accept_handler(const boost::system::error_code &ec)
{
   logger.Log( "Server::accept_handler-", &ec);

   // These are the socket, endpoint and io_service that will be used to communicate over the new 
   // connection. They must exist.
   assert( m_clientSock.get() );
   assert( m_clientEndpoint.get() );
   assert( m_client_io_service.get() );

   if ( !ec )      
   {
      // Create a new SERVER_CONNECTION and give it the things it needs to run. 
      boost::shared_ptr<SERVER_CONNECTION> newConnection( new SERVER_CONNECTION( m_clientSock, m_client_io_service, *m_clientEndpoint, this ) );

      // Insert the connection into m_connections. Use the array notation rather than insert() for the vanishingly small 
      // case where we have an existing connection with the same endpoint. (At the socket level that's nearly impossible, 
      // but more possible we might take too long to get rid of the entry for that (now closed) socket in our map.)
      m_connections[*m_clientEndpoint] = newConnection;      

      // Tell the connection to start. If m_handler for the connection so the connection will take care of that itself.
      newConnection->Start(m_handler);
   }
   else
   {
      // accept didn't work for some reason, so wait a very brief time (to avoid sucking up the whole CPU) then try again 
      // TODO - delay time should be a variable)
      boost::asio::deadline_timer timer(m_io_service, boost::posix_time::milliseconds(100));
      timer.wait(boost::system::error_code());
   }
   logger.Log( "Server::accept_handler 2", &ec);

   // create a socket and io_service to use for the next connection
   m_client_io_service.reset( new boost::asio::io_service );
   m_clientSock.reset( new boost::asio::ip::tcp::socket(*m_client_io_service) );

   // create an endpoint to use for the next connection
   m_clientEndpoint.reset( new boost::asio::ip::tcp::endpoint );

   // call async_accept
   m_acceptor->async_accept( *m_clientSock, *m_clientEndpoint, boost::bind( &Server::accept_handler, this, _1 ) );

   logger.Log( "Server::accept_handler+", &ec);

}

// private member
// Called asynchronously to delete a connection to the specified endpoint. If a connection doesn't exist nothing happens.
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::delete_connection_handler( const boost::asio::ip::tcp::endpoint& endpoint )
{
   m_connections.erase( endpoint );
}

// private member
// Called asynchronously to write the string to the specified endpoint. If the endpoint is
// not in m_connections it won't be sent.
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::invoke_write_handler( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& strOut )
{   
   ConnectionList::const_iterator iter = m_connections.find( endpoint );
   if ( iter != m_connections.end() )
   {
      boost::shared_ptr<SERVER_CONNECTION> pConnection = iter->second;

      pConnection->AsyncWrite( strOut );
   }
}

// private member
// Run_io_service will be running on m_thread
template < typename SERVER_CONNECTION >
void Server<SERVER_CONNECTION>::Run_io_service()
{
   // TODO - this will throw if the endpoint has an IP address that doesn't belong to this machine. 
   // How, in general, to handle exceptions?
   m_acceptor.reset( new boost::asio::ip::tcp::acceptor(m_io_service, m_localEndpoint, false) ); 

   // TODO - this will throw if the specified endpoint is already in use.
   boost::system::error_code ec;
   m_acceptor->listen( m_acceptor->max_connections, ec );
   boost::asio::ip::tcp::endpoint huh = m_acceptor->local_endpoint();
   assert( !ec );

   // create a socket and io_service to use for the next connection
   m_client_io_service.reset( new boost::asio::io_service );
   m_clientSock.reset( new boost::asio::ip::tcp::socket(*m_client_io_service) );

   // create an endpoint to use for the next connection
   m_clientEndpoint.reset( new boost::asio::ip::tcp::endpoint );

   // call  async_accept
   m_acceptor->async_accept( *m_clientSock, *m_clientEndpoint, boost::bind( &Server::accept_handler, this, _1 ) );

   m_io_service.run();

   m_acceptor.reset();

   m_clientSock.reset();
}

#endif // SERVER_INCLUDED_