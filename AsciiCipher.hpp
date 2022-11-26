#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include "Cipher.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
class AsciiCipher : public Cipher
{
  public:
    AsciiCipher(Key key): Cipher(key) {
      key_ = key;
    }

    std::string getCipherTypeString() const override
    {
      auto s = "ASCII";
      return s;
    }
    std::string encrypt(const std::string& plain_text) override
    {
      std::string cipher_text = preparePlainText(plain_text);
      std::vector<int> filelines;
      std::stringstream ss;
      std::string str;
      Key key_stuff = key_;
      ss >> str;
      int temp;
      temp = key_stuff % 10;
      for (int i = 0; i < int (cipher_text.length()); i++)
      {
        cipher_text[i] = cipher_text[i] - temp;
        filelines.push_back(cipher_text[i]);
      }
      std::stringstream result;
      
      std::copy(filelines.begin(), filelines.end(), std::ostream_iterator<int>(result, " "));
      std::string smth = result.str();
      smth = smth.substr(0, smth.length()-1);
      return smth;
    }
    std::string decrypt(const std::string& cipher_text) override
    {
      //std::cout << "Yo" << std::endl;
      std::string changed_text = cipher_text;
      std::vector<std::string> filelines;
      
      std::stringstream ss;
      Key key_stuf = key_;
    
      std::string str;

      int temp = 0;
      size_t pos;
      size_t end = 0;
      //std::cout << "Yo" << std::endl;
      while ((pos = changed_text.find_first_not_of(' ', end)) != std::string::npos) {
        end = changed_text.find(" ", pos);
        str = changed_text.substr(pos, end - pos);
        filelines.push_back(str);
        //std::cout << str << std::endl;
        //changed_text.erase(0, pos);
        //break;
        }

      //std::cout << "Yo" << std::endl;
      //std::cout << str << std::endl;
      temp = key_stuf % 10;
      for (int i = 0; i < int (filelines.size()); i++)
      {
        filelines[i] = stoi(filelines[i]) + temp;
      }
      //std::string final_final_text(filelines.begin(), filelines.end());
      
      std::string final_final_text;
      for (const auto &piece : filelines) final_final_text += piece;
      return final_final_text;
    }
};