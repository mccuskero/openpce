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

#include <stdio.h>
#include <string.h>
#include <string>

#include <pcdao/DaoException.h>
#include <pcdao/Result.h>
#include <pcdao/Connection.h>

#include "PgSqlResult.h"

using namespace std;
using namespace pcff;
using namespace pcdao;

PgSqlResult::PgSqlResult()
{
   _pPgResult = NULL;
}

PgSqlResult::~PgSqlResult()
{
   if (_pPgResult != NULL)
   {
      PQclear(_pPgResult);
   }
}

void PgSqlResult::initialize(PGresult* res)
{
   if (res == NULL)
   {
      throw DaoResultException(DaoResultException::RESULT_SET_IS_NULL,
                         PQresultErrorMessage(_pPgResult));
   }

   if ((PQresultStatus(res) == PGRES_TUPLES_OK) ||
           (PQresultStatus(res) == PGRES_COMMAND_OK))
   {
      _pPgResult = res;
   }
   else if (PQresultStatus(res) == PGRES_EMPTY_QUERY)
   {
      PQclear(res);
      throw DaoResultException(DaoResultException::RESULT_SET_EMPTY_QUERY,
                         "sql string is empty");
   }
   else if (PQresultStatus(res) == PGRES_BAD_RESPONSE)
   {
      PQclear(res);
      throw DaoResultException(DaoResultException::RESULT_SET_DRIVER_FAILURE,
                         PQresultErrorMessage(res));
   }
   else
   {
      PQclear(res);
      throw DaoResultException(DaoResultException::RESULT_SET_ERROR,
                         PQresultErrorMessage(res));
   }
}

long PgSqlResult::getNumRows()
{
   long num = 0;

   if (_pPgResult == NULL)
   {
      throw DaoResultException(DaoResultException::RESULT_SET_NOT_INITIALIZED,
                         "result set not initialized");
   }
   else
   {
      num = PQntuples(_pPgResult);
   }

   return num;
}

int PgSqlResult::getNumFields()
{
   int num = 0;

   if (_pPgResult == NULL)
   {
      throw DaoResultException(DaoResultException::RESULT_SET_NOT_INITIALIZED,
                         "result set not initialized");
   }
   else
   {
      num = PQnfields(_pPgResult);
   }

   return num;
}

inline char* PgSqlResult::getVal(long rowIdx, int fieldIdx)
{
   return PQgetvalue(_pPgResult, rowIdx, fieldIdx);
}

inline int PgSqlResult::getValAsInt(long rowIdx, int fieldIdx)
{
   return ::atoi(PQgetvalue(_pPgResult, rowIdx, fieldIdx));
}

inline long PgSqlResult::getValAsLong(long rowIdx, int fieldIdx)
{
   return ::atol(PQgetvalue(_pPgResult, rowIdx, fieldIdx));
}

inline float PgSqlResult::getValAsFloat(long rowIdx, int fieldIdx)
{
   return ::atof(PQgetvalue(_pPgResult, rowIdx, fieldIdx));
}

inline double PgSqlResult::getValAsDouble(long rowIdx, int fieldIdx)
{
   return ::strtod(PQgetvalue(_pPgResult, rowIdx, fieldIdx), NULL);
}
