#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Processor.hpp"

namespace Garand {
void Processor::Tick() { Clock += 1; }

void Processor::Step() {
    bool NextStageIsFree = true;
    for (int Stage = Stage.WRITE_BACK; Stage > 0; --Stage) {
        ++Pipeline[Stage].CycleCount;
        if (Pipeline[Stage].CycleCount >= Pipeline[Stage].CycleMax[Stage]) {
            if (Stage == Stage.FETCH) {
                Fetch();
            } else if (Stage == Stage.DECODE) {
                Decode();
            } else if (Stage == Stage.EXECUTE) {
                Execute();
            } else if (Stage == Stage.WRITE_BACK) {
                WriteBack();
            }
        }
        if (Pipeline[Stage].CycleCount >= Pipeline[Stage].CycleMax[Stage] &&
            NextStageIsFree) {
            Pipeline[Stage].CycleCount = 0;
            if (Stage != Stage.WRITE_BACK) {
                Pipeline[Stage + 1] = Pipeline[Stage];
            }
            Pipeline[Stage] = NULL;
            NextStageIsFree = true;
        } else {
            NextStageIsFree = !Pipeline[Stage];
        }
        Tick();
    }
}

void Processor::Queue(GarandInstruction Inst) {
    InstructionWk *Wk = (InstructionWk *)calloc(sizeof(InstructionWk), 0);
    Wk->Instruction = Inst;
    Wk->CycleMax = [ 1, 2, 3, 4 ];
    Pipeline[0] = Wk;
}

void Processor::Fetch(GarandInstruction Inst) {
    if (Pipeline[Stage.FETCH]) {
    }
}

void Processor::Decode() {
    if (Pipeline[Stage.DECODE]) {
        Pipeline[Stage.DECODE]->DecodedInstruction =
            Instruction::Decode(Pipeline[Stage.DECODE]->Instruction);
    }
}

void Processor::Execute() {
    if (Pipeline[Stage.EXECUTE]) {
        Garand::Instruction::Execute(Pipeline[Stage.DECODE]->DecodedInstruction, Pipeline[Stage.DECODE]->Instruction, &WkRAM, &WkRegs);
    } else {
    }
}

void Processor::WriteBack() {
    if (Pipeline[Stage.WRITE_BACK]) {
        // Something exists here; process it.
        // TODO: Do fetch.
    } else {
    }
}
} // namespace Garand