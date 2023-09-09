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

#include <pcdao/Cursor.h>
#include <pcdao/Transaction.h>
#include <pcdao/ConnectionDirector.h>

#include <pcdao/Database.h>

using namespace std;
using namespace pcdao;


char* Database::PgSQL = "PgSQL";
char* Database::MySQL = "MySQL";
char* Database::Oracle = "Oracle";

Database::Database()
{
   initMembers();
}

Database::Database(const Database& orig)
{
}

Database::~Database()
{
   if (_connDirector != NULL)
   {
      delete _connDirector;
      _connDirector = NULL;
   }

   if (_daoConfig != NULL)
   {
      delete _daoConfig;
      _daoConfig = NULL;
   }
}

void Database::initMembers()
{
   _connDirector = NULL;
   _daoConfig = NULL;
}

//
// TODO: calling member TesseraDatabase::initialize() passes in dsname
//

void Database::initialize(string &dsname)
{
   _daoName = dsname;
   _daoConfig = new DaoConfiguration;
   _daoConfig->initialize(_daoName);
   _connDirector = new ConnectionDirector(*_daoConfig);
}

void Database::close()
{

}

Transaction *Database::getTransaction()
{

}

Cursor *Database::getCursor()
{

}
