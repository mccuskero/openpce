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

#include <syslog.h>
#include <stdarg.h>

#include <pcff/TimeStamp.h>
#include <pcff/Log.h>


using namespace std;
using namespace pcff;

//
// Todo: make Log a singleton class, or use a LogMgr
// singleton. 
//

//
//
//
Log::Log( int         logTarget, 
          long        logSize,
          const char* fileName, 
          const char* serverName, 
          const char* hostName   )
  //   : logTarget(logNone), fileMutex(NULL), logFile(NULL)
  : _logTarget(logNone), _logFile(NULL)
{
  _logTarget = logTarget;
  _maxLogSize = logSize;
  // TODO: need to implement this constructor when needed
}

//
//
//
Log::~Log() 
{
  close();
}
    

// 
//
//
bool Log::init( int logTarget,
		const char* fileName,
		long        logSize,
		const char* serverName, 
		const char* hostName )
{
  _maxLogSize = logSize;
  _logTarget = logTarget;
  this->init( fileName, serverName, hostName );
}

// 
//
//
bool Log::init( const char* fileName, 
                const char* serverName, 
                const char* hostName )
{
   this->close();
   
   if( _logTarget & logToFile ) 
   {
      if( fileName == NULL ) return false;
      // 
      // TODO: create and lock a mutex 
      //
      string fullName = _glogRoot; 
      fullName += fileName;
      
      _logFile = new ofstream();
      
      _logFile->open( fullName.c_str(), 
                      ofstream::out | ofstream::app ); 
      
      // TODO: need to verify
      if ( _logFile->tellp() >= _maxLogSize )
      {
         logMessage("Max Log file size met, backing up log");
         _logFile->close();
         
         // TODO: make sure you can move onto other files...
         string newName = fullName.c_str();
         newName += ".bak";
         rename( fullName.c_str(), newName.c_str() );
         _logFile->open( fullName.c_str(), 
                         ofstream::out | ofstream::app ); 
      }
      
      if (!_logFile->is_open() )
      {
         //
         // TODO: get rid of Mutex
         //	  fileMutex = NULL;
         return false;
      }
      
      //  TODO: unlock Mutex
      _logTarget |= logToFile;
      // TODO: copy file to save file when MAX file size is met.
      // use tellp to get position
   }
   if( _logTarget & logToCrt )
   {
      _logTarget |= logToCrt;
   }
   if( _logTarget & logToSyslog )
   {
      // TODO: not implemented
   }
   if( _logTarget & logToDb ) 
   {
      // TODO: not implemented
   }
   
   return true;
}

//
//
//
void Log::close() 
{   
  if ( (_logTarget & logToFile) && _logFile != NULL )
     {
       //       ::CloseHandle( fileMutex ); fileMutex = NULL;
       _logFile->close();
       delete _logFile;
     }
  _logFile = NULL;
}

void Log::logMessage( const char* format, ... ) 
{   
	   char buf[Log::PATH_MAX_LEN+1];
	    va_list ap;
	    va_start(ap, format);
	    vsnprintf(buf, Log::PATH_MAX_LEN, format, ap);
	    va_end(ap);

   if( (_logTarget & logToFile) || 
       (_logTarget & logToDb)   || 
       (_logTarget & logToCrt)  ||
       (_logTarget & logToSyslog)    ) 
   {
      // build messages from args
      
      string text = buf;
      //       getMessage( text, id, p1, p2, p3 );
      if( _logTarget & logToFile  ) logMsgToFile   ( text.c_str() );
      if( _logTarget & logToDb    ) logMsgToDb     ( text.c_str() );
      if( _logTarget & logToCrt   ) logMsgToCrt    ( text.c_str() );
      if( _logTarget & logToSyslog   ) logMsgToSyslog ( text.c_str() );
   }
}

//
// 
// private
void Log::logMsgToFile( const char* text ) 
{
  TimeStamp d( TimeStamp::SetToNow );
  string sd = d.get("YYYY-MM-DD HH:NN:SS");
  stringstream sStr(stringstream::in | stringstream::out);

  sStr << sd 
       << " " 
       << text ;

  //  TODO: lock with timeout
  //  if( dw == WAIT_TIMEOUT ) return; // ? throw
  // logFile->SeekToEnd();
  // TODO: copy over the full file, and create another

  //
  // TODO: put logFile in a stream to log to
  // review ObjStream code
  //
  *_logFile << sStr.str() << endl;
  //  TODO: release Mutex
}

//
//
//
void Log::logMsgToDb( const char* text ) 
{


}

//
// 
//
void Log::logMsgToCrt( const char* text ) 
{
  TimeStamp d( TimeStamp::SetToNow );
  string sd = d.get("YYYY-MM-DD HH:NN:SS");
  stringstream sStr(stringstream::in | stringstream::out);

  sStr << sd
       << " " 
       << text ;

  // TODO: lock with timeout
  cout << sStr.str() << endl;
  // unlock
}

//
//
//
void Log::logMsgToSyslog( const char* text ) 
{
    syslog(LOG_DAEMON | LOG_NOTICE, "%s", text);   
}

//
//
//
//void Log::initialize(  const char* rootPath ) /
//{
  // 
  // TODO: set the root path
  //
//}

