#ifndef BASIC_CONNECTION_INCLUDED_
#define BASIC_CONNECTION_INCLUDED_ 1

#include "IClientConnectionHandler.h"

#include "TempLogger.h"

// A templated base class representing a connection to a remote endpoint. Intended to be used as a base class for
// a ClientConnection class and ServerConnection class. The difference between those classes is how the socket
// is created and what happens when the socket connection is closed. However, this class deals with the socket
// between those events.
//
// CONNECTION_HANDLER is IConnectionHandler or an interface or class derived from IConnectionHandler. 
// The m_handler pointer is a pointer to a class that implements that interface, and must be set by the derived class.
// 
// Member functions that use the C++ standard convention (lower case with underscores) are "handler" functions. These are
// all called asynchronously through m_io_service and will execute on m_thread. They are all private.
//
// The public methods are all thread safe.
//
// Be sure to read the comments to use the protected functions in derived classes

template < typename CONNECTION_HANDLER >
class BasicConnection : public ConnectionBase
{
public:

   BasicConnection() : m_handler(0), m_nIdleTimeoutSec(0), m_tLastSuccessTime(0), m_bDerivedIsHandler(false)
   {
      logger.Log( "BasicConnection ctor" );
      // This is a compile time check. If this line doesn't compile then CONNECTION_HANDLER
      // is not IConnectionHandler or an interface or class derived from IConnectionHandler.
      IConnectionHandler* compileCheck = m_handler;
      
   }

   virtual ~BasicConnection() 
   {
      // if m_bDerivedIsHandler then the derived class implemented IConnectionHandler.  However, at this point the
      // derived object doesn't exist anymore, so its implementations of IConnectionHandler methods are gone.  We have
      // to re-assign m_handler to an object that does implement IConnectionHandler methods in order to prevent purecalls
      // during Stop()
      std::auto_ptr<CONNECTION_HANDLER> tmpNoOpHandler;
      if ( m_bDerivedIsHandler )
      {
         tmpNoOpHandler.reset( new CNoOpHandler );
         const_cast<CONNECTION_HANDLER*&>(m_handler) = tmpNoOpHandler.get();
      }

      // will block until the comm thread ends
      Stop();      
   }

   /////////////////////////////////////////
   // public accessor functions
   /////////////////////////////////////////
   const boost::asio::ip::tcp::endpoint& GetRemoteEndpoint() const { return m_remoteEndpoint; }
   std::string GetRemoteIP() const { return m_remoteEndpoint.address().to_string(); }
   unsigned short GetRemotePort() const { return m_remoteEndpoint.port(); }
   const boost::asio::ip::tcp::endpoint& GetLocalEndpoint() const {  boost::mutex::scoped_lock( m_cs ); return m_localEndpoint; }
   virtual bool IsConnected();

   // get the idle timeout that is being used by the connection
   //virtual unsigned short GetOnIdleTimeout( bool* bValid = 0 );

   // set the idle timeout that is being used by the connection
   virtual void SetOnIdleTimeout( unsigned short nIdleTimeoutSec );
   /////////////////////////////////////////

   // Asynchronously write data over the socket. 
   // Can be called on any thread.
   void AsyncWrite( const std::string& strOut );

   // Synchronously write data over the socket. Returns the number of bytes written.
   // Can be called on any thread, HOWEVER since the data is sent synchronously, this class should not be used 
   // where a possible prolonged delay is unacceptable.
   std::size_t SyncWrite( const std::string& strOut );
   std::size_t SyncWrite( const std::string& strOut, boost::system::error_code& ec );


protected:

   /////////////////////////////////////////
   // protected accessor functions
   /////////////////////////////////////////
   CONNECTION_HANDLER& GetConnectionHandler() { assert( m_handler ); return *m_handler; }
   boost::asio::io_service& Get_io_service() { assert( m_io_service.get() ); return *m_io_service; }
   const boost::asio::ip::tcp::socket& GetSocket() const { assert( m_sock.get() ); return *m_sock; }
   boost::asio::ip::tcp::socket& GetSocket() { assert( m_sock.get() ); return *m_sock; }

