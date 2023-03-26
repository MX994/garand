#include "Memory.hpp"

namespace Garand {
    Memory::LoadSize *Memory::load(Memory::AddressSize address) {
        this->counter += 1;
        return (Memory::LoadSize *)(&this->memory_region[address]);
    }
    void Memory::store(Memory::AddressSize address, Memory::LoadSize value) {
        this->counter += 1;
        *(Memory::LoadSize *)(&this->memory_region[address]) = value;
    }
    size_t Memory::get_size() {
        return this->size;
    }
    size_t Memory::get_counter() {
        return this->counter;
    }
    size_t Memory::get_latency() {
        return this->latency;
    }
    uint8_t *Memory::get_raw() {
        return this->memory_region.data();
    }
}