


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

#include <pcff/BasicExceptionMgr.h>

#include <pcdao/DaoException.h>

#include "Test.h"
#include "TestList.h"
#include "TestDatabase.h"


using namespace std;
using namespace pcff;
using namespace pcdao;

void printBanner();
void printEndMessage();
void testMain(int argv, char** argc);
void logCB(char *fmt, ...);

int
main(int argv, char** argc)
{
   printBanner();
   testMain(argv, argc);
   printEndMessage();
}

/**
 * logging function
 */
void
logCB(const char *fmt, ...)
{
   static char str[500]; /* some insanely large number? */
   va_list ap;

   va_start(ap, fmt);

   vsprintf(str, fmt, ap);
   //  fprintf(stderr, str, va_arg( ap, va_list ) );

   va_end(ap);

   cout << "logCB: " << str << endl;
}




//
//
//

void
testMain(int argv, char** argc)
{

   string dsName = "test";
   TestDao *pTestDao = NULL;

   cout << endl;
   cout << "starting test..." << endl;

   try
   {
      // TestDatabase will create table here
      cout << "Initializing database" << endl;
      TestDatabase::instance().initialize(dsName);

      cout << "setting log callback" << endl;
      BasicExceptionMgr::instance()->setLogCallback(&logCB);
      BasicExceptionMgr::instance()->setLogLevel(LogLevel::Info);


      // perform some simple tests
      Test test;

      test.setLong(12345);
      test.setString("12345");
      test.store();
      test.setLong(54321);
      test.setString("54321");
      test.store();

      // perform retrieve test with Dao object
      TestList testList;
      testList.retrieve();

      // close the database, TestDatabase will drop table here
      TestDatabase::instance().close();
      TestDatabase::shutdown();
   }
   catch (DaoConnectionException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      // close the database
      TestDatabase::instance().close();
      TestDatabase::shutdown();
   }
   catch (DaoResultException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      // close the database
      TestDatabase::instance().close();
      TestDatabase::shutdown();
   }
   catch (BasicException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      // close the database
      TestDatabase::instance().close();
      TestDatabase::shutdown();
   }
   catch (...)
   {
      BasicExceptionMgr::instance()->handle();
      BasicExceptionMgr::instance()->handle("testMain: Unknown Exception");
      // close the database
      TestDatabase::instance().close();
      TestDatabase::shutdown();
   }
}

//
//
//

void
printBanner()
{
   cout << endl;
   cout << endl;
   cout << "Dao Test Client - v0.0.1" << endl;
   cout << endl;
   cout << endl;
}

//
//
//

void
printEndMessage()
{
   cout << endl;
   cout << "Unit Test Complete with " << BasicExceptionMgr::instance()->getNumErrors() << " errors;" << endl;
   cout << endl;
   cout << endl;
}
