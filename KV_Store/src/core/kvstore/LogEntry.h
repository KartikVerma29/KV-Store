#pragma once
#include "Record.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include "../../utils/CRC32.h"

class LogEntry{
private:
   Record record;
   uint32_t checksum; //CRC32

   LogEntry(Record r, uint32_t c): record(std::move(r)), checksum(c) {}

public:
   LogEntry(const Record& record): record(record){
      std::vector<uint8_t> bytes = record.serialize();
      checksum = utils::CRC32(bytes.data(), bytes.size());
   }

   const Record& getRecord() const{
      return record;
   }

   const uint32_t getcs() const{
      return checksum;
   }

   std::vector<uint8_t> encode() const{
      std::vector<uint8_t> bytes = record.serialize();
      uint32_t payloadSize = bytes.size();

      std::vector<uint8_t> buffer(sizeof(uint32_t)*2+payloadSize);

      std::memcpy(buffer.data(), &payloadSize, sizeof(uint32_t));
      std::memcpy(buffer.data()+sizeof(uint32_t), &checksum, sizeof(uint32_t));
      std::memcpy(buffer.data()+(sizeof(uint32_t)*2), bytes.data(), payloadSize);
      return buffer;
   }

   LogEntry static decode(const std::vector<uint8_t>& buffer) {
      uint32_t c;
      std::memcpy(&c, buffer.data()+sizeof(uint32_t), sizeof(uint32_t));
      std::vector<uint8_t> bytes(buffer.begin()+(sizeof(uint32_t)*2), buffer.end());
      return LogEntry(Record::deserialize(bytes),c);
   }

   bool validateChecksum() const{
      std::vector<uint8_t> bytes = record.serialize();
      uint32_t calculated  = utils::CRC32(bytes.data(), bytes.size());
      return checksum==calculated;
   }

};
