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

// query returns DaoObject, DaoObjectList, or ResultSet??
// probably a ResultSet. Then Result is parsed inthe DaoObject

#ifndef _CONNECTION_H
#define	_CONNECTION_H

#include <string>
#include <pcdao/Result.h>
#include <pcdao/ConnectionDirector.h>

namespace pcdao
{

class Connection
{
public:
   Connection(ConnectionDirector &myDirector);
   virtual ~Connection();

public:
   static void dbLoggingCB(void* arg, const char* msg);


public:
   virtual void initialize(std::string &daoName,
                           std::string &databaseName,
                           std::string &host,
                           int port,
                           std::string &user,
                           std::string &password) = 0;

public:

   typedef enum
   {
      Uninitialized,
      Initialized,
      Free,
      InUse,
      Broken,
      Closed
   } eConnStatus;

   void setIsFree()
   {
      _connStatus = Free;
   }

   void setIsInUse()
   {
      _connStatus = InUse;
   }

   void setIsBroken()
   {
      _connStatus = Broken;
   }

   bool isFree()
   {
      return (_connStatus == Free) ? true : false;
   }

public:
   virtual pcdao::Result* execQuery(std::string &sql) = 0;
   virtual int execUpdate(std::string &sql) = 0;


protected:
   virtual std::string createConnectionString(std::string &databaseName,
                                              std::string &host,
                                              int port,
                                              std::string &user,
                                              std::string &pswd) = 0;

public:

   std::string &getDBName()
   {
      return _databaseName;
   }

   std::string &getHost()
   {
      return _hostName;
   }

   int getPort()
   {
      return _port;
   }

   std::string &getUser()
   {
      return _user;
   }

   std::string &getPassword()
   {
      return _password;
   }

public:
   void inUse();
   void release();

protected:
   ConnectionDirector &_connDirector;
   std::string _daoName;
   std::string _databaseName;
   std::string _hostName;
   int _port;
   std::string _user;
   std::string _password;
   eConnStatus _connStatus;
};

};

#endif	/* _CONNECTION_H */
