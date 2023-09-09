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

#ifndef _PGSQLCONNECTION_H
#define	_PGSQLCONNECTION_H

#include <string>

#include <libpq-fe.h>

#include <pcdao/Result.h>
#include <pcdao/Connection.h>
#include <pcdao/ConnectionDirector.h>

namespace pcdao
{
   class Result;

class PgSqlConnection : public Connection
{
public:
   PgSqlConnection(ConnectionDirector &myDir);
   virtual ~PgSqlConnection();

public:
   virtual void initialize(std::string &daoName,
                           std::string &databaseName,
                           std::string &host,
                           int port,
                           std::string &user,
                           std::string &password);

private:
   void initMembers();

protected:
   virtual std::string createConnectionString(std::string &databaseName,
                                              std::string &host,
                                              int port,
                                              std::string &user,
                                              std::string &password);

public:
   virtual Result* execQuery(std::string &sqlStmt);
   virtual int execUpdate(std::string &sqlStmt);


private:
   PGconn* _pPgConn; 

};

};

#endif	/* _PGSQLCONNECTION_H */

