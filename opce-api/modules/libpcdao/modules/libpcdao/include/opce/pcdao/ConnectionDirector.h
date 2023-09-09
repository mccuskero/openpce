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

#ifndef _ConnectionDirector_H
#define	_ConnectionDirector_H

#include <vector>

#include <pcdao/DaoConfiguration.h>

namespace pcdao
{
class Connection;

class ConnectionDirector
{
public:
   ConnectionDirector(DaoConfiguration& config);
   virtual ~ConnectionDirector();

public:
   void initialize(DaoConfiguration &daoConfig);


public:
   Connection* getConnection();
   void releaseConnection(Connection* pConn);

private:
   void createConnections(std::string dsname,
                          std::string dbType,
                          std::string dbname,
                          std::string host,
                          int port,
                          std::string user,
                          std::string password,
                          int numConnections);
   Connection* getFreeConnection();



private:
   typedef std::vector<Connection *> ConnectionList;

private:
   DaoConfiguration &_daoConfig;
   ConnectionList _connectionList;
};

};

#endif	/* _ConnectionDirector_H */

