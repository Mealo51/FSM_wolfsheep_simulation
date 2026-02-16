// zip.hpp

#pragma once

// note: this is just example code, it is not intended to be used for anything serious!

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

struct ZipArchive {
   struct ZipFileInfo {
      uint32_t offset;
      uint32_t compressed;
      uint32_t uncompressed;
   };

   ZipArchive() = delete;
   ZipArchive(const std::string_view path);

   bool valid() const;
   bool contains(const std::string_view path) const;
   bool read_compressed_data(const std::string_view path, std::vector<uint8_t>& content) const;
   
   std::string m_filename;
   // note: instead of storing and comparing strings we compare a hash-value of the string content.
   std::unordered_map<uint64_t, ZipFileInfo> m_files;
};
