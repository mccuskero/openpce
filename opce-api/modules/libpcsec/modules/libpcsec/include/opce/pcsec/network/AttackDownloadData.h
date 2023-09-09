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

#ifndef AttackDownloadData_h_
#define AttackDownloadData_h_

#include <pcsec/network/honeytrap/honeytrapStructs.h>
#include <pcsec/network/AttackPayloadData.h>

namespace pcsec
{

class AttackDownloadData
{
	friend class AttackData;
	
public:
	AttackDownloadData();
	virtual ~AttackDownloadData();
	
private:
	void initMembers();
	
public:
	u_int8_t   &getDownloadType()		{ return _downloadType[0]; }
	u_int32_t	getRemoteAddr()			{ return _remoteAddr; }
	u_int32_t	getRemotePort()			{ return _remotePort; }
	u_int16_t	getProtocol()			{ return _protocol; }
	u_int16_t	getUserLen()			{ return _userLen; }
	u_int16_t	getUserCaptureLen()		{ return _userCaptureLen; }
	u_int8_t   &getUser()				{ return _user[0]; }
	u_int16_t	getPasswordLen()		{ return _passwordLen; }
	u_int16_t	getPasswordCaptureLen()	{ return _passwordCaptureLen; }
	u_int8_t   &getPassword()			{ return _password[0]; }
	u_int16_t	getFilenameLen()		{ return _filenameLen; }
	u_int16_t	getFilenameCaptureLen()	{ return _filenameCaptureLen; }
	u_int8_t   &getFilename()			{ return _filename[0]; }
	AttackPayloadData &getAttackPayloadData() { return _payload; }
	
protected:
	u_int8_t		_downloadType[DLTYPELEN]; /* (FTP, TFTP, VNC, ...) */
	u_int32_t		_remoteAddr;		            /* remote IP address */
	u_int16_t		_remotePort;		            /* remote port */
	u_int16_t		_protocol;	            /* protocol as in IP header */
	u_int16_t       _userLen;                /* user length */
	u_int16_t       _userCaptureLen;         /* user length */
	u_int8_t		_user[USERLEN];		    /* username for download connection */
	u_int16_t       _passwordLen;                /* user length */
	u_int16_t       _passwordCaptureLen;         /* user length */	
	u_int8_t		_password[PSWDLEN];		    /* user's password */
	u_int16_t       _filenameLen;                /* user length */
	u_int16_t       _filenameCaptureLen;         /* user length */	
	u_int8_t		_filename[FILENAMELEN];	/* filename of download */
	AttackPayloadData _payload;
};

};
#endif /*AttackDownloadData_h_*/
