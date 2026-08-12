#pragma once
#include <string_view>

class ILogger{
public:
   virtual void log(std::string_view action, std::string_view key, std::string_view status) = 0;
   virtual ~ILogger() = default;
};

