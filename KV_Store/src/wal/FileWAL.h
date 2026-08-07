#pragma once
#include "../core/kvstore/IWriteAheadLog.h"
#include <cstdio>
#include <mutex>
#include <string>

class FileWAL: public IWriteAheadLog{
private:
   FILE* fileHandle;
   std::string path;
   std::mutex writeMutex;

public:
   FileWAL(std::string p): path(std::move(p)){
      fileHandle = std::fopen(path.c_str(), "ab+");
      if(!fileHandle){
         throw std::runtime_error("Failed to open WAL file: "+path);
      }
   }
   FileOffset append(const LogEntry& log) override;
   std::vector<LogEntry> replay() override;
   void sync() override;
   ~FileWAL();
};
