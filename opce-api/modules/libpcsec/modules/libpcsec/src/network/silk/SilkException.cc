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

#include <pcsec/network/SilkException.h>

using namespace pcff;
using namespace pcsec;

SilkException::SilkException(int exceptionNum, const char* msg)
{

  this->setErrMsg(msg);

  switch (exceptionNum)
    {
    case ErrorOpeningSilkFile:
      _type = ERR;
	  setErrString("Error opening file.");
	  break;
    case ErrorClosingSilkFile:
      _type = ERR;
      setErrString("Error closing file.\n");
      break;
    case InvalidDatastore:
      _type = ERR;
      setErrString("Invalid datastore.\n");
      break;
    case InvalidFilename:
      _type = ERR;
      setErrString("Invalid filename.\n");
      break;
    case ErrorGettingRecord:
      _type = ERR;
      setErrString("Error getting silk record from file.\n");
      break;
    case SilkFrameworkNotInitialized:
      _type = ERR;
      setErrString("Silk framework was not initialized by SilkInterfaceDirector.\n");
      break;
    case NoMoreFiles:
      _type = ERR;
      setErrString("No more files in directory.\n");
      break;
    case NoMoreDaysInMonth:
      _type = ERR;
      setErrString("No more days dirs in current month.\n");
      break;
    case NoMoreMonthsInYear:
      _type = ERR;
      setErrString("No more months dirs in current year.\n");
      break;
    case NoMoreYearsInBinDir:
      _type = ERR;
      setErrString("No more year dirs in current bin directory.\n");
      break;
    case PathnameNotSet:
      _type = ERR;
      setErrString("Silk framework pathname has not been initialized.\n");
      break;
    default:
      _type = ERR;
      setErrString("Unknown error\n");
      break;
    }
}
