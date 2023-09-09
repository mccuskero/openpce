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


#include <stdlib.h>
#include <errno.h>

#include <pcff/filesystem/PathException.h>
#include <pcff/filesystem/PathExceptionHandler.h>

using namespace pcff;

void PathExceptionHandler::handleError(int errNum)
{
    switch (errNum)
    {
    case EBADF:
        throw PathException(PathException::InvalidDirStreamDescriptor, "Invalid stream descriptor");
    case EACCES:
        throw PathException(PathException::PermissionDenied, "Permission denied");
    case EMFILE:
        throw PathException(PathException::TooManyFileDescriptors, "Too many file descriptors");
    case ENFILE:
        throw PathException(PathException::TooManyOpenFiles, "Too many open files");
    case ENOENT:
        throw PathException(PathException::PathDoesNotExist, "Path does not exist");
    case ENOMEM:
        throw PathException(PathException::InsufficientMemory, "Insufficient memory");
    case ENOTDIR:
        throw PathException(PathException::PathNotADirectory, "Path is not a directory");
    default:
        throw PathException(PathException::UnknownErrorCode, "Unknown error code");
    }
}
