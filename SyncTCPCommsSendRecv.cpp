#include "stdafx.h"
#include "SyncTCPCommsSendRecv.h"


CSyncTCPCommsSendRecv::CSyncTCPCommsSendRecv(const std::string &p_strIP, const unsigned short p_usPort) :
CTCPCommsImpl(CommsUtilities::SYNC, p_strIP, p_usPort)
{
	//Initialize variables
	memset(this->m_strRecvBuffer, '\0', m_s_nMAX_MESSAGE_SIZE);
	this->m_sizeReadSize = -1;
	this->m_sizeWriteSize = -1;
	this->InitComms();
}

CSyncTCPCommsSendRecv::~CSyncTCPCommsSendRecv()
{
}

//Sync Read operation implemented with Async Methods and Timers to simulate Synchronous operations
//This method works by setting up timers to track the time passed, while this method calls the ReadAsync method with the 
//handlers from this class. Once a handler has been triggered, this method will check if there is an error, cancel the timer, and return
//the data read from the socket. If no handler is called before the timer expires, the timer will set the timer error parameter, which will
//cause the method to stop waiting for a response and return the error. 
bool CSyncTCPCommsSendRecv::Read(std::string& p_strMsg, boost::system::error_code& p_bstError, const long p_lTimeoutInMs)
{

	bool bResult = false;
	bool bReadSent = false;
    try
    {        
		//Ensure a comms object is available
		ASSERT( this->HasComms() );

		//Reset the Timeout and Read errors.
		this->m_bstTimeoutError.reset();
		this->m_bstReadError.reset();
		this->m_sizeReadSize = -1;
		
		//Lock the Timer
		boost::mutex::scoped_lock lock(this->m_bstTimerLock);

		//If connection isn't established, give until the timeout to establish the connection before sending the read.
		boost::posix_time::ptime bstStart( boost::posix_time::second_clock::local_time() );
		bool bConnectionTimedOut = false;
		while( !this->IsConnected() && !bConnectionTimedOut )
		{			
			// If not connected, sleep for a short time
			boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );

			// Get the current time and see if we've timed out
			boost::posix_time::ptime bstNow( boost::posix_time::second_clock::local_time() );
			boost::posix_time::time_duration bstDur = bstNow - bstStart;
			if( bstDur.total_milliseconds() >= p_lTimeoutInMs )
			{
				// If so, flag it as such
				bConnectionTimedOut = true;
			}
		}
						
		//Initiate the read command only if we are actually connected
		if( !bConnectionTimedOut )
		{
			//Prepare the Timer and set it to expire base on the TimeoutInMs parameter
			//Call the HandleTimeout method when the timer has expired
			this->m_bstReadTimer.reset( new boost::asio::deadline_timer( this->Get_IO_Service() ) );
			this->m_bstReadTimer->expires_from_now( boost::posix_time::milliseconds( p_lTimeoutInMs ) );
			this->m_bstReadTimer->async_wait( boost::bind( &CSyncTCPCommsSendRecv::HandleTimeout, 
															this, boost::asio::placeholders::error ) );

			// Send the Read command with the ReadHandler method to call when finished. 
			this->ReadAsync(this->m_strRecvBuffer, m_s_nMAX_MESSAGE_SIZE, boost::bind(&CSyncTCPCommsSendRecv::ReadHandler, this, _1, _2));
			// Set Recv Flag to True for the loop below
			m_bReceiving = true;
		}
		if( !this->IsConnected() )
		{
			TRACE("%s: WARNING: Could not connect to remote host! Read command was not sent!\n", __FUNCTION__ );
		}
		else
		{
			while( m_bReceiving )
			{							
				//The read error pointer has been set (doesn't mean we have an actual error)
				if( this->m_bstReadError.get() )
				{
					//Cancel the Timer
					if( this->m_bstReadTimer.get() )
						this->m_bstReadTimer->cancel( p_bstError );
					//check if there really is an error
					if( !*m_bstReadError )
					{
						p_strMsg = std::string( this->m_strRecvBuffer, this->m_sizeReadSize );
						bResult = true;
						m_bReceiving = false;
					}
					else
					{
						TRACE( "%s:READ ERROR: %s\n", __FUNCTION__, m_bstReadError->message().c_str() );
						p_bstError = *m_bstReadError;
						m_bReceiving = false;			

						if( p_bstError == boost::asio::error::eof )
						{
							this->TerminateComms();
							this->InitComms();
						}
					}
					this->m_bstReadError.reset();
					this->m_bstTimeoutError.reset();
				}
				else if( this->m_bstTimeoutError.get() )
				{
					boost::system::error_code bstCancelError;

					//Note: According to Boost Documentation, this will always return Operation Not Supported on Windows XP. 
					this->CancelIO(bstCancelError);

					//check if there really is an error					
					p_bstError.assign( boost::system::errc::timed_out, boost::system::get_generic_category() );

					TRACE( "%s: TIMEOUT ERROR: %s\n", __FUNCTION__, this->m_bstTimeoutError.get()->message().c_str() );

					if( !this->IsConnected() )
					{
						TRACE("%s: WARNING: %s\n", __FUNCTION__, "Could not connect to remote host! Read command was not sent!\n");
					}
					//Reset the Timer and TimeoutError
					this->m_bstReadTimer.reset();
					this->m_bstTimeoutError.reset();
					throw boost::system::system_error(	boost::system::error_code( boost::system::errc::timed_out, 
														boost::system::get_generic_category() ) );
					m_bReceiving = false;
				}
				else
				{
					//No changes, sleep the thread, then resume the while loop
					boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
				}
			}
		}
    }
    catch (std::exception& ex)
    {
		std::string strEx = ex.what();
		//TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
    }

    return bResult;	
}

