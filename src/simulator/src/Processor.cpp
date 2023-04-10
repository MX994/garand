#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <array>

#include "Processor.hpp"

namespace Garand {
void Processor::Tick() { Clock += 1; }

void Processor::Step() {
    bool NextStageIsFree = true;
    for (unsigned s = Stage::WRITE_BACK; s > 0; --s) {
        ++Pipeline[s]->CycleCount;
        if (Pipeline[s]->CycleCount >= Pipeline[s]->CycleMax[s]) {
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
                *Pipeline[s + 1] = *Pipeline[s];
            }
            Pipeline[s] = NULL;
            NextStageIsFree = true;
        } else {
            NextStageIsFree = !Pipeline[s];
        }
        Tick();
    }
}

void Processor::Queue(GarandInstruction Inst) {
    InstructionWk Wk;
    Wk.Instruction = Inst;
    std::array<unsigned, 4> CycleTime{ 1, 2, 3, 4 };
    std::transform(CycleTime.begin(), CycleTime.end(), Wk.CycleMax, [](unsigned x) {return x;});
    Wk.CycleCount = 0;
    InstructionQueue.push(Wk);
}

void Processor::Fetch() {
    Pipeline[Stage::FETCH] = std::make_unique<InstructionWk>(InstructionQueue.front());
    InstructionQueue.pop();
}

void Processor::Decode() {
    if (Pipeline[Stage::DECODE]) {
        Pipeline[Stage::DECODE]->DecodedInstruction =
            Instruction::Decode(Pipeline[Stage::DECODE]->Instruction);
    }
}

void Processor::Execute() {
    if (Pipeline[Stage::EXECUTE]) {
        Pipeline[Stage::DECODE]->WriteBack = Instruction::Execute(Pipeline[Stage::DECODE]->DecodedInstruction, Pipeline[Stage::DECODE]->Instruction, WkRAM, (uint64_t*)&WkRegs);
    }
}

void Processor::WriteBack() {
    if (Pipeline[Stage::WRITE_BACK]) {
        Garand::Instruction::WriteBack(Pipeline[Stage::WRITE_BACK]->WriteBack);
    }
}
} // namespace Garand