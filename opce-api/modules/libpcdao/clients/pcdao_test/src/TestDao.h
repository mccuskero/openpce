/* 
 * File:   testdao.h
 * Author: owenm
 *
 * Created on February 20, 2009, 9:24 PM
 */

#ifndef _TESTDAO_H
#define	_TESTDAO_H

#include <string>

#include <pcdao/DaoObject.h>
#include <pcdao/ResultList.h>

class Test;

class TestDao : public pcdao::DaoObject
{
public:
   TestDao();
   TestDao(const TestDao& orig);
   virtual ~TestDao();

public:
   enum
   {
      NumTestListQueryFields = 3
   };

public:
   void set(Test *pTest);

public:
   virtual void prepareInsertStatement(std::string &sqlStmt);
   virtual void prepareQueryStatement(std::string &sqlStmt);

public:
   virtual void store();
   virtual int retrieve(pcdao::ResultList &rList);
   virtual int retrieve(pcdao::ResultList &rList, std::string orderBy);

private:
   virtual int populateList(pcdao::ResultList &rList, pcdao::Result &result);

private:
   Test *_data;
};

#endif	/* _TESTDAO_H */

