// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@houston.rr.com
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform.
//////////////////////////////////////////////////////////////////////

// C++ Includes
#include <iostream>
#include <fstream>
#include <strstream>

//using namespace std;

// C Includes
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

// Local Includes
#include "ConfigFile.h"

#if defined(_WIN32)
#define iniEOL std::endl
#else
#define iniEOL '\r' <<std::endl
#endif


NAME_SPACE_MAIN_BEGIN

    //ConfigFile::ConfigFile(const std::string& iniPath)
    //ConfigFile::ConfigFile(const std::string iniPath)
    //{
    //    Path( iniPath);
    //    caseInsensitive = true;
    //}

    bool ConfigFile::ReadFile()
    {
        // Normally you would use ifstream, but the SGI CC compiler has
        // a few bugs with ifstream. So ... fstream used.
        std::fstream fs;
        std::string   line;
        std::string   keyname, valuename, value;
        std::string::size_type pLeft, pRight;

        fs.open(path.c_str(),std::ios::in);
        if (fs.fail())
        {
            return false;
        }

        while(getline(fs,line))
        {
            // To be compatible with Win32, check for existence of '\r'.
            // Win32 files have the '\r' and Unix files don't at the end of a line.
            // Note that the '\r' will be written to INI files from
            // Unix so that the created INI file can be read under Win32
            // without change.
            if (line[line.length() - 1] == '\r')
            {
                line = line.substr(0,line.length() - 1);
            }

            if (line.length())
            {
                // Check that the user hasn't openned a binary file by checking the first
                // character of each line!
                if ( !isprint( line[0]))
                {
                    printf( "Failing on char %d\n", line[0]);
                    fs.close();
                    return false;
                }
                if ((pLeft = line.find_first_of(";#[=")) != std::string::npos)
                {
                    switch ( line[pLeft])
                    {
                    case '[':
                        if ((pRight = line.find_last_of("]")) != std::string::npos &&pRight > pLeft)
                        {
                                keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
                                AddKeyName( keyname);
                        }
                        break;

                    case '=':
                        valuename = line.substr( 0, pLeft);
                        value = line.substr( pLeft + 1);
                        SetValue( keyname, valuename, value);
                        break;

                    //注释前缀 header of comments
                    case ';':
                    case '#':
                        if ( !names.size())
                            HeaderComment( line.substr( pLeft + 1));
                        else
                            KeyComment( keyname, line.substr( pLeft + 1));
                        break;
                    }
                }
            }
        }

        fs.close();
        if (names.size())
        {
            return true;
        }
        return false;
    }

    bool ConfigFile::WriteFile()
    {
        unsigned commentID, keyID, valueID;
        // Normally you would use ofstream, but the SGI CC compiler has
        // a few bugs with ofstream. So ... fstream used.
        std::fstream fs;

        fs.open( path.c_str(), std::ios::out);
        if (fs.fail())
        {
            return false;
        }

        // Write header comments.
        for ( commentID = 0; commentID < comments.size(); ++commentID)
        {
            fs << ';' << comments[commentID] << iniEOL;
        }

        if ( comments.size())
            fs << iniEOL;

        // Write keys and values.
        for ( keyID = 0; keyID < keys.size(); ++keyID)
        {
            fs << '[' << names[keyID] << ']' << iniEOL;
            // Comments.
            for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
            {
                fs << ';' << keys[keyID].comments[commentID] << iniEOL;
            }
            // Values.
            for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
            {
                fs << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << iniEOL;
            }
            fs << iniEOL;
        }

        fs.flush();  //确保实际写入

        fs.close();

        return true;
    }

    #include <sstream>

    bool ConfigFile::loadFromTextString(const std::string& sConf)
    {
        // Normally you would use ifstream, but the SGI CC compiler has
        // a few bugs with ifstream. So ... fstream used.


        std::stringstream fs;
        fs<<sConf;

        //std::fstream fs;
        std::string   line;
        std::string   keyname, valuename, value;
        std::string::size_type pLeft, pRight;

        //fs.open(path.c_str(),std::ios::in);
        //if (fs.fail())
        //{
        //    return false;
        //}

        while(getline(fs,line))
        {
            // To be compatible with Win32, check for existence of '\r'.
            // Win32 files have the '\r' and Unix files don't at the end of a line.
            // Note that the '\r' will be written to INI files from
            // Unix so that the created INI file can be read under Win32
            // without change.
            if (line[line.length() - 1] == '\r')
            {
                line = line.substr(0,line.length() - 1);
            }

            if (line.length())
            {
                // Check that the user hasn't openned a binary file by checking the first
                // character of each line!
                if (!isprint( line[0]))
                {
                    printf( "Failing on char %d\n", line[0]);

                    //fs.close();

                    return false;
                }
                if ((pLeft = line.find_first_of(";#[=")) != std::string::npos)
                {
                    switch ( line[pLeft])
                    {
                    case '[':
                        if ((pRight = line.find_last_of("]")) != std::string::npos &&pRight > pLeft)
                        {
                            keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
                            AddKeyName( keyname);
                        }
                        break;

                    case '=':
                        valuename = line.substr( 0, pLeft);
                        value = line.substr( pLeft + 1);
                        SetValue( keyname, valuename, value);
                        break;

                        //注释前缀 header of comments
                    case ';':
                    case '#':
                        if ( !names.size())
                        {
                            HeaderComment(line.substr(pLeft+1));
                        }
                        else
                        {
                            KeyComment(keyname,line.substr(pLeft+1));
                        }
                        break;
                    }
                }
            }
        }

        //fs.close();
        if (names.size())
        {
            return true;
        }
        return false;
    }

    //直接转成文本字符串,不保存到文件
    std::string ConfigFile::toTextString() const
    {
        unsigned int commentID, keyID, valueID;
        std::string sRes;
        sRes.reserve(1024);


#ifdef _WIN32   //换行符
        const char* wrapchar="\n";
#else
        const char* wrapchar="\r\n";
#endif

        // Write header comments.
        for ( commentID = 0; commentID < comments.size(); ++commentID)
        {
            //fs << ';' << comments[commentID] << iniEOL;
            sRes+=";";
            sRes+=comments[commentID];
            sRes+=wrapchar;
        }

        if (comments.size())
        {
            //fs << iniEOL;
            sRes+=wrapchar;
        }

        // Write keys and values.
        for ( keyID = 0; keyID < keys.size(); ++keyID)
        {
            //fs << '[' << names[keyID] << ']' << iniEOL;
            sRes+="[";
            sRes+= names[keyID];
            sRes+="]";
            sRes+=wrapchar;

            // Comments.
            for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
            {
                //fs << ';' << keys[keyID].comments[commentID] << iniEOL;
                sRes+=";";
                sRes+=keys[keyID].comments[commentID];
                sRes+=wrapchar;
            }
            // Values.
            for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
            {
                //fs << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << iniEOL;

                sRes+=keys[keyID].names[valueID];
                sRes+="=";
                sRes+=keys[keyID].values[valueID];
                sRes+=wrapchar;

            }
            //fs << iniEOL;
            sRes+=wrapchar;
        }
        //fs.close();

        return sRes;
    }

    int ConfigFile::FindKey(const std::string& keyname) const
    {
        for ( unsigned int keyID = 0; keyID < names.size(); ++keyID)
        {
            if (CheckCase( names[keyID])==CheckCase(keyname))
                return int(keyID);
        }
        return noID;
    }

    int ConfigFile::FindValue(unsigned int keyID,const std::string& valuename) const
    {
        if (!keys.size() || keyID >= keys.size())
            return noID;

        for (unsigned int valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
        {
            if ( CheckCase( keys[keyID].names[valueID]) == CheckCase( valuename))
            {
                return int(valueID);
            }
        }
        return noID;
    }

    size_t ConfigFile::AddKeyName(const std::string& keyname)
    {
        names.resize( names.size() + 1, keyname);
        keys.resize( keys.size() + 1);
        return names.size() - 1;
    }

    std::string ConfigFile::KeyName(unsigned int keyID) const
    {
        if ( keyID < names.size())
        {
            return names[keyID];
        }
        else
        {
            return "";
        }
    }

    size_t ConfigFile::NumValues(unsigned int keyID)
    {
        if ( keyID < keys.size())
        {
            return keys[keyID].names.size();
        }
        return 0;
    }

    size_t ConfigFile::NumValues(const std::string& keyname)
    {
        int keyID = FindKey( keyname);
        if (keyID == noID)
            return 0;
        return keys[keyID].names.size();
    }

    std::string ConfigFile::ValueName(unsigned int keyID,unsigned int valueID) const
    {
        if ( keyID < keys.size() && valueID < keys[keyID].names.size())
        {
            return keys[keyID].names[valueID];
        }
        return "";
    }

    std::string ConfigFile::ValueName(const std::string& keyname,unsigned int valueID) const
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return "";
        return ValueName( keyID, valueID);
    }

    bool ConfigFile::SetValue(unsigned int keyID,unsigned int valueID,const std::string& value)
    {
        if (keyID < keys.size() && valueID < keys[keyID].names.size())
        {
            keys[keyID].values[valueID] = value;

            return true;  //add by lizulin
        }
        return false;
    }

    bool ConfigFile::SetValue(const std::string& keyname,const std::string& valuename,const std::string& value, bool const create)
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
        {
            if(create)
                keyID = int(AddKeyName(keyname));
            else
                return false;
        }

        int valueID = FindValue( unsigned(keyID), valuename);
        if ( valueID == noID)
        {
            if (!create)
            {
                return false;
            }
            keys[keyID].names.resize( keys[keyID].names.size()+1,valuename);
            keys[keyID].values.resize( keys[keyID].values.size()+1,value);
        }
        else
        {
            keys[keyID].values[valueID] = value;
        }
        return true;
    }

    bool ConfigFile::SetValueI(const std::string& keyname,const std::string& valuename, int const value, bool const create)
    {
        char svalue[MAX_VALUEDATA];

        sprintf( svalue, "%d", value);
        return SetValue( keyname, valuename, svalue);
    }

    bool ConfigFile::SetValueF(const std::string& keyname,const std::string& valuename, double const value, bool const create)
    {
        char svalue[MAX_VALUEDATA];

        sprintf( svalue, "%f", value);
        return SetValue( keyname, valuename, svalue);
    }

    bool ConfigFile::SetValueV(const std::string& keyname,const std::string& valuename, char *format, ...)
    {
        va_list args;
        char value[MAX_VALUEDATA];
        va_start( args, format);
        vsprintf(value, format,args);
        va_end(args);
        return SetValue( keyname, valuename, value);
    }

    std::string ConfigFile::GetValue(unsigned int keyID,unsigned int valueID,const std::string& defValue) const
    {
        if ( keyID < keys.size() && valueID < keys[keyID].names.size())
        {
            return keys[keyID].values[valueID];
        }
        return defValue;
    }

    std::string ConfigFile::GetValue(const std::string& keyname,const std::string& valuename,const std::string& defValue) const
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return defValue;

        long valueID = FindValue( unsigned(keyID), valuename);
        if ( valueID == noID)
            return defValue;

        return keys[keyID].values[valueID];
    }

    int ConfigFile::GetValueI(const std::string& keyname,const std::string& valuename, int const defValue) const
    {
        char svalue[MAX_VALUEDATA];

        sprintf( svalue, "%d", defValue);
        return atoi( GetValue( keyname, valuename, svalue).c_str()); 
    }

    double ConfigFile::GetValueF(const std::string& keyname,const std::string& valuename, double const defValue) const
    {
        char svalue[MAX_VALUEDATA];

        sprintf( svalue, "%f", defValue);
        return atof( GetValue( keyname, valuename, svalue).c_str()); 
    }

    // 16 variables may be a bit of over kill, but hey, it's only code.
    unsigned ConfigFile::GetValueV(const std::string& keyname,const std::string& valuename, char *format,
        void *v1, void *v2, void *v3, void *v4,
        void *v5, void *v6, void *v7, void *v8,
        void *v9, void *v10, void *v11, void *v12,
        void *v13, void *v14, void *v15, void *v16)
    {
        std::string   value;
        // va_list  args;
        unsigned int nVals;

        value = GetValue( keyname, valuename);
        if ( !value.length())
            return false;
        // Why is there not vsscanf() function. Linux man pages say that there is
        // but no compiler I've seen has it defined. Bummer!
        //
        // va_start( args, format);
        // nVals = vsscanf( value.c_str(), format, args);
        // va_end( args);

        nVals = sscanf( value.c_str(), format,
            v1, v2, v3, v4, v5, v6, v7, v8,
            v9, v10, v11, v12, v13, v14, v15, v16);

        return nVals;
    }

    bool ConfigFile::DeleteValue(const std::string& keyname,const std::string& valuename)
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return false;

        int valueID = FindValue(unsigned(keyID), valuename);
        if ( valueID == noID)
            return false;

        // This looks strange, but is neccessary.
        std::vector<std::string>::iterator npos = keys[keyID].names.begin() + valueID;
        std::vector<std::string>::iterator vpos = keys[keyID].values.begin() + valueID;
        keys[keyID].names.erase( npos, npos + 1);
        keys[keyID].values.erase( vpos, vpos + 1);
        return true;
    }

    bool ConfigFile::DeleteKey(const std::string& keyname)
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return false;

        // Now hopefully this destroys the vector lists within keys.
        // Looking at <vector> source, this should be the case using the destructor.
        // If not, I may have to do it explicitly. Memory leak check should tell.
        // memleak_test.cpp shows that the following not required.
        //keys[keyID].names.clear();
        //keys[keyID].values.clear();

        std::vector<std::string>::iterator npos = names.begin() + keyID;
        std::vector<key>::iterator    kpos = keys.begin() + keyID;
        names.erase( npos, npos + 1);
        keys.erase( kpos, kpos + 1);

        return true;
    }

    void ConfigFile::Erase()
    {
        // This loop not needed. The vector<> destructor seems to do
        // all the work itself. memleak_test.cpp shows this.
        //for ( unsigned i = 0; i < keys.size(); ++i) {
        //  keys[i].names.clear();
        //  keys[i].values.clear();
        //}

        names.clear();
        keys.clear();
        comments.clear();
    }

    void ConfigFile::HeaderComment(const std::string& comment)
    {
        comments.resize( comments.size() + 1, comment);
    }

    std::string ConfigFile::HeaderComment(unsigned int commentID) const
    {
        if ( commentID < comments.size())
        {
            return comments[commentID];
        }
        return "";
    }

    bool ConfigFile::DeleteHeaderComment( unsigned commentID)
    {
        if ( commentID < comments.size())
        {
            std::vector<std::string>::iterator cpos = comments.begin() + commentID;
            comments.erase( cpos, cpos + 1);
            return true;
        }
        return false;
    }

    size_t ConfigFile::NumKeyComments(unsigned int keyID) const
    {
        if ( keyID < keys.size())
        {
            return keys[keyID].comments.size();
        }
        return 0;
    }

    size_t ConfigFile::NumKeyComments(const std::string& keyname) const
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return 0;
        return keys[keyID].comments.size();
    }

    bool ConfigFile::KeyComment(unsigned int keyID,const std::string& comment)
    {
        if ( keyID < keys.size())
        {
            keys[keyID].comments.resize(keys[keyID].comments.size()+1,comment);
            return true;
        }
        return false;
    }

    bool ConfigFile::KeyComment(const std::string& keyname,const std::string& comment)
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return false;
        return KeyComment( unsigned(keyID), comment);
    }

    std::string ConfigFile::KeyComment(unsigned int keyID,unsigned int commentID) const
    {
        if (keyID<keys.size()&&commentID<keys[keyID].comments.size())
        {
            return keys[keyID].comments[commentID];
        }
        return "";
    }

    std::string ConfigFile::KeyComment(const std::string& keyname,unsigned int commentID) const
    {
        long keyID = FindKey( keyname);
        if ( keyID == noID)
            return "";
        return KeyComment( unsigned(keyID), commentID);
    }

    bool ConfigFile::DeleteKeyComment(unsigned int keyID,unsigned int commentID)
    {
        if ( keyID < keys.size() && commentID < keys[keyID].comments.size())
        {
            std::vector<std::string>::iterator cpos = keys[keyID].comments.begin() + commentID;
            keys[keyID].comments.erase( cpos, cpos + 1);
            return true;
        }
        return false;
    }

    bool ConfigFile::DeleteKeyComment(const std::string& keyname,unsigned int commentID)
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return false;
        return DeleteKeyComment( unsigned(keyID), commentID);
    }

    bool ConfigFile::DeleteKeyComments(unsigned int keyID)
    {
        if ( keyID < keys.size())
        {
            keys[keyID].comments.clear();
            return true;
        }
        return false;
    }

    bool ConfigFile::DeleteKeyComments(const std::string& keyname)
    {
        int keyID = FindKey( keyname);
        if ( keyID == noID)
            return false;
        return DeleteKeyComments( unsigned(keyID));
    }

    std::string ConfigFile::CheckCase(std::string s) const
    {
        if ( caseInsensitive)
        {
            for (std::string::size_type i = 0; i < s.length(); ++i)
            {
                s[i] = tolower(s[i]);
            }
        }
        return s;
    }

NAME_SPACE_MAIN_END
