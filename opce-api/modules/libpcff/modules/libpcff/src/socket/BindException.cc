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


BindException::BindException(int exceptionNum, char* msg)
{
  _type = ERR;
  switch (exceptionNum)
    {
    case BadDescriptor:
      setErrString("bind: descriptor is invalid\n");
      break;
    case AlreadyBound:
      setErrString("bind: socket is already bound\n");
      break;
    case AddrInUse:
      setErrString("bind: address already in use\n");
      break;

      // Unix socket errors
    case BaddAddress:
      setErrString("bind: address argument is a null pointer\n");
      break;
    case IOError:
        setErrString("bind: I/O error occurred\n");
    	break;
    case LoopExists:
      setErrString("bind: loop exists in symbolic links encountered during resolution of the pathname in address\n");
      break;
    case AddressTooLong:
      setErrString("bind: component of a pathname exceeded {NAME_MAX} characters, or an entire pathname exceeded {PATH_MAX} characters\n");
      break;
    case AddressDoesNotExist:
      setErrString("bind: component of the pathname does not name an existing file or the pathname is an empty string\n");
      break;
    case AddressNotADirectory:
      setErrString("bind: component of the path prefix of the pathname in address is not a directory\n");
      break;
    case AddressWouldResideOnReadOnlyFS:
      setErrString("bind: name would reside on a read-only file system\n");
      break;

    //
    case Unknown:
      setErrString("bind: unknown error\n");
      break;
    default:
        setErrString("bind: unknown error\n");
    	break;
    }
}
