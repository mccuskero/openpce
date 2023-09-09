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

#include <pcff/BasicExceptionMgr.h>

#include <pcdao/Database.h>
#include <pcdao/DaoConnectionException.h>
#include <pcdao/DaoConfiguration.h>

#include "driver/pgsql/PgSqlConnection.h"

#include <pcdao/ConnectionDirector.h>

using namespace std;
using namespace pcff;
using namespace pcdao;

ConnectionDirector::ConnectionDirector(DaoConfiguration &daoConfig)
: _daoConfig(daoConfig)
{
}

ConnectionDirector::~ConnectionDirector()
{
}

void ConnectionDirector::initialize(DaoConfiguration &daoConfig)
{
   BasicExceptionMgr::instance()->log(LogLevel::Info, "\n\nConnection Info From rdbms.conf:\n \
            dataSource Name:    %s\n \
            database Type:      %s\n \
            database Name:      %s\n \
            host:               %s\n \
            port:               %i\n \
            userName:           %s\n \
            password:           %s\n \
            number connections: %i\n",
                                      daoConfig.getDaoName().c_str(),
                                      daoConfig.getDBType().c_str(),
                                      daoConfig.getDBname().c_str(),
                                      daoConfig.getHost().c_str(),
                                      daoConfig.getPort(),
                                      daoConfig.getUser().c_str(),
                                      daoConfig.getPassword().c_str(),
                                      daoConfig.getNumConnections());


   createConnections(daoConfig.getDaoName(),
                     daoConfig.getDBType(),
                     daoConfig.getDBname(),
                     daoConfig.getHost(),
                     daoConfig.getPort(),
                     daoConfig.getUser(),
                     daoConfig.getPassword(),
                     daoConfig.getNumConnections());
}

void ConnectionDirector::createConnections(string dsname,
                                           string dbType,
                                           string dbname,
                                           string host,
                                           int port,
                                           string user,
                                           string password,
                                           int numConnections)
{
   Connection *pConn = NULL;

   for (int i = 0; i < numConnections; i++)
   {
      if (dbType.compare(Database::PgSQL) == 0)
      {
         pConn = new PgSqlConnection(*this);
      }
      else if (Database::MySQL)
      {
         throw DaoConnectionException(DaoConnectionException::CONNECTION_TYPE_NOT_SUPPORTED,
                                      "MySQL not supported");
      }
      else if (Database::Oracle)
      {
         throw DaoConnectionException(DaoConnectionException::CONNECTION_TYPE_NOT_SUPPORTED,
                                      "Oracle not supported");
      }
      else
      {
         throw DaoConnectionException(DaoConnectionException::CONNECTION_TYPE_NOT_SUPPORTED,
                                      "Unknown type");
      }

      pConn->initialize(dsname, dbname, host, port, user, password);
      _connectionList.push_back(pConn);
   }
}

Connection* ConnectionDirector::getConnection()
{
   Connection* pConn = NULL;
   try
   {
      pConn = getFreeConnection();

      if (pConn == NULL)
      {
         throw DaoConnectionException(DaoConnectionException::GET_CONNECTION_ERROR,
                                      "Connection not found.");
      }
   }
   catch (DaoConnectionException& ex)
   {
      pConn = NULL;
      BasicExceptionMgr::instance()->handle(ex);
   }
   catch (BasicException& ex)
   {
      pConn = NULL;
      BasicExceptionMgr::instance()->handle(ex);
   }
   catch (...)
   {
      pConn = NULL;
      BasicExceptionMgr::instance()->handle("ConnectionDirector::getConnection: unknown ERROR");
   }

   pConn->setIsInUse();

   return pConn;
}

Connection* ConnectionDirector::getFreeConnection()
{
   int currIdx = 0;
   Connection *pConn = NULL;
   ConnectionList::iterator iter;

   iter = _connectionList.begin();
   while (iter != _connectionList.end() && pConn != NULL)
   {
      pConn = *iter;

      if (pConn->isFree() != true)
      {
         pConn = NULL;
      }

      ++iter;
   }

   if (iter == _connectionList.end() && pConn == NULL)
   {

      throw DaoConnectionException(DaoConnectionException::GET_CONNECTION_ERROR,
                                    "No Free Connections");
   }

   return pConn;
}

void ConnectionDirector::releaseConnection(Connection *pConn)
{
   try
   {
      if (pConn == NULL)
      {
         throw DaoConnectionException(DaoConnectionException::RELEASE_CONNECTION_ERROR,
                                      "Connection passed in is NULL.");
      }

      pConn->setIsFree();
   }
   catch (DaoConnectionException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
   }
   catch (BasicException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
   }
   catch (...)
   {
      BasicExceptionMgr::instance()->handle("ConnectionDirector::releaseConnection: unknown ERROR");
   }
}
