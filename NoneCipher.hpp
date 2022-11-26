#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include "Cipher.hpp"

class NoneCipher : public Cipher
{
  public:
    NoneCipher(Key key): Cipher(key) {
      key_ = key;
    }

    std::string getCipherTypeString() const override
    {
      auto s = "NONE";
      return s;
    }
    std::string encrypt(const std::string& plain_text) override
    {
      std::string changed_text = preparePlainText(plain_text);
      return changed_text;
    }
    std::string decrypt(const std::string& cipher_text) override
    {
      return cipher_text;
    }
};