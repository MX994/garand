#include "Memory.hpp"
#include <string.h>

namespace Garand {
    CacheBlock *Memory::FindBlock(CacheAddress Addr) {
        CacheBlock *Block = &Blocks[Addr.Index];
        if (Block->Tag != Addr.Tag) {
            // Miss; Replace block.
            counter += CACHE_MISS_PENALTY;
            Block->Tag = Addr.Tag;
            uint32_t BlockRealAddress = (Addr.Tag << 0x18) | (Addr.Index << 0x10);
            memcpy(Block->Data, &this->memory_region[BlockRealAddress], CACHE_BLOCK_SIZE);
        } else {
            counter += CACHE_HIT_PENALTY;
        }
        // Return the new block. Should be updated in memory as well.
        return Block;
    }

    LoadSize *Memory::load(AddressSize address) {
        CacheAddress Addr = {
            .Tag = (address >> 0x18) & 0xFF,
            .Index = ((address >> 0x10) & 0xFF) % 0x100,
            .Offset = address & 0xFFFF,
        };
        CacheBlock *Block = FindBlock(Addr);
        return (LoadSize *)(Block->Data);
    }

    void Memory::store(AddressSize address, LoadSize value) {
        CacheAddress Addr = {
            .Tag = (address >> 0x18) & 0xFF,
            .Index = ((address >> 0x10) & 0xFF) % 0x100,
            .Offset = address & 0xFFFF,
        };
        CacheBlock *Block = FindBlock(Addr);
        *(LoadSize *)(Block->Data[Addr.Offset]) = value;
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