   void UpdateLastSuccessTime() { boost::mutex::scoped_lock( m_cs ); m_tLastSuccessTime = time(0); }
   /////////////////////////////////////////

   // ProcessNewConnection() is called on the main thread just before m_thread is started.
   // It is used to do a post so that the first _handler callback will be to new_connection_handler().
   // It must be overridden to also post some other command to m_io_service or else the
   // thread will start, call new_connection_handler(), call ProcessTerminate() and then exit.
   // Overrides must call this base class implementation before doing anything else.
   virtual void ProcessNewConnection()
   {
   logger.Log( "ProcessNewConnection" );

      // will cause GetConnectionHandler().OnNewConnection() to be called on m_thread
      m_io_service->post( boost::bind( &BasicConnection::new_connection_handler, this ) );
   }

   // Called whenever there is an error on the socket or the socket is closed.
   // Will make sure the socket is closed and call the handler's OnDisconnect() function.
   // Any override must call this base class implementation before doing anything else.
   virtual bool ProcessDisconnect( const boost::system::error_code& ec ) 
   { 
      logger.Log( "ProcessDisconnect", &ec );

      bool bDoDisconnect = IsConnected();

      if ( bDoDisconnect )
      {
         if ( m_sock.get() )
         {
            logger.Log( "ProcessDisconnect CLOSE");
            boost::system::error_code closeEC;
            m_sock->close( closeEC );
         }

         ClearLastSuccessTime();
         GetConnectionHandler().OnDisconnect(m_remoteEndpoint);   
      }

      logger.Log( "ProcessDisconnect+");
      return bDoDisconnect;
   }   

   // Called whenever the thread is ending.
   // Will make sure the socket is closed and call the handler's OnTerminate() function.
   // Any override must call this base class implementation before doing anything else.
   virtual void ProcessTerminate() 
   { 
      logger.Log( "ProcessTerminate-" );

      if ( m_sock.get() )
      {
         logger.Log( "ProcessTerminate CLOSE");
         boost::system::error_code ec;
         m_sock->close( ec );
      }

      GetConnectionHandler().OnTerminate(m_remoteEndpoint); 

      logger.Log( "ProcessTerminate+" );
   }   

   // this must be called exactly once, and must be called before SetSocket()
   void Set_io_service( std::auto_ptr<boost::asio::io_service> io_service );

   // this must be called exactly once, and must be called before Start()
   void SetSocket( std::auto_ptr<boost::asio::ip::tcp::socket> sock );

   // this must be called exactly once, and must be called before Start()
   void SetRemoteEndpoint( const boost::asio::ip::tcp::endpoint& endpoint );      

   bool SetLocalEndpoint();

   // this must be called exactly once, and must be called before Start() or it will be called by Start()
   void SetConnectionHandler( CONNECTION_HANDLER* handler );

   void Start();

   // Generally this should only be called by the dtor. However, it might be needed in some special
   // circumstances. Once Stop() is called you can't use this object anymore.
   void Stop();

   // Stop the io_service but DO NOT wait for the thread to end. This can be called from within a handler
   // function (i.e., running on m_thread) to cause the io_service to stop running and ProcessTerminate() will
   // be called.
   void AsyncStop();

   // clear anything currently in the buffer and start an async read cycle
   void BeginNewAsyncRead();


private:

   // not implemented - copying is not allowed
   BasicConnection( const BasicConnection& );

   // Run_io_service will be running on m_thread
   void Run_io_service();

   // called asynchronously to set the timout used to check whether the connection has been used recently
   // and re-open it. 
   void set_idle_connection_timeout(unsigned short nIdleTimeoutSec);

   // called asynchronously to check whether the connection has been used recently, and if not then to close
   // and re-open it. 
   void idle_connection_handler(const boost::system::error_code &ec);

