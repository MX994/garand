#include "Registers.hpp"
#include "Memory.hpp"
#include "Instructions.hpp"

#ifndef GARAND_INSTRUCTION_HPP
#define GARAND_INSTRUCTION_HPP

namespace Garand {
    class Instruction {
        public:
            static Garand::GarandInstruction Encode(uint32_t);
            static Garand::DecodedInstruction Decode(Garand::GarandInstruction instr);
            static Garand::InstructionWriteBack Execute(Garand::DecodedInstruction decoded_instr, Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs);
            static void WriteBack(Garand::InstructionWriteBack write_back, Garand::Memory &mem);
    };
} // namespace Garand

#endif