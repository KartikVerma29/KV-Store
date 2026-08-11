#include "FileWAL.h"
#include <cstdint>
#include <unistd.h>

FileOffset FileWAL::append(const LogEntry& log){
   std::vector<uint8_t> bytes = log.encode();
   std::unique_lock<std::shared_mutex> lock(rwMutex);
   fseek(fileHandle, 0, SEEK_END);
   uint64_t offset = ftell(fileHandle);
   fwrite(bytes.data(), 1, bytes.size(), fileHandle);
   return FileOffset{offset, bytes.size()};
}

std::vector<std::pair<LogEntry,FileOffset>> FileWAL::replay(){
   fseek(fileHandle, 0, SEEK_SET);
   std::vector<std::pair<LogEntry,FileOffset>> logs;
   uint32_t payloadSize;

   while(std::fread(&payloadSize, sizeof(uint32_t),1, fileHandle)==1 ){
      uint64_t currOff = ftell(fileHandle)-sizeof(uint32_t);
      uint32_t totalRead = sizeof(uint32_t)*2 + payloadSize;
      std::vector<uint8_t> buffer(totalRead);
      std::memcpy(buffer.data(), &payloadSize, sizeof(uint32_t));

      size_t remain = totalRead-sizeof(uint32_t);
      size_t got = std::fread(buffer.data() + sizeof(uint32_t), 1, remain, fileHandle);

      if(got!=remain) break;

      logs.push_back(std::make_pair(LogEntry::decode(buffer), FileOffset{currOff, totalRead}));
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

Record FileWAL::read(const FileOffset& offset){
   std::shared_lock<std::shared_mutex> lock(rwMutex);
   fseek(fileHandle, offset.byteOffset, SEEK_SET);
   std::vector<uint8_t> buffer(offset.length);
   fread(buffer.data(), 1, offset.length, fileHandle);
   return LogEntry::decode(buffer).getRecord();
}

