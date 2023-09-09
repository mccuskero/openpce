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
 * \brief Ingests network flows from a output files 
 */

#ifndef SilkFileIngestor_h_
#define SilkFileIngestor_h_

#include <string>

#include <pcff/event/proxy/EventIngestor.h>

namespace pcsec 
{

class SilkFile;

class SilkFileIngestor : public pcff::EventIngestor
{
public:
	SilkFileIngestor();
	virtual ~SilkFileIngestor();
	
private:
	void initMembers();
	
public:
	virtual void initialize();
	// pathname could be a file or directory
	virtual void ingest();

private:
	virtual void testIngest();
	virtual void batchIngest();
	virtual void continuousIngest();

public:
	void setDatastore(const char *datastore)   { _datastore = datastore; }
	void setFilename( const char *filename )	 { _filename = filename; }

private:
	std::string _datastore;
	std::string _filename;
	std::string _snortLogDir;
	bool		_bIngest;
	SilkFile    *_silkFile;
};

};

#endif /*SNORTUNIFIEDFILEINGESTOR_H_*/
