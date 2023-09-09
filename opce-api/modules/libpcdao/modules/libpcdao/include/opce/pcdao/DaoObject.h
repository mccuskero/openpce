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

// The DaoObject is the statement
// Database->getDaoObject(type)
// obj->preparestatement()
// obj->executeQuery
//

#ifndef _DAOOBJECT_H
#define	_DAOOBJECT_H

#include <string>

#include <pcdao/DaoObject.h>
#include <pcdao/Result.h>
#include <pcdao/ResultList.h>

namespace pcdao
{
class DaoObjectList;

class DaoObject
{
public:
   DaoObject();
   DaoObject(const DaoObject& orig);
   virtual ~DaoObject();

public:
   virtual void prepareInsertStatement(std::string &sqlStmt) = 0;
   virtual void prepareQueryStatement(std::string &sqlStmt) = 0;

public:
   virtual void store() = 0;
   virtual int retrieve(ResultList &rList) = 0;
   virtual int retrieve(ResultList &rList, std::string orderBy) = 0;

private:
   virtual int populateList(ResultList &rList, Result &result) = 0;

protected:
   std::string _orderByClause;
   std::string _queryConstraint;

};

};

#endif	/* _DAOOBJECT_H */

