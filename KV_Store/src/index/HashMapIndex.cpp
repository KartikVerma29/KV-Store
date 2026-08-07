#include "HashMapIndex.h"

   
std::optional<FileOffset> HashMapIndex::get(const std::string& key) const {
   auto it = table.find(key);
   if(it!=table.end()) return it->second;
   return std::nullopt;
}
   
   


