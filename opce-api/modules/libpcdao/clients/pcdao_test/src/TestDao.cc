/* 
 * File:   testdao.cc
 * Author: owenm
 * 
 * Created on February 20, 2009, 9:24 PM
 */

#include <sstream>
#include <string>

#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>
#include <pcff/Timer.h>


#include <pcdao/DaoException.h>
#include <pcdao/Transaction.h>
#include <pcdao/ResultList.h>

#include "Test.h"
#include "TestList.h"
#include "TestDatabase.h"
#include "TestDao.h"

using namespace std;
using namespace pcff;
using namespace pcdao;

TestDao::TestDao()
{
}

TestDao::TestDao(const TestDao& orig)
{
}

TestDao::~TestDao()
{
}

void TestDao::set(Test *pTest)
{
   _data = pTest;
}

void TestDao::store()
{
   string sqlStmt;
   int numRecsEffected = 0;
   Transaction *pTrans = NULL;

   if (_data != NULL)
   {
      try
      {
         pTrans = TestDatabase::instance().getTransaction();
         pTrans->begin();
         prepareInsertStatement(sqlStmt);

         numRecsEffected = pTrans->execUpdate(sqlStmt);

         if (numRecsEffected <= 0)
         {
            BasicExceptionMgr::instance()->log(LogLevel::Error, "TestDao::store - network flow insert failed.");
         }
         else
         {
            BasicExceptionMgr::instance()->log(LogLevel::Debug, "TestDao::store - success. number records effected: %d", numRecsEffected);
         }

         pTrans->commit();
         delete pTrans;
      }
      catch (BasicException &ex)
      {
         BasicExceptionMgr::instance()->handle(ex);
         pTrans->rollback();
         delete pTrans;
      }
      catch (...)
      {
         BasicExceptionMgr::instance()->log(LogLevel::Error, "TestDao::store - exception caught.");
         pTrans->rollback();
         delete pTrans;
      }
   }
   else
   {
      BasicExceptionMgr::instance()->log(LogLevel::Error, "NetworkFlowDfsDataDbi::store - data is NULL.");
   }
}

void TestDao::prepareInsertStatement(string &sqlStmt)
{
   stringstream sstr;
   timeval tvReceiveTime;
   string recvTime = "";
   long recvTimeSec = 0;
   long recvTimeUsec = 0;
   time_t tt;
   DbTools *dbtools = NULL;


   tt = _data->_myTime;

   // TODOL use Timestamp to get string
   dbtools = TestDatabase::instance().getDbTools();
   recvTime = dbtools->time_t2tring(tt);

   // manage the tablename from the datastore
   sstr << "Insert into pcdao_test ( "\
			"test_string, "\
			"test_long, "\
			"receive_time) Values ";

   // get the Values
   sstr << "(" << _data->_myString << ", "\
					<< _data->_myLong << ", "\
					<< recvTime << ")";

   sqlStmt = sstr.str();
}

int TestDao::retrieve(ResultList &rList)
{
   TestList &contactList = (TestList &) rList;
   Transaction *pTrans = NULL;

   long numRecsEffected = 0;
   string stmt = "";
   Result *rs = NULL;
   int logLevel = BasicExceptionMgr::instance()->getLogLevel();
   Timer *chrono = NULL;

   pTrans = TestDatabase::instance().getTransaction();

   prepareQueryStatement(stmt);

   try
   {
      if (logLevel >= LogLevel::Debug)
      {
         chrono = new Timer;
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "Starting database transaction timing.");
         chrono->start();
      }

      pTrans->begin();
      rs = pTrans->execQuery(stmt);

      if (rs != NULL)
      {
         numRecsEffected = rs->getNumRows();
         populateList(contactList, *rs);
         delete rs;
      }
      else
      {
         throw DaoResultException(DaoResultException::RESULT_SET_NOT_INITIALIZED, "TestDao::retrieve");
      }
   }
   catch (...)
   {
      BasicExceptionMgr::instance()->log(LogLevel::Error, "Exception caught in ContactDbi::store");
      pTrans->rollback();

      if (chrono)
      {
         chrono->stop();
         delete chrono;
      }
   }

   pTrans->commit();

   if (logLevel >= LogLevel::Debug)
   {
      double elapsedTime = 0.0;

      chrono->stop();
      elapsedTime = chrono->getTime();
      // unless another method is setting numRecsEffected, it will always be zero.
      BasicExceptionMgr::instance()->log(LogLevel::Debug, "Query returned %d records in %.3f seconds", numRecsEffected, elapsedTime);
      delete chrono;
   }

   delete pTrans;

   return numRecsEffected;
}

