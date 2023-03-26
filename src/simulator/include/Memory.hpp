#include <stdint.h>
#include <vector>

#ifndef GARAND_MEMORY_HPP
#define GARAND_MEMORY_HPP

namespace Garand {
class Cache {
  private:
    std::vector<std::pair<size_t, size_t>> cache_counter;
    std::vector<size_t> cache_latency;

  public:
  // Need to rework to support customizing size and cache count
    Cache(std::initializer_list<size_t> latency) {
        this->cache_latency = {latency};
        this->cache_counter =
            decltype(this->cache_counter)(latency.size(), {0, 0});
    }
};

class Memory {
  public:
    using LoadSize = uint32_t;
    using AddressSize = uint32_t;

  private:
    // Placeholders for now
    size_t size = 0;
    std::vector<uint8_t> memory_region;
    size_t counter = 0;
    // nanosecond
    size_t latency = 100;
    Garand::Cache cache;

  public:
    Memory(AddressSize sz = 0x100000, Garand::Cache cache = {})
        : size(sz), cache(cache) {
        this->memory_region = std::vector<uint8_t>(sz, 0);
    };
    // These two are placeholders for now
    LoadSize *load(AddressSize address);
    void store(AddressSize address, LoadSize value);
    size_t get_size();
    size_t get_counter();
    size_t get_latency();
    uint8_t *get_raw();
};

} // namespace Garand

#endif