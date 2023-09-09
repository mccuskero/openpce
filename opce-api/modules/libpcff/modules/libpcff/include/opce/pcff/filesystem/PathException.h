/**
 * Copyright (c) 1994-2008 Owen McCusker <pcave@ct.metrocast.net>
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

#ifndef PATHEXCEPTION_H_
#define PATHEXCEPTION_H_

#include <pcff/BasicException.h>


namespace pcff
{

class PathException : public pcff::BasicException
{
public:
	enum Constants
	{
    InvalidDirStreamDescriptor,
    PermissionDenied,
    TooManyFileDescriptors,
    TooManyOpenFiles,
    PathDoesNotExist,
    InsufficientMemory,
    PathNotADirectory,
    UnknownErrorCode,
    Unknown
	};

public:
	PathException(int exceptionNum, const char* msg);
};

};


#endif /* PATHEXCEPTION_H_ */