void TestDao::prepareQueryStatement(string &sqlStmt)
{
   stringstream sstr;

   // manage the tablename from the datastore
   sstr << "Select test_string, test_long, receive_time from pcdao_test";

   if (_queryConstraint.length() != 0)
   {
      sstr << " ";
      sstr << _queryConstraint;
   }

   if (_orderByClause.length() != 0)
   {
      sstr << " ";
      sstr << _orderByClause;
   }

   sqlStmt = sstr.str();
}

int TestDao::retrieve(ResultList &rList, string orderBy)
{
   TestList &testList = (TestList &) rList;

   long numRecsEffected = 0;
   string stmt = "";
   Result *rs = NULL;
   Transaction *pTrans = NULL;
   int logLevel = BasicExceptionMgr::instance()->getLogLevel();
   Timer *chrono = NULL;

   pTrans = TestDatabase::instance().getTransaction();

   _orderByClause = orderBy;
   prepareQueryStatement(stmt);

   try
   {
      if (logLevel >= LogLevel::Debug)
      {
         chrono = new Timer;
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "Starting database transaction timing.");
         chrono->start();
      }

      pTrans->begin();

      rs = pTrans->execQuery(stmt);

      if (rs != NULL)
      {
         numRecsEffected = rs->getNumRows();
         populateList(testList, *rs);
         delete rs;
      }
      else
      {
         throw DaoResultException(DaoResultException::RESULT_SET_NOT_INITIALIZED, "TestDao::retrieve");
      }
   }
   catch (...)
   {
      BasicExceptionMgr::instance()->log(LogLevel::Error, "Exception caught in ContactDbi::store");
      pTrans->rollback();

      if (chrono)
      {
         chrono->stop();
         delete chrono;
      }
   }

   pTrans->commit();

   if (logLevel >= LogLevel::Debug)
   {
      double elapsedTime = 0.0;

      chrono->stop();
      elapsedTime = chrono->getTime();
      // unless another method is setting numRecsEffected, it will always be zero.
      BasicExceptionMgr::instance()->log(LogLevel::Debug, "Query returned %d records in %.3f seconds", numRecsEffected, elapsedTime);
      delete chrono;
   }

   delete pTrans;

   return numRecsEffected;
}

int TestDao::populateList(ResultList &rlist, Result &rs)
{
   TestList &tlist = (TestList &) rlist;
   Test *test = NULL;

   int numRows = rs.getNumRows();
   int numFields = rs.getNumFields();

   BasicExceptionMgr::instance()->log(LogLevel::Debug, "Retrieving data from result set");

   if (numFields == NumTestListQueryFields)
   {
      for (int rowIdx = 0; rowIdx < numRows; rowIdx++)
      {
         test = new Test;

         //         test->setContactId(rs.getValueAsInt(rowIdx, ContactDbi::ContactID_IDX));
         //         test->setInetAddr(rs.getValue(rowIdx, ContactDbi::InetAddr_IDX));
         //         test->setCidrAddr(rs.getValue(rowIdx, ContactDbi::Network_IDX));
         //         test->setFirstTime(rs.getValueAsLong(rowIdx, ContactDbi::FirstTime_IDX));
         //         test->setLatestTime(rs.getValueAsLong(rowIdx, ContactDbi::LatestTime_IDX));
         //         test->setContactType(string2contactTypeEnum(rs.getValue(rowIdx, ContactDbi::ContactType_IDX)));
         //         test->setContactDomainType(string2contactDomainTypeEnum(rs.getValue(rowIdx, ContactDbi::ContactDomainType_IDX)));
         //         test->setDomainType(string2domainTypeEnum(rs.getValue(rowIdx, ContactDbi::ContactDomainInstanceType_IDX)));
         //         test->setFlowCount(rs.getValueAsInt(rowIdx, ContactDbi::FlowCount_IDX));

         tlist.add(test);
      }
   }

   return numRows;
}
