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

#ifndef _DatagramSocket_h
#define _DatagramSocket_h


#include <pcff/socket/BaseSocket.h>

namespace pcff
{

class DatagramSocket : public BaseSocket
{
 public:
  DatagramSocket(int family = BaseSocket::AF_INET_IPv4);  // TODO: make compiler define family...
  virtual ~DatagramSocket() {}

  // server socket support
public:
  virtual void       init();
  virtual void       init(INETAddress &destAddr,   // for client
						  int port);
  virtual void        bind(INETAddress &localAddr, 
						   int port = 0,
						   bool bUseIPv6 = false);
  virtual void       bind( int port = 0,
						   bool bUseIPv6 = false);
  
public:
  virtual int     sendDatagram(void *pBuffer,        // for client
								   int bufLength);   // for client
  virtual int     sendDatagram(void *pBuffer, 
							   int bufLength,
							   INETAddress &destAddr, 
							   int destPort);
  virtual int     receiveDatagram(void *pBuffer, 
								  int bufLength,
								  INETAddress &sourceAddr, 
								  int &sourcePort);
  virtual void    getSockOpt(int codeLevel, 
							 int optionName, 
							 void *pOptionData, 
							 int &dataLength);
  virtual void    setSockOpt(int codeLevel, 
							 int optionName, 
							 const void *pOptionData, 
							 int dataLength);
protected:
	INETAddress _destAddr;   // for client socket
	int			_destPort;   // for client socket
};

}; // namespace

#endif
