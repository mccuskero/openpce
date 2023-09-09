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
*  Description: used to manage configuration optoins in processes,
*    both command line option, and config file options.
*
*  The developer uses all of the underlying functionality provided
* in Config to set the value in all of the tuples. 
*
* It maybe be good practive to hide the init routine in the
* getInstance routine of the derivced class.
* 
* Note: It is expected that a specialized version of the class will
* provide "get" routines to return the specific value, or indicate 
* boolean.
*
********************************************************************    
* Notes: 
*   program info: -V --version:  name build date version 
*   verbosity level: -v -vv -vvv 
*   make it a special case for config
*   groups:
*      in future it might be nice to group options, and have group
*       descriptions.
*   todo:
*   1) print startup values
*   2) print to log, or cout if null
*   3) read in config file (use -c as option for config)
*   4) use singelton pattern
********************************************************************    
*/

#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <pcff/BasicException.h>
#include <pcff/ConfigException.h>
#include <pcff/Config.h>

using namespace std;
using namespace pcff;

//
// function: 
// description:
//
Config::Config()  {
   initPrivateMembers();
}

//
//
//
Config::~Config() {
}


void Config::initPrivateMembers() {
    m_NameWidth = 14;
    m_VerbosityLevel = 0;
    m_bShowUsage = false;
}

void Config::init( int defaultSize ) {
  m_ConfigOptionsSize = defaultSize;
}

//
// TODO: how to add more than one param and value to
// go to option. like -testType and -t for testType???
//
void Config::setAttribute( char *name, 
                           ConfigType type,
                           OptionType opType,
                           char *defaultVal,
                           char *desc ) {

   ConfigTuple *pTuple = new ConfigTuple();
   pTuple->init( name, type, desc, opType, defaultVal );
   
   m_ConfigOptions[pTuple->getName()] = *pTuple;

   delete pTuple;
}

bool Config::showUsageString( int argc, char** argv ) {
    int i = 0;
    bool retVal = false;

    // emit the usage if asked for
    while(i < argc) {
        if(!strcmp(argv[i], "--help") && i < argc) {
            retVal = true;
            printUsage( argv[0] );
        }
        i++;
    } // while(i < argc)

    return retVal;
}


bool Config::showVersionString( int argc, char** argv ) {
    int i = 0;
    bool retVal = false;

    // emit the usage if asked for
    while(i < argc) {
        if(!strcmp(argv[i], "--version") && i < argc) {
            retVal = true;
            printVersionHelper();
        }
        i++;
    } // while(i < argc)
    
    return retVal;
}

void Config::printUsage(char *progName) {
    map<string,ConfigTuple>::iterator iter;
 
    // TODO: use a stringstream to hold usage from derived class. 
    printUsageHelper(progName);

    cout << "-" << setw(m_NameWidth) << setiosflags(ios::left) << "v(vvvv)";
    cout << ": ";
    cout << "used to set the verbosity level (1-5 default is 0)" << endl;
    
    cout << "-" << setw(m_NameWidth) << setiosflags(ios::left) << "(c | config)";
    cout << ": ";
    cout << "config path" << endl;

    cout << "-" << setw(m_NameWidth) << setiosflags(ios::left) << "-version";
    cout << ": ";
    cout << "print version" << endl;
    
    for( iter = m_ConfigOptions.begin(); iter != m_ConfigOptions.end();  iter++) {
        if ((iter->second).getOptionType() == Config::CMDLINE || (iter->second).getOptionType() == Config::BOTH ) {
            cout << "-" << setw(m_NameWidth) << setiosflags(ios::left) << (iter->second).getName();
            cout << ": ";
            cout << (iter->second).getDesc() << endl;
        }
    }
}

//
// should be overwritten
//
void Config::printVersionHelper() {
    cout << "Config (unknown) 0.0.0 (Linux)" << endl;
    cout << "Copyright (C) 2004 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl;
}

void Config::printUsageHelper( char* progName ) {
    cout << "Usage: " << progName << " [OPTION] " << endl;
    cout << "(no example)" << endl;
}

