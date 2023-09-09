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
 /**-------------------------------------------------------------
   Filename:       BaseSocket.h
   Creation Date:  1997-09-02
   Author:         Owen E. McCusker
   Brief Desc:     BaseSocket encapsulates all driver specific 
                   calls. This way it can be ported to many
                   platforms.

  --Notes -------------------------------------------------------
  WinSock support
          create a BaseSocket that is compiled in for Windows support
          using the same interface from base socket. Use
          compile time switcheds.
**/

#ifndef _BaseSocket_h
#define _BaseSocket_h

#include <sys/socket.h> // BSD socket functions and data structures.
#include <netinet/in.h> // AF_INET and AF_INET6 address families. Widely used on the Internet, these include IP addresses and TCP and UDP port numbers.
#include <sys/un.h>     // AF_UNIX address family. Used for local communication between programs running on the same computer. Not used on networks.
#include <arpa/inet.h>  // used for manipulating numeric IP addresses.
#include <netdb.h>      // used for translating protocol names and host names into numeric addresses. Searches local data as well as DNS.

#include <pcff/socket/INETAddress.h>
#include <pcff/socket/UnixSocketAddress.h>
#include <pcff/socket/SocketAddress.h>

#include <string>

namespace pcff
{

class BaseSocket
{
 public:
  BaseSocket(int  addrFamily = BaseSocket::AF_INET_IPv4,
             int  socketType = SOCK_STREAM);
  virtual         ~BaseSocket();
  virtual    void init(); 

 public:
  virtual void    connect(INETAddress &servAddr, 
                          int servPortNo);
  virtual void    connect(const char *socketPath);     // unix domain socket
  virtual void    bind(INETAddress &localAddr, 
                       int localPort = 0);
  virtual void    bind(int localPort = 0);
  virtual void    bind(const char *socketPath);  
  virtual void          listen(int backLog=1);
  void            accept(BaseSocket *pConnection, 
                         INETAddress &remoteAddr, 
                         int &remotePortNo);  
  void			  accept(BaseSocket *pConnection, 
						UnixSocketAddress &remoteAddr);
  virtual int     recv(void *pBuffer, 
                       int bufLength, 
                       unsigned int flags);
  int             recvFrom(void *pBuffer, 
                           int bufLength, 
                           unsigned int flags,
                           INETAddress &sourceIP, 
                           int &sourcePortNumber);
  int			recvFrom(void  *pBuffer, 
							int               bufLength, 
							unsigned int      flags,
							UnixSocketAddress &unixAddress);
  virtual int     send(void *pBuffer, 
                       int bufLength, 
                       unsigned int flags);
  int             sendTo(void *pBuffer, 
                         int bufLength, 
                         unsigned int uiFlags,
                         INETAddress &destIP, 
                         int destPortNumber);
  int             sendTo(	void		*pBuffer, 
							int				bufLength, 
							unsigned int	flags,
							UnixSocketAddress &unixAddress);
  void            getSockName(INETAddress &localAddr, 
                              int &localPort);
  void            getPeerName(INETAddress &remoteAddr, 
                              int &remotePort);
  
  void            getSockOpt(int codeLevel, 
                             int optionName, 
                             void *pOptionData, 
                             int &dataLength);
  void            setSockOpt(int codeLevel, 
                             int optionName, 
                             const void *pOptionData, 
                             int dataLength);
 public:
  INETAddress     &localINETAddr(); 
  INETAddress     &remoteINETAddr();
 
  std::string      socketPath();
 
  int             localPort();
  int             remotePort();
  int             getSockDesc() { return _sockDesc; }   // returns the socket descriptor
  
 protected:
  void            _resetSocketDescriptor(int newSockDesc, 
				         int addrFamily);
  void            _handleError(int context, int errNum);
  void            _setLocal();
  void            _setConnected();
  SocketAddress    *_getSocketAddress();

 protected:
  int             _sockDesc;
  int             _addrFamily;
  int             _socketType;

  INETAddress     *_pLocalAddress; 
  int             _localPort; 
  INETAddress     *_pRemoteAddress;
  int             _remotePort;
  std::string          _socketPath; // for unix domain socket

  int             _socketState;
  int             _socketLocale;

   public:
      enum SocketAFConstants {
        AF_INET_IPv4 = AF_INET,
        AF_INET_IPv6 = AF_INET6,
		AF_UNIX_DOMAIN = AF_UNIX
      };

   protected:
      enum eSocketState {
         Uninitialized,
         Initialized,
         Connected,
         Bound,
         Disconnected
      };

      enum eSocketType {
         Local,
         Remote
      };

};

}; // namespace

#endif
