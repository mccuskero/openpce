/**
 * Copyright (c) 1994-2009 Owen McCusker <mccuskero@ct.metrocast.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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


#include <pcdao/DaoConnectionException.h>

using namespace pcff;
using namespace pcdao;

DaoConnectionException::DaoConnectionException(int exType, char *msg)
{
  _type = exType;

  setErrMsg( msg );

  switch (exType)
    {
    case CONNECTION_ERROR:
      setErrString("Connection error.");
      _type = ERR;
      break;
    case GET_CONNECTION_ERROR:
      setErrString("Get Connection error.");
      _type = ERR;
      break;
    case RELEASE_CONNECTION_ERROR:
      setErrString("Release Connection error.");
      _type = ERR;
      break;
    case CONNECTION_TYPE_NOT_SUPPORTED:
      setErrString("Connection type not supported error.");
      _type = ERR;
      break;
    case ERROR_CONNECTING_TO_DATABASE:
      setErrString("Error connecting to database.");
      _type = ERR;
      break;
    case BAD_HOST_NAME:
      setErrString("Bad host name in configuration.");
      _type = ERR;
      break;
    case BAD_DATABASE_NAME:
      setErrString("Bad database in configuraiton.");
      _type = ERR;
      break;
    case BAD_USER_NAME:
      setErrString("Bad user name in configuration.");
      _type = ERR;
      break;
    case EXECUTE_QUERY_FAILED:
      setErrString("Execute query failed.");
      _type = ERR;
      break;
    case EXECUTE_UPDATE_FAILED:
      setErrString("Execute updated failed.");
      _type = ERR;
      break;
    default:
        setErrString("An unknown error occurred");
        _type = ERR;
      break;
    }
}

