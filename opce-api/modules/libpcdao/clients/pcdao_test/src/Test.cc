/* 
 * File:   test.cc
 * Author: owenm
 * 
 * Created on March 25, 2009, 10:28 PM
 */

#include <pcff/TimeStamp.h>

#include "TestDatabase.h"
#include "TestDao.h"

#include "Test.h"

using namespace pcff;
using namespace pcdao;

Test::Test()
{
    _myLong = 10;
    _myString = "myString";
    _myTime = time(NULL);
}

Test::Test(const Test& orig)
{
}

Test::~Test()
{
}

void Test::initialize()
{
   TimeStamp time(TimeStamp::SetToNow);

   time.get(_myTime);
}


void Test::store()
{
    TestDao *testDao = NULL;
    testDao = TestDatabase::instance().getTestDao();
    testDao->set(this);
    testDao->store();
    delete testDao;
}

void Test::retrieve(pcdao::ResultList &rl)
{
    TestDao *testDao = NULL;
    testDao = TestDatabase::instance().getTestDao();
    testDao->retrieve(rl);
}
