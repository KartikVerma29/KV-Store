#pragma once
#include "../engine/KVStoreEngine.h"
#include "../decorator/LoggingDecorator.h"
#include "EngineConfig.h"
#include "../index/HashMapIndex.h"
#include "../wal/FileWAL.h"
#include "../logger/ConsoleLogger.h"
#include "../logger/FileLogger.h"
#include <memory>

class StorageEngineFactory{
public:
   std::unique_ptr<IStorageEngine> createEngine(EngineConfig& configs){
      std::unique_ptr<IIndex> index; 
      std::unique_ptr<IWriteAheadLog> aheadlog;
      std::unique_ptr<IStorageEngine> engine;

      index = std::make_unique<HashMapIndex>();
      aheadlog = std::make_unique<FileWAL>(configs.walPath);
      engine = std::make_unique<KVStoreEngine>(std::move(index), std::move(aheadlog));

      if(configs.enableLogging){
         std::unique_ptr<ILogger> logger;
         if(configs.logtype==LoggerType::CONSOLE){
            logger = std::make_unique<ConsoleLogger>();
         }else logger = std::make_unique<FileLogger>(configs.logPath);
         engine = std::make_unique<LoggingDecorator>(std::move(engine),std::move(logger));
        }
      return engine;
    } 

};