/* 
 * File:   testdatabase.h
 * Author: owenm
 *
 * Created on February 20, 2009, 9:24 PM
 */

#ifndef _TESTDATABASE_H
#define	_TESTDATABASE_H

#include <string>

#include <pcdao/Transaction.h>
#include <pcdao/Cursor.h>
#include <pcdao/Database.h>

#include "TestDao.h"

class TestDatabase : pcdao::Database
{
public:
   static TestDatabase &instance();
   static void shutdown();

public:
   void initialize(std::string &dsname);
   void close();

private:
   TestDatabase();
   TestDatabase(const TestDatabase& orig);
   virtual ~TestDatabase();
   void initMembers();

public:
   virtual pcdao::Transaction *getTransaction();
   virtual pcdao::Cursor *getCursor();
    virtual pcdao::DbTools *getDbTools();

public:
   TestDao *getTestDao();

public:
   static TestDatabase *_instance;

};

#endif	/* _TESTDATABASE_H */