   // Called asynchronously when m_thread begins running to let the handler know there is a new BasicConnection.
   // Note that this does not necessarily mean that a socket level connection has successfully been established,
   // or that if a socket level connection was established it hasn't already closed, just that there is a new
   // BasicConnection object for talking to that endpoint. This will be called exactly once per BasicConnection
   // (unless the thread is never started, in which case it will never be called).
   void new_connection_handler()
   {
      logger.Log( "new_connection_handler" );
      GetConnectionHandler().OnNewConnection(m_remoteEndpoint, *this);
   }

   // called asynchronously after an attempt is made to read data
   void read_handler(const boost::system::error_code &ec, std::size_t nOffset, std::size_t nBytesTransferred);

   // called asynchronously after SyncWrite() is called
   void sync_write_handler(bool bSucceeded);

   // called asynchronously after AsyncWrite() is called
   void async_write_handler( const std::string& strOut );

   void ClearLastSuccessTime() {  boost::mutex::scoped_lock( m_cs ); m_tLastSuccessTime = 0; }

   time_t GetLastSuccessTime() const {  boost::mutex::scoped_lock( m_cs ); return m_tLastSuccessTime; }

   ///////////////////////////////////////
   // private data members
   std::auto_ptr<boost::thread> m_thread;
   std::string m_buffer;
   unsigned short m_nIdleTimeoutSec;
   std::auto_ptr<boost::asio::deadline_timer> m_idleTimer;
   bool m_bDerivedIsHandler;

   // members that must be set by derived classes
   std::auto_ptr<boost::asio::io_service> m_io_service;
   std::auto_ptr<boost::asio::ip::tcp::socket> m_sock;
   CONNECTION_HANDLER* const m_handler;
   const boost::asio::ip::tcp::endpoint m_remoteEndpoint;
   bool m_bLocalEndpointSet;

   // the following private members can be accessed from any thread and so must be protected
   mutable boost::mutex m_cs;
   boost::asio::ip::tcp::endpoint m_localEndpoint;
   time_t m_tLastSuccessTime;

   // Private class used only in ~BasicConnection() and only when the class derived from BasicConnection implements
   // IConnectionHandler. All methods are no-ops. The only ones that we think should ever be called are OnDisconnect() 
   // and OnTerminate(). If we find that others wind up being called we need to make sure the no-op behavior is ok, then remove the assert.
   class CNoOpHandler : public IClientConnectionHandler
   {
      // IClientConnectionHandler methods.
      virtual void OnConnect( const boost::asio::ip::tcp::endpoint& endpoint ) { assert(0); }
      virtual void OnConnectFailed( const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code &ec ) { assert(0); }

      // IConnectionHandler methods. 
      virtual void OnNewConnection( const boost::asio::ip::tcp::endpoint& endpoint, ConnectionBase& connection ) { assert(0); }
      virtual void OnDisconnect( const boost::asio::ip::tcp::endpoint& endpoint ) {}  // No assert - ok if this is called here
      virtual bool OnRead( const boost::asio::ip::tcp::endpoint& endpoint, const std::string& buf, std::size_t& nBytesAvailable, std::size_t& nBytesConsumed ) { assert(0); return false; }
      virtual bool OnIdleTimeout( const boost::asio::ip::tcp::endpoint& endpoint, time_t tCurIdleTime ) { assert(0); return false; }
      virtual void OnTerminate( const boost::asio::ip::tcp::endpoint& endpoint ) {}; // No assert - ok if this is called here
      virtual std::size_t GetMaxMessageSize( const boost::asio::ip::tcp::endpoint& endpoint ) const { assert(0); return 0; }
	  virtual bool OnWrite( const boost::system::error_code &ec, std::size_t& nBytesSent) {assert(0); return 0; }
	
   };
};

