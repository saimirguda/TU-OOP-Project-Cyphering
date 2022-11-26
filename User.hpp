#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
//#include "IO.hpp"
//#include "Cipher.hpp"
#include "Utils.hpp"
#include "NoneCipher.hpp"
#include "AsciiCipher.hpp"
#include "CaesarCipher.hpp"
#include <fstream>
#include <iomanip>
//#include "Config.hpp"
class User
{
  private:
    std::string name_;
    std::string password_;
    std::map<User*, Cipher*, bool(*)(User*, User*)> contacts_;
  public:
    User(const std::string& name, const std::string& password)
    :contacts_{Utils::sortContactsAlphabetically}
    {
      name_ = name;
      password_ = password;
      //std::map<User*, Cipher*, sortContactsAlphabetically> contacts_= {};
      //contacts_=  new std::map < User*, Cipher*, bool(*)(User*, User*)>  { Utils::sortContactsAlphabetically};
    }
    ~User()
    {
      for(auto const& [contact, cipher] : contacts_)
      {
        delete cipher;
      }
    }
    std::string getName() const
    {
      return name_;
    }
    std::string getPassword() const
    {
      return password_;
    }
    const auto& getContacts() const
    {
      return contacts_;
    }
    bool verifyPassword(const std::string& password) const
    {
      if(password_ == password)
        return true;
      else
        return false;
    }
    bool hasContact(const std::string& name) const
    {
      for (auto const& [key, val] : contacts_)
      {
        if(key->name_ == name)
          return true;
      }
      return false;
    }
    void addContact(User* contact, CipherType type, Key key);
    bool sendMessage(const std::string& recipient, const std::string& filename, const std::string& plain_text) const;
    bool readMessage(const std::string& filename) const;
};