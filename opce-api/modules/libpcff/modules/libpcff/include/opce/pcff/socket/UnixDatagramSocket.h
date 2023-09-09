/**
 * Copyright (c) 1994 Owen McCusker <pcave@myeastern.com>
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

#ifndef _UnixDatagramSocket_h
#define _UnixDatagramSocket_h

#include <pcff/socket/UnixSocketAddress.h>
#include <pcff/socket/DatagramSocket.h>

namespace pcff
{

class UnixSocketAddress;

class UnixDatagramSocket : public DatagramSocket
{
 public:
  UnixDatagramSocket();
  UnixDatagramSocket(int family); 
  virtual ~UnixDatagramSocket() {}
  
 public:
  int        sendDatagram(void *pBuffer, 
							int len, 
							int direction = UnixDatagramSocket::FromClientToServer);
  int        receiveDatagram(void *pBuffer, int len);

 public:
 void		 init();
 void		 init(const char*socketPath);
 void        bind();
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
  public:
     const char *getFromAddressPath();

// direction of datagram
  enum eUnixDomainSocketConstants
  {
	FromServerToClient,
	FromClientToServer
  };

protected:
	UnixSocketAddress _localAddress;
	UnixSocketAddress _serverAddress;
	UnixSocketAddress _fromAddress;
};

}; // namespace

#endif
