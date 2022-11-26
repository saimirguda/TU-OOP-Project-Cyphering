#include "User.hpp"
#include "IO.hpp"
#include "Config.hpp"

void User::addContact(User* contact, CipherType type, Key key)
    {
      //Cipher c1 = Cipher(key);
      if(type == CipherType::NONE_CIPHER)
      {
        NoneCipher* c1 = new NoneCipher(key);
        contacts_.insert({contact, c1 });
      }
      else if(type == CipherType::ASCII_CIPHER)
      {
        AsciiCipher* c1 = new AsciiCipher(key);
        contacts_.insert({ contact, c1 });
      }
      else if(type == CipherType::CAESAR_CIPHER)
      {
        CaesarCipher* c1 = new CaesarCipher(key);
        contacts_.insert({ contact, c1 });
      }
    }
bool User::sendMessage(const std::string& recipient, const std::string& filename, const std::string& plain_text) const
    {
      std::string changed_text = plain_text;
      std::fstream input_stream_1;
      
      input_stream_1.open(filename, std::ios::in | std::fstream::out | std::fstream::app);
      if(input_stream_1.fail() == true) {
        return false;
      }
      if(!hasContact(recipient))
      {
        return false;
      }
      //Cipher:encrypt(plain_text);
      for (auto const& key : contacts_)
        {
          if(key.first->name_ == recipient)
          {
            changed_text = key.second->encrypt(changed_text);
          }
        }
        input_stream_1 << "Recipient: " << recipient << '\n';
        input_stream_1 << "Sender: "<< name_ << '\n';
        input_stream_1 << changed_text << '\n';
        std::string r = "Recipient: ";
        std::string s = "Sender: ";
        r+= recipient;
        r+="\n";
        s+=name_;
        s+="\n";
        r+=s;
        r+=changed_text;
        r+="\n";
        IO::printEncryptedMessage(r);
        return true;
    }
    bool User::readMessage(const std::string& filename) const
    {
      std::vector<std::string> filelines;
      std::fstream input_stream;
      std::string line;
      std::string recipent;
      std::string sender;
      std::string cipher_text;
      //auto npos = line.find(":");
      size_t pos = 0;
      //std::vector<std::string> v;
      input_stream.open(filename, std::ios::in);
      if(input_stream.fail() == true) {
        return false;
      }
      while(getline(input_stream, line))
      {
        filelines.push_back(line);
      }
      while ((pos = filelines[0].find(' ')) != std::string::npos) {
        recipent = filelines[0].substr(pos + 1);
        filelines[0].erase(0, pos + 1);
        }
      while ((pos = filelines[1].find(' ')) != std::string::npos) {
        sender = filelines[1].substr(pos + 1);
        filelines[1].erase(0, pos + 1);
        }
      cipher_text = filelines[2];
      
      if(recipent.compare(name_) != 0)
      {
        return false;
      }
      if(!hasContact(sender))
      {
        return false;
      }
      for(auto const& [key, val] : contacts_)
      {
        if(key->name_ == sender)
        {
          //std::cout << key->name_<< " " << val->getkey() << std::endl;
          cipher_text = val->decrypt(cipher_text);
        }
      }
      //std::cout << recipent << std::endl;
      IO::printDecryptedMessage(recipent, sender, cipher_text);
      return true;
      }