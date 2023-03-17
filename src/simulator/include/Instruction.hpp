#include "Registers.hpp"

#ifndef GARAND_INSTRUCTION_HPP
#define GARAND_INSTRUCTION_HPP

namespace Garand {
    struct GarandInstruction {
        uint8_t ConditionFlags : 4;
        uint8_t Operation : 6;
        uint64_t InstructionSpecific: 60;
    }
    class Instruction {
        public:
            static Instruction Encode();
    }
} // namespace Garand

#endif