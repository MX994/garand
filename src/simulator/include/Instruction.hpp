#include "Registers.hpp"
#include "Memory.hpp"
#include "Instructions.hpp"

#ifndef GARAND_INSTRUCTION_HPP
#define GARAND_INSTRUCTION_HPP

namespace Garand {

    class Instruction {
        public:
            static GarandInstruction Encode(uint32_t);
            static DecodedInstruction Decode(GarandInstruction);
            static InstructionWriteBack Execute(DecodedInstruction decoded_instr, GarandInstruction instr, Memory &mem, Registers *regs);
            static uint64_t WriteBack(InstructionWriteBack write_back, Memory &mem);
            static DecodedParameter DecodeParameter(GarandInstruction, DecodedMnemonic);
            static DecodedMnemonic DecodeMnemonic(GarandInstruction);
    };
} // namespace Garand

#endif