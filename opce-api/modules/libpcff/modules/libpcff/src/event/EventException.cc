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

#include <pcff/event/EventException.h>

using namespace pcff;

EventException::EventException(int exceptionNum, const char* msg)
{
   _type = ERR;

   this->setErrMsg(msg);

   switch (exceptionNum)
   {
   case InvalidProxyMode:
      setErrString("Proxy service set to invalid proxy mode during initialization");
      break;
   case IngestorIsNull:
      setErrString("Ingestor is NULL during initialization");
      break;
   case ProcessorIsNull:
      setErrString("Processor is NULL during initialization");
      break;
   case InvalidBufferSize:
      setErrString("Invalid event raw buffer size. \n");
      break;
   case InvalidMapIndex:
      setErrString("Map index is invalid. \n");
      break;
   case EventHandlerNotInitialized:
      setErrString("Event handlers are not intialized.\n");
      break;
   case Unknown:
      setErrString("Unknown error\n");
      break;
   default:
      setErrString("Unknown error\n");
      break;
   }
}

