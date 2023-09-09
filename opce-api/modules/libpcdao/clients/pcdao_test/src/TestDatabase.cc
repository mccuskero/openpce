/* 
 * File:   testdatabase.cc
 * Author: owenm
 * 
 * Created on February 20, 2009, 9:24 PM
 */

#include <pcdao/Database.h>

#include "TestDao.h"
#include "TestDatabase.h"

using namespace std;
using namespace pcdao;

TestDatabase *TestDatabase::_instance = NULL;

TestDatabase &TestDatabase::instance()
{
   if (_instance == NULL)
   {
      _instance = new TestDatabase;
   }

   return *_instance;
}

void TestDatabase::shutdown()
{
   if (_instance != NULL)
   {
      delete _instance;
      _instance = NULL;
   }

}

TestDatabase::TestDatabase()
{
   initMembers();
}

TestDatabase::TestDatabase(const TestDatabase& orig)
{
}

TestDatabase::~TestDatabase()
{
}

void TestDatabase::initMembers()
{
   _instance = NULL;
}

void TestDatabase::initialize(string &dsname)
{
   Transaction *pTrans = NULL;
   string stmt = "create table test (id integer not null primary key, name varchar(40), test_test timestamp not null)";

   Database::initialize(dsname);

   pTrans = getTransaction();

   if (pTrans != NULL)
   {
      pTrans->execQuery(stmt);
      delete pTrans;
   }
   else
   {
      // TODO: throw exception
   }
}

void TestDatabase::close()
{
   Transaction *pTrans = NULL;
   string stmt = "drop table test";
   // to specific TestDatabase stuff here
   // create table
   pTrans = getTransaction();

   if (pTrans != NULL)
   {
      pTrans->execQuery(stmt);
      delete pTrans;
   }
   else
   {
      // TODO: throw exception
   }

   Database::close();
}

TestDao *TestDatabase::getTestDao()
{
   TestDao *pTestDao = NULL;

   return pTestDao;
}

Transaction *TestDatabase::getTransaction()
{
   Transaction *pTrans = NULL;

   return pTrans;
}

Cursor *TestDatabase::getCursor()
{
   Cursor *pCursor = NULL;

   return pCursor;
}

DbTools *TestDatabase::getDbTools()
{
   DbTools *dbTools = NULL;

   return dbTools;
}
