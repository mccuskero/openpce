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

#ifndef __BasicException_h
#define __BasicException_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

namespace pcff 
{

class BasicException 
{
 public:
  enum DEFAULT_ERROR_LIST {
    BadLogCallback,          
    InputErrorArg,           
    UnknownError,             
    UnitTestNotFound,         
    NoTestDefinedInArgs,      
    InputArgError,            
    NEXT_AVAILABLE_ERROR_VAL   
  };

 
  enum ERROR_TYPES {
    INFO = 0,
    WARNING = 1,
    ERR = 2
  };


  enum CONSTANTS {
    MAX_ERROR_MSG_LEN = 1024,
    DEFAULT_LOG_MSG_BASE_LEN = 64
  };

 public:
  BasicException() 
  {
    init();
  }
 
  BasicException(int errType, 
		int majorErr, 
		char* errStr,
		char* errMsg = NULL) 
  {
    init();
    _type = errType;
    _majorErrorCode = majorErr;
    _minorErrorCode = UnknownError;
    setErrString( errStr );
    setErrMsg( errMsg );
  }

  BasicException(int errType, 
		int majorErr, 
		int minorErr, 
		char* errStr ) 
  {
    init();
    _type = errType;
    _majorErrorCode = majorErr;
    _minorErrorCode = minorErr;
    setErrString( errStr );
  }

  virtual ~BasicException();
  
 public:
 
  void init()
  {
    _type = ERR; 
    _majorErrorCode = _minorErrorCode = UnknownError;
  }

  int getType()         { return _type; }
  char* getString()        { return (char *)_errString.c_str(); }
  char* getMsg()        { return (char *)_errMsg.c_str(); }
  int getMajorErrCode() { return _majorErrorCode; }
  int getMinorErrCode() { return _minorErrorCode; } 
  char* getLogMsg();

 protected:
    void setErrString( char *errString );
    void setErrString( const char *errString );
    void setErrMsg( char *errString );
    void setErrMsg( const char *errString );
    
 protected:
  int  			_type;				// exception type
  std::string 	_errString;
  std::string 	_errMsg;
  int  			_majorErrorCode;
  int  			_minorErrorCode;

 private:
  static const char *ErrTypes[];
};

};


#endif
