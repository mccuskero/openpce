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

SendToException::SendToException(int exceptionNum, char* msg)
{
  _type = ERR;
  switch (exceptionNum)
    {
    case KernelMemory:         
      setErrString("sendto: out of kernel memory\n");
      break;
    case BadDescriptor:        
      setErrString("sendto: invalid socket descriptor\n");
      break;
    case IllegalPointer:       
      setErrString("sendto: illegal pointer\n");
      break;
    case NotConnected:         
      setErrString("sendto: socket not connected\n");
      break;
    case WouldBlock:           
      setErrString("sendto: non-blocking, requested operation would block\n");
      break;
    case Interrupted:          
      setErrString("sendto: transfer interupted by signal\n");
      break;
    case InvalidArgument:      
      setErrString("sendto: invalid argument\n");
      break;
    case MessageSizeTooBig:    
      setErrString("sendto: message too big\n");
      break;
    case Unknown:              
      setErrString("sendto: unknown error\n");
    }
}
