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

#include <map>
#include <string>

#ifndef Config_h_H
#define Config_h_H

namespace pcff 
{

class   Config  
{
    public:
        Config();
        virtual ~Config();

    private:
        void initPrivateMembers();
        virtual void initAttributes() = 0;
        void printUsage( const char * name);     
        virtual void printUsageHelper( const char *name );
        virtual void printVersionHelper();

    public: 
        typedef enum {
            UNKNOWN=-1,
            BOOL=1,
            LONG,
            INT,
            FLOAT,
            STR
        } ConfigType;
      
        typedef enum {
            CMDLINE,
            FILE,
            BOTH
        } OptionType;
        
    public:
        void   init( int approxNumConfigOptions );

   protected:
        void   setAttribute(const char *name, ConfigType type, OptionType opType, const char *defaultVal, const char *desc);
      
   public:
      void  showInitialValues();  
      bool  showUsageString( int argc, char** argv );        
      bool  showVersionString( int argc, char** argv );        
    
      bool  checkVerbosityOption( char* argv );
      void  scanVerbosityOption( int argc, char** argv);
    
      void  parseCommandLine( int argc, char  **argv );

      void  setPathname( char *pathname );
      bool  readConfigFile( int argc, char **argv);   // uses --config option to get config file

	public:
      virtual void  readConfigFile();                         // need to call setPathname
      virtual void  readConfigFile(std::string sPathname);
      virtual void  readConfigFile( char *pathname);
    
    public:
        bool   showUsage()                      { return m_bShowUsage; }
        bool   verbosityLevel( int inLevel )    { return (inLevel<=m_VerbosityLevel); }
        int    getVerbosityLevel()              { return m_VerbosityLevel; } 
    // handle env variables
    public:
        static char *getEnvVar( char *envVar );
    
	private:
		char* removeFirstDash( char *argv );
        virtual void  processAttributeValuePair( std::string attr, std::string val);
	
    // inner class 
    public:
            class ConfigTuple {
                public: 
                    ConfigTuple();
                    ConfigTuple(const ConfigTuple &in);
                    virtual ~ConfigTuple();
                
                public:
                    void init( const char *name, ConfigType type, const char *desc, OptionType optType, const char *val );
                    
                public:    
                    ConfigTuple& operator=(ConfigTuple& in);
                    bool operator==(ConfigTuple& in);
                    bool operator<(ConfigTuple& in);
               
               public:
                  std::string getName()       { return m_Name; }
                  ConfigType  getConfigType() { return m_ConfigType; }
                  std::string getDesc()       { return m_Desc; }
                  OptionType  getOptionType() { return m_OptionType; }

                  std::string getValue()       { return m_Value; }
                  bool getValueAsBool()        { return (m_Value == "true") ? true : false;  }			
                  int   getValueAsInt()        { return atoi(m_Value.c_str());               }
                  long getValueAsLong()	       { return atol(m_Value.c_str());               }
                  float getValueAsFloat()	   { return atof(m_Value.c_str());               }
                
                public:
                  void setValue(std::string val)    { m_Value = val; m_OptionEnabled = true; }
		          void setValue(const char *val)        { m_Value = val; m_OptionEnabled = true; }
		          bool optionEnabled()            { return m_OptionEnabled; }  // true of user passed in arg in command line   
		         
		            
                private:
                  std::string  m_Name;
                  ConfigType   m_ConfigType;
                  std::string  m_Desc;
                  OptionType   m_OptionType;
                  std::string  m_Value;
                  bool         m_OptionEnabled;
            };
        
    protected:
        std::map<std::string, ConfigTuple>   m_ConfigOptions;   

	private:
		int                              m_VerbosityLevel;
		bool                           	m_bShowUsage;
        int                            m_NameWidth;
        int                            m_ConfigOptionsSize;   // size of the array
        int                            m_NumConfigOptions;
      std::string                      m_ConfigFilePathname;
  }; // class
};

#endif
