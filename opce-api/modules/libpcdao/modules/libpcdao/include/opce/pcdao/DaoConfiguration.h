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

#ifndef _DaoConfiguration_H
#define	_DaoConfiguration_H

#include <string>

namespace pcdao
{

class DaoConfiguration
{
public:
   DaoConfiguration();
   DaoConfiguration(const DaoConfiguration& orig);
   virtual ~DaoConfiguration();

public:
   void initialize(std::string daoName);
   void initialize(std::string daoName, std::string pathname);
   void open();
   void close();

public:
   void setDaoName();

   std::string getDaoName()
   {
      return _dbType;
   }

   void setDBType();

   std::string getDBType()
   {
      return _dbType;
   }

   void setHost();

   std::string getHost()
   {
      return _host;
   }

   void setPort();

   int getPort()
   {
      return _port;
   }

   void setDBname();

   std::string getDBname()
   {
      return _dbName;
   }

   void setUser();

   std::string getUser()
   {
      return _user;
   }

   void setPassword();

   std::string getPassword()
   {
      return _password;
   }

   void setNumConnections();

   int getNumConnections()
   {
      return _numConnections;
   }

protected:
   std::string _configPathname;
   std::string _daoName;
   std::string _dbType;
   std::string _host;
   int _port;
   std::string _dbName;
   std::string _user;
   std::string _password;
   int _numConnections;
   // TODO: scott
   // pcff::XMLFile _xmlConfigFile;

protected:
   static char *DAO_CONFIG_FILE;
};

};

#endif	/* _DaoConfiguration_H */

