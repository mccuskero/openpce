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

#include <stdlib.h>
#include <string>

#include <pcff/BasicException.h>

using namespace std;
using namespace pcff;

const char *BasicException::ErrTypes[] = { "INF", "WRN", "ERR" };

// TODO: watch buffer offerflow condition on sprintf, use vnsprintf

BasicException::~BasicException()
{
}

void BasicException::setErrString( char *errString )
{
      if ( errString != NULL )
      {
    	  _errString = errString;
      }
}

void BasicException::setErrString( const char *errString )
{
      if ( errString != NULL )
      {
    	  _errString = errString;
      }
}

void BasicException::setErrMsg( char *errMsg )
{
	  if ( errMsg != NULL )
      {
		  if ( strlen(errMsg) < MAX_ERROR_MSG_LEN)
		  {
			  _errMsg = errMsg;
		  }
      }
}

void BasicException::setErrMsg( const char *errMsg )
{
	  if ( errMsg != NULL )
    {
		  if ( strlen(errMsg) < MAX_ERROR_MSG_LEN)
		  {
			  _errMsg = errMsg;
		  }
    }
}


char *BasicException::getLogMsg( void )
{
  char *logMsg = NULL;

  try
    {
      if ( (_errString.length() > 0) && 
	   (_errMsg.length() > 0) )
	{
	  logMsg = new char[_errMsg.length() + _errString.length() +  DEFAULT_LOG_MSG_BASE_LEN + 1];
	  sprintf(logMsg,
		  "%s %d %d %s:%s",
		  ErrTypes[_type],
		  _majorErrorCode,
		  _minorErrorCode,
		  _errString.c_str(),
		  _errMsg.c_str());
	}
      else if ( (_errString.length() > 0) && 
		(_errMsg.length() == 0) )
	{
	  logMsg = new char[_errString.length() +  DEFAULT_LOG_MSG_BASE_LEN + 1];
	  sprintf(logMsg,
		  "%s %d %d %s",
		  ErrTypes[_type],
		  _majorErrorCode,
		  _minorErrorCode,
		  _errString.c_str());
	}
      else if ( (_errString.length() == 0) && 
		(_errMsg.length() > 0) )
	{
	  logMsg = new char[_errMsg.length() +  DEFAULT_LOG_MSG_BASE_LEN + 1];
	  sprintf(logMsg,
		  "%s %d %d %s",
		  ErrTypes[_type],
		  _majorErrorCode,
		  _minorErrorCode,
		  _errMsg.c_str());
	}
      else if ( (_errString.length() == 0) && 
		(_errMsg.length() == 0) )
	{
	  logMsg = new char[DEFAULT_LOG_MSG_BASE_LEN + 1];
	  sprintf(logMsg,
		  "%s %d %d",
		  ErrTypes[_type],
		  _majorErrorCode,
		  _majorErrorCode);
	}
    }
  catch (...)
    {
      logMsg = NULL;
    }

  return logMsg;
}
