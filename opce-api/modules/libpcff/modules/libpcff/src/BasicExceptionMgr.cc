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

/**
 * \class BasicExceptionMgr
 */

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <string.h>
#include <stdarg.h>

#include <pcff/LogLevel.h>
#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

using namespace std;
using namespace pcff;

/**
 * definitions in header file to make it easier to 
 * extend. Maybe in future we can make a separate
 * definition file.
 */

BasicExceptionMgr* BasicExceptionMgr::pInstance = NULL;

BasicExceptionMgr* BasicExceptionMgr::instance() 
{
  if ( pInstance == NULL )
    pInstance = new BasicExceptionMgr();

  return pInstance;
}

BasicExceptionMgr::BasicExceptionMgr()
{ 
	initMembers();
}

void BasicExceptionMgr::initMembers()
{
	nNumExceptions=0; 
	loggingCB=NULL; 
	logLevel=LogLevel::Error; 
	myLog = NULL;	
}

void BasicExceptionMgr::setLogLevel( int loglevel )   
{ 
	this->logLevel = loglevel; 
	
	if (myLog != NULL)
	{
		myLog->setLogLevel(logLevel);
	}
}

//
//
//
void BasicExceptionMgr::handle() 
{
  nNumExceptions++;

  this->logMessage( "Error:  ", "Unknown Error occured" );
}

//
//
//
void BasicExceptionMgr::handle( const char* msg ) 
{
  nNumExceptions++;

  this->logMessage( "Error:  ", msg );
}

//
//
//
void BasicExceptionMgr::handle( BasicException& ex ) 
{
  nNumExceptions++;    

  /** 
   * call the process exception handler. This can be 
   * overloaded by derived class for special processing
   * of derived exceptions.
   */
  this->process( ex );

  char *msg = ex.getLogMsg();
  this->logMessage( msg );
  delete [] msg;
}


//
// TODO: check ret val of vsnprintf for error
// if (n  > -1 && n < size) good, else bad
//
void BasicExceptionMgr::log( int logLevel , const char* format, ...) 
{  
	if (myLog != NULL)
	{
		char buf[Log::PATH_MAX_LEN+1];
		va_list ap;
		va_start(ap, format);
		vsnprintf(buf, Log::PATH_MAX_LEN, format, ap);
//		logMessage(buf);
		myLog->logMessage( buf, logLevel );
		va_end(ap);

	}
	else 
	{
		cout << "(BasicExceptionMgr::myLog ... does note exist using std::cout)" << endl;
	}
}

//
//
//
void BasicExceptionMgr::log( const char* msg, int logLevel ) 
{  
	if (myLog != NULL)
	{
		myLog->logMessage( msg, logLevel );
	}
	else 
	{
		cout << "(BasicExceptionMgr::myLog does note exist using std::cout)" << endl;
		cout << msg << endl;
	}
}

//
//
//
void BasicExceptionMgr::createLogger( char *filename )
{
  if ( myLog != NULL )
  {
    this->logMessage( "Replacing BasicExceptionMgr log object" );  
    delete myLog;
    myLog = NULL;
  }

  myLog = new LogLevel();
  myLog->init(Log::logToFile | Log::logToCrt, filename );
}


/**
 * sets the logging callback function. 
 * LogFunction is defined in ExceptionConstants.h
 * @param logfunc
 * @return void
 *
 */
void BasicExceptionMgr::setLogCallback( EXLogFunction logfunc ) 
{
  char errMsg[BasicException::MAX_ERROR_MSG_LEN];

  try 
  {
    if ( logfunc != NULL ) 
    {
      this->loggingCB = logfunc;
    }
    else 
    {
      sprintf(errMsg, 
	      "logging callback function is NULL");
      throw BasicException( BasicException::ERR,
			   BasicException::BadLogCallback,
			   errMsg );
    }    
  }
  catch( BasicException& ex) 
  {
    BasicExceptionMgr::instance()->handle( ex );
  }
  catch(...) 
  {
    BasicExceptionMgr::instance()->handle( "BasicExceptionMgr::setLogCallback  unknown ERROR" );
  }  
}

void  BasicExceptionMgr::logMessage( const char* msg ) 
{
  if ( this->loggingCB == NULL && this->myLog == NULL ) 
  {
    cerr << endl << msg << endl;
  }
  else 
  {
    if ( this->loggingCB != NULL )
    {
      this->loggingCB( "%s", msg );
    }
    if ( myLog != NULL )
    {
      myLog->logMessage( msg );
    }
  }
}

/**
 * logMessage either uses logCB or passes information to cerr
 * logCB is set by client program following logFunction typedef
 * 
 */
void  BasicExceptionMgr::logMessage( const char* type, const char* msg ) 
{
  if ( this->loggingCB == NULL && this->myLog == NULL ) 
  {
     cerr << type << msg << endl; cerr.flush();
  }
  else 
  {
    if ( this->loggingCB != NULL )
    {
      this->loggingCB( " %s %s", type, msg );
    }

    if ( myLog != NULL )
    {
      myLog->logMessage( msg );
    }
  }
}

/**
 * 
 */
void  BasicExceptionMgr::logMessage( char* type, int major, const char* msg ) 
{

}

/**
 * 
 */
void  BasicExceptionMgr::logMessage( char* type, int major, int minor, const char* msg ) 
{

}



/**
 * 
 */
void  BasicExceptionMgr::process( BasicException& ex) 
{

}
