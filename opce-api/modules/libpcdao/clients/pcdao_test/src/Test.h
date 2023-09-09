/* 
 * File:   test.h
 * Author: owenm
 *
 * Created on March 25, 2009, 10:28 PM
 */

#ifndef _TEST_H
#define	_TEST_H

#include <time.h>

#include <pcdao/ResultListItem.h>
#include <pcdao/ResultList.h>

class Test : public pcdao::ResultListItem
{
    friend class TestDao;

public:
    Test();
    Test(const Test& orig);
    virtual ~Test();

public:
   void initialize();


public:
   void setLong(long in) { _myLong = in; }
   void setString(std::string in) { _myString = in; }
   void setTime(time_t in) { _myTime = in; }

public:
    virtual void store();
    virtual void retrieve(pcdao::ResultList &rl);

private:
    std::string _myString;
    long        _myLong;
    time_t      _myTime;
};

#endif	/* _TEST_H */

