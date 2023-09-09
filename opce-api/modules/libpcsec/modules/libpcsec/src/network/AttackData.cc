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

#include <pcsec/network/EthernetHeaderInfo.h>
#include <pcsec/network/WifiHeaderInfo.h>
#include <pcsec/network/IPHeaderInfo.h>
#include <pcsec/network/ICMPInfo.h>
#include <pcsec/network/UDPHeaderInfo.h>
#include <pcsec/network/TCPHeaderInfo.h>

#include <pcsec/network/AttackData.h>

using namespace std;
using namespace pcff;
using namespace pcsec;


AttackData::AttackData()
{
}

AttackData::~AttackData()
{
}

void AttackData::initialize()
{
	// TODO: memset the contain classes
}

void AttackData::initialize(AttackPacket &attack)
{
	_virtualAttack 		= attack.virtual_flag;	/* flag for marking virtual attacks */
	_startTime 			= attack.start_time;		/* time of attack start */
	_endTime 			= attack.end_time;			/* time of attack end */
	_operationalMode 	= attack.op_mode;	/* mode of operation (none, ignore, normal, proxy, mirror) */
	_downloadCount 		= attack.dl_count;	/* number of downloads */

	retrieveConnectionData(attack);
	retrieveProxyConnectionData(attack);
	retrieveDownloadData(attack);
}

void AttackData::retrieveConnectionData(AttackPacket &attack)
{
	_attackConnection._localAddr = attack.sAttackConn.l_addr;
	_attackConnection._remoteAddr = attack.sAttackConn.r_addr;
	_attackConnection._localPort = attack.sAttackConn.l_port;
	_attackConnection._remotePort = attack.sAttackConn.r_port;
	_attackConnection._protocol = attack.sAttackConn.protocol;

	memcpy(_attackConnection._payload._md5sum,    attack.sAttackConn.sPayload.md5sum, MDA5SUM_SIZE);
	memcpy(_attackConnection._payload._sha512sum, attack.sAttackConn.sPayload.sha512sum, SHA512SUM_SIZE);
	_attackConnection._payload._dataCaptureSize = attack.sAttackConn.sPayload.data_capture_size;
	_attackConnection._payload._dataSize        = attack.sAttackConn.sPayload.data_size;
	memcpy(_attackConnection._payload._data,      attack.sAttackConn.sPayload.data, SNAPLEN);
}

void AttackData::retrieveProxyConnectionData(AttackPacket &attack)
{
	_attackProxyConnection._localAddr  = attack.sProxyConn.l_addr;
	_attackProxyConnection._remoteAddr = attack.sProxyConn.r_addr;
	_attackProxyConnection._localPort  = attack.sProxyConn.l_port;
	_attackProxyConnection._remotePort = attack.sProxyConn.r_port;
	_attackProxyConnection._protocol   = attack.sProxyConn.protocol;

	memcpy(_attackProxyConnection._payload._md5sum,    attack.sProxyConn.sPayload.md5sum, MDA5SUM_SIZE);
	memcpy(_attackProxyConnection._payload._sha512sum, attack.sProxyConn.sPayload.sha512sum, SHA512SUM_SIZE);
	_attackProxyConnection._payload._dataCaptureSize = attack.sProxyConn.sPayload.data_capture_size;
	_attackProxyConnection._payload._dataSize        = attack.sProxyConn.sPayload.data_size;
	memcpy(_attackConnection._payload._data,           attack.sProxyConn.sPayload.data, SNAPLEN);
}

void AttackData::retrieveDownloadData(AttackPacket &attack)
{
	memcpy(_downloads[0]._downloadType, attack.sDownload.downloadType, DLTYPELEN);
	_downloads[0]._remoteAddr 	= attack.sDownload.r_addr;
	_downloads[0]._remotePort 	= attack.sDownload.r_port;
	_downloads[0]._protocol   	= attack.sDownload.protocol;

	_downloads[0]._userLen    	  = attack.sDownload.userLen;
	_downloads[0]._userCaptureLen = attack.sDownload.userCaptureLen;
	memcpy(_downloads[0]._user,     attack.sDownload.user, USERLEN);

	_downloads[0]._passwordLen 	       = attack.sDownload.passwordLen;
	_downloads[0]._passwordCaptureLen  = attack.sDownload.passwordCaptureLen;
	memcpy(_downloads[0]._password,      attack.sDownload.password, PSWDLEN);

	_downloads[0]._filenameLen         = attack.sDownload.filenameLen;
	_downloads[0]._filenameCaptureLen  = attack.sDownload.filenameCaptureLen;
	memcpy(_downloads[0]._filename,      attack.sDownload.filename, FILENAMELEN);

	memcpy(_downloads[0]._payload._md5sum,    attack.sDownload.sPayload.md5sum, MDA5SUM_SIZE);
	memcpy(_downloads[0]._payload._sha512sum, attack.sDownload.sPayload.sha512sum, SHA512SUM_SIZE);
	_downloads[0]._payload._dataCaptureSize = attack.sDownload.sPayload.data_capture_size;
	_downloads[0]._payload._dataSize        = attack.sDownload.sPayload.data_size;
	memcpy(_downloads[0]._payload._data,      attack.sDownload.sPayload.data, SNAPLEN);
}

//
// TODO: when honeytrap provides ability to capture more than one
// download, then update.
//
AttackDownloadData   &AttackData::getDownloadData(int idx)
{
	return _downloads[0];
}
