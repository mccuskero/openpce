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

#ifndef _UnixSocket_h
#define _UnixSocket_h

#include <pcff/socket/Socket.h>

namespace pcff
{

class UnixSocket : public Socket
{
 public:
  UnixSocket();
  UnixSocket(int family); 
  virtual ~UnixSocket() {}
  
 public:
  void      init();
  void      init(const char *);
  void      connect(const char *socketPath);
  void      listen(int backlog=1);
  UnixSocket *accept();
  void       disConnect();
  int        send(void *pBuffer, int len);
  int        recv(void *pBuffer, int len);

 public:
 void        bind(const char *socketPath);
 
 public:
  void    getSockOpt(int codeLevel, 
		     int optionName, 
		     void *pOptionData, 
		     int &dataLength);
  void    setSockOpt(int codeLevel, 
		     int optionName, 
		     const void *pOptionData, 
		     int dataLength);
   
};

}; // namespace

#endif
