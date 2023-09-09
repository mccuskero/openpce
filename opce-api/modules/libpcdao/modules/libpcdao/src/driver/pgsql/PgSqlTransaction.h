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

#ifndef _PgSqlTransaction_H
#define	_PgSqlTransaction_H

#include <pcdao/Connection.h>
#include <pcdao/Result.h>
#include <pcdao/Transaction.h>

namespace pcdao
{

class PgSqlTransaction : public Transaction
{
public:
    PgSqlTransaction(Connection &conn);
    virtual ~PgSqlTransaction();

public:
   virtual void close();
   virtual void begin();
   virtual void commit();
   virtual void rollback();

public:
   virtual Result* execQuery(std::string &sql);
   virtual int execUpdate(std::string &sql);

private:

};

};

#endif	/* _PgSqlTransaction_H */

