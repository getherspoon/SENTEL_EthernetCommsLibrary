#ifndef SYNC_WRITE_CONNECTION_INCLUDED_
#define SYNC_WRITE_CONNECTION_INCLUDED_ 1

#include "BasicConnection.h"


// See the comments for BasicConnection.
//
// The intended usage life-cycle is for the user to instantiate the object, then call its SyncWrite() method (in the BasicConnection
// base class) one or more times to write data to the socket. The user could also call AsyncWrite(). There is a single instance of
// a IConnectionHandler class used by all instances of SyncWriteConnection. This instance is static so that it is guaranteed to
// still exist when any callbacks are made by BasicConnection. Since the purpose of this class is just to be a simple way to allow  
// the user to write data over a socket, we aren't concerned about the data provided by the callbacks. 
// 
// The connection attempt is made synchronously, so this class should not be used where a possible prolonged delay is unacceptable.
class SyncWriteConnection : public BasicConnection<IConnectionHandler> 
{
public:

   // strIp - the remote IP to connect to
   // nPort - the remote port number to connect to
   SyncWriteConnection( const std::string& strIp, unsigned short nPort, boost::system::error_code& ec ) //: BasicConnection<IConnectionHandler>()
   {
      logger.Log( "SyncWriteConnection::SyncWriteConnection-");

      // A static instance of the Handler class that will be used by all instances of SyncWriteConnection.
      static Handler handler;
      SetConnectionHandler(&handler);

      // create an endpoint for this IP and port
      boost::asio::ip::address addr = boost::asio::ip::address::from_string( strIp, ec );
      if ( !ec )
      {
         SetRemoteEndpoint( boost::asio::ip::tcp::endpoint(addr, nPort) );

         // create an io_service
         std::auto_ptr<boost::asio::io_service> io_service( new boost::asio::io_service );
         Set_io_service( io_service );

         // create a socket to use for comms
         std::auto_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(Get_io_service()) );

         // Make an SYNCHRONOUS call to connect the socket. This error code could be provided via the ctor or some function member if needed.
         sock->connect( GetRemoteEndpoint(), ec );

         if ( !ec )
         {
            boost::asio::socket_base::linger option(true, 10);
            sock->set_option(option, ec);

            // give the socket away
            SetSocket( sock );

            SetLocalEndpoint();

            if ( !ec )
            {
               // no use starting the io_service if we didn't connect
               BasicConnection::Start();
            }
         }
      }
      logger.Log( "SyncWriteConnection::SyncWriteConnection+");
   }

private:

   virtual void ProcessNewConnection()
   {
      // will make an async call to new_connection_handler() 
      BasicConnection::ProcessNewConnection();

      // will make an async call to begin reading
      BeginNewAsyncRead();
   }

   // none of these base class methods make sense in this derived class. Make them private so they can't be called.
   virtual bool IsConnected() { return false; };
   virtual unsigned short GetOnIdleTimeout( bool* bValid = 0 ) { return 0; };
   virtual void SetOnIdleTimeout( unsigned short nIdleTimeoutSec ) {};

   // A class to implement the IConnectionHandler interface for ALL instances of SyncWriteConnection. See the comment in 
   // the SyncWriteConnection constructor.
   class Handler : public IConnectionHandler
   {
   public: 
      virtual void OnNewConnection( const boost::asio::ip::tcp::endpoint& endpoint, ConnectionBase& connection ) {};
      virtual void OnDisconnect( const boost::asio::ip::tcp::endpoint& endpoint ) {};
      virtual void OnTerminate( const boost::asio::ip::tcp::endpoint& endpoint ) {};

      // since the reconnect timeout given to ClientConnection is 0, this should never be called
      virtual bool OnIdleTimeout( const boost::asio::ip::tcp::endpoint& endpoint, time_t tCurIdleTime ) { return false; };

      // We're not going to be doing anything with any messages that we do receive, but have to specify some buffer size
      virtual std::size_t GetMaxMessageSize( const boost::asio::ip::tcp::endpoint& endpoint ) { return 100; };

      virtual bool OnRead( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& buf, std::size_t& nBytesAvailable, std::size_t& nBytesConsumed ) 
      { 
         // say we consumed all the bytes and return true.
         nBytesConsumed = nBytesAvailable;
         return true;
      }
   };

};



#endif // SYNC_WRITE_CONNECTION_INCLUDED_