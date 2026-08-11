#include "KVStoreEngine.h"

void KVStoreEngine::put(const std::string& key, const std::string& value){
   std::unique_lock<std::shared_mutex> lock(rwLock);
   FileOffset offset = wal->append(LogEntry(Record(key, value)));
   index->put(key, offset);
}

std::optional<std::string> KVStoreEngine::get(const std::string &key){
   std::shared_lock<std::shared_mutex> lock(rwLock);
   auto offset = index->get(key);
   if(offset.has_value()){
     return wal->read(offset.value()).getValue();
   }
  return std::nullopt;
}

void KVStoreEngine::remove(const std::string& key){
   std::unique_lock<std::shared_mutex> lock(rwLock);
   Record record = Record::createTomb(key);
   wal->append(LogEntry(record));
   index->remove(key);
}

void KVStoreEngine::recover(){
   auto recoved = wal->replay();
   for(const auto &[log, offset]:recoved){
      if(!log.validateChecksum()) continue;
      Record record = log.getRecord();
      if(record.isTomb()) index->remove(record.getKey());
      else index->put(record.getKey(), offset);
   }
}
