#pragma once
#include "../core/kvstore/ILogger.h"
#include <iostream>

class ConsoleLogger : public ILogger{
public:
   void log(std::string_view action, std::string_view key, std::string_view status) override{
      std::cout<<"Action : "<<action<<" Key : "<<key<<" Status : "<<status<<std::endl;
   }
};