//
// TODO: assume that a log is being used, if Log == NULL use cout
//
void Config::showInitialValues() {
    map<string,ConfigTuple>::iterator iter;
    
    cout << setw(m_NameWidth) << setiosflags(ios::left) << "verbosity"; 
    cout << "= ";
    cout << m_VerbosityLevel << endl;

   // print out the config pathname if applicable
   if (!m_ConfigFilePathname.empty()) {
        cout << setw(m_NameWidth) << setiosflags(ios::left) << "Config pathname";
        cout << "= ";
        cout << m_ConfigFilePathname << endl;
   }

    // TODO: print to a sstream, then output to log or cout
    for( iter = m_ConfigOptions.begin(); iter != m_ConfigOptions.end();  iter++) {
        cout << setw(m_NameWidth) << setiosflags(ios::left) << (iter->second).getName();
        cout << "= ";
        cout << (iter->second).getValue() << endl;
    }
}

void Config::scanVerbosityOption( int argc, char** argv ) {
    int i = 1;
    bool foundVerbosityOption = false;

    while( i < argc && foundVerbosityOption == false ) {
        foundVerbosityOption = checkVerbosityOption( argv[i] );
        i++;
   }    
}

bool Config::checkVerbosityOption( char* argv ) {
    bool retVal = false;
    int i = 1;

   if(!strcmp(argv, "-v")) {
       m_VerbosityLevel = 1;    
       retVal = true;
   }
   else if(!strcmp(argv, "-vv")) {
       m_VerbosityLevel = 2;    
       retVal = true;
   }
   else if(!strcmp(argv, "-vvv")) {
       m_VerbosityLevel = 3;    
       retVal = true;
   }
   else if(!strcmp(argv, "-vvvv")) {
       m_VerbosityLevel = 4;    
       retVal = true;
   }            
   else if(!strcmp(argv, "-vvvvv")) {
       m_VerbosityLevel = 5;    
       retVal = true;
   }            
       
   
    return retVal;
}



//
//
// 
void  Config::parseCommandLine(int argc, char** argv) {
    map<string,ConfigTuple>::iterator iter;
    string sArgv;
    int i = 1;
    bool setConfigTuple = false;
  
    // this is done twice, need to set the flag for 
    // use in this class.
    scanVerbosityOption( argc, argv );
   
    while( i < argc ) {
        setConfigTuple = false;
   		for(    iter = m_ConfigOptions.begin(); 
                    iter != m_ConfigOptions.end() && setConfigTuple != true;
                    iter++) {
            if ( checkVerbosityOption( argv[i] ) == true ) {
                setConfigTuple = true;                  
            }
            // TODO: check config option
            // if checkConfigOption(argc, argv) {
            // setConfigTuple = true;
            // }
            else {
                 // if the config option is for either both file and cmdline or just cmdline, check it against arguments
                if ((iter->second).getOptionType() == Config::CMDLINE || (iter->second).getOptionType() == Config::BOTH ) {
           
                    sArgv = removeFirstDash( argv[i] );				
    
                    if (verbosityLevel(5) == true) {
                        // TODO: put to logger in future
                        cout << "scanCommandLine: " << (iter->second).getName() << " " << sArgv << "i=" << i << endl;
                    }
                    if ( (iter->second).getName().compare(sArgv) == 0) {
                        if ( (iter->second).getConfigType() == Config::BOOL ) {
                            // TODO: what about false!!!!
                            (iter->second).setValue("true");
                        }
                        else {
                            i++;
                            (iter->second).setValue(argv[i]);
                        }
                        setConfigTuple = true;
                    }
                } // if
            }            
        }	 // for
        
        if ( setConfigTuple == false ) {
              printUsage("");
              throw ConfigException( ConfigException::InvalidArgument,  argv[i]);
        }         
		i++;
   	} // while
}

void Config::processAttributeValuePair( string attr, string val) {
map <string, ConfigTuple>::iterator iter;
bool setConfigTuple = false;


   for ( iter = m_ConfigOptions.begin();
         iter != m_ConfigOptions.end() && !setConfigTuple;
         iter++ ) {
      // only set options that are suppose to be set from FILE or BOTH
      if ((iter->second).getOptionType() == Config::FILE || (iter->second).getOptionType() == Config::BOTH) {
         // send output to cout
         // for debugging
         if (verbosityLevel(5) == true) {
             // TODO: put to logger in future
             cout << "Config Option: " << (iter->second).getName() << " " << attr << "  " << val << endl;
         }
         
         // if attr matches config option name, then set
         if ( (iter->second).getName().compare(attr) == 0) {
            // if BOOL, then check val, and set appropriately
            if( (iter->second).getConfigType() == Config::BOOL ) {
               (iter->second).setValue("true");
            }
            else {
               (iter->second).setValue(val);
            }
            setConfigTuple = true;
         }
      }
   }
}

