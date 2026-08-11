#pragma once
#include "FileOffset.h"
#include "LogEntry.h"
#include <utility>
#include <vector>

class IWriteAheadLog{
public:
   virtual FileOffset append(const LogEntry& log) = 0;
   virtual std::vector<std::pair<LogEntry, FileOffset>> replay() = 0;
   virtual void sync() = 0;
   virtual Record read(const FileOffset& offset) = 0;
   virtual ~IWriteAheadLog() = default;
};
