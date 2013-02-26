#ifndef I_CONNECTION_HANDLER_INCLUDED
#define I_CONNECTION_HANDLER_INCLUDED 1

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif

#include <boost/asio.hpp> 
#include <boost/thread.hpp>
#include <ctime>
#include <assert.h>

// A base class for all connection objects. Its purpose is to allow RTTI to work for different connection types - otherwise
// the types wouldn't be related.
class ConnectionBase
{
public:
   virtual ~ConnectionBase() {};
};


// The "On" functions are callback methods. Those methods defined in IConnectionHandler will run on the comm thread, 
// so the developer must consider thread safety for any objects they access.
// 
// The other two functions are called prior to starting the comm thread.
class IConnectionHandler
{
public:
   
   virtual ~IConnectionHandler() {};

   // Called immediately after the io_service thread begins. The void pointer is a pointer to the connection object that is calling the function. 
   // It will have to be cast on the other side to be of any use.
   virtual void OnNewConnection( const boost::asio::ip::tcp::endpoint& endpoint, ConnectionBase& connection ) = 0;

   // called whenever there is a error on the socket or the socket is intentionally closed by either side.
   virtual void OnDisconnect( const boost::asio::ip::tcp::endpoint& endpoint ) = 0;

   // called after any bytes are read from the socket
   virtual bool OnRead( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& buf, std::size_t& nBytesAvailable, std::size_t& nBytesConsumed ) = 0;

   // called when the socket level connection is open but has been idle for a specified period of time
   virtual bool OnIdleTimeout( const boost::asio::ip::tcp::endpoint& endpoint, time_t tCurIdleTime ) = 0;

   // called when the connection thread ends
   virtual void OnTerminate( const boost::asio::ip::tcp::endpoint& endpoint ) = 0;

   // return the maximum size of any valid message that will be read from the remote endpoint
   virtual std::size_t GetMaxMessageSize( const boost::asio::ip::tcp::endpoint& endpoint ) const = 0;

   //called after any bytes are sent on the socket
   virtual bool OnWrite( const boost::system::error_code &ec, std::size_t& nBytesSent) = 0;
	
};

#endif