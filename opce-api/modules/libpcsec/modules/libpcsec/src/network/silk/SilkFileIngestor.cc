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
 * \class SilkFileIngestor
 * \brief Ingests flows  from  SilK output files
 */

#include <string>

#include <pcff/DumpBuffer.h>
#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcsec/network/SilkInterfaceDirector.h>
#include <pcsec/network/SilkException.h>
#include <pcsec/network/SilkRecord.h>
#include <pcsec/network/SilkFile.h>
#include <pcsec/network/SilkFileIngestor.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkFileIngestor::SilkFileIngestor()
{
    initMembers();
}

SilkFileIngestor::~SilkFileIngestor()
{
    if (_silkFile != NULL)
    {
        delete _silkFile;
        _silkFile = NULL;
    }
}

void SilkFileIngestor::initMembers()
{
    _bIngest = true;
    _silkFile = NULL;
    _filename = "";
    _datastore = "";
}

void SilkFileIngestor::initialize()
{
    string pathname;

    if (SilkInterfaceDirector::instance().isInitialized() == false)
    {
        throw SilkException(SilkException::SilkFrameworkNotInitialized, "SilkFileIngestor::initialize");
    }

    pathname = _datastore;
    pathname += "/";
    pathname += _filename;

    BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkFileIngestor data filename: %s", pathname.c_str());

    if (_datastore.compare("") == 0) throw SilkException(SilkException::InvalidDatastore, "SilkFileIngestor::initialize");
    if (_filename.compare("") == 0) throw SilkException(SilkException::InvalidFilename, "SilkFileIngestor::initialize");

    _silkFile = new SilkFile();
    _silkFile->initialize(pathname.c_str());
}

void SilkFileIngestor::ingest()
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

void SilkFileIngestor::testIngest()
{

}

void SilkFileIngestor::batchIngest()
{

}

void SilkFileIngestor::continuousIngest()
{
    SilkFile *silkFile = NULL;
    SilkRecord *pSilkRec = NULL;
    int retVal = SilkFile::Record_OK;

    try
    {
        DumpBuffer::instance().init();

        // TODO: capture signal, TERM, then tell proxy service to shutdown, top ingesting
        while (_bIngest == true)
        {
            BasicExceptionMgr::instance()->log(LogLevel::Debug, "SilkFileIngestor::continuousIngest - starting...");

            retVal = _silkFile->getNextRecord(&pSilkRec);

            if (retVal == SilkFile::Record_OK)
            {
                BasicExceptionMgr::instance()->log(LogLevel::Debug, "SilkFileIngestor::continuousIngest - received record");
                if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
                {
                    //				    DumpBuffer::instance().dump( pSilkRec->getAsStreamBuf() );
                }

                _proxyService->process(*pSilkRec);

                delete pSilkRec;

            }
            else
            {
                if (retVal == SilkFile::Record_EOF)
                {
                    BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkFileIngestor::continuousIngest - EOF received during ingest.");
                }
                else
                {
                    BasicExceptionMgr::instance()->log(LogLevel::Info, "SilkFileIngestor::continuousIngest - Unknown return value.");
                }

                _proxyService->setFinished();

                _bIngest = false;
            }
        }

        BasicExceptionMgr::instance()->log("SilkFileIngestor::continuousIngest - exiting...", LogLevel::Info);
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Unknown exception");
    }
}

