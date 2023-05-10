#include "Memory.hpp"
// #include <fmt/format.h>

namespace Garand {
CacheBlock *Memory::CacheCheckHitMiss(CacheAddress Addr) {
    CacheBlock *Block = &Blocks[Addr.Index];
    // fmt::print("Cache block {} used\n", id);
    if (!Memory::IsBlockInCache(Addr)) {
        // Miss; Replace block.
        Block->Tag = Addr.Tag;
        Block->Valid = true;
        AddressSize BlockRealAddress =
            ((Addr.Tag << CACHE_BLOCK_INDEX_BIT) | Addr.Index)
            << CACHE_BLOCK_OFFSET_BIT;
        memcpy(Block->Data, &this->memory_region[BlockRealAddress],
               CACHE_BLOCK_SIZE);
    }
    // Return the new block. Should be updated in memory as well.
    return Block;
}

bool Memory::IsCacheHit(AddressSize address) {
    auto Addr = toCacheAddress(address);
    return IsBlockInCache(Addr);
}

uint64_t Memory::GetCacheCycle(AddressSize address) {
    return IsCacheHit(address) ? CACHE_HIT_CYCLES : CACHE_MISS_CYCLES;
}

bool Memory::IsBlockInCache(CacheAddress Addr) {
    auto *Block = &Blocks[Addr.Index];
    return (Block->Tag == Addr.Tag) && Block->Valid;
}

size_t Memory::get_size() { return this->memory_region.size(); }

size_t Memory::get_counter() { return this->counter; }

size_t Memory::get_latency() { return this->latency; }

uint8_t *Memory::get_raw() { return this->memory_region.data(); }

void Memory::invalidate() {
    for (auto &block : this->Blocks) {
        block.Valid = 0;
    }
}
void Memory::invalidate_block(uint32_t index) { this->Blocks[index].Valid = 0; }

CacheAddress toCacheAddress(AddressSize address) {
    return {
        .Tag = static_cast<uint32_t>(
            (address >> (CACHE_BLOCK_INDEX_BIT + CACHE_BLOCK_OFFSET_BIT)) &
            (CACHE_BLOCK_TAG - 1)),
        .Index = static_cast<uint32_t>(((address >> CACHE_BLOCK_OFFSET_BIT) &
                                       (CACHE_BLOCK_COUNT - 1))),
        .Offset = static_cast<uint32_t>(address & (CACHE_BLOCK_SIZE - 1)),
    };
}

} // namespace Garand