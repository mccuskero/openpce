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

#ifndef _PgSqlTools_h_
#define	_PgSqlTools_h_

#include <string>

#include <pcdao/DbTools.h>

namespace pcdao
{

class PgSqlTools : public DbTools
{
public:
   virtual std::string time_t2string(time_t tt);
   virtual time_t string2time_t(std::string str);

public:
   virtual std::string escapeStr(std::string str);
   virtual unsigned char *escapeBinaryStr(const unsigned char *fromStr, size_t fromStrLen, size_t *toStrLen);
   virtual unsigned char *unescapeBinaryStr(const unsigned char *fromStr, size_t *toStrLen);
   virtual void freeEscapeBinaryStr(unsigned char * escapedStr);
};

};

#endif	/* _PgSqlTools_h_ */

