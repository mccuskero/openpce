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

// TODO: scott
// #include <pcff/XMLFile.h>

#include <pcdao/DaoConfiguration.h>

using namespace std;
using namespace pcdao;

char* DaoConfiguration::DAO_CONFIG_FILE = "$PCDAO_CONFIG_SEARCH_PATH/pcdao.xml";

DaoConfiguration::DaoConfiguration()
{
}

DaoConfiguration::DaoConfiguration(const DaoConfiguration& orig)
{
}

DaoConfiguration::~DaoConfiguration()
{

}

void DaoConfiguration::initialize(string daoName)
{
   _daoName = daoName;
   _configPathname = DAO_CONFIG_FILE;
   initialize(daoName, _configPathname);
}

void DaoConfiguration::initialize(string daoName, string configPathname)
{
   open();
   setDaoName();
   setDBType();
   setHost();
   setPort();
   setDBname();
   setUser();
   setPassword(); // need to check this out, make secure
   close();
}

void DaoConfiguration::open()
{
   //_xmlConfigFile.open(_configPathname)
}

void DaoConfiguration::close()
{
   //_xmlConfigFile.close()
}

void  DaoConfiguration::setDaoName()
{
   _dbType = "test";
}

void DaoConfiguration::setDBType()
{
   _dbType = "pgsql";
}

void DaoConfiguration::setHost()
{
   _host = "localhost";
}

void DaoConfiguration::setPort()
{
   _port = 5432;
}

void DaoConfiguration::setDBname()
{
   _dbName = "test";
}

void DaoConfiguration::setUser()
{
   _user = "owenm";
}

void DaoConfiguration::setPassword()
{
   _password = "";
}

void DaoConfiguration::setNumConnections()
{
   _numConnections = 10;
}
