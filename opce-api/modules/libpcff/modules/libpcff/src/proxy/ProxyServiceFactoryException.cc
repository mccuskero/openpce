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

#include <stdlib.h>


#include <pcff/proxy/ProxyServiceFactoryException.h>

using namespace pcff;

ProxyServiceFactoryException::ProxyServiceFactoryException(int exType, const char *msg)
{
  _type = exType;

  setErrMsg( msg );
  switch (exType)
    {
    case IngestorIsNull:
      setErrString("Ingestor is NULL");
      _type = ERR;
      break;
    case ProcessorIsNull:
      setErrString("Processor is NULL");
      _type = ERR;
      break;
    case UnknownIngestorType:
      setErrString("Uknown Ingestor Type");
      _type = ERR;
      break;
    case UnknownProcessorType:
      setErrString("Unknown Processor Type");
      _type = ERR;
      break;
    default:
      break;
    }
}
