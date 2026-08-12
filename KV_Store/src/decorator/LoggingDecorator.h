#pragma once
#include "StorageEngineDecorator.h"
#include "../core/kvstore/ILogger.h"

class LoggingDecorator: public StorageEngineDecorator{
private:
   std::unique_ptr<ILogger> logger;
public:
   LoggingDecorator(std::unique_ptr<IStorageEngine> wrap, std::unique_ptr<ILogger> logging) 
      :StorageEngineDecorator(std::move(wrap)), logger(std::move(logging)){}

   void put(const std::string& key, const std::string& value) override{
      logger->log("PUT",key,"running");
      wrapped->put(key,value);
      logger->log("PUT",key,"Success");
   }

   std::optional<std::string> get(const std::string& key) override{
      logger->log("GET", key, "running");
      std::optional<std::string> result = wrapped->get(key);
      if(!result) logger->log("GET", key, "fail");
      else logger->log("GET", key, "Success");
      return result;
   }

   void remove(const std::string& key) override{
      logger->log("REMOVE", key, "running");
      wrapped->remove(key);
      logger->log("REMOVE", key, "Success");
   }

};

