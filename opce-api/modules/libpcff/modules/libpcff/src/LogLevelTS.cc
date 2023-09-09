/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@myeastern.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * All rights reserved.
 */

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <pcff/TimeStamp.h>
#include <pcff/thread/Thread.h>
#include <pcff/thread/ThreadDirector.h>
#include <pcff/LogLevelTS.h>


using namespace std;
using namespace pcff;


//
// Todo: make Log a singleton class, or use a LogMgr
// singleton. 
//

//
//
//
LogLevelTS::LogLevelTS( int         logTarget, 
                        long        logSize,
                        int         logLevel,
                        const char* fileName, 
                        const char* serverName, 
                        const char* hostName   )
  : LogLevel(logTarget, logSize, logLevel, fileName, serverName, hostName)
{
  _bEnableThreadID = false;
  _logMutex   = NULL;
}

//
//
//
LogLevelTS::~LogLevelTS() 
{
  if ( _logMutex == NULL )
  {
     delete _logMutex;
     _logMutex = NULL;
  }
}
    
//
//
//
bool LogLevelTS::init( int logTarget,
                     const char* fileName,
                     long        logSize,
                     const char* serverName, 
                     const char* hostName )
{
   _logMutex = new Mutex();
   _logMutex->init();
   return LogLevel::init(logTarget, 
                         fileName, 
                         logSize, 
                         serverName, 
                         hostName);
}   

// 
//
//
bool LogLevelTS::init( const char* fileName, 
                const char* serverName, 
                const char* hostName )
{
   _logMutex = new Mutex();
   _logMutex->init();
   return LogLevel::init(fileName,
                         serverName,
                         hostName);
}


//
//
//
void LogLevelTS::close() 
{
   _logMutex->lock();

   LogLevel::close();
   
  _logMutex->unlock();
}

//
// TODO: use variable arguments?
//
void LogLevelTS::logMessage( const char* msg, int logLevel ) 
{
   if (logLevel <= _logLevel)
   {
      _logMutex->lock();

      char buf[32];
      stringstream sStr(stringstream::in | stringstream::out);
      pthread_t hThd = Thread::gGetThreadHandle();
      sprintf(buf, "%lu", hThd);
      
      
      if ( ThreadDirector::instance().isInitialized() == true )
      {
         sStr << buf;
         sStr << " " << msg ;            
         LogLevel::logMessage(sStr.str().c_str(), logLevel);
      }
      else
      {
         LogLevel::logMessage(msg, logLevel);
      }
      
      _logMutex->unlock();   
   }
}
