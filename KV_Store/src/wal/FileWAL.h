#pragma once
#include "../core/kvstore/IWriteAheadLog.h"
#include <cstdio>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <stdexcept>

class FileWAL: public IWriteAheadLog{
private:
   FILE* fileHandle;
   std::string path;
   std::shared_mutex rwMutex;

public:
   FileWAL(std::string p): path(std::move(p)){
      fileHandle = std::fopen(path.c_str(), "ab+");
      if(!fileHandle){
         throw std::runtime_error("Failed to open WAL file: "+path);
      }
   }
   FileOffset append(const LogEntry& log) override;
   std::vector<std::pair<LogEntry, FileOffset>> replay() override;
   Record read(const FileOffset& offset) override;
   void sync() override;
   ~FileWAL();
};
