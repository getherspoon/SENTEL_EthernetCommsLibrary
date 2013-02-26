#ifndef MEMBER_ACCESSOR_FUNCTOR_DEFINED
#define MEMBER_ACCESSOR_FUNCTOR_DEFINED 1

#include <memory>
#include <boost/asio.hpp> 

// Two functor templates to use for getting and setting values of member variables without adding any extra synchronization
// and the attendant risks of deadlock or similar problems.  
// For examples see BasicConnection::GetOnIdleTimeout() and BasicConnection::SetOnIdleTimeout(). 
template < typename MEMBER_TYPE >
class MemberGetHandler
{
public:

   MemberGetHandler( boost::asio::io_service& io_service, const MEMBER_TYPE& member ) 
      : m_io_service(io_service), m_member(member)
   {
      // allocate memory for a single MEMBER_TYPE, but don't actually create one (it might not
      // have a default ctor, and anyway we don't need to).
      m_curVal.reset( std::allocator<MEMBER_TYPE>().allocate(1) );

      m_valueValid.reset( new bool(false) );
   };

   // Important - the default copy ctor will make a copy that has a m_curVal shared_ptr that 
   // points to the memory location allocated in the original (non-copy) ctor, which is the way we want it.   
   // MemberGetHandler( const MemberGetHandler& );

   // Get a copy of m_member in a threadsafe way and return a reference to it. If the thread and io_service aren't
   // running then Get() will return a bogus value. You can check to make sure whether good data is returned by
   // supplying an argument to bValid.
   const MEMBER_TYPE& Get( bool* bValid = 0 )
   {
      // Will use this object as a functor, thereby calling operator()(), on the m_io_service thread.
      // Since it is being performed by m_io_service on that thread, no thread synchronization is necessary.
      // If m_io_service isn't running then operator()() won't be called, in which case the returned m_curValue is bogus.
      m_io_service.dispatch( *this );

      if ( bValid )
      {
         *bValid = *m_valueValid;
      }

      return *m_curVal;
   }

   // will be run on the m_io_service thread when dispatch(*this) is called
   void operator()() 
   { 
      // create a copy of m_member in the memory location that was previously allocated
      std::allocator<MEMBER_TYPE>().construct( m_curVal.get(), m_member );

      // since operator() was actually invoked then the value returned by Get() is real
      *m_valueValid = true;
   }

private:
   boost::asio::io_service& m_io_service;
   const MEMBER_TYPE& m_member;
   boost::shared_ptr<MEMBER_TYPE> m_curVal;
   boost::shared_ptr<bool> m_valueValid;
};

template < typename MEMBER_TYPE >
class MemberSetHandler
{
public:
   MemberSetHandler( boost::asio::io_service& io_service, MEMBER_TYPE& member, const MEMBER_TYPE& newVal ) 
      : m_io_service(io_service), m_member(member), m_newVal(newVal) {};

   void Set()
   {
      m_io_service.post( *this );
   }

   void operator()() { m_member = m_newVal; }

private:
   boost::asio::io_service& m_io_service;
   MEMBER_TYPE& m_member;
   MEMBER_TYPE m_newVal;
};

#endif