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

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/IPv4SocketAddress.h>

using namespace pcff;

IPv4SocketAddress::IPv4SocketAddress()
{
    bzero(&_sockAddrStruct, sizeof(_sockAddrStruct));

    _pAddr = (sockaddr *)&_sockAddrStruct;
    _pIP4Addr = (sockaddr_in *)&_sockAddrStruct;
}

void IPv4SocketAddress::setINETAddressToAny()
{
   _pAddr->sa_family = AF_INET;
   _pIP4Addr->sin_addr.s_addr = htonl(INADDR_ANY);
}

void IPv4SocketAddress::setINETAddress(INETAddress &addr)
{
   _pAddr->sa_family = addr.getAddressFamily();
   if (_pAddr->sa_family == AF_INET)
   {
      bcopy(addr.getInAddr(), &(_pIP4Addr->sin_addr), addr.getInAddrLen());
   }
   else
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }
}

void IPv4SocketAddress::getINETAddress(INETAddress &addr)
{
   if (_pAddr->sa_family == AF_INET)
   {
      addr.setAddress((unsigned char*)&_pIP4Addr->sin_addr, sizeof(in_addr));
   }
   else
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }
}

void IPv4SocketAddress::setPort(int port)
{
   if (_pAddr->sa_family == AF_INET)
   {
       _pIP4Addr->sin_port = htons(port);
   }
   else
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }
}

void IPv4SocketAddress::getPort(int &port)
{
   if (_pAddr->sa_family == AF_INET)
   {
      port = ntohs(_pIP4Addr->sin_port);
   }
   else
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }
}

socklen_t IPv4SocketAddress::sizeOf()
{
  return(sizeof(sockaddr_in));
}
