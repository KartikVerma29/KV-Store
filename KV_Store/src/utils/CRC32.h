#pragma once

#include <cstddef>
#include <cstdint>
#include <array>

namespace utils
{
   constexpr std::array<uint32_t, 256> generate_table(){
      std::array<uint32_t, 256> table{};
      uint32_t poly = 0xEDB88320;
      for(uint32_t i=0; i<256; i++){
         uint32_t rem=i;
         for(int j=0; j<8; j++){
            if(rem&1) rem = (rem>>1)^poly;
            else rem>>=1;
         }
         table[i]=rem;
      }
      return table;
   }

   constexpr auto crc32Table = generate_table();

   inline uint32_t CRC32(const uint8_t* data, size_t size){
      uint32_t rem = 0xFFFFFFFF;
      for(size_t i=0; i<size; i++){
         uint8_t byte = data[i];
         rem=(rem>>8)^crc32Table[(rem^byte)&0xFF];
      }
      return rem^0XFFFFFFFF;
   }

}