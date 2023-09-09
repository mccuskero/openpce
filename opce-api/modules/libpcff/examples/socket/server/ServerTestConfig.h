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

#include <string>

#ifndef _ServerTestConfig_h
#define _ServerTestConfig_h


#include <pcff/Config.h>


class ServerTestConfig : public pcff::Config 
{
public:
   static ServerTestConfig &instance();
   virtual void init();
   virtual void initAttributes();  
   
private:
   ServerTestConfig();
   virtual ~ServerTestConfig();
   
public: 
   int         getListenPort();
   std::string getListenHost();
   int        getMaxLogSize();
   int        getPoolSize();
   bool        noDaemon();
   int         getAddrFamily();
   
public:
      typedef enum {
         Unknown,
         Exception,
         Timer
      };
   
private:
    	virtual void printVersionHelper();
	
private:    
      static ServerTestConfig *_instance;
};


#endif
