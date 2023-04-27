#include <array>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Processor.hpp"

namespace Garand {
void Processor::Tick() { Clock += 1; }

void Processor::Step() {
    bool NextStageIsFree = true;
    for (int s = Stage::WRITE_BACK; s >= Stage::FETCH; --s) {
        if (Pipeline[s]) {
            ++Pipeline[s]->CycleCount;
            if (s == Stage::EXECUTE && Pipeline[s]->CycleMax[s] == 0) {
                // This will modify the CycleMax
                ExecuteMemload();
            }
            if (Pipeline[s]->CycleCount == Pipeline[s]->CycleMax[s]) {
                if (s == Stage::FETCH) {
                    Fetch();
                } else if (s == Stage::DECODE) {
                    Decode();
                } else if (s == Stage::EXECUTE) {
                    Execute();
                } else if (s == Stage::WRITE_BACK) {
                    WriteBack();
                }
            }
            if (Pipeline[s]->CycleCount >= Pipeline[s]->CycleMax[s] &&
                NextStageIsFree) {
                Pipeline[s]->CycleCount = 0;
                if (s != Stage::WRITE_BACK) {
                    Pipeline[s + 1] = Pipeline[s];
                }
                Pipeline[s] = NULL;
                NextStageIsFree = true;
            } else {
                NextStageIsFree = !Pipeline[s];
            }
        } else {
            NextStageIsFree = true;
        }
    }
    // Fetch stage exclusive
    if (!Pipeline[Stage::FETCH] && !InstructionQueue.empty()) {
        Pipeline[Stage::FETCH] =
            std::make_shared<InstructionWk>(InstructionQueue.front());
        InstructionQueue.pop();
    }
    Tick();
}

void Processor::Queue(GarandInstruction Inst) {
    InstructionWk Wk;
    Wk.Instruction = Inst;
    std::array<unsigned, 4> const CycleTime{3, 4, 5, 6};
    std::transform(CycleTime.begin(), CycleTime.end(), Wk.CycleMax,
                   [](unsigned x) { return x; });
    Wk.CycleCount = 0;
    InstructionQueue.push(Wk);
}

void Processor::Fetch() {
    // Psuedo
}

void Processor::Decode() {
    if (Pipeline[Stage::DECODE]) {
        Pipeline[Stage::DECODE]->DecodedInstruction =
            Instruction::Decode(Pipeline[Stage::DECODE]->Instruction);
    }
}

void Processor::ExecuteMemload() {
    auto ins = Pipeline[Stage::EXECUTE]->DecodedInstruction;
    if ((ins == Garand::DecodedInstruction::MREAD ||
         ins == Garand::DecodedInstruction::MWRITE) &&
        Pipeline[Stage::EXECUTE]->CycleMax[Stage::EXECUTE] == 0) {
        // Memory reads and writes are a special case.

        // Parse the arguments.
        uint32_t Args =
            Pipeline[Stage::EXECUTE]->Instruction.InstructionSpecific;
        uint32_t BaseAddr = Args >> 8 & 0b111111;

        // Just in case we want to work with these...
        // Uncomment them :).

        // uint32_t Shift = Args >> 2 & 0b111111;
        // BaseAddr += Shift;

        // Parse address.
        CacheAddress Addr = {
            .Tag = (uint8_t)((BaseAddr >> 0x18) & 0xFF),
            .Index = ((BaseAddr >> 0x10) & 0xFF) % 0x100,
            .Offset = BaseAddr & 0xFFFF,
        };

        Pipeline[Stage::EXECUTE]->CycleMax[Stage::EXECUTE] =
            WkRAM.IsBlockInCache(Addr, &Garand::Blocks[Addr.Index])
                ? CACHE_HIT_CYCLES
                : CACHE_MISS_CYCLES;
    }
}

void Processor::Execute() {
    if (Pipeline[Stage::EXECUTE]) {
        Pipeline[Stage::EXECUTE]->WriteBack = Instruction::Execute(
            Pipeline[Stage::EXECUTE]->DecodedInstruction,
            Pipeline[Stage::EXECUTE]->Instruction, WkRAM, (uint64_t *)&WkRegs);
    }
}

void Processor::WriteBack() {
    if (Pipeline[Stage::WRITE_BACK]) {
        Garand::Instruction::WriteBack(Pipeline[Stage::WRITE_BACK]->WriteBack);
    }
}

decltype(Processor::Pipeline) &Processor::View() { return Pipeline; }

decltype(Processor::WkRegs) const &Processor::Regs() { return WkRegs; }

void Processor::ResetRegs() { WkRegs = Garand::Registers{}; }

decltype(Processor::Clock) Processor::ReadClock() { return Clock; }

Memory &Processor::ReadMem() { return WkRAM; }

} // namespace Garand