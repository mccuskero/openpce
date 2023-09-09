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

#include <string.h>

#include <pcff/socket/SocketExceptions.h>

using namespace pcff;


AcceptException::AcceptException(int exceptionNum, char* msg)
{
  _type = ERR;
  switch (exceptionNum)
    {
    case Memory:               
      setErrString("accept: not enough memory\n");
      break;
    case BadDescriptor:        
      setErrString("accept: descriptor is invalid\n");
      break;
    case IllegalPointer:       
      setErrString("accept: illegal pointer\n");
      break;
    case NotStreamSock:        
      setErrString("accept: not a socket.\n");
      break;
    case NoPendingConnections: 
      setErrString("accept: no pending connections\n");
      break;
    case Firewall:             
      setErrString("accept: firewall blocked connection\n");
      break;
    case Unknown:              
      setErrString("accept: unknown error\n");
    }
}