//This method works by setting up timers to track the time passed, while this method calls the WriteAsync method with the 
//handlers from this class. Once a handler has been triggered, this method will check if there is an error, cancel the timer, and return
//the number of bytes written from the socket. If no handler is called before the timer expires, the timer will set the timer error parameter, which will
//cause the method to stop waiting for a response and return the error. 
bool CSyncTCPCommsSendRecv::Send( const std::string& p_strMessage, boost::system::error_code& p_bstError, const long p_lTimeoutInMs)
{
	bool bResult = false;
	bool bWriteSent = false;
	try
	{
		//Ensure a comms object is available
		ASSERT( this->HasComms() );
		
		//Reset the timeout Error and Write Error pointers
		this->m_bstTimeoutError.reset();
   		this->m_bstWriteError.reset();
		this->m_sizeWriteSize = -1;

		//Prepare the Timer and set it to expire base on the TimeoutInMs parameter
		//Call the HandleTimeout method when the timer has expired
		this->m_bstWriteTimer.reset( new boost::asio::deadline_timer( this->Get_IO_Service() ) );
		this->m_bstWriteTimer->expires_from_now( boost::posix_time::milliseconds( p_lTimeoutInMs ) );
		this->m_bstWriteTimer->async_wait( boost::bind( &CSyncTCPCommsSendRecv::HandleTimeout, 
															this, boost::asio::placeholders::error ) );
		m_bWriting = true;

		while( m_bWriting )
		{
			
			//Connection is up, and we have not sent the write command yet. 
			if(this->IsConnected() && !bWriteSent)
			{
				this->WriteAsync(p_strMessage, boost::bind(&CSyncTCPCommsSendRecv::WriteHandler, this, _1, _2));
				bWriteSent = true;
			}
			
			//The connection isn't established, give until the timeout to establish the connection before sending the write.
			else if(!this->IsConnected() && !this->m_bstTimeoutError.get() )
			{
				boost::this_thread::sleep( boost::posix_time::milliseconds(100));
			}
			
			//The write error pointer has been set (doesn't mean we have an actual error)
			else if(this->m_bstWriteError.get())
			{
				this->m_bstWriteTimer->cancel(p_bstError);
				
				//check if there really is an error
				if(!*m_bstWriteError)
				{
					bResult = true;
					m_bWriting = false;
				}
				else
				{
					TRACE( "%s: ERROR: %s\n", __FUNCTION__, m_bstWriteError->message().c_str() );
					p_bstError = *m_bstWriteError;
					m_bWriting = false;					
				}
			this->m_bstWriteError.reset();
			this->m_bstTimeoutError.reset();
			}
			
			//Timeout has been triggered. 
			else if( this->m_bstTimeoutError.get() )
			{
				//Note: According to Boost Documentation, this will always return Operation Not Supported on Windows XP. 
				this->CancelIO(p_bstError);
				
				//check if there really is an error
				if(p_bstError)
				{
					TRACE( "%s: ERROR: %s\n", __FUNCTION__, p_bstError.message().c_str() );
				}
				
				if( !bWriteSent )
				{
					TRACE("%s: WARNING: %s\n", __FUNCTION__, "Could not connect to remote host! Write command was not sent!\n");
				}

				//Reset the Timer and TimeoutError
				this->m_bstWriteTimer.reset();
				this->m_bstTimeoutError.reset();
				throw boost::system::system_error(	boost::system::error_code( boost::system::errc::timed_out, 
													boost::system::get_generic_category() ) );
				m_bWriting = false;
			}
			
			else
			{
				//No changes, sleep the thread, then resume the while loop
				boost::this_thread::sleep( boost::posix_time::milliseconds(100));
			}

		}
		
	}
	
	catch (std::exception& ex)
    {
		std::string strEx = ex.what();
		//TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
    }

    return bResult;
}

void CSyncTCPCommsSendRecv::ReadHandler(const boost::system::error_code &p_bstError, const std::size_t p_sizeBytesRecvd)
{
	//TRACE("----- READ HANDLER -------\n" );
	//Set the ReadError to the one returned from the Connection
	m_bstReadError.reset( new boost::system::error_code( p_bstError ) );
    //If we have no error, set the ReadSize to the BytesReceived Parameter
	if( !p_bstError )
    {
		this->m_sizeReadSize = p_sizeBytesRecvd;
    }
	else
	{
		std::string strError = p_bstError.message();
		TRACE( "%s:READ ERROR: %s\n", __FUNCTION__, strError.c_str() );
	}
}

void CSyncTCPCommsSendRecv::WriteHandler(const boost::system::error_code &p_bstError, const std::size_t p_sizeWriteSize)
{
	//Set the WriteError to the one returned from the Connection
	this->m_bstWriteError.reset(new boost::system::error_code( p_bstError ) );
	//If we have no error, set the WriteSize to the WriteSize parameter
	if( !p_bstError )
    {
		this->m_sizeWriteSize = p_sizeWriteSize;
	}
}

void CSyncTCPCommsSendRecv::HandleTimeout(const boost::system::error_code &p_bstError)
{
	//On timeout, set the TimeoutError to the errorcode received from the timer. 
	if( p_bstError.value() == boost::system::errc::success )
	{
		m_bstTimeoutError.reset( new boost::system::error_code( p_bstError ) );
	}
}

