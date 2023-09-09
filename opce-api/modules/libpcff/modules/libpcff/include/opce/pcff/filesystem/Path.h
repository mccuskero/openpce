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

#ifndef PATH_H_
#define PATH_H_

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#include <string>
#include <vector>

namespace pcff
{

class Path
{
public:
    class PathEntry
    {
    public:
        PathEntry();
        PathEntry(const PathEntry &in);
        virtual ~PathEntry();

    private:
        void initMembers();

    public:
        void initialize(struct dirent *entry, std::string parentPath);

    public:
        PathEntry & operator=(const PathEntry &in);

    public:

        typedef enum
        {
            FileType,
            DirType,
            SocketType,
            OtherType,
            EndOfEntries,
            UnknownType
        } ePathItemType;

    public:
        void setEndOfEntries()
        {
            _pathItemType = EndOfEntries;
        }

        std::string getName()
        {
            return _entryName;
        }


        std::string getPathname()
        {
            return _pathname;
        }

        std::string getParentPathname()
        {
            return _parentPathname;
        }

    public:
        bool isDir()
        {
            return (_pathItemType == DirType) ? true : false;
        }

        bool isFile()
        {
            return (_pathItemType == FileType) ? true : false;
        }

        bool isSocket()
        {
            return (_pathItemType == SocketType) ? true : false;
        }

        bool isOther()
        {
            return (_pathItemType == OtherType) ? true : false;
        }

        bool isEndOfEntries()
        {
            return (_pathItemType == EndOfEntries) ? true : false;
        }

        bool isCurrentDir()
        {
            return _isCurrDir;
        } // is '.'

    public:
        std::string getTypeInfo(ePathItemType type);
        std::string getInfo();

    public:
        std::string _entryName;

    private:
        std::string _pathname;
        std::string _parentPathname;
        struct stat _statInfo;
        ePathItemType _pathItemType;
        bool          _isCurrDir;
    };

public:
    Path();
    Path(const Path &in);
    virtual ~Path();

private:
    void initMembers();

public:
    Path &operator=(const Path &in);

public:
    void initialize(std::string path);

public:
    void open();
    void close();

public:
    void sort();

public:
    void print();

public:
    std::string getPathname()  {    return _parentPathname; }
    long       getNumEntries() {    return _entries.size(); }
    PathEntry &getEntry(long idx);
    PathEntry &getNextDirEntry(); // return NULL if none left, or in list
    PathEntry &getNextFileEntry(); // return NULL if none left, or in list

    // static helpers
public:
    static bool exists(std::string path);
    static bool haveAccess(std::string path);

private:
    std::string _parentPathname;
    std::vector<PathEntry> _entries;
    bool _isOpened;
    int _fileCount;
    int _dirCount;
    int _otherCount;
    Path::PathEntry _eofEntry;
};

};

#endif /* PATH_H_ */
