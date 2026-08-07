#pragma once
#include "../core/kvstore/IIndex.h"
#include <optional>
#include <string>
#include <unordered_map>

class HashMapIndex:public IIndex{
private:
   std::unordered_map<std::string, FileOffset> table;

public:
   void put(const std::string& key,const FileOffset& offset) override{
      table[key] = offset;
   }


   std::optional<FileOffset> get(const std::string& key) const override;

   void remove(const std::string& key) override{
      table.erase(key);
   }

   bool contains(const std::string& key) const override{
      return table.contains(key);
   }
};
