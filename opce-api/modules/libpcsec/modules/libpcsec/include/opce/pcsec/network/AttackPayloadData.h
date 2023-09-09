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

#ifndef AttackPayloadData_h_
#define AttackPayloadData_h_

#include <pcsec/network/honeytrap/honeytrapStructs.h>

namespace pcsec
{

class AttackPayloadData
{
	friend class AttackData;

public:
	AttackPayloadData();
	virtual ~AttackPayloadData();

private:
	void initMembers();
	
public:
	u_int8_t	&getMD5Sum() 			{ return _md5sum[0];    }
	u_int8_t	&getSHA512Sum() 		{ return _sha512sum[0]; }
	u_int32_t    getDataCaptureSize() 	{ return _dataCaptureSize; }
	u_int32_t    getDataSize() 			{ return _dataCaptureSize; }
	u_int8_t	&getData()				{ return _data[0]; }
	
protected:
	u_int8_t		_md5sum[MDA5SUM_SIZE];	/* md5 checksum */
	u_int8_t		_sha512sum[SHA512SUM_SIZE];	/* sha512 checksum */
	u_int32_t		_dataCaptureSize;		/* length of attack string */
	u_int32_t		_dataSize;		        /* length of attack string */
	u_int8_t		_data[SNAPLEN];	        /* attack string */	
};

};
#endif /*AttackPayloadData_h_*/
