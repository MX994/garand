#include <stdint.h>
#include <vector>

#ifndef GARAND_MEMORY_HPP
#define GARAND_MEMORY_HPP

namespace Garand {
using LoadSize = uint32_t;
using AddressSize = uint32_t;
constexpr uint16_t CACHE_BLOCK_COUNT = 0x100; // 2^8.
constexpr uint32_t CACHE_BLOCK_SIZE = 0x10000; // 2 ^ 16.
constexpr uint32_t CACHE_MISS_PENALTY = 0xDFA;
constexpr uint32_t CACHE_HIT_PENALTY = 0x1FA;

struct CacheAddress {
  uint32_t IsDirty : 1; // Dirty bit.
  uint8_t Tag : 8;     // :)
  uint32_t Index : 8;  // We have 2^8 lines.
  uint32_t Offset : 16; // Block size is 2^16.
};

struct CacheBlock {
  // Each block is 64KB.
  uint8_t Tag;
  uint8_t Valid;
  uint8_t Data[CACHE_BLOCK_SIZE];
};

inline CacheBlock Blocks[CACHE_BLOCK_COUNT];

class Cache {
  private:
    size_t counter;
    size_t latency;

  public:
    Cache(size_t latency = 0x10) : latency(latency) {}
    CacheBlock ReadCacheBlock(uint32_t Offset);
};

class Memory {
  public:
  private:
    // Placeholders for now
    size_t size = 0;
    std::vector<uint8_t> memory_region;
    size_t counter = 0;
    // nanosecond
    size_t latency = 100;
    Garand::Cache cache;

  public:
    Memory(AddressSize sz = 0x100000, Garand::Cache cache = {}): size(sz), cache(cache) {
        this->memory_region = std::vector<uint8_t>(sz, 0);
    };
    // These two are placeholders for now
    LoadSize *load(AddressSize address);
    CacheBlock *FindBlock(CacheAddress Addr);
    void store(AddressSize address, LoadSize value);
    size_t get_size();
    size_t get_counter();
    size_t get_latency();
    uint8_t *get_raw();
};

} // namespace Garand

#endif