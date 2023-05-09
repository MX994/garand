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

    bool Memory::IsCacheHit(AddressSize address) {
        auto Addr = toCacheAddress(address);
        return IsBlockInCache(Addr, &Blocks[Addr.Index]);
    }

    uint64_t Memory::GetCacheCycle(AddressSize address) {
        return IsCacheHit(address) ? CACHE_HIT_CYCLES : CACHE_MISS_CYCLES;
    }

    bool Memory::IsBlockInCache(CacheAddress Addr, CacheBlock *Block) {
        return Block->Tag == Addr.Tag && Block->Valid;
    }

    size_t Memory::get_size() {
        return this->memory_region.size();
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

    void Memory::invalidate() {
        for (auto &block: this->Blocks) {
            block.Valid = 0;
        }
    }
    void Memory::invalidate_block(uint32_t index) {
        this->Blocks[index].Valid = 0;
    }

    CacheAddress toCacheAddress(AddressSize address) {
        return {
            .Tag = (uint8_t)((address >> 0x18) & 0xFF),
            .Index = static_cast<uint32_t>(((address >> 0x10) & 0xFF) % 0x100),
            .Offset = static_cast<uint32_t>(address & 0xFFFF),
        };
    }

}