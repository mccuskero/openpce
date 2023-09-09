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

#include <sstream>

#include <pcff/BasicExceptionMgr.h>

#include <pcsec/network/AttackDownloadData.h>

using namespace std;
using namespace pcff;
using namespace pcsec;


AttackDownloadData::AttackDownloadData()
{
	initMembers();
}

AttackDownloadData::~AttackDownloadData()
{
}

void AttackDownloadData::initMembers()
{
	memset( _downloadType, '0', DLTYPELEN);
	_remoteAddr = 0;
	_remotePort	= 0;
	_protocol	= 0;
	_userLen	= 0;
	_userCaptureLen	= 0;
	memset( _user, '0', USERLEN);
	_passwordLen	= 0;
	_passwordCaptureLen	= 0;
	memset( _password, '0', PSWDLEN);
	_filenameLen	= 0;
	_filenameCaptureLen = 0;
	memset( _filename, '0', FILENAMELEN);
}
