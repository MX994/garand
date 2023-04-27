#include "Memory.hpp"
// #include <fmt/format.h>

namespace Garand {
    CacheBlock *Memory::CacheCheckHitMiss(CacheAddress Addr) {
        CacheBlock *Block = &Blocks[Addr.Index];
        auto id = Addr.Index;
        // fmt::print("Cache block {} used\n", id);
        if (!Memory::IsBlockInCache(Addr, Block)) {
            // Miss; Replace block.
            Block->Tag = Addr.Tag;
            Block->Valid = true;
            uint32_t BlockRealAddress = (Addr.Tag << 0x18) | (Addr.Index << 0x10);
            memcpy(Block->Data, &this->memory_region[BlockRealAddress], CACHE_BLOCK_SIZE);
        }
        // Return the new block. Should be updated in memory as well.
        return Block;
    }

    bool Memory::IsBlockInCache(CacheAddress Addr, CacheBlock *Block) {
        return Block->Tag == Addr.Tag && Block->Valid;
    }

    LoadSize *Memory::load(AddressSize address) {
        CacheAddress Addr = {
            .Tag = (uint8_t)((address >> 0x18) & 0xFF),
            .Index = ((address >> 0x10) & 0xFF) % 0x100,
            .Offset = address & 0xFFFF,
        };
        CacheBlock *Block = CacheCheckHitMiss(Addr);
        return (LoadSize *)(Block->Data);
    }

    void Memory::store(AddressSize address, LoadSize value) {
        CacheAddress Addr = {
            .Tag = (uint8_t)((address >> 0x18) & 0xFF),
            .Index = ((address >> 0x10) & 0xFF) % 0x100,
            .Offset = address & 0xFFFF,
        };
        CacheBlock *Block = CacheCheckHitMiss(Addr);
        *(LoadSize *)(&Block->Data[Addr.Offset]) = value;
        *(LoadSize *)(&this->memory_region[address]) = value;
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