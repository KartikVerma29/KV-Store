#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>

class Record{
private:
   std::string key;
   std::string value;
   bool tombstone;
   uint64_t timestamp;

   Record(std::string k, std::string v, bool tomb, uint64_t ts)
      :key(std::move(k)), value(std::move(v)), tombstone(tomb), timestamp(ts)
   {}

public:

   Record(const std::string& key, const std::string& value):key(key), value(value), tombstone(false){
      timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
   }

   const std::string& getKey() const{
      return key;
   }

   const std::string& getValue() const { return value;}
   
   bool isTomb() const{
      return tombstone;
   }

   std::vector<uint8_t> serialize() const{
      std::vector<uint8_t> buffer(size());
      size_t offset=0;
      auto write = [&](const void* data, size_t len){
         std::memcpy(buffer.data()+offset, data, len);
         offset+=len;
      };

      size_t kLen = static_cast<uint32_t>(key.length());
      write(&kLen, sizeof(kLen));
      write(key.data(), kLen);

      size_t vLen = static_cast<uint32_t>(value.length());
      write(&vLen, sizeof(vLen));
      write(value.data(), vLen);

      write(& tombstone, sizeof(tombstone));
      write(& timestamp, sizeof(timestamp));
      return buffer;
   }

   Record static deserialize(const std::vector<uint8_t>& buffer){
      size_t offset = 0;

      auto read = [&](void* dest, size_t len){
         std::memcpy(dest, buffer.data()+offset,len);
         offset+=len;
      };

      uint32_t kLen;
      read(&kLen, sizeof(kLen));
      std::string k(reinterpret_cast<const char*>(buffer.data()+offset), kLen);
      offset+=kLen;

      uint32_t vLen;
      read(&vLen, sizeof(vLen));
      std::string v(reinterpret_cast<const char*>(buffer.data()+offset), vLen);
      offset+=vLen;

      bool tomb; read(&tomb, sizeof(tomb));

      uint64_t ts; read(&ts, sizeof(ts));
      
      return Record(std::move(k), std::move(v), tomb, ts);
   }

   static Record createTomb(const std::string& key){
      uint64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      return Record(key, "", true,ts);
   }

   size_t size() const{
      return sizeof(bool) + sizeof(uint64_t) + (sizeof(size_t)*2) + key.length() + value.length();
   };

};
