/**
 * Copyright (c) 1994-2009 Owen McCusker <mccuskero@ct.metrocast.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string>

#include "libpq-fe.h"

#include <pcff/TimeStamp.h>

#include "PgSqlTools.h"

using namespace std;
using namespace pcff;
using namespace pcdao;

string PgSqlTools::time_t2string(time_t unixTime)
{
   TimeStamp newtime(unixTime);
   string retStr;

   retStr = newtime.get("%Y-%m-%d %T+00");

   return retStr;
}

time_t PgSqlTools::string2time_t(string timeStr)
{
   TimeStamp newtime;
   time_t unixTime = 0;

   newtime.set(timeStr.c_str(), "%Y-%m-%d %T");
   unixTime = newtime.get(unixTime);
   
   return unixTime;
}

string PgSqlTools::escapeStr(string fromStr)
{
   string toStr("");

   int strSize = (fromStr.length()*2) + 1;
   char *toCharStr = NULL;

   try
   {
      toCharStr = new char[strSize];

      PQescapeString(toCharStr, fromStr.c_str(), fromStr.length());

      toStr = toCharStr;

      delete [] toCharStr;
   }
   catch (...)
   {
   }

   return toStr;
}

unsigned char *PgSqlTools::escapeBinaryStr(const unsigned char *fromBuf,
                                           size_t fromBufLen,
                                           size_t *toBufLen)
{
   unsigned char *toCharStr = NULL;

   try
   {
      toCharStr = PQescapeBytea(fromBuf, fromBufLen, toBufLen);

   }
   catch (...)
   {

   }

   return toCharStr;
}

unsigned char *PgSqlTools::unescapeBinaryStr(const unsigned char *fromBuf,
                                             size_t *toBufLen)
{
   unsigned char *toCharStr = NULL;

   try
   {
      toCharStr = PQunescapeBytea(fromBuf, toBufLen);

   }
   catch (...)
   {

   }

   return toCharStr;
}

void PgSqlTools::freeEscapeBinaryStr(unsigned char * escapedStr)
{
   PQfreemem(escapedStr);
}
