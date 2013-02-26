/* 
 * File:   Trace.h
 * Author: Josh
 *
 * Created on November 21, 2012, 3:43 PM
 */

#ifndef TRACE_H
#define	TRACE_H

#ifdef _WINDOWS_

#include <windows.h>
#ifdef _DEBUG
bool win_trace(const TCHAR *format, ...)
{
   TCHAR buffer[1000];

   va_list argptr;
   va_start(argptr, format);
   wvsprintf(buffer, format, argptr);
   va_end(argptr);

   OutputDebugString(buffer);

   return true;
}
#define TRACE win_trace
#else
#define TRACE
#endif

#elif _LINUX_

#ifdef _DEBUG
#define TRACE
#else
#define TRACE
#endif

#elif _OSX_

#ifdef _DEBUG
#define TRACE
#else
#define TRACE
#endif

#else

#define TRACE

#endif
    
#endif	/* TRACE_H */

