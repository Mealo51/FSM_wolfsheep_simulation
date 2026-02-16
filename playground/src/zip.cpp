// zip.cpp

#include "zip.hpp"
#include <stdio.h>
#include <assert.h> 

// note: zip file structure signatures
static constexpr uint32_t ZIP_CENTRAL_SIGNATURE = 0x02014B50u;
static constexpr uint32_t ZIP_LOCAL_SIGNATURE   = 0x04034B50u;
static constexpr uint32_t ZIP_EOCD_SIGNATURE    = 0x06054B50u;

// note: pragma pack changes the alignment of all properties of the structs below.
//       normally we should never do this, but in this case we want that because
//       we read the while content of a struct in one `fread` call.
//    
//       more information:
//       - https://learn.microsoft.com/en-us/cpp/preprocessor/pack?view=msvc-170
//       
#pragma pack(push, 1)
struct ZipLocalFileHeader {
   uint32_t signature;
   uint16_t version;
   uint16_t bit_flags;
   uint16_t method;
   uint16_t last_mod_time;
   uint16_t last_mod_date;
   uint32_t crc32;
   uint32_t size_compressed;
   uint32_t size_uncompressed;
   uint16_t filename_length;
   uint16_t extra_field_length;
};

struct ZipCentralFileHeader {
   uint32_t signature;
   uint16_t version;
   uint16_t minimum_version;
   uint16_t bit_flags;
   uint16_t method;
   uint16_t last_mod_time;
   uint16_t last_mod_date;
   uint32_t crc32;
   uint32_t size_compressed;
   uint32_t size_uncompressed;
   uint16_t filename_length;
   uint16_t extra_field_length;
   uint16_t comment_length;
   uint16_t disk_num_file_start;
   uint16_t internal_file_attribs;
   uint32_t external_file_attribs;
   uint32_t local_header_offset;
};

struct ZipEndOfCentralDirectory {
   uint32_t signature;
   uint16_t disk_count;
   uint16_t disk_cdir_start;
   uint16_t local_cdir_count;
   uint16_t cdir_record_count;
   uint32_t cdir_size;
   uint32_t cdir_offset;
   uint16_t comment_length;
};
#pragma pack(pop)

// note: here we reduce an arbitrary length string to a 8-byte (64-bit) number (unsigned long long).
//       important to note is that we can have collisions between the resulting hash values calculated.
// 
//       more information: 
//       - https://en.wikipedia.org/wiki/Hash_function
//       - http://www.isthe.com/chongo/tech/comp/fnv/index.html
//       - https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17
//       - https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//    
static uint64_t 
hash_fnv1a64(const std::string_view str)
{
   // note: hash = FNV_offset_basis
   uint64_t hash = 14695981039346656037ull;

   // note: iterate over all characters in the string
   for (const auto ch : str) {
      // note: hash XOR byte_of_data
      hash ^= uint64_t(ch);
      // note: hash x FNV_Prime
      hash *= 1099511628211ull;
   }

   return hash;
}

ZipArchive::ZipArchive(const std::string_view path)
{
   FILE* file = nullptr;
   fopen_s(&file, path.data(), "rb");
   if (file == nullptr) {
      assert(false);
      return;
   }

   m_filename = path;

   // note: get file size
   fseek(file, 0, SEEK_END);
   long file_size = ftell(file);
   fseek(file, 0, SEEK_SET);

   // note: find end of central record
   ZipEndOfCentralDirectory eocd{};
   fseek(file, file_size - sizeof(eocd), SEEK_CUR);
   fread(&eocd, sizeof(eocd), 1, file);
   if (eocd.signature != ZIP_EOCD_SIGNATURE) {
      fclose(file);
      assert(false);
      return;
   }

   // note: go to beginning of central directory
   fseek(file, eocd.cdir_offset, SEEK_SET);
   for (uint32_t index = 0; index < eocd.cdir_record_count; index++) {
      ZipCentralFileHeader header{};
      fread(&header, sizeof(header), 1, file);
      if (header.signature != ZIP_CENTRAL_SIGNATURE) {
         fclose(file);
         assert(false);
         return;
      }

      std::string name(header.filename_length, '\0');
      fread(name.data(), name.size(), 1, file);

      uint32_t offset = header.local_header_offset;
      offset += sizeof(ZipLocalFileHeader);
      offset += header.filename_length;

      ZipFileInfo info{};
      info.offset = offset;
      info.compressed = header.size_compressed;
      info.uncompressed = header.size_uncompressed;

      const uint64_t name_hash_value = hash_fnv1a64(path);
      m_files.emplace(name_hash_value, info);
      
      // note: if we have extra information after the central header 
      //       we need to move forward to the next entry
      if (header.extra_field_length > 0) {
         fseek(file, header.extra_field_length, SEEK_CUR);
      }
   }

   fclose(file);
}

bool ZipArchive::valid() const
{
   return !m_filename.empty() && !m_files.empty();
}

bool ZipArchive::contains(const std::string_view path) const
{
   const uint64_t path_hash_value = hash_fnv1a64(path);
   return m_files.count(path_hash_value) > 0;
}

bool ZipArchive::read_compressed_data(const std::string_view path, std::vector<uint8_t>& content) const
{
   if (!valid()) {
      return false;
   }

   const uint64_t path_hash_value = hash_fnv1a64(path);
   auto iter = m_files.find(path_hash_value);
   if (iter == m_files.end()) {
      return false;
   }

   FILE* file = nullptr;
   fopen_s(&file, m_filename.c_str(), "rb");
   if (file == nullptr) {
      return false;
   }

   ZipFileInfo info = iter->second;
   content.resize(info.compressed);

   fseek(file, info.offset, SEEK_SET);
   fread(content.data(), 1, content.size(), file);
   fclose(file);

   return true;
}
