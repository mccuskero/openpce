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
 * \class SilkFile 
 * \brief This is a Silk Data file retreived from a data store.
 */

#ifndef SILKFILE_H_
#define SILKFILE_H_

extern "C"
{
#include <silk/silk.h>
#include <silk/rwpack.h>
#include <silk/dynlib.h>
}

namespace pcsec
{

class SilkRecord;

class SilkFile
{
public:
	SilkFile();
	virtual ~SilkFile();
	
private:
	void initMembers();
	
public:
	enum eSiLKFile
	{
            FileReadError = -1,
            Record_OK = 1,
            Record_EOF = 2
	};
	
public:
	void 	initialize(const char *dataFileName);
	int 	getNextRecord(SilkRecord **silkRec);
	
private:
	rwIOStruct_t *_sRWIOStructIn;	
};

};
#endif /*SILKFILE_H_*/