// Asynchronously write data over the socket. Actually makes an asynchronous call to SyncWrite(), 
// Can be called on any thread
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::AsyncWrite( const std::string& strOut )
{  
   logger.Log( "AsyncWrite" );
   m_io_service->post( boost::bind( &BasicConnection::async_write_handler, this, strOut ) );
}



// Synchronously write data over the socket. Returns the number of bytes written.
// Can be called on any thread
template <typename CONNECTION_HANDLER>
std::size_t BasicConnection<CONNECTION_HANDLER>::SyncWrite( const std::string& strOut )
{  
   boost::system::error_code ec;
   return SyncWrite( strOut, ec );
}

// Synchronously write data over the socket. Returns the number of bytes written.
// Can be called on any thread
template <typename CONNECTION_HANDLER>
std::size_t BasicConnection<CONNECTION_HANDLER>::SyncWrite( const std::string& strOut, boost::system::error_code& ec )
{  
   logger.Log( "SyncWrite-", &ec );

   // can't call this after it has been stopped.
   std::size_t nWritten = GetSocket().send( boost::asio::buffer(strOut), 0, ec );
   boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );

   if ( !ec )
   {
      // according to boost documentation write() shouldn't return until all bytes are written
      // or until an error occurs
      assert(nWritten == strOut.size() );

      // We wrote the data, so the connection is up. Make an async call to sync_write_handler(true)
      m_io_service->post( boost::bind( &BasicConnection::sync_write_handler, this, true ) );
   }
   else
   {
      // according to boost documentation write() shouldn't return until all bytes are written
      // or until an error occurs
      assert( nWritten != strOut.size() );

      // write() failed, so something is wrong. Make an async call to sync_write_handler(false)
      m_io_service->post( boost::bind( &BasicConnection::sync_write_handler, this, false ) );
   }
   logger.Log( "SyncWrite+", &ec );

   return nWritten;
}

// find out whether the state of this object is "connected."  Connected here doesn't necessarily mean that the socket level
// connection is open at this exact time - it means that this object thinks that the socket level connection is open. So 
// difference is that m_socket->open() might return a different value (however m_socket->open() was seen to return true
// when m_socket never even connected to the remote endpoint... not sure why).
template <typename CONNECTION_HANDLER>
bool BasicConnection<CONNECTION_HANDLER>::IsConnected()  
{ 
    boost::mutex::scoped_lock( m_cs );
   return m_tLastSuccessTime != 0; 
}


// set the idle timeout that is being used by the connection
// Can be called anytime, but if called before the thread and io_service are running then the value won't actually 
// be set until they are running.
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::SetOnIdleTimeout( unsigned short nIdleTimeoutSec )
{   
   Get_io_service().post( boost::bind( &BasicConnection::set_idle_connection_timeout, this, nIdleTimeoutSec ) );
}

// protected member
// this must be called exactly once, and must be called before SetSocket()
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::Set_io_service( std::auto_ptr<boost::asio::io_service> io_service ) 
{ 
   assert( io_service.get() );
   assert( !m_io_service.get() ); 
   m_io_service = io_service; 
}

// protected member
// this must be called exactly once, and must be called before Start()
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::SetSocket( std::auto_ptr<boost::asio::ip::tcp::socket> sock ) 
{ 
   assert( sock.get() );
   assert( !m_sock.get() ); 
   assert( m_io_service.get() );
   assert( &sock->get_io_service() == m_io_service.get() );
   m_sock = sock;
}

// protected member
// this must be called exactly once, and must be called before Start()
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::SetRemoteEndpoint( const boost::asio::ip::tcp::endpoint& endpoint ) 
{
   assert( endpoint != boost::asio::ip::tcp::endpoint() );
   assert( m_remoteEndpoint == boost::asio::ip::tcp::endpoint() );
   const_cast<boost::asio::ip::tcp::endpoint&>(m_remoteEndpoint) = endpoint;
}

