#include <stdint.h>
#include <string.h>
#include <vector>

#ifndef GARAND_MEMORY_HPP
#define GARAND_MEMORY_HPP

namespace Garand {
using LoadSize = uint32_t;
using AddressSize = uint64_t;
constexpr uint16_t CACHE_BLOCK_COUNT = 0x100; // 2^8.
constexpr uint32_t CACHE_BLOCK_SIZE = 0x10000; // 2 ^ 16.
constexpr uint32_t CACHE_MISS_CYCLES = 0x15;
constexpr uint32_t CACHE_HIT_CYCLES = 0x5;

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

class Memory {
  private:
    // Placeholders for now
    size_t size = 0;
    std::vector<uint8_t> memory_region;
    size_t counter = 0;
    // nanosecond
    size_t latency = 100;

  public:
    Memory(AddressSize sz = 0x100000): size(sz) {
        this->memory_region = std::vector<uint8_t>(sz, 0);
        this->Blocks.resize(CACHE_BLOCK_COUNT);
    };
    std::vector<CacheBlock> Blocks;
    // These two are placeholders for now
    LoadSize *load(AddressSize address);
    CacheBlock *CacheCheckHitMiss(CacheAddress Addr);
    bool IsBlockInCache(CacheAddress Addr, CacheBlock *Block);
    void store(AddressSize address, LoadSize value);
    size_t get_size();
    size_t get_counter();
    size_t get_latency();
    uint8_t *get_raw();
};

}; // namespace Garand

#endif