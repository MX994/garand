#include <stdint.h>
#include <string.h>
#include <vector>

#ifndef GARAND_MEMORY_HPP
#define GARAND_MEMORY_HPP
#include "Registers.hpp"

namespace Garand {
using LoadSize = uint64_t;
using AddressSize = uint32_t;
constexpr uint8_t CACHE_BLOCK_INDEX_BIT = 0x8;
constexpr AddressSize CACHE_BLOCK_COUNT = 1ULL << CACHE_BLOCK_INDEX_BIT; // 2^8.
constexpr uint8_t CACHE_BLOCK_OFFSET_BIT = 0x10;
constexpr AddressSize CACHE_BLOCK_SIZE = 1ULL << CACHE_BLOCK_OFFSET_BIT; // 2 ^ 16.
constexpr uint8_t CACHE_BLOCK_TAG_BIT =
    sizeof(AddressSize) * 8 - CACHE_BLOCK_OFFSET_BIT - CACHE_BLOCK_INDEX_BIT;
constexpr AddressSize CACHE_BLOCK_TAG = 1ULL << CACHE_BLOCK_TAG_BIT;
constexpr uint32_t CACHE_MISS_CYCLES = 0x30;
constexpr uint32_t CACHE_HIT_CYCLES = 0x5;

struct CacheAddress {
  AddressSize Tag : CACHE_BLOCK_TAG_BIT;     // :)
  AddressSize Index : CACHE_BLOCK_INDEX_BIT;  // We have 2^8 lines.
  AddressSize Offset : CACHE_BLOCK_OFFSET_BIT; // Block size is 2^16.
};

CacheAddress toCacheAddress(AddressSize address);

struct CacheBlock {
  // Each block is 64KB.
  AddressSize Tag;
  bool Valid;
  uint8_t Data[CACHE_BLOCK_SIZE];
};

class Memory {
  private:
    // Placeholders for now
    std::vector<uint8_t> memory_region;
    size_t counter = 0;
    // nanosecond
    size_t latency = 100;

  public:
    Memory(AddressSize sz = 0x100000) {
        this->memory_region = std::vector<uint8_t>(sz, 0);
        this->Blocks.resize(CACHE_BLOCK_COUNT);
    };
    std::vector<CacheBlock> Blocks;
    CacheBlock *CacheCheckHitMiss(CacheAddress Addr);
    bool IsCacheHit(AddressSize);
    uint64_t GetCacheCycle(AddressSize);
    bool IsBlockInCache(CacheAddress Addr);
    template<typename T>
    T *load(AddressSize address) {
        if (!cacheOn) {
          return reinterpret_cast<T*>(memory_region.data() + address);
        }
        CacheAddress Addr = toCacheAddress(address);
        CacheBlock *Block = CacheCheckHitMiss(Addr);
        return reinterpret_cast<T *>(Block->Data + Addr.Offset);
    }
    template<typename T>
    void store(AddressSize address, T value) {
        if (!cacheOn) {
          *reinterpret_cast<T*>(memory_region.data() + address) = value;
          return;
        }
        CacheAddress Addr = toCacheAddress(address);
        CacheBlock *Block = CacheCheckHitMiss(Addr);
        *(T *)(&Block->Data[Addr.Offset]) = value;
        *reinterpret_cast<T*>(memory_region.data() + address) = value;
    }
    size_t get_size();
    size_t get_counter();
    size_t get_latency();
    uint8_t *get_raw();
    void invalidate();
    void invalidate_block(AddressSize);

    // By default, Memory uses cache.
    // Set this to false to turn it off,
    // ..which change IsCacheHit and CacheCheckHitMiss
    bool cacheOn = true;
};

}; // namespace Garand

#endif