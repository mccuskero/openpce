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

#ifndef _PGSQLRESULT_H
#define	_PGSQLRESULT_H

#include <libpq-fe.h>

#include <pcdao/Result.h>

namespace pcdao
{

class PgSqlResult : public Result
{
public:
   PgSqlResult();
   PgSqlResult(const PgSqlResult& orig);
   virtual ~PgSqlResult();

public:
   void initialize(PGresult* _pPgResult);


public:
   virtual long getNumRows();
   virtual int getNumFields();

public:
   virtual char *getVal(long rowIdx, int fieldIdx);
   virtual int getValAsInt(long rowIdx, int fieldIdx);
   virtual long getValAsLong(long rowIdx, int fieldIdx);
   virtual float getValAsFloat(long rowIdx, int fieldIdx);
   virtual double getValAsDouble(long rowIdx, int fieldIdx);

private:
   PGresult* _pPgResult; // postgres result set handle

};

};

#endif	/* _PGSQLRESULTSET_H */