// protected member
// should be called by the derived class after a socket level connection is established. If the connection is closed
// and re-established it should be called again as both the local IP and port could have changed.
template <typename CONNECTION_HANDLER>
bool BasicConnection<CONNECTION_HANDLER>::SetLocalEndpoint()
{
   bool bSuccess = false;

   boost::mutex::scoped_lock( m_cs );
   sockaddr name;
   socklen_t namelen = sizeof(name);
   if ( ::getsockname( GetSocket().native_handle(), &name, &namelen ) == 0 )
   {
      // code only works with IPV4
      assert( name.sa_family == AF_INET );

      // cast the name structure since it is IPV4
      sockaddr_in& name_in = reinterpret_cast<sockaddr_in&>( name );

      // get the IP address and the port number
      std::string strIp( ::inet_ntoa(name_in.sin_addr) );
      unsigned short sPort = name_in.sin_port;

      boost::system::error_code ec;
      boost::asio::ip::address addr = boost::asio::ip::address::from_string( strIp, ec );

      assert( !ec ); // should never happen

      m_localEndpoint =  boost::asio::ip::tcp::endpoint( addr, sPort );      
      bSuccess = true;
   }

   return bSuccess;
}        


// protected member
// This must be called exactly once, and must be called before Start() or it will be called by Start().
// The handler can be any object of type CONNECTION_HANDLER. If this is a class derived from BasicConnection<CONNECTION_HANDLER>
// and CONNECTION_HANDLER then you can pass either 'this', null or no parameter. In the latter two cases
// the function will do a dynamic cast to get 'this'.
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::SetConnectionHandler( CONNECTION_HANDLER* handler = 0 )
{
   logger.Log( "SetConnectionHandler");

   // make sure this is only called once and that the connection hasn't already been started
   assert( !m_handler );
   assert( !m_thread.get() );

   const_cast<CONNECTION_HANDLER*&>(m_handler) = handler;

   if ( !m_handler )
   {
      // see if this object must be its own CONNECTION_HANDLER.
      const_cast<CONNECTION_HANDLER*&>(m_handler) = dynamic_cast<CONNECTION_HANDLER*>(this);

      // Either SetConnectionHandler() must be called with a non-null handler or 
      // this object must be its own CONNECTION_HANDLER
      assert( m_handler );

      // we need to know this in the destructor
      m_bDerivedIsHandler = true;
   }
}

// protected member
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::Start()
{
   std::string strLog = "BasicConnection<CONNECTION_HANDLER>::Start ";
   strLog += GetRemoteEndpoint().address().to_string();
   logger.Log( strLog );

   // make sure this connection hasn't already been started
   assert( !m_thread.get() );

   if ( !m_handler )
      SetConnectionHandler(0);

   // these have to be set before Start can be called
   assert( m_handler );
   assert( m_sock.get() );
   assert( m_io_service.get() );
   assert( m_remoteEndpoint != boost::asio::ip::tcp::endpoint() );

   // m_sock has to use m_io_service
   assert( &GetSocket().get_io_service() == m_io_service.get() );

   // the buffer needs to be as large as the largest possible message
   // (with a minimum of 1 just to prevent stupidity and needless error checking)
   m_buffer.resize( std::max<std::size_t>( 1, GetConnectionHandler().GetMaxMessageSize(m_remoteEndpoint) ) );

   ProcessNewConnection();

   logger.Log( "BasicConnection<CONNECTION_HANDLER>::Start SPAWN");

   // create a thread to run io_service on.
   m_thread.reset( new boost::thread(&BasicConnection::Run_io_service, this) );

   // TODO - should wait here until the m_io_service is running. but polling isn't safe and is a kludge
   // Use a signal? Has to be bulletproof.
   // TODO - there are places that check m_thread.get() with the implicit assumption that that means
   // that m_io_service is running, but there is at least a little time after the thread is created before
   // m_io_service is started, and some time after it is stopped before the thread ends.
}

