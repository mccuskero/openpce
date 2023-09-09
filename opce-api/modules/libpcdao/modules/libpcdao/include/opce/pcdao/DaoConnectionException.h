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

#ifndef _DaoConnectionException_h_
#define	_DaoConnectionException_h_

#include <pcff/BasicException.h>

namespace pcdao
{

class DaoConnectionException : public pcff::BasicException
{

public:
  typedef enum DAO_EXCEPTION_ENUMS
  {
    CONNECTION_ERROR = pcff::BasicException::NEXT_AVAILABLE_ERROR_VAL,
    GET_CONNECTION_ERROR,
    RELEASE_CONNECTION_ERROR,
    CONNECTION_TYPE_NOT_SUPPORTED,
    ERROR_CONNECTING_TO_DATABASE,
    BAD_HOST_NAME,
    BAD_DATABASE_NAME,
    BAD_USER_NAME,
    EXECUTE_QUERY_FAILED,
    EXECUTE_UPDATE_FAILED,
    BAD_SQL_CMD,
    UNKNOWN_PCDAO_ERROR
  };

public:
    DaoConnectionException(int exType, char *msg);
};

};

#endif	/* _DaoConnectionException_h_ */

