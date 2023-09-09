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

/*
 * The Database object is used by an instance of a Dao to retreive
 * a result, and then turn it into a business object.
 *
 * daObj->retrieve(list);
 *
 * The dao is also gets a transtaction to store an object.
 * daObj->store();
 *
 * Note, one Dao can store to multiple tables in one transaction.
 *
 */

#ifndef _DATABASE_H
#define	_DATABASE_H

#include <string>

#include <pcdao/DbTools.h>
#include <pcdao/DaoConfiguration.h>
#include <pcdao/Cursor.h>
#include <pcdao/Transaction.h>

namespace pcdao
{
class ConnectionDirector;

class Database
{
public:
   Database();
   Database(const Database& orig);
   virtual ~Database();

private:
   void initMembers();

public:
   virtual void initialize(std::string &daoName) = 0;
   virtual void close() = 0;

public:
   virtual Transaction *getTransaction() = 0;
   virtual Cursor *getCursor() = 0;
   virtual DbTools *getDbTools() = 0;

public:
   enum
   {
      MAX_HOST_STRING_LEN = 255,
      MAX_DATABASENAME_LEN = 255,
      MAX_USERNAME_LEN = 255,
   };

public:
   static char *PgSQL;
   static char *MySQL;
   static char *Oracle;

private:
   DaoConfiguration *_daoConfig;
   ConnectionDirector *_connDirector;
   std::string _daoName;
   int _driverType;
   int _numConnections;
};

};

#endif	/* _DATABASE_H */

