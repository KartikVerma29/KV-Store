#include "FileWAL.h"
#include <cstdint>
#include <unistd.h>

FileOffset FileWAL::append(const LogEntry& log){
   std::vector<uint8_t> bytes = log.encode();
   std::lock_guard<std::mutex> lock(writeMutex);
   fseek(fileHandle, 0, SEEK_END);
   uint64_t offset = ftell(fileHandle);
   fwrite(bytes.data(), 1, bytes.size(), fileHandle);
   return FileOffset{offset, bytes.size()};
}

std::vector<LogEntry> FileWAL::replay(){
   fseek(fileHandle, 0, SEEK_SET);
   std::vector<LogEntry> logs;
   uint32_t payloadSize;

   while(std::fread(&payloadSize, sizeof(uint32_t),1, fileHandle)==1 ){
      uint32_t totalRead = sizeof(uint32_t)*2 + payloadSize;
      std::vector<uint8_t> buffer(totalRead);
      std::memcpy(buffer.data(), &payloadSize, sizeof(uint32_t));

      std::fread(buffer.data() + sizeof(uint32_t), 1, totalRead - sizeof(uint32_t), fileHandle);
      logs.push_back(LogEntry::decode(buffer));
   }
   return logs;
}

void FileWAL::sync(){
   fflush(fileHandle);
   fsync(fileno(fileHandle));
}

FileWAL::~FileWAL(){
   std::fclose(fileHandle);
}