// protected member
// Generally this should only be called by the dtor. However, it might be needed in some special
// circumstances. Once Stop() is called you can't use this object anymore.
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::Stop()
{
   logger.Log( "BasicConnection<CONNECTION_HANDLER>::Stop-");

   // Get rid of the idle timer before ending the thread. If there is a timer waiting when the thread ends it
   // causes a crash.
   m_idleTimer.reset();

   // io_service won't exist if Set_io_service() wasn't called
   if ( m_io_service.get() )
   {
      if ( m_sock.get() )
      {
         logger.Log( "BasicConnection<CONNECTION_HANDLER>::Stop CLOSE");
         boost::system::error_code ec;
         m_sock->close( ec );
      }

      // stop the io_service and wait for the thread to end
      m_io_service->stop();

      // thread might not exist if Start() was never called
      if ( m_thread.get() )
      {
         // We told m_io_service to stop above. that should cause Run_io_service() to end and then m_thread
         // should exit. Here we wait for that to happen.
         m_thread->join();
      }
   }

   logger.Log( "BasicConnection<CONNECTION_HANDLER>::Stop+");

   // If this isn't done here then there is a deadlock and/or memory exception when the
   // socket is deleted by continuing ~ClientConnection() because, by experimentation,
   // the socket can't be deleted before the io_service. 
   m_sock.reset();
}

// protected member
// Stop the io_service but DO NOT wait for the thread to end. This can be called from within a handler
// function (i.e., running on m_thread) to cause the io_service to stop running and ProcessTerminate() will
// be called.
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::AsyncStop()
{
   logger.Log( "BasicConnection<CONNECTION_HANDLER>::AsyncStop");

   // Don't explicitly close the socket here before closing the service. That would potentially cause something to happen
   // that would result in ProcessDisconnect() being called. But if it hasn't already been called for whatever reason, before
   // AsyncStop(), then we don't want it to get called now (it might try to reconnect or something).
   m_io_service->stop();
}


// private member
// called asynchronously to set the timout for idle_connection_handler()
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::set_idle_connection_timeout(unsigned short nIdleTimeoutSec)
{
   m_nIdleTimeoutSec = nIdleTimeoutSec;

   // Run_io_service() creates m_idleTimer immediately so that any callback function can be sure it exists
   assert( m_idleTimer.get() );

   // cancel any pending operations
   m_idleTimer->cancel();

   if ( nIdleTimeoutSec != 0 )
   {
      // reset the idle timer to make sure idle_connection_handler() gets called. (If m_nIdleTimeoutSec was
      // previously 0 then it would never get called.)
      m_idleTimer->expires_from_now( boost::posix_time::seconds(nIdleTimeoutSec) );
      m_idleTimer->async_wait(boost::bind( &BasicConnection::idle_connection_handler, this, _1 ));
   }
}


// private member
// called asynchronously to check whether the connection has been used recently, and if not then to let the handler know
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::idle_connection_handler(const boost::system::error_code &ec)
{
   // ec can be set if the timer was cancelled. 
   if ( !ec )
   {
      // We don't do anything if m_nIdleTimeoutSec has been set to 0. It is possible for idle_connection_handler() 
      // to be called even if m_nIdleTimeoutSec is 0 if previously it was > 0 but then SetOnIdleTimeout(0) was called.
      // If that is the case just don't do anything and the idle_connection_handler() won't be called again.
      if ( m_nIdleTimeoutSec != 0 )
      {
         // reset the on_idle timer using the current m_nIdleTimeoutSec
         set_idle_connection_timeout(m_nIdleTimeoutSec);

         // We don't do anything if the socket isn't open. That is taken care of elsewhere. 
         if ( IsConnected() )
         {
            // Let the handler know how long the connection has been idle. 
            // If OnIdleTimeout() returns true then it means that the hanlder wants us to close and restart
            // the connection. False means do nothing (perhaps OnIdleTimeout() sent a keep alive or took other action).
            time_t curTime = time(0);
            time_t curIdleTime = curTime - GetLastSuccessTime();

            if ( GetConnectionHandler().OnIdleTimeout( m_remoteEndpoint, curIdleTime ) )
            {
               boost::system::error_code ec;
               ProcessDisconnect(ec);
            }
         }
      }
   }
   else
   {
      // for debugging - just to be able to tell when timeouts have been canceled.
      int x=10;
   }
}

