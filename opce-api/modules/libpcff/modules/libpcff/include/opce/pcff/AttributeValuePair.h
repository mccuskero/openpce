/**
 * Copyright (c) 1994-2007 Owen McCusker <mccuskero@ct.metrocast.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#ifndef AttributeValuePair_h_
#define AttributeValuePair_h_

#include <stdlib.h>

namespace pcff
{

class AttributeValuePair 
{
public: 
	AttributeValuePair();
	AttributeValuePair(const AttributeValuePair &in);
    virtual ~AttributeValuePair();

public:
    typedef enum 
    {
        UNKNOWN=-1,
        BOOL=1,
        LONG,
        INT,
        FLOAT,
        STR
    } ValueType;
    
public:
    void init( const char *name, ValueType type, const char *val );
    void init( const char *name, ValueType type, std::string &val );
    void init( const char *name, ValueType type, int val );
    void init( const char *name, ValueType type, long val );
    void init( const char *name, ValueType type, float val );
        
public:    
	AttributeValuePair& operator=(AttributeValuePair& in);
    bool operator==(AttributeValuePair& in);
    bool operator<(AttributeValuePair& in);
   
public:
  const std::string 	 getName()      { return _name; }
  ValueType  getTupleType()  	{ return _tupleType; }

  const std::string getValue()        { return _value; }
  bool        getValueAsBool()  { return (_value == "true") ? true : false;  }			
  int         getValueAsInt()   { return atoi(_value.c_str());               }
  long        getValueAsLong()	{ return atol(_value.c_str());               }
  float       getValueAsFloat()	{ return atof(_value.c_str());               }

public:
  void setValue(std::string val)  { _value = val; }
  void setValue(char *val)        { _value = val;  }         
  
private:
  std::string  	_name;
  ValueType _tupleType;
  std::string  	_value;
};

};
#endif /* AttributeValuePair_h_*/
