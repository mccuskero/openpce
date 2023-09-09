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
#include <pcff/LogLevel.h>


using namespace std;
using namespace pcff;

const char* Log::_glogRoot      = "./";

//
//
//
LogLevel::LogLevel( int         logTarget, 
                    long        logSize,
                    int         logLevel,
                    const char* fileName, 
                    const char* serverName, 
                    const char* hostName   )
: Log(logTarget, logSize, fileName, serverName, hostName)
{
   _logLevel = logLevel;
}

//
//
//
LogLevel::~LogLevel() 
{
}

bool LogLevel::init( int logTarget,
                const char* fileName,
                long        logSize,
                const char* serverName, 
                const char* hostName )
{
   Log::init(logTarget, fileName, logSize, serverName, hostName);
}   

bool LogLevel::init( const char* fileName, 
                const char* serverName, 
                const char* hostName )
{
   Log::init(fileName, serverName, hostName);
}   

void LogLevel::close() 
{
   Log::close();
}

void LogLevel::logMessage( int logLevelIn, const char* format, ...) 
{
   if (logLevelIn <= _logLevel)
   {
	   char buf[Log::PATH_MAX_LEN+1];
	   va_list ap;
	   va_start(ap, format);
	   vsnprintf(buf, Log::PATH_MAX_LEN, format, ap);
	   logMessage(buf);
	   va_end(ap);

	   Log::logMessage(buf);
   }   
}

void LogLevel::logMessage( const char* msg, int logLevelIn ) 
{
   if (logLevelIn <= _logLevel)
   {
      Log::logMessage(msg);
   }   
}

