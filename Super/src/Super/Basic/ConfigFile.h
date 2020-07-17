// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform. Tested on Windows/Linux/Irix
//////////////////////////////////////////////////////////////////////

#ifndef CconfigFile_H
#define CconfigFile_H

#include "global.h" 





// C++ Includes
#include <string>
#include <vector>

// C Includes
#include <stdlib.h>

//NEW
#include <iostream>
//using namespace std;

#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048


//文件名ConfigFile与mac的xcode编译器内部类同名，加上命名空间
NAME_SPACE_MAIN_BEGIN

class BASIC_API ConfigFile  
{
private:
  bool   caseInsensitive;
  std::string path;
  struct key {
    std::vector<std::string> names;
    std::vector<std::string> values;
    std::vector<std::string> comments;
  };
  std::vector<key>    keys;
  std::vector<std::string> names;
  std::vector<std::string> comments;
  std::string CheckCase( std::string s) const;

public:
  enum errors{ noID = -1};
  //ConfigFile(const std::string& iniPath = "");
  ConfigFile(const std::string iniPath="")
  {
      Path(iniPath);
      caseInsensitive = true;
  }

  virtual ~ConfigFile()                            {}

  // Sets whether or not keynames and valuenames should be case sensitive.
  // The default is case insensitive.
  void CaseSensitive()                           {caseInsensitive = false;}
  void CaseInsensitive()                         {caseInsensitive = true;}

  // Sets path of ini file to read and write from.
  void Path(const std::string& newPath)                {path = newPath;}
  std::string Path() const                            {return path;}
  void SetPath(const std::string& newPath)             {Path( newPath);}

  // Reads ini file specified using path.
  // Returns true if successful, false otherwise.
  bool ReadFile();
  
  // Writes data stored in class to ini file.
  bool WriteFile(); 

  //add by lizulin 20191024
  bool loadFromTextString(const std::string& sConf);
  std::string toTextString() const;   
  
  // Deletes all stored ini data.
  void Erase();
  void Clear()                                   {Erase();}
  void Reset()                                   {Erase();}

  // Returns index of specified key, or noID if not found.
  int FindKey(const std::string& keyname) const;

  // Returns index of specified value, in the specified key, or noID if not found.
  int FindValue(unsigned int keyID,const std::string& valuename) const;

  // Returns number of keys currently in the ini.
  size_t NumKeys() const                       {return names.size();}
  size_t GetNumKeys() const                    {return NumKeys();}

  // Add a key name.
  size_t AddKeyName(const std::string& keyname);

  // Returns key names by index.
  std::string KeyName(unsigned int keyID) const;
  std::string GetKeyName(unsigned int keyID) const {return KeyName(keyID);}

  // Returns number of values stored for specified key.
  size_t NumValues(unsigned int keyID);
  size_t GetNumValues(unsigned int keyID)   {return NumValues( keyID);}
  size_t NumValues(const std::string& keyname);
  size_t GetNumValues(const std::string& keyname)   {return NumValues( keyname);}

  // Returns value name by index for a given keyname or keyID.
  std::string ValueName(unsigned int keyID,unsigned int valueID) const;
  std::string GetValueName(unsigned int keyID,unsigned int valueID) const {
    return ValueName( keyID, valueID);
  }
  std::string ValueName(const std::string&  keyname,unsigned int valueID) const;
  std::string GetValueName(const std::string& keyname,unsigned int valueID) const {
    return ValueName( keyname, valueID);
  }

  // Gets value of [keyname] valuename =.
  // Overloaded to return string, int, and double.
  // Returns defValue if key/value not found.
  std::string GetValue(unsigned int keyID,unsigned int valueID,const std::string& defValue = "") const;
  std::string GetValue(const std::string& keyname,const std::string& valuename,const std::string& defValue = "") const;
  int    GetValueI(const std::string& keyname,const std::string& valuename, int const defValue = 0) const;
  bool   GetValueB(const std::string& keyname,const std::string& valuename, bool const defValue = false) const {
    return bool( GetValueI( keyname, valuename, int( defValue)));
  }
  double   GetValueF(const std::string& keyname,const std::string& valuename, double const defValue = 0.0) const;
  // This is a variable length formatted GetValue routine. All these voids
  // are required because there is no vsscanf() like there is a vsprintf().
  // Only a maximum of 8 variable can be read.
  unsigned GetValueV(const std::string& keyname,const std::string& valuename, char *format,
		      void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
  		      void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
  		      void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
  		      void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);

  // Sets value of [keyname] valuename =.
  // Specify the optional paramter as false (0) if you do not want it to create
  // the key if it doesn't exist. Returns true if data entered, false otherwise.
  // Overloaded to accept string, int, and double.
  bool SetValue(unsigned int keyID,unsigned int valueID,const std::string& value);
  bool SetValue(const std::string& keyname,const std::string& valuename,const std::string& value, bool const create = true);
  bool SetValueI(const std::string& keyname,const std::string& valuename, int const value, bool const create = true);
  bool SetValueB(const std::string& keyname,const std::string& valuename, bool const value, bool const create = true) {
    return SetValueI( keyname, valuename, int(value), create);
  }
  bool SetValueF(const std::string& keyname,const std::string& valuename, double const value, bool const create = true);
  bool SetValueV(const std::string& keyname,const std::string& valuename, char *format, ...);

  // Deletes specified value.
  // Returns true if value existed and deleted, false otherwise.
  bool DeleteValue(const std::string& keyname,const std::string& valuename);
  
  // Deletes specified key and all values contained within.
  // Returns true if key existed and deleted, false otherwise.
  bool DeleteKey(const std::string& keyname);

  // Header comment functions.
  // Header comments are those comments before the first key.
  //
  // Number of header comments.
  size_t NumHeaderComments()                  {return comments.size();}
  // Add a header comment.
  void     HeaderComment(const std::string& comment);
  // Return a header comment.
  std::string   HeaderComment(unsigned int commentID) const;
  // Delete a header comment.
  bool     DeleteHeaderComment( unsigned commentID);
  // Delete all header comments.
  void     DeleteHeaderComments()               {comments.clear();}

  // Key comment functions.
  // Key comments are those comments within a key. Any comments
  // defined within value names will be added to this list. Therefore,
  // these comments will be moved to the top of the key definition when
  // the CIniFile::WriteFile() is called.
  //
  // Number of key comments.
  size_t NumKeyComments(unsigned int keyID) const;
  size_t NumKeyComments(const std::string& keyname) const;
  // Add a key comment.
  bool     KeyComment(unsigned int keyID,const std::string& comment);
  bool     KeyComment(const std::string& keyname,const std::string& comment);
  // Return a key comment.
  std::string   KeyComment(unsigned int keyID,unsigned int commentID) const;
  std::string   KeyComment(const std::string& keyname,unsigned int commentID) const;
  // Delete a key comment.
  bool     DeleteKeyComment(unsigned int keyID,unsigned int commentID);
  bool     DeleteKeyComment(const std::string& keyname,unsigned int commentID);
  // Delete all comments for a key.
  bool     DeleteKeyComments(unsigned int keyID);
  bool     DeleteKeyComments(const std::string& keyname);
};

NAME_SPACE_MAIN_END

#endif


