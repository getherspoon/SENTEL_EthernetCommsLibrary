/* 
 * File:   TempLogger.h
 * Author: josh
 *
 * Created on February 24, 2013, 11:51 AM
 */

#ifndef TEMPLOGGER_H
#define	TEMPLOGGER_H

#include <boost/thread.hpp>
#include <iostream>
#include <fstream>

#ifndef NO_MFC
#include <afxmt.h>


#undef min
#undef max

class Templogger
{
public:
   Templogger() 
   {
      m_nWritten = 0;
      m_h = ::CreateFile( "/data/logs/temp.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0/*FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH*/, 0 );
      ::SetFilePointer( m_h, 0, 0, FILE_END );
      static std::string str("New session 5\n");
      DWORD nWritten = 0;
      ::WriteFile( m_h, str.c_str(), str.size(), &nWritten, 0 );
   }

   ~Templogger() 
   {
      static std::string str("End of session\n\n");
      DWORD nWritten = 0;
      ::WriteFile( m_h, str.c_str(), str.size(), &nWritten, 0 );
      ::CloseHandle(m_h);
   }

   void Log( const std::string& strWhat, const boost::system::error_code* ec = 0)
   {
      CSingleLock l(&m_mutex,TRUE);

      std::stringstream st;
      time_t t = time(0) - 1332200000;

      st << t << " T" << ::GetCurrentThreadId() << ": " << strWhat;
      if ( ec )
         st << " ec value = " << ec->value();
      st << "\n";

      std::string str = st.str();
      DWORD nWritten;
      int err = 0;
      BOOL bHuh = ::WriteFile( m_h, str.c_str(), str.size(), &nWritten, 0 );
      if ( !bHuh )
         err = ::GetLastError();
      m_nWritten += nWritten;
   }

   HANDLE m_h;
   CMutex m_mutex;
   int m_nWritten;
};
#else
#include <boost/thread.hpp>
class Templogger
{
public:
   Templogger() 
   {
      m_nWritten = 0;
      m_ofFile.open( "/data/logs/temp.txt", std::ios::in | std::ios::out | std::ios::app );
      static std::string str("New session 5\n");
      m_ofFile << str;
   }

   ~Templogger() 
   {
      static std::string str("End of session\n\n");
      m_ofFile << str;
      m_ofFile.close();
   }

   void Log( const std::string& strWhat, const boost::system::error_code* ec = 0)
   {
      boost::mutex::scoped_lock( m_mutex );

      std::stringstream st;
      time_t t = time(0) - 1332200000;

      st << t << " T" << boost::this_thread::get_id() << ": " << strWhat;
      if ( ec )
         st << " ec value = " << ec->value();
      st << "\n";

      std::string str = st.str();
      m_ofFile << str;      
   }

   std::ofstream        m_ofFile;
   boost::mutex         m_mutex;
   int m_nWritten;
};

#endif

extern Templogger logger;

#endif	/* TEMPLOGGER_H */

