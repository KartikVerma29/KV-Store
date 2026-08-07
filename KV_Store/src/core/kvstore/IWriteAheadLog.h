#pragma once
#include "FileOffset.h"
#include "LogEntry.h"
#include <vector>

class IWriteAheadLog{
public:
   virtual FileOffset append(const LogEntry& log) = 0;
   virtual std::vector<LogEntry> replay() = 0;
   virtual void sync() = 0;
   virtual ~IWriteAheadLog() = default;
};
