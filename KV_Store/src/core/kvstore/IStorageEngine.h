#pragma once

#include <optional>
#include <string>
class IStorageEngine{
public:
   virtual void put(const std::string& key, const std::string& value) = 0;
   virtual std::optional<std::string> get(const std::string& key) = 0;
   virtual void remove(const std::string& key) = 0;
   virtual void flush() = 0;
   virtual ~IStorageEngine() = default;
};
