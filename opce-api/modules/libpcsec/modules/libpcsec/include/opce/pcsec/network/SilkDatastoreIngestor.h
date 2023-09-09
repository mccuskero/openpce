/**
 * Copyright (c) 1994 Owen McCusker <pcave@myeastern.com>
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

/** \class SilkDatastoreIngestor
 * \brief Manages ingestion of Silk datastore respositories
 * \author Owen McCusker
 * \version
 * \date : 04/17/2008
 *
 * This class manages the ingestion of silk datastore respositories.
 * The client calling on the class will subscribe to various bins
 * where silk data is stored in the repository.
 *
 * The client call also subscribe to custom bins using strings
 * representing directory names in the high level directory of the datastore.
 *
 * \bug
 * \warning
 * \todo
 * Debug mechanisms: printFiles, printBins, ...
 *
 */

#ifndef SilkDatastoreIngestor_h_
#define SilkDatastoreIngestor_h_

#include <string>

#include <pcff/event/proxy/EventIngestor.h>

#include <pcsec/network/SilkDatastore.h>

namespace pcsec
{

class SilkDatastoreIngestor : public pcff::EventIngestor
{
public:
    SilkDatastoreIngestor();
    virtual ~SilkDatastoreIngestor();

public:
    virtual void initialize();

public:
    void subscribe(SilkDatastore::eSilkBin bin);
    void subscribe(std::string binName);

public:
    // pathname could be a file or directory
    virtual void ingest();

private:
    virtual void testIngest();
    virtual void batchIngest();
    virtual void continuousIngest();

public:
    void printFiles();
    void printBins();

public:

    void setDatastorePathname(const char *pathname);

    void setLogDir(const char *logDir)
    {
        _snortLogDir = logDir;
    }

private:
    SilkDatastore _datastore;
    std::string   _pathname;
    std::string   _snortLogDir;
};

};

#endif /*SilkDatastoreIngestor_h_*/
