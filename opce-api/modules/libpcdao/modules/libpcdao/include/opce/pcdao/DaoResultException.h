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

#ifndef _PGSQLRESULTEXCEPTION_H
#define	_PGSQLRESULTEXCEPTION_H

#include <pcff/BasicException.h>

namespace pcdao
{

class DaoResultException : public pcff::BasicException
{

public:
   typedef enum DAO_EXCEPTION_ENUMS
   {
      RESULT_SET_ERROR = pcff::BasicException::NEXT_AVAILABLE_ERROR_VAL,
      RESULT_SET_NOT_INITIALIZED,
      RESULT_SET_DRIVER_FAILURE,
      RESULT_SET_EMPTY_QUERY,
      RESULT_SET_IS_NULL,
      UNKNOWN_RESULT_SET_ERROR
   };

public:
   DaoResultException(int exType, char *msg);
};


};

#endif	/* _PGSQLRESULTEXCEPTION_H */

