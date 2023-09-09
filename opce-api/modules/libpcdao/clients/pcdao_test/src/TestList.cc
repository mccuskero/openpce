/* 
 * File:   testlist.cc
 * Author: owenm
 * 
 * Created on July 30, 2009, 11:01 AM
 */

#include "TestDatabase.h"
#include "Test.h"

#include "TestList.h"

using namespace std;
using namespace pcdao;

TestList::TestList()
{
}

TestList::~TestList()
{
	  Test *pTest = NULL;
	  for (TestListVector::iterator it = _testListVector.begin(); it != _testListVector.end(); it++ )
	  {
		  pTest = *it;
		  if (pTest != NULL)
		  {
			delete pTest;
		  }
	  }

	  _testListVector.clear();
}

void TestList::add(ResultListItem *test)
{
	if (test != NULL)
	{
		_testListVector.push_back((Test *)test);
	}
	else
	{
//		throw TestException(TestException::ListItemIsNull, "TestList::add");
	}
}

void TestList::clear(void)
{
	if (_testListVector.size() > 0)
		_testListVector.clear();
}

void TestList::set(TestListVector &clv)
{
	_testListVector.assign(clv.begin(), clv.end());
}

ResultListItem &TestList::operator[](int idx)
{
	Test *pTest = NULL;

	if ( idx < _testListVector.size() )
	{
		pTest = _testListVector[idx];
	}
	else
	{
//		throw TestException(TestException::InvalidIdxForList, "TestList::operator[]");
	}

	return *pTest;
}

int TestList::size()
{
	return _testListVector.size();
}

void TestList::retrieve()
{
    TestDao *testDao = NULL;
    testDao = TestDatabase::instance().getTestDao();
    testDao->retrieve(*this);
}
