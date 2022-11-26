#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include "User.hpp"
#include "IO.hpp"
#include "Cipher.hpp"
#include "Utils.hpp"
#include "NoneCipher.hpp"
#include "AsciiCipher.hpp"
#include "CaesarCipher.hpp"
#include <iostream>
#include <fstream>
#include <vector>

class Config
{
  private:
    std::fstream file_;
    std::string filename_;
    bool modified_;
    std::vector <User*> users_;
  public:
    Config(const std::string& filename)
    {
      file_.open(filename, std::ios::in);
      filename_ = filename;
      modified_ = false;
    }
    ~Config()
    {
      for(auto i : users_)
        delete i;
    }
    void setConfigModified()
    {
      modified_ = true;
    }
    bool isOpen() const
    {
      if(!file_) {
        return false;
      }
      else
        return true;
    }
    bool is_character(char chosen_letter)
    {
      return (chosen_letter >= 'A' && chosen_letter <= 'Z') || (chosen_letter >= 'a' && chosen_letter <= 'z') || (chosen_letter >= '0' && chosen_letter <= '9');
    }
    bool parseFile()
    {
      std::vector<std::string> filelines;
      std::string line;
      
      int j;
      while(getline(file_, line))
      {
        filelines.push_back(line);
      }
      int filelines_size = filelines.size();

      std::vector<std::string> myVector[filelines_size];
      std::vector<std::string> name_vector[filelines_size];
      std::vector<std::string> cipher_vector[filelines_size];
      std::vector<std::string> key_vector[filelines_size];
      //User sm_user[filelines_size];
      User * sm_user[filelines_size];
      size_t pos = 0;
      std::string token;
      for(int i=0; i < filelines_size; i++){
        while ((pos = filelines[i].find(';')) != std::string::npos) {
          token = filelines[i].substr(0, pos);
          myVector[i].push_back(token);
          filelines[i].erase(0, pos + 1);
          }
      }
      j = 0;
      for(int i = 0 ; i < filelines_size - 1; i++)
      {
        j = i;
        for(j++; j < filelines_size; j++)
        {
          if(myVector[i][0].compare(myVector[j][0]) == 0)
          {
            return false;
          }
        }
      }

      for(int i = 0 ; i < filelines_size; i++)
      {
        for( j = 0 ; j < int (myVector[i][0].length()); j++)
          if(!(is_character(myVector[i][0][j])))
          {
            return false;
          }
      }
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 2; j < int (myVector[i].size()); j++)
        {
          pos = myVector[i][j].find(':');
          token = myVector[i][j].substr(0, pos);
          name_vector[i].push_back(token);
          myVector[i][j].erase(0, pos + 1);
        }
      }
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 0; j < int (name_vector[i].size()); j++)
          if(myVector[i][0].compare(name_vector[i][j]) == 0)
          {
            return false;
          }
      }
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 2; j < int (myVector[i].size()); j++)
        {
          pos = myVector[i][j].find(',');
          token = myVector[i][j].substr(0, pos);
          cipher_vector[i].push_back(token);
          myVector[i][j].erase(0, pos);
        }
      }
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 2; j < int (myVector[i].size()); j++)
        {
          pos = myVector[i][j].find(',');
          token = myVector[i][j].substr(pos + 1);
          //std::cout << token << std::endl;
          for(int p = 0; p < int (token.length()); p++)
          {
            //std::cout << token[p] << std::endl;
            if(!(token[p] >= '0' && token[p] <= '9') && !(token[p] >= 'a' && token[p] <= 'f'))
              return false;
          }
          
          key_vector[i].push_back(token);
        }
      }
      
      int y = 0;
      for(int i = 0; i < filelines_size; i++)
      {
        //std::cout << myVector[i][0] << " and " << myVector[i][1] << std::endl;
        sm_user[i] = new User(myVector[i][0], myVector[i][1]);
        //std::cout << sm_user[i]->getName() << " and " << myVector[i][1] << std::endl;
        users_.push_back(sm_user[i]);
      }
      //modified_ = false;
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 0; j < int (name_vector[i].size()); j++)
        {
          if(!containsUser(name_vector[i][j]))
          {
            return false;
          }
        }
      }
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 0; j + 1  < int (name_vector[i].size());j++)
        {
          if(name_vector[i][j].compare(name_vector[i][j+1]) == 0)
          {
            return false;
          }
        }
      }
      
      for(int i = 0; i < filelines_size; i++)
        {
          y = 0;
          
          //std::cout << cipher_vector[i][1] << std::endl;
          for(j = 2; j < int (myVector[i].size()); j++)
          {
            Key keyy;
            Utils::convertHexTo64BitNumber(key_vector[i][y], keyy);
            if(cipher_vector[i][y] == "ASCII")
              sm_user[i]->addContact(getUser(name_vector[i][y]), CipherType::ASCII_CIPHER, keyy);
            else if(cipher_vector[i][y] == "NONE")
              sm_user[i]->addContact(getUser(name_vector[i][y]), CipherType::NONE_CIPHER, keyy);
            else if(cipher_vector[i][y] == "CAESAR")
              sm_user[i]->addContact(getUser(name_vector[i][y]), CipherType::CAESAR_CIPHER, keyy);
            else 
              return false;
            y++;
          }
        
        }
      for(int i = 0; i < filelines_size; i++)
      {
        for(j = 0; j + 1  < int (name_vector[i].size());j++)
        {
          if(!(getUser(myVector[i][0])->hasContact(name_vector[i][j]) && getUser(name_vector[i][j])->hasContact(myVector[i][0])))
          {
            return false;
          }
        }
      }
      file_.close();
      return true;
    }
    bool containsUser(const std::string& name) const
    {
      for(int i = 0; i < int (users_.size()); i++)
      {
        if(users_[i]->getName() == name)
        {
          return true;
        }
      }
      return false;
    }
    User* getUser(const std::string& name) const
    {
      for(int i = 0; i < int (users_.size()); i++)
      {
        if(users_[i]->getName() == name)
        {
          return users_[i];
        }
      }
      return nullptr;
    }
    User* registerUser(const std::string& name, const std::string& password)
    {
      User * a = new User(name, password);
      //User * b = &a;
      users_.push_back(a);
      setConfigModified();
      return a;
    }
    User* loginUser(const std::string& name, const std::string& password) const
    {
      for(int i = 0; i < int (users_.size()); i++)
      {
        if(users_[i]->getName() == name)
        {
          if(users_[i]->verifyPassword(password))
          {
            return users_[i];
          }
          else
            return nullptr;
        }
      }
      return nullptr;
    }
    bool updateConfigFile()
    {
      if(modified_ == false)
        return true;
      file_.open(filename_,std::ios::trunc | std::ios::out);
      int len = users_.size();
      std::string poopoo;
      if(!isOpen())
        return false;
      for(int i = 0; i < len; i++)
      {
        if(i > 0)
            file_ << "\n";
        //std::cout << users_[i]->getName() << ";" << users_[i]->getPassword() << std::endl;
        file_ << users_[i]->getName() << ";" << users_[i]->getPassword() << ";";
        for(auto const& [contact, cipher] : users_[i]->getContacts())
        {
          poopoo = cipher->decToHexa(cipher->getkey());
          reverse(poopoo.begin(), poopoo.end());
          file_ << contact->getName() << ":" << cipher->getCipherTypeString() << "," << poopoo << ";" ;
          //std::cout << contact->getName() << ":" << cipher->getCipherTypeString() << "," << cipher->getkey() << ";" << std::endl;
        }
        if(i == len -1)
            file_ << "\n";
      }
      file_.close();
      return true;
    }
};