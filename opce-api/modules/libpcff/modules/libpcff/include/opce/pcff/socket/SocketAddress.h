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

#ifndef _SocketAddress_h
#define _SocketAddress_h

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>    
#include <unistd.h>
#include <string.h>

namespace pcff
{

class INETAddress;
class UnixSocketAddress;

class SocketAddress
{
 public:
  virtual void                setINETAddressToAny()					{}
  virtual void                setINETAddress(INETAddress &addr)		{}
  virtual void                getINETAddress(INETAddress &addr)		{}
  virtual void                setPort(int port)						{}
  virtual void                getPort(int &port)					{}
  virtual socklen_t           sizeOf() = 0;
  virtual sockaddr            *getSockAddrStruct() { return _pAddr; }

// Unix Domain sockets
public: 
  virtual void                getUnixAddress(UnixSocketAddress &addr)		{}
  virtual void				  setPathAddress(const char *socketPath)        {}
  virtual void				  getPathAddress(char *&socketPath)		  {}
  virtual const char         *getPathAddress()                        {}
  virtual void                updateAddressPathFromStruct()           {}

 protected:
  struct sockaddr     *_pAddr;
};

};

#endif
