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

#include <string>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>

#include <pcdao/DaoConnectionException.h>
#include <pcdao/Database.h>

#include "PgSqlResult.h"


#include "PgSqlConnection.h"

using namespace std;
using namespace pcff;
using namespace pcdao;

PgSqlConnection::PgSqlConnection(ConnectionDirector &myDir)
: Connection(myDir)
{
   initMembers();
}

PgSqlConnection::~PgSqlConnection()
{
   PQfinish(_pPgConn);
}

void PgSqlConnection::initMembers()
{
   _pPgConn = NULL;
}

void PgSqlConnection::initialize(std::string &daoName,
                                 std::string &databaseName,
                                 std::string &host,
                                 int port,
                                 std::string &user,
                                 std::string &password)
{
   string sConnStr;

   if (_pPgConn != NULL)
   {
      PQfinish(_pPgConn);
   }

   Connection::initialize(daoName, databaseName, host, port, user, password);

   sConnStr = createConnectionString(databaseName, host, port, user, password);

   _pPgConn = PQconnectdb(sConnStr.c_str());

   if (PQstatus(_pPgConn) == CONNECTION_BAD)
   {
      PQfinish(_pPgConn);

      throw DaoConnectionException(DaoConnectionException::ERROR_CONNECTING_TO_DATABASE, (char *)sConnStr.c_str());
   }

   PQsetNoticeProcessor(_pPgConn, &dbLoggingCB, NULL);
}

string PgSqlConnection::createConnectionString(std::string &databaseName,
                                               std::string &host,
                                               int port,
                                               std::string &user,
                                               std::string &password)
{
   stringstream connectSS;
   string connString;

   if (host.length() > 0)
   {
      if (host.length() >= Database::MAX_HOST_STRING_LEN)
      {
         throw DaoConnectionException(DaoConnectionException::BAD_HOST_NAME,
                                      (char *)host.c_str());
      }
      else
      {
         if (host.compare("localhost") != 0)
         {
            connectSS << " host=" << host << " ";
         }
      }
   }

   if (port > 0)
   {
      connectSS << "port=" << port << " ";
   }

   if (databaseName.length() > 0)
   {
      if (databaseName.length() >= Database::MAX_DATABASENAME_LEN)
      {
         throw DaoConnectionException(DaoConnectionException::BAD_DATABASE_NAME,
                                      "PgSqlConnection::createConnectionString");
      }
      else
      {
         connectSS << "dbname=" << databaseName << " ";
      }
   }

   if (user.length() > 0)
   {
      if (user.length() >= Database::MAX_USERNAME_LEN)
      {
         throw DaoConnectionException(DaoConnectionException::BAD_USER_NAME,
                                      "PgSqlConnection::createConnectionString");
      }
      else
      {
         connectSS << "user=" << user << " ";
      }
   }

   return connectSS.str();
}

Result* PgSqlConnection::execQuery(string &sqlStmt)
{
   PgSqlResult* pResult = NULL;
   PGresult* pPgRes = NULL;

   try
   {
      if (sqlStmt.length() == 0)
      {
         throw DaoConnectionException(DaoConnectionException::BAD_SQL_CMD,
                                      "sql string is zero length ");
      }

      if (_pPgConn == NULL)
      {
         throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                      "Postgres connection not initialized. ");
      }

      if (PQstatus(_pPgConn) == CONNECTION_BAD)
      {
         throw DaoConnectionException(DaoConnectionException::CONNECTION_ERROR,
                                      "Connection failed in query execution ");
      }

      pPgRes = PQexec(_pPgConn, sqlStmt.c_str());
      pResult = new PgSqlResult();
      pResult->initialize(pPgRes);
   }
   catch (DaoConnectionException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      delete pResult;
      pResult = NULL;
      throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                   (char *)sqlStmt.c_str());
   }
   catch (BasicException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      delete pResult;
      pResult = NULL;
      throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                   (char *)sqlStmt.c_str());
   }
   catch (...)
   {
      delete pResult;
      pResult = NULL;
      throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                   (char *)sqlStmt.c_str());
   }

   return pResult;
}

int PgSqlConnection::execUpdate(string &sqlStmt)
{
   PGresult* pPgRes = NULL;
   int numRowsEffected = 0;

   try
   {
      if (_pPgConn == NULL)
      {
         throw DaoConnectionException(DaoConnectionException::EXECUTE_UPDATE_FAILED,
                                      "Postgres connection not initialized. ");
      }

      if (sqlStmt.length() == 0)
      {
         throw DaoConnectionException(DaoConnectionException::BAD_SQL_CMD,
                                      "sql string is zero length ");
      }

      if (PQstatus(_pPgConn) == CONNECTION_BAD)
      {
         throw DaoConnectionException(DaoConnectionException::CONNECTION_ERROR,
                                      "Connection failed in query update.");
      }

      pPgRes = PQexec(_pPgConn, sqlStmt.c_str());

      if (pPgRes == NULL)
      {
         throw DaoConnectionException(DaoConnectionException::EXECUTE_UPDATE_FAILED,
                                      (char *)sqlStmt.c_str());
      }
      else
      {
         if (PQresultStatus(pPgRes) != PGRES_COMMAND_OK)
         {
            PQclear(pPgRes);
            throw DaoConnectionException(DaoConnectionException::EXECUTE_UPDATE_FAILED,
                               PQresultErrorMessage(pPgRes));
         }
         else if (PQresultStatus(pPgRes) == PGRES_EMPTY_QUERY)
         {
            PQclear(pPgRes);
            throw DaoConnectionException(DaoConnectionException::EXECUTE_UPDATE_FAILED,
                               "Sql command string is zero length.");
         }
         else
         {
            numRowsEffected = atoi(PQcmdTuples(pPgRes));
         }

         PQclear(pPgRes);
      }
   }
   catch (DaoConnectionException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      PQclear(pPgRes);
      throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                   (char *)sqlStmt.c_str());
   }
   catch (BasicException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      PQclear(pPgRes);
      throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                   (char *)sqlStmt.c_str());
   }
   catch (...)
   {
      PQclear(pPgRes);
      throw DaoConnectionException(DaoConnectionException::EXECUTE_QUERY_FAILED,
                                   (char *)sqlStmt.c_str());
   }

   return numRowsEffected;
}
