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

#ifndef InformationList_h_
#define InformationList_h_

#include <map>
#include <string>

#include <pcff/AttributeValuePair.h>

namespace pcff
{


class InformationList
{
public:
	InformationList();
	virtual ~InformationList();

private:
    void initPrivateMembers();

public:
    void init( int approxNumConfigOptions );

public:
    void  set(const char *name, pcff::AttributeValuePair::ValueType type, const char *val);
    void  set(const char *name, pcff::AttributeValuePair::ValueType type, std::string &val);
    void  set(const char *name, pcff::AttributeValuePair::ValueType type, int  val);
    void  set(const char *name, pcff::AttributeValuePair::ValueType type, long val);
    void  set(const char *name, pcff::AttributeValuePair::ValueType type, float val);
    pcff::AttributeValuePair   &get(const char *name);
	
private:
    int                            _listSize;   // size of the array
    std::map<std::string, pcff::AttributeValuePair>   _informationList;   
};

};
#endif /*InformationList_h_*/
