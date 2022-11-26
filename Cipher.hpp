#pragma once
#ifndef Cipher_hpp
#define Cipher_hpp
#include <iostream>
#include <cstring>
#include <string.h>
#include <algorithm>

enum class CipherType { INVALID, NONE_CIPHER, ASCII_CIPHER, CAESAR_CIPHER };

typedef uint64_t Key;

class Cipher
{
  protected:
    Key key_;
  public:
    Cipher(Key key)
    {
      key_ = key;
    }
    Key getkey()
    {
      return key_;
    }
    std::string decToHexa(uint64_t n){
    std::string hexaDeciNum = "xxxxxxxxxxxxxxxx";
 
    int i = 0;
    while (n != 0) {
       
        int temp = 0;
 
        
        temp = n % 16;
 

        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
 
        n = n / 16;
    }
    for (int j = int(hexaDeciNum.length()) - 1; j >= 0; j--)
        {if(hexaDeciNum[j] == 'x')
        {
          hexaDeciNum[j] = '0';
        }}
    for(int peepee = 0; peepee < 16; peepee++)
    {
      if(hexaDeciNum[peepee] >= 'A' && hexaDeciNum[peepee] <= 'Z')
        hexaDeciNum[peepee] = hexaDeciNum[peepee] - ('Z' - 'z');
    }
      return hexaDeciNum;
}
    virtual std::string getCipherTypeString() const = 0;
    std::string preparePlainText(const std::string& plain_text) const
    {
      std::string changed_text = plain_text;
      changed_text.erase(std::remove(changed_text.begin(), changed_text.end(), ' '), changed_text.end());
      for (int x = 0; x < int (changed_text.length()); x++)
      {
        changed_text[x] = toupper(changed_text[x]);
      }
      return changed_text;
    }
    virtual std::string encrypt(const std::string& plain_text) = 0;
    virtual std::string decrypt(const std::string& cipher_text) = 0;
    static CipherType getCipherType(const std::string& type)
    {
      if(type == "NONE")
        return CipherType::NONE_CIPHER;
      else if(type == "ASCII")
        return CipherType::ASCII_CIPHER;
      else if(type == "CAESAR")
        return CipherType::CAESAR_CIPHER;
      else
        return CipherType::INVALID;
    }
};
#endif
