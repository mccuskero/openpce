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

#include <iostream>

#include <pcff/Log.h>
#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/BaseSocket.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/UnixSocketAddress.h>

using namespace pcff;
using namespace std;

UnixSocketAddress::UnixSocketAddress()
{
    bzero(&_sockAddrStruct, sizeof(_sockAddrStruct));

    _pAddr = (sockaddr *)&_sockAddrStruct;
    _pUnixAddr = (sockaddr_un *)&_sockAddrStruct;
	_pAddr->sa_family = BaseSocket::AF_UNIX_DOMAIN;
}


void UnixSocketAddress::setPathAddress(UnixSocketAddress &unixAddress)
{
   if (_pAddr->sa_family == BaseSocket::AF_UNIX_DOMAIN)
   {
	   //
	   // The following line is note needed under BSD Darwin
	   // sun_len is not defined under Linux Suse 10.1 install
	   // _pUnixAddr->sun_len = UnixSocketAddress::sizeOf();
       _pUnixAddr->sun_family = BaseSocket::AF_UNIX_DOMAIN; 
       strcpy(_pUnixAddr->sun_path, unixAddress.getPathAddress() );
	   _unixSocketPath = _pUnixAddr->sun_path;
   }
   else
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }
}

void UnixSocketAddress::setPathAddress(const char *pathAddr)
{
   if (strlen(pathAddr) > Log::PATH_MAX_LEN)
   {
      throw SocketException(SocketException::BadUnixPathAddress);   
   }
   
   if (_pAddr->sa_family == BaseSocket::AF_UNIX_DOMAIN)
   {
	   //
	   // The following line is note needed under BSD Darwin
	   // sun_len is not defined under Linux Suse 10.1 install
	   //       _pUnixAddr->sun_len = UnixSocketAddress::sizeOf();
       _pUnixAddr->sun_family = BaseSocket::AF_UNIX_DOMAIN; 
	   _unixSocketPath = pathAddr;
       strcpy(_pUnixAddr->sun_path, pathAddr);
   }
   else
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }
}

const char *UnixSocketAddress::getPathAddress()
{
   if (_pAddr->sa_family != BaseSocket::AF_UNIX_DOMAIN)
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }

   if (_unixSocketPath.length() == 0)
   {
      return "";   
   }

   return _unixSocketPath.c_str();
}

void UnixSocketAddress::updateAddressPathFromStruct()
{
	// TODO: check for bounds!
	_unixSocketPath = _pUnixAddr->sun_path;
}

socklen_t UnixSocketAddress::sizeOf()
{
	int sizeAddr = sizeof(sockaddr_un);

//	cout <<  "UnixSocketAddress::sizeOf -  Size is: " << sizeAddr << endl;
	return (sizeAddr);
}
