
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

#ifndef __BasicExceptionMgr_h
#define __BasicExceptionMgr_h

#include <pcff/ExceptionConstants.h>
#include <pcff/LogLevel.h>

namespace pcff 
{
  class BasicException;
 
  class BasicExceptionMgr 
  {

  protected:
    BasicExceptionMgr(); 

  private:
	  void initMembers();
	  
  public:
    static BasicExceptionMgr* instance();

  public:
     int getNumErrors() { return nNumExceptions; }

  public:
    virtual void handle(); 
    virtual void handle( const char* msg ); // 
    virtual void handle( BasicException& ex ); // 
    
  public:
    virtual void log( const char* msg, int logLevel = LogLevel::NotInitialized);
    virtual void log(int logLevel, const char* format, ...);
    virtual void createLogger( char *filename );

  public:
    void       setLogCallback( EXLogFunction logfunc );
    void       setLog( LogLevel *log )       { myLog = log; }
    void       setLogLevel( int loglevel );
    int        getLogLevel()                 { return this->logLevel; }

  protected:
    void logMessage( const char* msg );
    void logMessage( const char* type, const char* msg );
    void logMessage( char* type, int major, const char* msg );
    void logMessage( char* type, int major, int minor, const char* msg );

  protected:
    /**
     * this member function is overloaded for special processing of 
     */
    virtual void process( BasicException& ex ); 

  protected:
    int           nNumExceptions;
    EXLogFunction loggingCB;
    int           logLevel;
    LogLevel      *myLog;

  protected:
    static BasicExceptionMgr* pInstance;

  };
}; // namespace


#endif
