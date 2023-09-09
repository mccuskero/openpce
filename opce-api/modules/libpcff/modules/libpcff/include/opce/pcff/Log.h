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

#ifndef _Log_h
#define _Log_h


#include <fstream>

namespace pcff
{

class Log 
{
 public:   
  enum LogType 
   { 
      logNone    = 0, 
      logToFile  = 1, 
      logToDb    = 2, 
      logToCrt   = 8,
      logToSyslog = 16
   };

   enum Constants
   {
      PATH_MAX_LEN = 1024
   }; 
 
 public:
  // CONSTRUCTION, DESTRUCTION
  Log( int logTarget = logNone, 
       long logSize = 1024048,
       const char* fileName = NULL, 
       const char* serverName = NULL, 
       const char* hostName = NULL );
  virtual ~Log();
  
 public:
  // ATTRIBUTES
  int getLogTarget() const { return _logTarget; }
  
   // OPERATIONS
  virtual bool init( int logTarget = logNone,
                     const char* fileName = NULL,
                     long logSize = 1024048, 
                     const char* serverName = NULL, 
                     const char* hostName = NULL );

  virtual bool init( const char* fileName = NULL, 
                     const char* serverName = NULL, 
                     const char* hostName = NULL );
  void close();
  
  // get and log the message defined by the in code and parameters
  virtual void logMessage(const char *format,...);
  
  // IMPLEMENTATION
 private:
  void logMsgToFile    ( const char* text );
  void logMsgToDb      ( const char* text );
  void logMsgToCrt     ( const char* text );
  void logMsgToSyslog  ( const char* text );

  static int getSeverity( long id );
  static int getEventType( long id );
  
 private:
  int                 _logTarget;
  long                _maxLogSize;
  std::ofstream*      _logFile;

 private:
  static const char*  _glogRoot;
};

};
#endif //
