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

#ifndef ThreadConfig_h
#define ThreadConfig_h

#include <pcff/Config.h>


class ThreadConfig : public pcff::Config {
   public:
      static ThreadConfig &instance();

   private:
      ThreadConfig();
      virtual ~ThreadConfig();
      virtual void initAttributes();  

   public: 
      virtual    void  printUsageHelper( char *name = NULL );
      int              getTestType(); 
      int              getIterations();
      int              getNumThreads();

   public:
      typedef enum {
         BasicTest = 1,
         ContainmentTest,
         PoolTest,
         DerivedPoolTest,
         UnknownTest
      };

    private:
      virtual void printVersionHelper();
	
    private:    
      int                 _testType;
      static ThreadConfig *_instance;
};


#endif
