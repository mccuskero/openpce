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

#ifndef _LogLevelTS_h
#define _LogLevelTS_h


#include <fstream>

#include <pcff/thread/Mutex.h>
#include <pcff/LogLevel.h>

#include <pcff/thread/ThreadDirector.h>

namespace pcff
{

   
class LogLevelTS : public LogLevel
{
 public:
  // CONSTRUCTION, DESTRUCTION
   LogLevelTS( int logTarget = logNone, 
              long logSize = 1024048,
              int  logLevel = LogLevel::NotInitialized,
              const char* fileName = NULL, 
              const char* serverName = NULL, 
              const char* hostName = NULL );
  virtual ~LogLevelTS();
  
 public:
  virtual bool init( int logTarget = logNone,
             const char* fileName = NULL,
             long logSize = 1024048, 
             const char* serverName = NULL, 
             const char* hostName = NULL );

  virtual bool init( const char* fileName = NULL, 
             const char* serverName = NULL, 
             const char* hostName = NULL );
  void close();
  
  void setEnableThreadID()       { _bEnableThreadID = true;  }
  void resetEnableThreadID()     { _bEnableThreadID = false;  }

  // get and log the message defined by the in code and parameters
  virtual void logMessage( const char* p1 = NULL, int logLevel = LogLevel::NotInitialized );
  

 private:
   bool                 _bEnableThreadID;
   pcff::Mutex          *_logMutex;
};

};
#endif //