void Config::setPathname( char *pathname ) {
   if (pathname == NULL ) throw ConfigException(ConfigException::InvalidPathname);

   m_ConfigFilePathname = pathname;
}

void Config::readConfigFile() {
   if (m_ConfigFilePathname.empty()) throw ConfigException(ConfigException::PathnameNotSet);
   readConfigFile(m_ConfigFilePathname);
}

void Config::readConfigFile(string sPathname) {
   char *pathname = NULL;
   if (sPathname.empty()) throw ConfigException(ConfigException::PathnameNotSet);

   pathname = (char *)m_ConfigFilePathname.c_str();

   readConfigFile(pathname);
}


// use this when passing option is allowed
// note: if --config is not passed in then will not read,
// then you can use hardcoded config pathname following your
// programming conventions
bool Config::readConfigFile( int argc, char **argv) {
   int   i=0;
   bool  foundConfigOption = false;
   // scan for --config 
   while ( i < argc && foundConfigOption == false) {
      if(strcmp(argv[i], "--config") == 0) {
         foundConfigOption = true;
         setPathname(argv[i]);
         readConfigFile(argv[i]);
      }
      i++;
   }
   
   return foundConfigOption;
}

// TODO: look into using ifstream
// use XML parser!! for config files
void Config::readConfigFile( char *pathname ) {
   string currLine;
   string attr;
   string val;

   if (pathname == NULL ) throw ConfigException(ConfigException::InvalidPathname);

   // open the file
   ifstream configFile (pathname);

   // check to see if file exists
   if (!configFile.fail() ) {

      // get the first line
      while (std::getline(configFile, currLine)) {

         // get the first line
         istringstream  ssLine( currLine );

         // get the first attribute, compare against all file type attributes
         std::getline(ssLine, attr, ' ');
         std::getline(ssLine, val);

         // trim leading whitespace
         string::size_type notwhite = val.find_first_not_of(" \t\n");
         val.erase(0,notwhite);

         // trim trailing whitespace
         notwhite = val.find_last_not_of(" \t\n");
         val.erase(notwhite+1);

         //  handle attribute
         // process the string to see if it is a 
         processAttributeValuePair(attr, val);
      }
      configFile.close();
   }
   else { // TODO: file open failed not found
   
      configFile.close();
    
   }

}

char* Config::removeFirstDash( char *argv ) {
	char *tmpStr = argv;

    if (*tmpStr == '-') {	
	    tmpStr++;
    }

	return tmpStr;	
}

// ConfigTuple
// Inner class
//

Config::ConfigTuple::ConfigTuple() {
    m_Name = "";
    m_ConfigType = UNKNOWN;
    m_Desc = "";
    m_OptionType = CMDLINE;
    m_Value = "";    
}

Config::ConfigTuple::ConfigTuple(const ConfigTuple &in) {
    m_Name = in.m_Name;
    m_ConfigType = in.m_ConfigType;
    m_Desc = in.m_Desc;
    m_OptionType = in.m_OptionType;
    m_Value = in.m_Value;    
}

Config::ConfigTuple::~ConfigTuple() {
    
}

void Config::ConfigTuple::init( char *name, ConfigType type, char *desc, OptionType optType, char* val ) {
    m_Name = name;
    m_ConfigType = type;
    m_Desc = desc;
    m_OptionType = optType;  
    m_Value = val;
}

Config::ConfigTuple& Config::ConfigTuple::operator=(Config::ConfigTuple& in) {
    m_Name = in.m_Name;
    m_ConfigType = in.m_ConfigType;
    m_Desc = in.m_Desc;
    m_OptionType = in.m_OptionType;
    m_Value = in.m_Value;    
}

bool Config::ConfigTuple::operator==(Config::ConfigTuple& in) {
    throw "ConfigTuple: not implemented";
}

bool Config::ConfigTuple::operator<(Config::ConfigTuple& in) {
    throw "ConfigTuple: not implemented";    
}
