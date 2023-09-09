/**
 * Copyright (c)  1994-2007 Owen McCusker <pcave@myeastern.com>
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
 * \class SilkDatastore 
 * \brief Manages SiLK datastore repository. 
 * 
 */


#include <sstream>

#include <pcff/BasicExceptionMgr.h>

#include <pcsec/network/SilkException.h>
#include <pcsec/network/SilkRecord.h>

#include <pcsec/network/SilkFile.h>

using namespace pcff;
using namespace pcsec;

SilkFile::SilkFile()
{
    initMembers();
}

SilkFile::~SilkFile()
{
    if (_sRWIOStructIn != NULL)
    {
        skStreamDestroy(&(_sRWIOStructIn));
    }
}

void SilkFile::initMembers()
{
    _sRWIOStructIn = NULL;
}

void SilkFile::initialize(const char *datafile)
{
    int rv = RWIO_OK;
    int r_rv = RWIO_OK;

    // open the data file
    if ((r_rv = skStreamCreate(&_sRWIOStructIn, SK_IO_READ, SK_CONTENT_SILK_FLOW))
            || (r_rv = skStreamBind(_sRWIOStructIn, datafile))
            || (r_rv = skStreamOpen(_sRWIOStructIn))
            || (r_rv = skStreamReadSilkHeader(_sRWIOStructIn, NULL)))
    {
        if (r_rv == RWIO_OK || r_rv == SKSTREAM_ERR_EOF)
        {
            r_rv = 0;
        }
        else
        {
            skStreamPrintLastErr(_sRWIOStructIn, r_rv, &skAppPrintErr);
            throw SilkException(SilkException::ErrorOpeningSilkFile, (char*) datafile);
        }
    }
}

int SilkFile::getNextRecord(SilkRecord **silkRecord)
{
    int ret = SilkFile::Record_OK;
    int r_rv = RWIO_OK;
    rwRec rwrec;

    memset(&rwrec, '0', sizeof (rwRec));
    *silkRecord = NULL;

    r_rv = skStreamReadRecord(_sRWIOStructIn, &rwrec);

    if (r_rv == RWIO_OK)
    {
        *silkRecord = new SilkRecord();
        (*silkRecord)->initialize(rwrec);
    }
    else if (r_rv == SKSTREAM_ERR_EOF)
    {
        ret = SilkFile::Record_EOF;
    }
    else
    {
        skStreamPrintLastErr(_sRWIOStructIn, r_rv, &skAppPrintErr);
        throw SilkException(SilkException::ErrorGettingRecord, "SilkFile::getNextRecord");
    }

    return ret;
}
