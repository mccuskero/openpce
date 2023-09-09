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

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/filesystem/PathException.h>
#include <pcff/filesystem/PathExceptionHandler.h>

#include <pcff/filesystem/Path.h>

using namespace std;
using namespace pcff;

static bool ascendingSort(const Path::PathEntry &e1, const Path::PathEntry &e2);

Path::Path()
{
    initMembers();
}


Path::Path(const Path &in)
{
    initMembers();
    *this = in;
}


Path::~Path()
{
}

void Path::initMembers()
{
    _parentPathname = "NotSet";
    _isOpened = false;
    _fileCount = 0;
    _dirCount = 0;
    _otherCount = 0;
    _eofEntry.setEndOfEntries();
}

Path &Path::operator=(const Path &in)
{
    _parentPathname = in._parentPathname;
    _entries  = in._entries;
    _isOpened = in._isOpened;
    _fileCount = in._fileCount;
    _dirCount = in._dirCount;
    _otherCount = in._otherCount;

    return *this;
}

void Path::initialize(std::string path)
{
    PathEntry *pathEntry = NULL;
    struct dirent *entry = NULL;
    DIR * dir = NULL;

    _parentPathname = path;

    BasicExceptionMgr::instance()->log(LogLevel::Debug, "Path::initialize path: %s", path.c_str());

    try
    {
       dir = opendir(path.c_str());

       if (dir != NULL)
       {
           dirent * entry(NULL);
           do
           {
               entry = readdir(dir);
               if (entry != NULL)
               {
                   pathEntry = new PathEntry;
                   pathEntry->initialize(entry, _parentPathname);

                   if (pathEntry->isFile() == true)
                   {
                       _entries.push_back(*pathEntry);
                        BasicExceptionMgr::instance()->log(LogLevel::Debug, "Path::initialize (isFile) adding: %s", pathEntry->getName().c_str() );
                       _fileCount++;
                   }
                   else if (pathEntry->isDir() == true)
                   {
                       _entries.push_back(*pathEntry);
                        BasicExceptionMgr::instance()->log(LogLevel::Debug, "Path::initialize (isDir) adding: %s", pathEntry->getName().c_str() );
                       _dirCount++;
                   }
                   else
                   {
                       // TODO: lets not manage other types right now
//                       _entries.push_back(*pathEntry);
                       _otherCount++;
                   }

                   delete pathEntry;
                   pathEntry = NULL;
               }
           }
           while (entry != NULL);

           sort();

           if (closedir(dir))
           {
               PathExceptionHandler::handleError(errno);
           }
       }
       else
       {
           PathExceptionHandler::handleError(errno);
       }
    }
    catch (PathException &ex)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error, "Path::initialize");
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error, "Path::initialize: unknown exception occurred");
    }
}

void Path::print()
{
    vector<PathEntry>::iterator it;

    cout << _parentPathname << endl;

    for (it = _entries.begin(); it != _entries.end(); ++it)
    {
        cout << "   " << (*it).getInfo() << endl;
    }

}

void Path::open()
{

}

void Path::close()
{

}

bool ascendingSort(const Path::PathEntry &e1, const Path::PathEntry &e2)
{
//    string s1 = e1.getName();
//    string s2 = e2.getName();

    return ( e1._entryName < e2._entryName );
}

void Path::sort()
{
    std::sort(_entries.begin(), _entries.end(), ascendingSort );
}

Path::PathEntry &Path::getEntry(long idx)
{
    if (idx < _entries.size())
    {
        return _entries[idx];
    }
    else
    {
        return _eofEntry;
    }
}

bool Path::exists(std::string path)
{

}

//
// ============================ Inner Class Definitions
//

Path::PathEntry::PathEntry()
{
    initMembers();
}

Path::PathEntry::PathEntry(const PathEntry &in)
{
    initMembers();
    *this = in;
}

Path::PathEntry::~PathEntry()
{

}

void Path::PathEntry::initMembers()
{
    _entryName = "NotSet";
    _pathname = "NotSet";
    _parentPathname = "NotSet";
    memset(&_statInfo, 0, sizeof (struct stat));
    _pathItemType = UnknownType;
    _isCurrDir = false;
}

Path::PathEntry &Path::PathEntry::operator=(const Path::PathEntry &in)
{
    _entryName = in._entryName;
    _pathname = in._pathname;
    _parentPathname = in._parentPathname;
    memcpy(&_statInfo, &in._statInfo, sizeof (struct stat));
    _pathItemType = in._pathItemType;
    _isCurrDir = in._isCurrDir;

    return *this;
}

//
// TODO: handle links
//

void Path::PathEntry::initialize(struct dirent *entry, string parentPath)
{
    struct stat info;
    string path;

    try
    {
        // no reason to include the . or .. as entries
        if (entry->d_name[0] != '.' || (entry->d_name[0] != '.' && entry->d_name[1] != '.') )
        {
            _parentPathname = parentPath;
            _entryName = entry->d_name;
            path = _parentPathname;
            path += "/";
            path += entry->d_name;

            _pathname = path;

            if (stat(path.c_str(), &info) != 0)
            {
                PathExceptionHandler::handleError(errno);
            }
            else if (S_ISREG(info.st_mode))
            {
                _pathItemType = PathEntry::FileType;
            }
            else if (S_ISDIR(info.st_mode))
            {
                _pathItemType = PathEntry::DirType;
            }
            else if (S_ISSOCK(info.st_mode))
            {
                _pathItemType = PathEntry::SocketType;
            }
            else
            {
                _pathItemType = PathEntry::OtherType;
            }
        }
    }
    catch (PathException &ex)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error, "Path::PathEntry::initialize");
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error, "Path::PathEntry::initialize: unknown exception occurred");
    }
}

string Path::PathEntry::getInfo()
{
    ostringstream pathEntryInfo;

    pathEntryInfo << getTypeInfo(_pathItemType) << "," << _entryName;

    return pathEntryInfo.str();
}

string Path::PathEntry::getTypeInfo(ePathItemType type)
{
    string sType;

    switch (type)
    {
    case PathEntry::FileType :
                sType = "f";
        break;
    case PathEntry::DirType :
                sType = "d";
        break;
    case PathEntry::SocketType :
                sType = "s";
        break;
    case PathEntry::OtherType :
                sType = "o";
        break;
    default:
        sType = "u";
        break;
    }

    return sType;
}
