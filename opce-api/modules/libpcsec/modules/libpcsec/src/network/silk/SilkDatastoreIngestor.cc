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
/*
 * \class SilkDatastoreIngestor
 * \brief Ingests flows  from  SilK output files
 */

#include <string>

#include <pcff/DumpBuffer.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcsec/network/SilkRecord.h>
#include <pcsec/network/SilkFile.h>
#include <pcsec/network/SilkDatastore.h>
#include <pcsec/network/SilkInterfaceDirector.h>
#include <pcsec/network/SilkException.h>

#include <pcsec/network/SilkDatastoreIngestor.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkDatastoreIngestor::SilkDatastoreIngestor()
{
}

SilkDatastoreIngestor::~SilkDatastoreIngestor()
{
}

void SilkDatastoreIngestor::initialize()
{
    BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastoreIngestor::initialize");

    if (SilkInterfaceDirector::instance().isInitialized() == false)
    {
        throw SilkException(SilkException::SilkFrameworkNotInitialized, "SilkDatastoreIngestor::initialize");
    }

    // initialize the datastore
    _datastore.initialize(_pathname);
}

void SilkDatastoreIngestor::setDatastorePathname(const char *pathname)
{
    _pathname = pathname;
    _datastore.setPathname(_pathname);
}

void SilkDatastoreIngestor::subscribe(SilkDatastore::eSilkBin ebin)
{
    BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastoreIngestor::subscribe" );

    _datastore.subscribe(ebin);
}

void SilkDatastoreIngestor::subscribe(string binName)
{
    BasicExceptionMgr::instance()->log(LogLevel::Finer, "SilkDatastoreIngestor::subscribe (%s)", binName.c_str());

    _datastore.subscribe(binName);
}

void SilkDatastoreIngestor::ingest()
{
    switch (_mode)
    {
    case EventIngestor::Test :
                testIngest();
        break;
    case EventIngestor::Batch :
                batchIngest();
        break;
    case EventIngestor::Continuous :
                continuousIngest();
        break;
    default:
        break;
    }
}

void SilkDatastoreIngestor::testIngest()
{
    BasicExceptionMgr::instance()->log(LogLevel::Warn, "SilkDatastoreIngestor::testIngest - not implemented");
}

void SilkDatastoreIngestor::batchIngest()
{
    BasicExceptionMgr::instance()->log(LogLevel::Warn, "SilkDatastoreIngestor::batchIngest - not impelemented");
}

void SilkDatastoreIngestor::continuousIngest()
{
    bool bNotEOF = false;
    bool bMoreFiles = false;
    SilkFile *silkFile = NULL;
    SilkRecord *pSilkRec = NULL;
    int retVal = SilkFile::Record_OK;
    int retValDatastore = SilkDatastore::GotFilePathname;

    BasicExceptionMgr::instance()->log(LogLevel::Debug, "SilkDatastoreIngestor::continuousIngest - starting...");

    try
    {
        DumpBuffer::instance().init();

        retValDatastore = _datastore.getNextFile(silkFile);

        while  (_datastore.hasMoreFiles() == true)
        {
            if (retValDatastore == SilkDatastore::GotFilePathname)
            {
                while (bNotEOF == false && silkFile != NULL)
                {

                    try {
                        retVal = silkFile->getNextRecord(&pSilkRec);
                    }
                    // handle bad files...
                    catch (SilkException& ex)
                    {
                        retVal = SilkFile::Record_EOF;
                        BasicExceptionMgr::instance()->handle(ex);
                    }

                    if (retVal == SilkFile::Record_OK)
                    {
                        BasicExceptionMgr::instance()->log(LogLevel::Debug, "SilkDatastoreIngestor::continuousIngest - received record");
                        if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Debug)
                        {
    //                        DumpBuffer::instance().dump( pSilkRec->getAsStreamBuf() );
                        }

                        _proxyService->process(*pSilkRec);

                        delete pSilkRec;
                    }
                    else
                    {
                        if (retVal == SilkFile::Record_EOF)
                        {
                            BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkDatastoreIngestor::continuousIngest - EOF received during ingest.");
                        }
                        else
                        {
                            BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkDatastoreIngestor::continuousIngest - Unknown return value.");
                        }

                        bNotEOF = true;
                    }
                } // while notEOF

                delete silkFile;
                silkFile = NULL;
            }
            else if (retValDatastore == SilkDatastore::SilkFileError)
            {
                BasicExceptionMgr::instance()->log(LogLevel::Error, "SilkDatastoreIngestor::continuousIngest - Silk file error.");
            }

            retValDatastore = _datastore.getNextFile(silkFile);
            bNotEOF = false;   // reset EOF
        } // while still more files
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Unknown exception");
    }

    BasicExceptionMgr::instance()->log("SilkDatastoreIngestor::continuousIngest - exiting...", LogLevel::Info);
}

