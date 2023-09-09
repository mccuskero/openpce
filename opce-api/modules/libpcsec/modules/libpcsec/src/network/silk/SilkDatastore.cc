/**
 * Copyright (c) 1994 Owen McCusker <pcave@ct.metrocast.net>
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

#include <iostream>
#include <sstream>
#include <string>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/filesystem/Path.h>

#include <pcsec/network/SilkException.h>
#include <pcsec/network/SilkFile.h>

#include <pcsec/network/SilkDatastore.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkDatastore::SilkDatastore()
{
    initMembers();
}

SilkDatastore::SilkDatastore(const string &pathname)
{
    initMembers();

    _datastorePathname = pathname;
}

SilkDatastore::~SilkDatastore()
{
}

void SilkDatastore::initMembers()
{
    _datastorePathname = "";
    _numBins = 0;
    _currBinIndex = 0;
    _bNoMoreFiles = false;
}

void SilkDatastore::setPathname(string &pathname)
{
    _datastorePathname = pathname;
}

void SilkDatastore::subscribe(eSilkBin bin)
{
    string binName;

    switch (bin)
    {
    case SilkDatastore::In :
                binName = "in";
        addBin(binName);
        break;
    case SilkDatastore::InIcmp :
                binName = "inicmp";
        addBin(binName);
        break;
    case SilkDatastore::InWeb :
                binName = "inweb";
        addBin(binName);
        break;
    case SilkDatastore::InNull :
                binName = "innull";
        addBin(binName);
        break;
    case SilkDatastore::Int2Int :
                binName = "int2int";
        addBin(binName);
        break;
    case SilkDatastore::Out :
                binName = "out";
        addBin(binName);
        break;
    case SilkDatastore::OutIcmp :
                binName = "outicmp";
        addBin(binName);
        break;
    case SilkDatastore::OutWeb :
                binName = "outweb";
        addBin(binName);
        break;
    case SilkDatastore::OutNull :
                binName = "outnull";
        addBin(binName);
        break;
    case SilkDatastore::Out2Out :
                binName = "out2out";
        addBin(binName);
        break;
    case SilkDatastore::AllBins :
                subscribeToAllBins();
        break;
    case SilkDatastore::AllInBins :
                subscribeToAllInBins();
        break;
    case SilkDatastore::AllOutBins :
                subscribeToAllOutBins();
        break;
    case SilkDatastore::NoBins :
        break;
    default:
        break;
    }
}

void SilkDatastore::subscribeToAllBins()
{
    string binName;

    binName = "in";
    addBin(binName);
    binName = "inicmp";
    addBin(binName);
    binName = "inweb";
    addBin(binName);
    binName = "innull";
    addBin(binName);
    binName = "int2int";
    addBin(binName);
    binName = "out";
    addBin(binName);
    binName = "outicmp";
    addBin(binName);
    binName = "outweb";
    addBin(binName);
    binName = "outnull";
    addBin(binName);
    binName = "out2out";
    addBin(binName);
}

void SilkDatastore::subscribeToAllInBins()
{
    string binName;

    binName = "in";
    addBin(binName);
    binName = "inicmp";
    addBin(binName);
    binName = "inweb";
    addBin(binName);
    binName = "innull";
    addBin(binName);
    binName = "int2int";
    addBin(binName);
}

void SilkDatastore::subscribeToAllOutBins()
{
    string binName;

    binName = "out";
    addBin(binName);
    binName = "outicmp";
    addBin(binName);
    binName = "outweb";
    addBin(binName);
    binName = "outnull";
    addBin(binName);
    binName = "out2out";
    addBin(binName);
}

void SilkDatastore::subscribeToInOutBins()
{
    string binName;

    binName = "in";
    addBin(binName);
    binName = "out";
    addBin(binName);
}

void SilkDatastore::subscribeToDefaultBins()
{
    string binName;

    binName = "in";
    addBin(binName);
    binName = "out";
    addBin(binName);
    binName = "inweb";
    addBin(binName);
    binName = "outweb";
    addBin(binName);
}

//
// custom bin subscriptions
//

void SilkDatastore::subscribe(std::string &dirName)
{
    if (dirName.length() != 0)
    {
       addBin(dirName);
    }
}

void SilkDatastore::addBin(string binName)
{
    SilkDatastore::SilkDatastoreBin silkBin;

    BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::addBin (%s)",
                                   binName.c_str());

    if (_datastorePathname.compare("") == 0)
    {
        throw SilkException(SilkException::PathnameNotSet, "SilkDatastore::addBin");
    }

    silkBin.initialize(binName, _datastorePathname);

    _currBinList.push_back(silkBin);

}

void SilkDatastore::initialize(std::string &pathname)
{
    _datastorePathname = pathname;

    BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::initialize (pathname: %s, currBinList.size: %d) ",
                                   pathname.c_str(),
                                   _currBinList.size());


    // Check subscribed bin size
    if (_currBinList.size() == 0)
    {
        subscribeToDefaultBins();
    }

    _numBins = _currBinList.size();
    _currBinIndex = 0;
}

void SilkDatastore::validate()
{
    // does the path exist

    //	throw SilkException(SilkException::InvalidSilkPath, path.c_str() );
}

int SilkDatastore::getNextFilePathname(string &filePathname)
{
    string binPathname;
    int retVal = SilkDatastore::GotFilePathname;
    int binRetVal = SilkDatastore::GotFilePathname;

    if (_currBinIndex >= _numBins)
    {
        _bNoMoreFiles = true;
        retVal = SilkDatastore::NoMoreFiles;
    }
    else
    {
        binRetVal = _currBinList[_currBinIndex].getNextFilePathname(binPathname);

        if (binRetVal == SilkDatastore::GotFilePathname)
        {
            filePathname = _datastorePathname;
            filePathname += "/";
            filePathname += binPathname;

            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::getNextFilePathname (_currBinIndex: %d), (num bins: %d) (filePathname: %s)",
                                               _currBinIndex,
                                               _numBins,
                                               filePathname.c_str());

        }
        else if (binRetVal == SilkDatastore::LastYearInBin)
        {
            filePathname = _datastorePathname;
            filePathname += "/";
            filePathname += binPathname;
            _currBinIndex++;

            if (_currBinIndex >= _numBins)
            {
                _bNoMoreFiles = true;
            }

            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::getNextFilePathname (_currBinIndex: %d), (num bins: %d) (filePathname: %s)",
                                               _currBinIndex,
                                               _numBins,
                                               filePathname.c_str());
        }
        else
        {
            throw SilkException(SilkException::NoMoreYearsInBinDir, "SilkDatastore::getNextFilePathname");
        }
    }

    return retVal;
}

int SilkDatastore::getNextFilePathnameRotateBins(string &filePathname)
{
    string binPathname;
    int retVal = SilkDatastore::GotFilePathname;
    int binRetVal = SilkDatastore::GotFilePathname;


    if (_currBinIndex >= _numBins)
    {
        _bNoMoreFiles = true;
        retVal = SilkDatastore::NoMoreFiles;
    }
    else
    {

        binRetVal = _currBinList[_currBinIndex].getNextFilePathname(binPathname);

        if (binRetVal == SilkDatastore::GotFilePathname)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::getNextFilePathname (_currBinIndex: %d), (num bins: %d)",
                                               _currBinIndex,
                                               _numBins);

            filePathname = _datastorePathname;
            filePathname += "/";
            filePathname += binPathname;
        }
        else if (binRetVal == SilkDatastore::LastYearInBin)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::getNextFilePathname (_currBinIndex: %d), (num bins: %d)",
                                               _currBinIndex,
                                               _numBins);

            filePathname = _datastorePathname;
            filePathname += "/";
            filePathname += binPathname;

            if (_currBinIndex + 1 >= _numBins)
            {
                _bNoMoreFiles = true;
                retVal = SilkDatastore::NoMoreFiles;
            }
        }
        else
        {
            throw SilkException(SilkException::NoMoreYearsInBinDir, "SilkDatastore::getNextFilePathname");
        }


        if (++_currBinIndex >= _numBins)
        {
            _currBinIndex = 0;
        }
    }

    return retVal;
}


// return NoMoreFiles,
// ErrorRetreivingFile,
// FileOK

int SilkDatastore::getNextFile(SilkFile*& silkFile)
{
    int retVal = SilkDatastore::GotFilePathname;
    string silkFilename = "filename.not.set";

    try
    {
//        retVal = getNextFilePathname(silkFilename);
        retVal = getNextFilePathnameRotateBins(silkFilename);


        if (retVal == SilkDatastore::GotFilePathname)
        {
            if (silkFilename.compare("") == 0) throw SilkException(SilkException::InvalidFilename, "SilkFileIngestor::initialize");

            BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkDatastore::getNextFile data filename: %s", silkFilename.c_str());
            cout << "SilkDatastore::getNextFile data filename: " << silkFilename.c_str() << endl;

            silkFile = new SilkFile();
            silkFile->initialize(silkFilename.c_str());
        }
        else if (retVal == SilkDatastore::NoMoreFiles)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkDatastore::getNextFile No more files");
        }
    }
    catch (SilkException &ex)
    {
        BasicExceptionMgr::instance()->handle(ex);

        if (silkFile != NULL)
        {
            delete silkFile;
            silkFile = NULL;
        }

        retVal = SilkDatastore::SilkFileError;
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkDatastore::getNextFile No more files");

        if (silkFile != NULL)
        {
            delete silkFile;
            silkFile = NULL;
        }

        retVal = SilkDatastore::SilkFileError;
    }

    return retVal;
}



//============ SilkDatastoreBin

SilkDatastore::SilkDatastoreBin::SilkDatastoreBin()
{
    initMembers();
}

SilkDatastore::SilkDatastoreBin::SilkDatastoreBin(const SilkDatastore::SilkDatastoreBin &in)
{
    initMembers();

    *this = in;
}

SilkDatastore::SilkDatastoreBin::~SilkDatastoreBin()
{
}

SilkDatastore::SilkDatastoreBin &SilkDatastore::SilkDatastoreBin::operator=(const SilkDatastore::SilkDatastoreBin &in)
{
    _binName = in._binName; // bin name e.g. int2int
    _pathname = in._pathname; // datastore pathanme e.g. /data/silk/
    _currYearIndex = in._currYearIndex; // points to the current bin index in list
    _lastRead = in._lastRead;
    _binPath = in._binPath;
    _silkYearDirList = in._silkYearDirList; // TODO: verify that this works!!!
    _currPathname = in._currPathname; // e.g. /data/silk/int2int
    _currFilename = in._currFilename; // e.g. 2008
    _currYear = in._currYear;
    _currMonth = in._currMonth;
    _currDay = in._currDay;
    _currHour = in._currHour;
    _bNoMoreYears = in._bNoMoreYears;

    _numYears = in._numYears;
    _currYearIndex = in._currYearIndex; // points to the current bin index in list



    return *this;
}

void SilkDatastore::SilkDatastoreBin::initMembers()
{
    _binName = ""; // bin name e.g. int2int
    _pathname = ""; // datastore pathanme e.g. /data/silk/
    _currPathname = ""; // e.g. /data/silk/int2int
    _currFilename = ""; // e.g. 2008
    _currYear = "";
    _currMonth = "";
    _currDay = "";
    _currHour = "";
    _bNoMoreYears = false;
    _numYears = 0;
    _currYearIndex = 0; // points to the current bin index in list
}

void SilkDatastore::SilkDatastoreBin::initialize(std::string &binName, std::string &pathname)
{
    _binName = binName;
    _pathname = pathname;


    initializeBin();
}

void SilkDatastore::SilkDatastoreBin::initializeBin()
{
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _binName;

    // initialize path to get years
    _binPath.initialize(pathname);

    // _binPath.print();

    // initialize year list
    initializeYearList();

    _numYears = _silkYearDirList.size();

    // TODO: initialize path, Year, Month, Day
    //validate();

}

void SilkDatastore::SilkDatastoreBin::initializeYearList()
{
    Path::PathEntry pathEntry; // is a year
    string year;
    long numEntries = _binPath.getNumEntries();

    for (long i = 0; i < numEntries; i++)
    {
        pathEntry = _binPath.getEntry(i);

        if (pathEntry.isDir())
        {
            year = pathEntry.getName();
            addYear(year);
        }
        else if (pathEntry.isEndOfEntries())
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkDatastoreBin::initializeYearList End of entries %ld",
                                               i);
        }
        else
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkDatastoreBin::initializeYearList Expected a directory in path: %s/%s",
                                               _binPath.getPathname().c_str(),
                                               year.c_str());
        }
    }
}

void SilkDatastore::SilkDatastoreBin::addYear(string year)
{
    SilkDatastore::SilkYearDir silkYearDir;
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _binName;

    if (_pathname.compare("") == 0)
    {
        throw SilkException(SilkException::PathnameNotSet, "SilkDatastore::SilkDatastoreBin::addYear");
    }

    BasicExceptionMgr::instance()->log(LogLevel::Fine,
                                       "SilkDatastore::SilkDatastoreBin::addYear adding %s",
                                       pathname.c_str());

    silkYearDir.initialize(year, pathname);

    _silkYearDirList.push_back(silkYearDir);
}

int SilkDatastore::SilkDatastoreBin::getNextFilePathname(string &filePathname)
{
    int retVal = SilkDatastore::GotFilePathname;
    int yearRetVal = SilkDatastore::GotFilePathname;

    string yearPathname;

    if (_currYearIndex >= _numYears)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Info,
                                           "SilkDatastore::SilkDatastoreBin::getNextFilePathname No more year directories in dir: %s (currYearIdx: %d), (num years: %d)",
                                           _binName.c_str(),
                                           _currYearIndex,
                                           _numYears);

        throw SilkException(SilkException::NoMoreYearsInBinDir, "SilkDatastore::SilkDatastoreBin::getNextFilePathname");
    }
    else
    {
        yearRetVal = _silkYearDirList[_currYearIndex].getNextFilePathname(yearPathname);

        if (yearRetVal == SilkDatastore::GotFilePathname)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkDatastoreBin::getNextFilePathname (currYearIdx: %d), (num years: %d), (year pathname: %s)",
                                               _currYearIndex,
                                               _numYears,
                                               yearPathname.c_str());
            filePathname = _binName;
            filePathname += "/";
            filePathname += yearPathname;
        }
        else if (yearRetVal == SilkDatastore::LastMonthInYear)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkDatastoreBin::getNextFilePathname (last month in year) (currYearIdx: %d), (num years: %d), (year pathname: %s)",
                                               _currYearIndex,
                                               _numYears,
                                               yearPathname.c_str());
            filePathname = _binName;
            filePathname += "/";
            filePathname += yearPathname;
            _currYearIndex++;

            if (_currYearIndex >= _numYears)
            {
                retVal = SilkDatastore::LastYearInBin;
                _bNoMoreYears = true;
            }
        }
        else
        {
            throw SilkException(SilkException::NoMoreMonthsInYear, "SilkDatastore::SilkDatastoreBin::getNextFilePathname");
        }
    }

    return retVal;
}

//============ SilkYearDir

SilkDatastore::SilkYearDir::SilkYearDir()
{
    initMembers();
}

SilkDatastore::SilkYearDir::SilkYearDir(const SilkDatastore::SilkYearDir &in)
{
    initMembers();

    *this = in;
}

SilkDatastore::SilkYearDir::~SilkYearDir()
{
}

SilkDatastore::SilkYearDir &SilkDatastore::SilkYearDir::operator=(const SilkDatastore::SilkYearDir &in)
{
    _pathname = in._pathname; // e.g. /data/silk/int2int
    _year = in._year; // e.g. 2008
    _numFilesInYear = in._numFilesInYear;
    _currMonthIndex = in._currMonthIndex;
    _numMonthsInYearDir = in._numMonthsInYearDir;
    _silkMonthDirList = in._silkMonthDirList;
    _bNoMoreMonths = in._bNoMoreMonths;

    return *this;
}

void SilkDatastore::SilkYearDir::initMembers()
{
    _pathname = ""; // e.g. /data/silk/int2int
    _year = ""; // e.g. 2008
    _numFilesInYear = 0;
    _currMonthIndex = 0;
    _numMonthsInYearDir = 0;
    _bNoMoreMonths = false;
}

void SilkDatastore::SilkYearDir::initialize(std::string &yearDirName, std::string &pathname)
{
    _year = yearDirName;
    _pathname = pathname;

    initializeBin();
}

void SilkDatastore::SilkYearDir::initializeBin()
{
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _year;

    // initialize path to get years
    _yearPath.initialize(pathname);

    // _binPath.print();

    // initialize year list
    initializeMonthList();

    // TODO: initialize path, Year, Month, Day
    //validate();

}

void SilkDatastore::SilkYearDir::initializeMonthList()
{
    Path::PathEntry pathEntry; // is a year
    string month;
    _numMonthsInYearDir = _yearPath.getNumEntries();

    BasicExceptionMgr::instance()->log(LogLevel::Finer,
                                       "SilkDatastore::SilkYearDir::initializeMonthList number months %d",
                                       _numMonthsInYearDir);

    for (long i = 0; i < _numMonthsInYearDir; i++)
    {
        pathEntry = _yearPath.getEntry(i);

        if (pathEntry.isDir())
        {
            month = pathEntry.getName();
            addMonth(month);
        }
        else if (pathEntry.isEndOfEntries())
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkYearDir::initializeMonthList End of entries %ld",
                                               i);
        }
        else
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkYearDir::initializeMonthList Expected a directory in path: %s/%s",
                                               _yearPath.getPathname().c_str(),
                                               month.c_str());
        }
    }
}

void SilkDatastore::SilkYearDir::addMonth(string &month)
{
    SilkDatastore::SilkMonthDir silkMonthDir;
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _year;

    if (_pathname.compare("") == 0)
    {
        throw SilkException(SilkException::PathnameNotSet, "SSilkDatastore::SilkYearDir::addMonth");
    }

    BasicExceptionMgr::instance()->log(LogLevel::Finer,
                                       "SilkDatastore::SilkYearDir::addMonth adding %s",
                                       pathname.c_str());

    silkMonthDir.initialize(month, pathname);

    _silkMonthDirList.push_back(silkMonthDir);
}

int SilkDatastore::SilkYearDir::getNextFilePathname(string &filePathname)
{
    int retVal = SilkDatastore::GotFilePathname;
    int monthRetVal = SilkDatastore::GotFilePathname;
    string monthPathname;

    if (_currMonthIndex >= _numMonthsInYearDir)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error,
                                           "SilkDatastore::SilkYearDir::getNextFilePathname No more month directories in year %s (currMonthIdx: %d), (num months: %d)",
                                           _currMonthIndex,
                                           _numMonthsInYearDir,
                                           _year.c_str());

        throw SilkException(SilkException::NoMoreMonthsInYear, "SilkDatastore::SilkYearDir::getNextFilePathname");
    }
    else
    {
        monthRetVal = _silkMonthDirList[_currMonthIndex].getNextFilePathname(monthPathname);

        if (monthRetVal == SilkDatastore::GotFilePathname)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkYearDir::getNextFilePathname (currMonthIdx: %d), (num months: %d), (month pathname: %s)",
                                               _currMonthIndex,
                                               _numMonthsInYearDir,
                                               monthPathname.c_str());
            filePathname = _year;
            filePathname += "/";
            filePathname += monthPathname;
        }
        else if (monthRetVal == SilkDatastore::LastDayInMonth)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkYearDir::getNextFilePathname (last day in month) (currMonthIdx: %d), (num months: %d), (month pathname: %s)",
                                               _currMonthIndex,
                                               _numMonthsInYearDir,
                                               monthPathname.c_str());
            filePathname = _year;
            filePathname += "/";
            filePathname += monthPathname;
            _currMonthIndex++;

            if (_currMonthIndex >= _numMonthsInYearDir)
            {
                _bNoMoreMonths = true;
                retVal = SilkDatastore::LastMonthInYear;
            }
        }
        else
        {
            throw SilkException(SilkException::NoMoreDaysInMonth, "SilkDatastore::SilkYearDir::getNextFilePathname");
        }
    }


    return retVal;
}

//============ SilkMonthDir

SilkDatastore::SilkMonthDir::SilkMonthDir()
{
    initMembers();
}

SilkDatastore::SilkMonthDir::SilkMonthDir(const SilkDatastore::SilkMonthDir &in)
{
    initMembers();

    *this = in;
}

SilkDatastore::SilkMonthDir::~SilkMonthDir()
{
}

SilkDatastore::SilkMonthDir &SilkDatastore::SilkMonthDir::operator=(const SilkDatastore::SilkMonthDir &in)
{
    _pathname = in._pathname; // e.g. /data/silk/int2int/2008
    _month = in._month; // e.g. 01
    _numFilesInMonth = in._numFilesInMonth;
    _currDayIndex = in._currDayIndex;
    _bNoMoreDays = in._bNoMoreDays;
    _silkDayDirList = in._silkDayDirList;
    _numDaysInMonthDir = in._numDaysInMonthDir;

    return *this;
}

void SilkDatastore::SilkMonthDir::initMembers()
{
    _pathname = ""; // e.g. /data/silk/int2int/2008
    _month = ""; // e.g. 01
    _numFilesInMonth = 0;
    _currDayIndex = 0;
    _bNoMoreDays = false;
    _numDaysInMonthDir = 0;
}

void SilkDatastore::SilkMonthDir::initialize(std::string &monthDirName, std::string &pathname)
{
    _month = monthDirName;
    _pathname = pathname;

    initializeBin();
}

void SilkDatastore::SilkMonthDir::initializeBin()
{
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _month;

    // initialize path to get years
    _monthPath.initialize(pathname);

    // _binPath.print();

    // initialize year list
    initializeDayList();

    // TODO: initialize path, Year, Month, Day
    // validate();

}

void SilkDatastore::SilkMonthDir::initializeDayList()
{
    Path::PathEntry pathEntry; // is a year
    string day;
    _numDaysInMonthDir = _monthPath.getNumEntries();

    BasicExceptionMgr::instance()->log(LogLevel::Finer,
                                       "SilkDatastore::SilkMonthDir::initializeDayList number days %d",
                                       _numDaysInMonthDir);


    for (long i = 0; i < _numDaysInMonthDir; i++)
    {
        pathEntry = _monthPath.getEntry(i);

        if (pathEntry.isDir())
        {
            day = pathEntry.getName();
            addDay(day);
        }
        else if (pathEntry.isEndOfEntries())
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkMonthDir::initializeDayList End of entries %ld",
                                               i);
        }
        else
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkMonthDir::initializeDayList Expected a directory in path: %s/%s",
                                               _monthPath.getPathname().c_str(),
                                               day.c_str());
        }
    }
}

void SilkDatastore::SilkMonthDir::addDay(string day)
{
    SilkDatastore::SilkDayDir silkDayDir;
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _month;

    if (_pathname.compare("") == 0)
    {
        throw SilkException(SilkException::PathnameNotSet, "SilkDatastore::SilkMonthDir::addDay");
    }

    BasicExceptionMgr::instance()->log(LogLevel::Finer,
                                       "SilkDatastore::SilkMonthDir::addDay adding %s",
                                       pathname.c_str());

    silkDayDir.initialize(day, pathname);

    _silkDayDirList.push_back(silkDayDir);
}

int SilkDatastore::SilkMonthDir::getNextFilePathname(string &filePathname)
{
    int retVal = SilkDatastore::GotFilePathname;
    int dayRetVal = SilkDatastore::GotFilePathname;
    string dayPathname;

    if (_currDayIndex >= _numDaysInMonthDir)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error,
                                           "SilkDatastore::SilkMonthDir::getNextFilePathname No more day directories in month %s (currDayIdx: %d), (num days: %d)",
                                           _month.c_str(),
                                           _currDayIndex,
                                           _numDaysInMonthDir);

        throw SilkException(SilkException::NoMoreDaysInMonth, "SilkDatastore::SilkMonthDir::getNextFilePathname");
    }
    else
    {
        dayRetVal = _silkDayDirList[_currDayIndex].getNextFilePathname(dayPathname);

        if (dayRetVal == SilkDatastore::GotFilePathname)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkMonthDir::getNextFilePathname (currDayIdx: %d), (num days: %d), (day pathname: %s)",
                                               _currDayIndex,
                                               _numDaysInMonthDir,
                                               dayPathname.c_str());
            filePathname = _month;
            filePathname += "/";
            filePathname += dayPathname;
        }
        else if (dayRetVal == SilkDatastore::LastFileInDay)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkMonthDir::getNextFilePathname (last file in day) (currDayIdx: %d), (num days: %d), (day pathname: %s)",
                                               _currDayIndex,
                                               _numDaysInMonthDir,
                                               dayPathname.c_str());
            filePathname = _month;
            filePathname += "/";
            filePathname += dayPathname;
            _currDayIndex++;

            if (_currDayIndex >= _numDaysInMonthDir)
            {
                _bNoMoreDays = true;
                retVal = SilkDatastore::LastDayInMonth;
            }

        }
        else
        {
            throw SilkException(SilkException::NoMoreFiles, "SilkDatastore::SilkMonthDir::getNextFilePathname");
        }
    }

    return retVal;
}


//============ SilkDayDir

SilkDatastore::SilkDayDir::SilkDayDir()
{
    initMembers();
}

SilkDatastore::SilkDayDir::SilkDayDir(const SilkDatastore::SilkDayDir &in)
{
    initMembers();

    *this = in;
}

SilkDatastore::SilkDayDir::~SilkDayDir()
{
}

SilkDatastore::SilkDayDir &SilkDatastore::SilkDayDir::operator=(const SilkDatastore::SilkDayDir &in)
{
    _pathname = in._pathname; // e.g. /data/silk/int2int/2008/01
    _day = in._day; // e.g. 01
    _numFilesInDay = in._numFilesInDay;
    _currFileIndex = in._currFileIndex;
    _bNoMoreFiles = in._bNoMoreFiles;
    _fileList = in._fileList;

    return *this;
}

void SilkDatastore::SilkDayDir::initMembers()
{
    _pathname = ""; // e.g. /data/silk/int2int/2008/01
    _day = ""; // e.g. 01
    _numFilesInDay = 0;
    _currFileIndex = 0;
    _bNoMoreFiles = false;
}

void SilkDatastore::SilkDayDir::initialize(std::string &dayDirName, std::string &pathname)
{
    _day = dayDirName;
    _pathname = pathname;

    initializeBin();
}

void SilkDatastore::SilkDayDir::initializeBin()
{
    string pathname;

    pathname = _pathname;
    pathname += "/";
    pathname += _day;

    // initialize path to get years
    _dayPath.initialize(pathname);

    // _binPath.print();

    // initialize year list
    initializeFileList();

    // TODO: initialize path, Year, Month, Day
    //validate();
}

void SilkDatastore::SilkDayDir::initializeFileList()
{
    Path::PathEntry pathEntry; // is a year
    string file;
    _numFilesInDay = _dayPath.getNumEntries();

    BasicExceptionMgr::instance()->log(LogLevel::Finer,
                                       "SilkDatastore::SilkDayDir::initializeFileList number files %d",
                                       _numFilesInDay);

    for (long i = 0; i < _numFilesInDay; i++)
    {
        pathEntry = _dayPath.getEntry(i);

        if (pathEntry.isFile())
        {
            file = pathEntry.getName();

            BasicExceptionMgr::instance()->log(LogLevel::Fine,
                                               "SilkDatastore::SilkDayDir::initializeFileList adding %s/%s",
                                               _dayPath.getPathname().c_str(),
                                               file.c_str());

            _fileList.push_back(file);
        }
        else if (pathEntry.isEndOfEntries())
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkDayDir::initializeDayList End of entries %ld",
                                               i);
        }
        else
        {
            BasicExceptionMgr::instance()->log(LogLevel::Info,
                                               "SilkDatastore::SilkDayDir::initializeDayList Expected a file in path: %s/%s",
                                               _dayPath.getPathname().c_str(),
                                               file.c_str());
        }
    }
}

int SilkDatastore::SilkDayDir::getNextFilePathname(string &filePathname)
{
    int retVal = SilkDatastore::GotFilePathname;
    string filename;

    if (_bNoMoreFiles == true)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error,
                                           "SilkDatastore::SilkDayDir::getNextFilePathname No more files directories in day %s, (_currFileIndex: %d), (num files: %d)",
                                           _day.c_str(),
                                           _currFileIndex,
                                           _numFilesInDay);
        throw SilkException(SilkException::NoMoreFiles, "SilkDatastore::SilkDayDir::getNextFilePathname");
    }

    if (_currFileIndex >= _numFilesInDay)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error,
                                           "SilkDatastore::SilkDayDir::getNextFilePathname No more files directories in day %s, (_currFileIndex: %d), (num files: %d)",
                                           _day.c_str(),
                                           _currFileIndex,
                                           _numFilesInDay);

        throw SilkException(SilkException::NoMoreFiles, "SilkDatastore::SilkDayDir::getNextFilePathname");
    }
    else
    {
        filePathname = _day;
        filename = _fileList[_currFileIndex];
        filePathname += "/";
        filePathname += filename;

        BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastore::SilkDayDir::getNextFilePathname (_currFileIndex: %d), (num files: %d), (filename: %s)",
                                           _currFileIndex,
                                           _numFilesInDay,
                                           filename.c_str());
        _currFileIndex++;

        if (_currFileIndex >= _numFilesInDay)
        {
            _bNoMoreFiles = true;
            retVal = SilkDatastore::LastFileInDay;
        }
    }

    return retVal;
}
