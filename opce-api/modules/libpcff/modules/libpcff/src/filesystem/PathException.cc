/*
 * PathException.cc
 *
 *  Created on: Sep 25, 2008
 *      Author: owenm
 */

#include <pcff/filesystem/PathException.h>

using namespace std;
using namespace pcff;

PathException::PathException(int exceptionNum, const char* msg)
{
  _type = ERR;
  switch (exceptionNum)
    {
    case InvalidDirStreamDescriptor:
      setErrString("Invalid directory stream descriptor.\n");
      break;
    case PermissionDenied:
      setErrString("Permission denied.\n");
      break;
    case TooManyFileDescriptors:
      setErrString("Too many file descriptors in use by process.\n");
      break;
    case TooManyOpenFiles:
      setErrString("Too many files are currently open in the system.\n");
      break;
    case PathDoesNotExist:
      setErrString("Entered path name does  not  exist, or is an empty string.\n");
      break;
    case InsufficientMemory:
      setErrString("Insufficient memory to complete the operation.\n");
      break;
    case PathNotADirectory:
      setErrString("Entered path is not a directory.\n");
      break;
    case UnknownErrorCode:
      setErrString("Unexpected error code");
      break;
    case Unknown:
      setErrString("Unknown error\n");
      break;
    default:
      setErrString("Unknown error\n");
      break;
    }
}
