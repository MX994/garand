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
                if (s == Stage::DECODE) {
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
        if (!Pipeline[s] && s == Stage::FETCH) {
            Fetch();
        }
    }
    Tick();
}

void Processor::Queue(GarandInstruction Inst) {
    InstructionWk Wk;
    Wk.Instruction = Inst;
    std::array<unsigned, 4> const CycleTime{1, 1, 1, 1};
    std::transform(CycleTime.begin(), CycleTime.end(), Wk.CycleMax,
                   [](unsigned x) { return x; });
    Wk.CycleCount = 0;
    InstructionQueue.push(Wk);
}

void Processor::Fetch() {
    if (!Pipeline[Stage::FETCH]) {
        if (InstructionQueue.empty()) {
            auto &ins = *WkRAM.load<uint32_t>(WkRegs.ProgramCounter);
            InstructionWk Wk;
            Wk.Instruction = Instruction::Encode(ins);
            std::array<unsigned, 4> const CycleTime{1, 1, 1, 1};
            std::transform(CycleTime.begin(), CycleTime.end(), Wk.CycleMax,
                           [](unsigned x) { return x; });
            Wk.CycleCount = 0;
            Wk.Pointer = WkRegs.ProgramCounter;
            Pipeline[Stage::FETCH] = std::make_shared<InstructionWk>(Wk);
            WkRegs.ProgramCounter += 4;
        } else {
            Pipeline[Stage::FETCH] =
                std::make_shared<InstructionWk>(InstructionQueue.front());
            InstructionQueue.pop();
        }
    }
}

void Processor::Decode() {
    if (Pipeline[Stage::DECODE]) {
        Pipeline[Stage::DECODE]->decodedInstruction =
            Instruction::Decode(Pipeline[Stage::DECODE]->Instruction);
    }
}

void Processor::ExecuteMemload() {
    auto ins = Pipeline[Stage::EXECUTE]->decodedInstruction;
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
            WkRAM.IsBlockInCache(Addr, &WkRAM.Blocks[Addr.Index])
                ? CACHE_HIT_CYCLES
                : CACHE_MISS_CYCLES;
    }
}

void Processor::Execute() {
    auto &ins = Pipeline[Stage::EXECUTE];
    if (ins) {
        auto write_back =
            Instruction::Execute(ins->decodedInstruction, ins->Instruction,
                                 WkRAM, (uint64_t *)&WkRegs);
        ins->StagnatePCDiff = WkRegs.ProgramCounter - ins->Pointer;
        ins->WriteBack = write_back;
        if (write_back.reg == &WkRegs.ProgramCounter) {
            Flush(Stage::EXECUTE);
        }
    }
}

void Processor::WriteBack() {
    if (Pipeline[Stage::WRITE_BACK]) {
        auto &wb = Pipeline[Stage::WRITE_BACK]->WriteBack;
        auto &state = Pipeline[Stage::WRITE_BACK];
        using Garand::DecodedInstruction;
        constexpr std::array<DecodedInstruction, 15> rel_branch = {
            BCC_AL, BCC_EQ, BCC_NE, BCC_LO, BCC_HS, BCC_LT, BCC_GE, BCC_HI,
            BCC_LS, BCC_GT, BCC_LE, BCC_VC, BCC_VS, BCC_PL, BCC_NG};
        if (std::binary_search(rel_branch.begin(), rel_branch.end(),
                               state->decodedInstruction)) {
            // Since the write-back happens after PC is updated in Fetch stage
            // We has to fix by using the offset
            wb.value -= state->StagnatePCDiff;
        }
        Garand::Instruction::WriteBack(wb, WkRAM);
        if (wb.reg == &WkRegs.ProgramCounter) {
            Flush(Stage::WRITE_BACK);
        }
    }
}

void Processor::Flush(Stage current) {
    for (int s = Stage::FETCH; s != current; ++s) {
        Pipeline[s] = nullptr;
    }
    while (!InstructionQueue.empty()) {
        InstructionQueue.pop();
    }
}

decltype(Processor::Pipeline) &Processor::View() { return Pipeline; }

decltype(Processor::WkRegs) const &Processor::Regs() { return WkRegs; }

void Processor::ResetRegs() { WkRegs = Garand::Registers{}; }

decltype(Processor::Clock) Processor::ReadClock() { return Clock; }

Memory &Processor::ReadMem() { return WkRAM; }

AddressSize Processor::ReadExecPC() {
    if (Pipeline[Stage::EXECUTE]) {
        return Pipeline[Stage::EXECUTE]->Pointer;
    } else {
        return 0;
    }
}

} // namespace Garand