// private member
// called asynchronously after SyncWrite() is called 
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::sync_write_handler(bool bSucceeded)
{
   logger.Log( "sync_write_handler-" );
   if ( bSucceeded )
   {
      UpdateLastSuccessTime();
      logger.Log( "sync_write_handler POS" );
   }
   else
   {
      boost::system::error_code ec;
      ProcessDisconnect(ec);
      logger.Log( "sync_write_handler FAIL" );
   }
   logger.Log( "sync_write_handler+" );
}

// private member
// called asynchronously after AsyncWrite() is called
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::async_write_handler( const std::string& strOut )
{
   logger.Log( "async_write_handler-" );

   if ( IsConnected() )
   {
      // can't call this after it has been stopped.
      boost::system::error_code ec;
      std::size_t nWritten = GetSocket().send( boost::asio::buffer(strOut), 0, ec );
	  GetConnectionHandler().OnWrite(ec, nWritten);
      if ( !ec )
      {
         // according to boost documentation write() shouldn't return until all bytes are written
         // or until an error occurs
         assert(nWritten == strOut.size() );

         // We wrote the data, so the connection is up. Make a synchronous call to UpdateLastSuccessTime()
         UpdateLastSuccessTime();
      }
      else
      {
         // according to boost documentation write() shouldn't return until all bytes are written
         // or until an error occurs
         assert( nWritten != strOut.size() );

         // write() failed, so something is wrong. Make a synchronous call to ProcessDisconnect(ec)
         ProcessDisconnect(ec);
      }
   }
   else
   {
      logger.Log( "async_write_handler DIS" );
   }

   logger.Log( "async_write_handler+" );
}

