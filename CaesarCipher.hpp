#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include "Cipher.hpp"

class CaesarCipher : public Cipher
{
  public:
    CaesarCipher(Key key): Cipher(key) {
      key_ = key;
    }

    std::string getCipherTypeString() const override
    {
      auto s = "CAESAR";
      return s;
    }
    std::string encrypt(const std::string& plain_text) override
    {
      std::string cipher_text = preparePlainText(plain_text);
      std::vector<std::string> bite_vector;
      std::vector<int> filelines;
      std::string str;
      Key key_stuff = key_;
      std::string p;
      int j =0;
      
      std::string hexa_key = decToHexa(key_stuff);
      hexa_key = preparePlainText(hexa_key);
      reverse(hexa_key.begin(), hexa_key.end());
      for(int i = 0; i < 16; i++)
      {
        p = hexa_key[i];
        p += hexa_key[i+1];
        bite_vector.push_back(p);
        i++;
      }

      int len = bite_vector.size();
      for (int i = 0 ; i < len ; i++)
      {
        int base = 1;
        int temp = 0;
        for(int j = 1; j >= 0; j--)
        {
          if (bite_vector[i][j] >='0' && bite_vector[i][j] <='9')
          {
            temp += (bite_vector[i][j] - 48)*base;
            base = base * 16;
          }
          else if (bite_vector[i][j]>='A' && bite_vector[i][j]<='F')
          {
            temp += (bite_vector[i][j] - 55)*base;
            base = base*16;
          }
        }
        filelines.push_back(temp);
      }
      j = 0;
      for(int i = 0; i < int (cipher_text.length()); i++)
      {
        if(j == len)
          j = 0;
        cipher_text[i] = cipher_text[i] + filelines[j] % 26;
        if(!(cipher_text[i] >= 'A' && cipher_text[i] <= 'Z'))
          cipher_text[i] = cipher_text[i] - 26;
        j++;
      }
      return cipher_text;
    }
    std::string decrypt(const std::string& cipher_text) override
    {
      std::string decipher_text = preparePlainText(cipher_text);
      std::vector<std::string> bite_vector;
      std::vector<int> filelines;
      std::string str;
      Key key_stuff = key_;
      std::string p;
      int j =0;
      
      std::string hexa_key = decToHexa(key_stuff);
      hexa_key = preparePlainText(hexa_key);
      reverse(hexa_key.begin(), hexa_key.end());
      
      for(int i = 0; i < 16; i++)
      {
        p = hexa_key[i];
        p += hexa_key[i+1];
        bite_vector.push_back(p);
        i++;
      }

      int len = bite_vector.size();
      for (int i = 0 ; i < len ; i++)
      {
        int base = 1;
        int temp = 0;
        for(int j = 1; j >= 0; j--)
        {
          if (bite_vector[i][j] >='0' && bite_vector[i][j] <='9')
          {
            temp += (bite_vector[i][j] - 48)*base;
            base = base * 16;
          }
          else if (bite_vector[i][j]>='A' && bite_vector[i][j]<='F')
          {
            temp += (bite_vector[i][j] - 55)*base;
            base = base*16;
          }
        }
        filelines.push_back(temp);
      }
      j = 0;
      for(int i = 0; i < int (decipher_text.length()); i++)
      {
        if(j == len)
          j = 0;
        decipher_text[i] = decipher_text[i] - filelines[j] % 26;
        if(!(decipher_text[i] >= 'A' && decipher_text[i] <= 'Z'))
          decipher_text[i] = decipher_text[i] + 26;
        j++;
      }
      
      return decipher_text;
}
};

