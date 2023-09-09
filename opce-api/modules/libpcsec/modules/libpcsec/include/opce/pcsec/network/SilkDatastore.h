/**
 * Copyright (c) 2001 Owen McCusker <pcave@ct.metrocast.net>
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

/** \class SilkDatastore
 * \brief Manages ingestion of Silk datastore respositories
 * \author Owen McCusker
 * \version
 * \date : 04/17/2008
 *
 * This class manages the silk datastore respositories.
 *
 * In order to use this class, the client must subscribe to the bins they are
 * interested in. If not subscribtions are made, then all bins are then subscribed.
 *
 * The client call also subscribes to custom bins using strings
 * representing directory names in the high level directory of the datastore.
 *
 * Initialization:
 * The client will subscribe to bins. The system then initialized each Bin separately.
 * Bin Initialization entail collecting lists of years, months in years, and then days
 * months. The day will contain a list of files associated with it.
 *
 *
 * * (from silk tools documentation)
 * The root of the directory tree that contains the packed, binary SiLK Flow files is set
 * by the root-directory switch; this directory is called the SILK_DATA_ROOTDIR.
 * Immediately underneath it are subdirectories corresponding to the traffic categories
 * (directions) discussed above. Under these are directories representing
 * the year, month, and day in YYYY/MM/DD format. That is:
 *
 *  $SILK_DATA_ROOTDIR/in/{$YEAR}/{$MONTH}/{$DAY}/*
 *  $SILK_DATA_ROOTDIR/inweb/{$YEAR}/{$MONTH}/{$DAY}/*
 *  $SILK_DATA_ROOTDIR/innull/{$YEAR}/{$MONTH}/{$DAY}/*
 * $SILK_DATA_ROOTDIR/out/{$YEAR}/{$MONTH}/{$DAY}/*
 *  $SILK_DATA_ROOTDIR/outweb/{$YEAR}/{$MONTH}/{$DAY}/*
 *  $SILK_DATA_ROOTDIR/outnull/{$YEAR}/{$MONTH}/{$DAY}/*
 * For example, output web files for October 4th, 2003 are recorded in $SILK_DATA_ROOTDIR/outweb/2003/10/04/
 * The names of the files in these directories include all of this information, and are written in the form:
 *
 *  I<flowType>-I<sensorName>_YYYYMMDD.HH
 * where flowType encodes the category and sensorName is the sensor where the flow was collected.
 *
 * The above tests allow rwflowpack to support various categories of traffic;
 * each type is stored in a different subdirectory. In a typical installation of SiLK, t
 * he categories are:
 *
 * in      -flows for incoming routed-non-web traffic
 * inicmp  -
 * inweb   -flows for incoming routed-web traffic
 * innull  -flows for incoming not-routed traffic
 * int2int -
 * out     -flows for outgoing routed-non-web traffic
 * outicmp -
 * outweb  -flows for outgoing routed-web traffic
 * outnull -flows for outgoing not-routed traffic
 * ext2ext -
 *
 * \bug
 * \warning
 * \todo
 * TODO: ensure that inner classes only load "dirs" into queues!
 * TODO: ensure that dirs are numerical! for now... check dates later on...
 *
 * Time Management
 * Current files; - need a mechamism to get the current file, currentDataFlag
 * Time window    - need to mechamism to only ingest file between current times
 * Specific Time
 *
 */

#ifndef SILKDATASTORE_H_
#define SILKDATASTORE_H_

#include <string>
#include <queue>
#include <vector>
#include <map>

#include <pcff/TimeStamp.h>
#include <pcff/filesystem/Path.h>

namespace pcsec
{

class SilkFile;

class SilkDatastore
{
    //==============Inner classes to handle years/months/days lists within datstore bin

    class SilkDayDir
    {
    public:
        SilkDayDir();
        SilkDayDir(const SilkDayDir &in);
        virtual ~SilkDayDir();

    private:
        void initMembers();

    public:
        void initialize(std::string &dayPathname, std::string &day);

    private:
        void initializeBin();
        void initializeFileList();

    public:
        SilkDayDir & operator=(const SilkDayDir &in);

    public:
        // returns e.g. 01/I<flowType>-I<sensorName>_YYYYMMDD.HH
        int getNextFilePathname(std::string &filePathname);
        long getNumFiles();

    private:
        std::string _pathname; // e.g. /data/silk/int2int/2008/01
        std::string _day; // e.g. 01
        long _numFilesInDay;
        pcff::Path _dayPath;
        bool _bNoMoreFiles;
        long _currFileIndex;
        std::vector<std::string> _fileList; // I<flowType>-I<sensorName>_YYYYMMDD.HH
    };

    class SilkMonthDir
    {
    public:
        SilkMonthDir();
        SilkMonthDir(const SilkMonthDir &in);
        virtual ~SilkMonthDir();

    private:
        void initMembers();

    public:
        void initialize(std::string &monthPathname, std::string &month);
        void addDay(std::string day);

    private:
        void initializeBin();
        void initializeDayList();