// private member
// called asynchronously after an attempt is made to read data
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::read_handler(const boost::system::error_code &ec, std::size_t nOffset, std::size_t nBytesTransferred) 
{ 
   logger.Log( "read_handler-", &ec );
   if (!ec) 
   { 
      assert( nBytesTransferred <= m_buffer.size() );

      // we read data, so the connection is up...
      UpdateLastSuccessTime();

      // nBytesTransferred were just read into m_buffer, starting at nOffset. If nOffset is non-zero
      // then it means that some bytes were left over from previous reads. Get the total bytes available.
      std::size_t nTotalBytes = nOffset + nBytesTransferred;

      // Call the OnRead() function to let the user get whatever data he wants (if any) from the buffer.
      // OnRead() should set nBytesConsumed to tell us how many bytes it copied from the buffer. We can
      // shift everything down to make more room at the end of the buffer.
      // If there is something wrong with the data such that it can never be processed then
      // the OnRead() handler should return false.
      // TODO - this could be implemented as a circular buffer to eliminate the shifts.
      size_t nBytesConsumed = 0;
	  std::size_t nAvailableBuffer;
      bool bValid = GetConnectionHandler().OnRead( m_remoteEndpoint, m_buffer, nTotalBytes, nBytesConsumed );

      if( bValid && nBytesConsumed > nTotalBytes )
      {
         // OnRead() said it consumed more bytes than were actually available
         assert(0); // just to alert the developer - won't cause a crash
         bValid = false;
      }

      if ( !bValid )
      {
         // the data in the buffer is invalid, so get rid of it all
		nTotalBytes = 0;
		//The following two lines are required to actually clear the buffer for the next read after OnRead has rejected it.
		//OnRead can contain multiple messages after pausing which may be incomplete messages that can negatively impact a parsing routine.
		//These lines will erase the string, and resize it to the max message size from the handler. 
		m_buffer.clear();
		m_buffer.resize( std::max<std::size_t>( 1, GetConnectionHandler().GetMaxMessageSize(m_remoteEndpoint) ) );	
      }
	  //All data was consumed on the read, clear the buffer. 
	  else if (nBytesTransferred == nBytesConsumed)
	  {
		nTotalBytes = 0;
		m_buffer.clear();
		m_buffer.resize( std::max<std::size_t>( 1, GetConnectionHandler().GetMaxMessageSize(m_remoteEndpoint) ) );	
	  }
	  else
      {
         // calculate the number of bytes that have been read into the buffer but weren't consumed by the caller
         std::size_t nBytesRemaining = nTotalBytes - nBytesConsumed;

         if ( nBytesRemaining == m_buffer.size() )
         {
            // The available buffer was full and OnRead() didn't consume any data, so we can't read any more data.
            // So clear the buffer so we can continue reading data - something is probably wrong with OnRead().
            nTotalBytes = 0;
			m_buffer.clear();
			m_buffer.resize( std::max<std::size_t>( 1, GetConnectionHandler().GetMaxMessageSize(m_remoteEndpoint) ) );	
         }
         else 
         {
            if ( nBytesConsumed > 0 && nBytesRemaining > 0 )
            {
               // Some but not all data was consumed. Shift the remaining bytes to the beginning of the buffer.
               assert( m_buffer.size() >= nBytesRemaining + nBytesConsumed );
               //m_buffer.replace( 0, nBytesRemaining, &m_buffer[nBytesConsumed] );
               m_buffer.replace( 0, nBytesRemaining, m_buffer, nBytesConsumed, nBytesRemaining );
            }

            nTotalBytes = nBytesRemaining;
         }

	  }

      // figure out how much room is available for reading
      nAvailableBuffer = m_buffer.size()-nTotalBytes;      
	  assert( nAvailableBuffer > 0 );

      // read some more
      GetSocket().async_read_some(boost::asio::buffer( &m_buffer[nTotalBytes], nAvailableBuffer ), 
         boost::bind( &BasicConnection::read_handler, this, _1, nTotalBytes, _2) ); 
   } 
   else
   {
      // read failed for some reason
      ProcessDisconnect(ec);
   }
   logger.Log( "read_handler+", &ec );
}          

// protected member
// clear anything currently in the buffer and start an async read cycle
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::BeginNewAsyncRead()
{
   logger.Log( "BeginNewAsyncRead-" );
   assert( m_sock.get() );

   // Do a read into m_buffer. Calling async_read_some() will cause read_hander() to get called, 
   // which in turn calls async_read_some() again, so from this point
   // reads will continue so long as read_handler() keeps calling async_read_some().
   std::size_t nMaxRead = std::min( m_buffer.size(), std::size_t(100) );
   GetSocket().async_read_some(boost::asio::buffer(&m_buffer[0], nMaxRead), 
      boost::bind( &BasicConnection::read_handler, this, _1, 0, _2) ); 
   logger.Log( "BeginNewAsyncRead+" );
}

// private member
// Run_io_service will be running on m_thread
template <typename CONNECTION_HANDLER>
void BasicConnection<CONNECTION_HANDLER>::Run_io_service()
{
   logger.Log( "Run_io_service-" );
   // construct the idleTimer, although it isn't used if m_nIdleTimeoutSec == 0
   m_idleTimer.reset( new boost::asio::deadline_timer(*m_io_service) );

   set_idle_connection_timeout( m_nIdleTimeoutSec );

   logger.Log( "Run_io_service2" );
   m_io_service->run();
   logger.Log( "Run_io_service3" );

   // be sure the socket is closed before calling ProcessTerminate(). Doubt that it is automatically closed when io_service::run() ends.
   if ( m_sock.get() )
   {
      boost::system::error_code closeEC;
      m_sock->close( closeEC );
   }

   ProcessTerminate();
   logger.Log( "Run_io_service+" );
}

#endif // BASIC_CONNECTION_INCLUDED_ 

