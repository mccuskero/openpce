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

#ifndef _TRANSACTION_H
#define	_TRANSACTION_H

#include <string>

#include <pcdao/Result.h>
#include <pcdao/Connection.h>

namespace pcdao
{

class Transaction
{
public:
   Transaction(Connection& conn);
   virtual ~Transaction();

public:
   virtual void close() = 0;
   virtual void begin() = 0;
   virtual void commit() = 0;
   virtual void rollback() = 0;

public:
   static const char *BEGIN;
   static const char *COMMIT;
   static const char *ROLLBACK;

public:
   virtual Result* execQuery(std::string &sql) = 0;
   virtual int execUpdate(std::string &sql) = 0;

protected:
   Connection &_conn;
};

};

#endif	/* _TRANSACTION_H */

