#pragma once
#include "../core/kvstore/IStorageEngine.h"
#include <memory>
#include <shared_mutex>
#include "../core/kvstore/IIndex.h"
#include "../core/kvstore/IWriteAheadLog.h"
#include <mutex>

class KVStoreEngine: public IStorageEngine{
private:
   std::unique_ptr<IIndex> index;
   std::unique_ptr<IWriteAheadLog> wal;
   std::shared_mutex rwLock;

   void recover();
public:

   KVStoreEngine(std::unique_ptr<IIndex> idx, std::unique_ptr<IWriteAheadLog> log):
      index(std::move(idx)), wal(std::move(log)){
         recover();
      }

   void put(const std::string &key, const std::string& value) override;
   std::optional<std::string> get(const std::string& key) override;
   void remove(const std::string& key) override;
   void flush() override{
      wal->sync();
   }
};
