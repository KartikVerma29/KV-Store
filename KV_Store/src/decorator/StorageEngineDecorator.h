#pragma once
#include "../core/kvstore/IStorageEngine.h"
#include <memory>

class StorageEngineDecorator:public IStorageEngine{
protected:
   std::unique_ptr<IStorageEngine> wrapped;
public:
   StorageEngineDecorator(std::unique_ptr<IStorageEngine> wrap): wrapped(std::move(wrap)){}

   void put(const std::string& key, const std::string& value) override{
      wrapped->put(key, value);
   }

   std::optional<std::string> get(const std::string& key) override{
      return wrapped->get(key);
   }

   void remove(const std::string& key) override{
      wrapped->remove(key);
   }

   void flush() override{
      wrapped->flush();
   }

   virtual ~StorageEngineDecorator() = default;
};
