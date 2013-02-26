#ifndef CLIENT_CONNECTION_HANDLER_INCLUDED_
#define CLIENT_CONNECTION_HANDLER_INCLUDED_ 1

#include "IConnectionHandler.h"

// See comments for IConnectionHandler
class IClientConnectionHandler : public IConnectionHandler
{
public:
   // called when the socket level connection is established (either initially made or when it is re-established)
   virtual void OnConnect( const boost::asio::ip::tcp::endpoint& endpoint ) = 0;

   // called when the socket level connection attempt fails (either initially or failed attempts to re-establish it)
   virtual void OnConnectFailed( const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code &ec ) = 0;
};

#endif // CLIENT_CONNECTION_HANDLER_INCLUDED_