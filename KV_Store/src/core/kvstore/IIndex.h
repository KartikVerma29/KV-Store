#pragma once
#include "FileOffset.h"
#include <string>
#include <optional>
#include <cstring>

class IIndex{
public:
   virtual void put(const std::string& key,const FileOffset& offset) = 0;
   virtual std::optional<FileOffset> get(const std::string& key) const= 0;
   virtual void remove(const std::string& key) = 0;
   virtual bool contains(const std::string& key) const= 0;

   virtual ~IIndex() = default;
};


