#pragma once
#include "../core/kvstore/ILogger.h"
#include <fstream>
#include <chrono>
#include <format>

class FileLogger : public ILogger{
private:
   std::ofstream logFile;

   std::string read(uint64_t ts_ms) {
    using namespace std::chrono;
    
    sys_time<milliseconds> tp{milliseconds{ts_ms}};
    
    auto local_tp = current_zone()->to_local(tp);
    
    auto ms = duration_cast<milliseconds>(local_tp.time_since_epoch()) % 1000;

    return std::format("{:%Y-%m-%d %H:%M:%S}.{:03}", local_tp, ms.count());
}

public:
   FileLogger(std::string& path){
      logFile.open(path, std::ios::app);
   }
   ~FileLogger(){
      logFile.close();
   }

   void log(std::string_view action, std::string_view key, std::string_view status) override{
      uint64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      logFile<<read(ts)<<":"<<action<<" "<<key<<" "<<status<<"\n";
   }
};
