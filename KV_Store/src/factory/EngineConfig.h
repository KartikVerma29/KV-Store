#pragma once
#include <string>

enum class LoggerType{CONSOLE,FILE};

struct EngineConfig{
   std::string walPath;
   bool enableLogging;
   std::string logPath;
   LoggerType logtype;
};
