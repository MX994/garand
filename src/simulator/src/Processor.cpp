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
            if (!Pipeline[s]->Processed) {
                std::optional<Cycle> res;
                if (s == Stage::DECODE) {
                    res = Decode();
                } else if (s == Stage::EXECUTE) {
                    res = Execute();
                } else if (s == Stage::WRITE_BACK) {
                    res = WriteBack();
                }
                if (res) {
                    if (auto &cycle = Pipeline[s]->CycleNeeded[s]; cycle == 0) {
                        cycle = *res;
                    }
                    Pipeline[s]->Processed = true;
                }
            }
            ++Pipeline[s]->CycleCounter;
            if (Pipeline[s]->Processed &&
                Pipeline[s]->CycleCounter >= Pipeline[s]->CycleNeeded[s] &&
                NextStageIsFree) {
                Pipeline[s]->CycleCounter = 0;
                Pipeline[s]->Processed = false;
                if (s != Stage::WRITE_BACK) {
                    Pipeline[s + 1] = Pipeline[s];
                }
                Pipeline[s] = nullptr;
                NextStageIsFree = true;
            } else {
                NextStageIsFree = !Pipeline[s];
            }
        } else {
            NextStageIsFree = true;
        }
    }
    if (!Pipeline[Stage::FETCH]) {
        if (auto res = Fetch(); res) {
            if (auto &cycle = Pipeline[Stage::FETCH]->CycleNeeded[Stage::FETCH];
                cycle == 0) {
                cycle = *res;
            }
            Pipeline[Stage::FETCH]->Processed = true;
        }
    }
    Tick();
}

void Processor::Queue(GarandInstruction Inst) {
    InstructionWk Wk;
    Wk.Instruction = Inst;
    std::array<unsigned, 4> const CycleTime{1, 1, 1, 1};
    std::transform(CycleTime.begin(), CycleTime.end(), Wk.CycleNeeded,
                   [](unsigned x) { return x; });
    Wk.CycleCounter = 0;
    InstructionQueue.push(Wk);
}

std::optional<Cycle> Processor::Fetch() {
    auto is_occupied = false;
    for (auto i = 0; i <= Stage::WRITE_BACK; ++i) {
        is_occupied |= !!Pipeline[i];
    }
    if (!pipeline && is_occupied) {
        return std::nullopt;
    }
    if (!Pipeline[Stage::FETCH]) {
        if (InstructionQueue.empty()) {
            auto pc = static_cast<AddressSize>(WkRegs.ProgramCounter);
            auto cost = WkRAM.GetCacheCycle(pc);
            auto &ins = *WkRAM.load<uint32_t>(pc);
            InstructionWk Wk;
            Wk.Instruction = Instruction::Encode(ins);
            Wk.Pointer = static_cast<AddressSize>(WkRegs.ProgramCounter);
            Pipeline[Stage::FETCH] = std::make_shared<InstructionWk>(Wk);
            WkRegs.ProgramCounter += 4;
            return {cost};
        } else {
            Pipeline[Stage::FETCH] =
                std::make_shared<InstructionWk>(InstructionQueue.front());
            InstructionQueue.pop();
        }
        return {1};
    }
    return std::nullopt;
}

std::optional<Cycle> Processor::Decode() {
    if (Pipeline[Stage::DECODE]) {
        Pipeline[Stage::DECODE]->decodedInstruction =
            Instruction::Decode(Pipeline[Stage::DECODE]->Instruction);
        return {1};
    }
    return std::nullopt;
}

std::optional<Cycle> Processor::Execute() {
    auto &ins = Pipeline[Stage::EXECUTE];
    
    if (ins) {
        if (auto &ahead = Pipeline[Stage::WRITE_BACK];
            ahead && ahead->WriteBack.write_back) {
            auto const &regs = ins->decodedInstruction.parameter.Registers;
            for (auto const &reg: regs) {
                if (load_reg(&WkRegs, reg) == ahead->WriteBack.reg) {
                    return std::nullopt;
                }
            }

        }
        auto write_back = Instruction::Execute(
            ins->decodedInstruction, ins->Instruction, WkRAM, &WkRegs);
        ins->StagnatePCDiff =
            static_cast<AddressSize>(WkRegs.ProgramCounter - ins->Pointer);
        ins->WriteBack = write_back;
        // if (write_back.reg == &WkRegs.ProgramCounter) {
        //     Flush(Stage::EXECUTE);
        // }
        return {write_back.execute_cost};
    }
    return std::nullopt;
}

std::optional<Cycle> Processor::WriteBack() {
    if (Pipeline[Stage::WRITE_BACK]) {
        auto &wb = Pipeline[Stage::WRITE_BACK]->WriteBack;
        auto &state = Pipeline[Stage::WRITE_BACK];
        constexpr std::array<DecodedMnemonic, 15> rel_branch = {
            BCC_AL, BCC_EQ, BCC_NE, BCC_LO, BCC_HS, BCC_LT, BCC_GE, BCC_HI,
            BCC_LS, BCC_GT, BCC_LE, BCC_VC, BCC_VS, BCC_PL, BCC_NG};
        if (std::binary_search(rel_branch.begin(), rel_branch.end(),
                               state->decodedInstruction.mnemonic)) {
            // Since the write-back happens after PC is updated in Fetch stage
            // We has to fix by using the offset
            wb.value -= state->StagnatePCDiff;
        }
        auto cost = Instruction::WriteBack(wb, WkRAM);
        if (wb.reg == &WkRegs.ProgramCounter) {
            Flush(Stage::WRITE_BACK);
        }
        return {cost};
    }
    return std::nullopt;
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