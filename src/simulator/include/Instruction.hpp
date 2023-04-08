#include "Registers.hpp"
#include "Memory.hpp"
#include "Instructions.hpp"

#ifndef GARAND_INSTRUCTION_HPP
#define GARAND_INSTRUCTION_HPP

namespace Garand {
    class Instruction {
        public:
            static Instruction Encode();
            Garand::DecodedInstruction Decode(Garand::GarandInstruction instr);
            Garand::InstructionWriteBack Execute(Garand::DecodedInstruction decoded_instr, Garand::GarandInstruction instr, Garand::Memory mem, uint64_t* regs);
            void WriteBack(Garand::InstructionWriteBack write_back);
    };
} // namespace Garand

#endif