    public:
        SilkMonthDir & operator=(const SilkMonthDir &in);

    public:
        // returns e.g. 01/01/I<flowType>-I<sensorName>_YYYYMMDD.HH
        int getNextFilePathname(std::string &filePathname);
        long getNumFiles();

    private:
        std::string _pathname; // e.g. /data/silk/int2int/2008
        std::string _month; // e.g. 01  (for January)
        long _numFilesInMonth;
        int _numDaysInMonthDir;
        pcff::Path _monthPath;
        long _currDayIndex;
        bool _bNoMoreDays;
        std::vector<SilkDayDir> _silkDayDirList;
    };

    class SilkYearDir
    {
    public:
        SilkYearDir();
        SilkYearDir(const SilkYearDir &in);
        virtual ~SilkYearDir();

    private:
        void initMembers();

    public:
        void initialize(std::string &yearPathname, std::string &year);

    private:
        void initializeBin();
        void initializeMonthList();
        void addMonth(std::string &month);

    public:
        SilkYearDir & operator=(const SilkYearDir &in);

    public:
        // returns e.g. 2008/01/01/I<flowType>-I<sensorName>_YYYYMMDD.HH
        int getNextFilePathname(std::string &filePathname);
        long getNumFiles();

    private:
        std::string _pathname; // e.g. /data/silk/int2int
        std::string _year; // e.g. 2008
        long _numFilesInYear;
        int _numMonthsInYearDir;
        pcff::Path _yearPath;
        long _currMonthIndex;
        bool _bNoMoreMonths;
        std::vector<SilkMonthDir> _silkMonthDirList;

    };
    //==============


    //==============Inner classes to handle datstore bin

    class SilkDatastoreBin
    {
    public:
        SilkDatastoreBin();
        SilkDatastoreBin(const SilkDatastoreBin &in);
        virtual ~SilkDatastoreBin();

    public:
        SilkDatastoreBin & operator=(const SilkDatastoreBin &in);

    private:
        void initMembers();

    private:
        void createCurrentPathname();
        void createCurrentFilename();

    public:
        void initialize(std::string &binName, std::string &pathname);

    private:
        void initializeBin();
        void initializeYearList();
        void addYear(std::string year);

    public:
        // returns e.g. int2int/2008/01/01/I<flowType>-I<sensorName>_YYYYMMDD.HH
        int getNextFilePathname(std::string &filePathname);

    private:
        std::string getNextMonth();
        std::string getNextDay();

    private:
        std::string _binName; // bin name e.g. int2int
        std::string _pathname; // datastore pathanme e.g. /data/silk/
        pcff::TimeStamp _lastRead;
        pcff::Path _binPath;
        int _numYears;
        int _currYearIndex; // points to the current bin index in list
        std::vector<SilkYearDir> _silkYearDirList;

        // members are used to track the current file list for the bin associated
        // year, month, day directory path for the bin.
        std::string _currPathname;
        std::string _currFilename;
        std::string _currYear;
        std::string _currMonth;
        std::string _currDay;
        std::string _currHour;
        //        std::queue<std::string> _currFileList;
        bool _bNoMoreYears;

    };
    //==============

public:

    enum eBinReadConstants
    {
        SilkFileError = -1,
        GotFilePathname,
        LastYearInBin,
        LastMonthInYear,
        LastDayInMonth,
        LastFileInDay,
        NoMoreFiles
    };


    // datastore bin types

    typedef enum
    {
        In,
        InIcmp,
        InWeb,
        InNull,
        Int2Int,
        Out,
        OutIcmp,
        OutWeb,
        OutNull,
        Out2Out,
        AllBins,
        AllInBins,
        AllOutBins,
        NoBins
    } eSilkBin;

public:
    SilkDatastore();
    SilkDatastore(const std::string &pathname);
    virtual ~SilkDatastore();

private:
    void initMembers();

public:
    void setPathname(std::string &pathname);
    void subscribe(std::string &dirName); // custom subscription
    void subscribe(eSilkBin bin); // searches directory for bin name, if not found reports warning...
    void initialize(std::string &path);
    void validate();

private:
    void subscribeToInOutBins();
    void subscribeToAllBins();
    void subscribeToAllInBins();
    void subscribeToAllOutBins();
    void subscribeToDefaultBins();

public:
    int getNextFile(SilkFile*& silkFile);

    bool hasMoreFiles()
    {
        return (_bNoMoreFiles == false) ? true : false;
    }

private:
    int getNextFilePathname(std::string &filePathname);
    int getNextFilePathnameRotateBins(std::string &filePathname);

private:
    void validateDatastorePath(std::string &path);

private:
    void addBin(std::string binName);
    void getNextBin();

private:
    std::string _datastorePathname;
    eSilkBin _currentBinID;
    eSilkBin _binSubscription;
    std::string _currentBinPath;
    std::string _currentMonth;
    std::vector<SilkDatastoreBin> _currBinList;
    int _numBins;
    int _currBinIndex;
    bool _bNoMoreFiles;
};

};

#endif /*SILKDATASTORE_H_*/
