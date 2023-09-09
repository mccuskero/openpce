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

#include <pcdao/DaoResultException.h>

using namespace std;
using namespace pcdao;

DaoResultException::DaoResultException(int exType, char *msg)
{
   _type = exType;

   setErrMsg(msg);

   switch (exType)
   {
   case RESULT_SET_ERROR:
      setErrString("Result Set Error.");
      _type = ERR;
      break;
   case RESULT_SET_NOT_INITIALIZED:
      setErrString("Result Set is NULL.");
      _type = ERR;
      break;
   case RESULT_SET_DRIVER_FAILURE:
      setErrString("Result set database server error.");
      _type = ERR;
      break;
   case RESULT_SET_EMPTY_QUERY:
      setErrString("Result set return indicates query was empty.");
      _type = ERR;
      break;
   case RESULT_SET_IS_NULL:
      setErrString("Result set handle is NULL.");
      _type = ERR;
      break;
   default:
      setErrString("An unknown error occurred");
      _type = ERR;
      break;
   }
}

