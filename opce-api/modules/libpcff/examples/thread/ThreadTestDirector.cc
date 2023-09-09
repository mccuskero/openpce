/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@myeastern.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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

#include <stdio.h>
#include <string.h>
#include <iostream>


#include <pcff/BasicException.h>
#include <pcff/Task.h>

#include "ThreadTask.h"
#include "ThreadTaskContain.h"
#include "ThreadTaskPool.h"
#include "ThreadTaskServerPool.h"
#include "ThreadConfig.h"
#include "ThreadTestDirector.h"

using namespace std;
using namespace pcff;

//
//
//
bool ThreadTestDirector::run(int argc, char** argv) 
{

  Task* task = getTask( argc, argv );

  if ( task != NULL ) 
  {
    task->run();
  }
  else 
  {
    throw BasicException( BasicException::ERR,
			  BasicException::UnitTestNotFound,
			  "ThreadTestDirector::run Unknown task");
  }

  delete task;

  return true;
}

//
//
//
Task* ThreadTestDirector::getTask(int argc, char** argv) 
{
  Task* task= NULL;

  if ( ThreadConfig::instance().getTestType() == ThreadConfig::BasicTest )
  {
    task = new ThreadTask();
  }
  else if ( ThreadConfig::instance().getTestType() == ThreadConfig::ContainmentTest )
  {
     task = new ThreadTaskContain();
  }  
  else if ( ThreadConfig::instance().getTestType() == ThreadConfig::PoolTest )
  {
     task = new ThreadTaskPool();
  }  
  else if ( ThreadConfig::instance().getTestType() == ThreadConfig::DerivedPoolTest )
  {
     task = new ThreadTaskServerPool();
  }  
  else
  {
    throw BasicException( BasicException::ERR,
		   	  BasicException::UnitTestNotFound,
			  "task is undefined"  );
  }

  return task;
}
