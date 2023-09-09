/* 
 * File:   testlist.h
 * Author: owenm
 *
 * Created on July 30, 2009, 11:01 AM
 */


// uses a TestDao to retrieve a list of results from a database
// may need to set a constraint sometime

#ifndef _TESTLIST_H
#define	_TESTLIST_H

#include <vector>

#include <pcdao/ResultListItem.h>
#include <pcdao/ResultList.h>

#include "Test.h"

class TestList : public pcdao::ResultList
{
public:
   TestList();
   virtual ~TestList();

public:
   typedef std::vector<Test *> TestListVector;

public:
   void set(TestListVector &clv);

public:
   virtual pcdao::ResultListItem & operator[](int idx);
   virtual int size();
   virtual void add(pcdao::ResultListItem *rlItem);
   virtual void clear();

public:
   virtual void retrieve();

private:
   TestListVector _testListVector;
};

#endif	/* _TESTLIST_H */

