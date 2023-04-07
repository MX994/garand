#include "Registers.hpp"
#include "Memory.hpp"
#include "Instructions.hpp"

#ifndef GARAND_INSTRUCTION_HPP
#define GARAND_INSTRUCTION_HPP

namespace Garand {
    struct GarandInstruction {
        uint8_t ConditionFlags : 4;
        uint8_t Operation : 6;
        uint64_t InstructionSpecific: 60;
    };

    class Instruction {
        public:
            static Instruction Encode();
            DecodedInstruction Decode(GarandInstruction instr);
            InstructionWriteBack Execute(DecodedInstruction instr);
    };

    struct InstructionWriteBack {
        uint64_t value;
        uint64_t location;
    };

    enum DecodedInstruction {
        MREAD,
        MWRITE,
        BIND,
        UNBIND,
        BRUHCC_AL,
        BRUHCC_EQ,
        BRUHCC_NE,
        BRUHCC_LO,
        BRUHCC_HS,
        BRUHCC_LT,
        BRUHCC_GE,
        BRUHCC_HI,
        BRUHCC_LS,
        BRUHCC_GT,
        BRUHCC_LE,
        BRUHCC_VC,
        BRUHCC_VS,
        BRUHCC_PL,
        BRUHCC_NG,
        BCC_AL,
        BCC_EQ,
        BCC_NE,
        BCC_LO,
        BCC_HS,
        BCC_LT,
        BCC_GE,
        BCC_HI,
        BCC_LS,
        BCC_GT,
        BCC_LE,
        BCC_VC,
        BCC_VS,
        BCC_PL,
        BCC_NG,
        ADD,
        ADDI,
        FX_ADD,
        FX_ADDI,
        SUB,
        SUBI,
        CMP,
        CMPI,
        FX_SUB,
        FX_SUBI,
        MUL,
        MULI,
        MADD,
        FX_MUL,
        FX_MULI,
        FX_MADD,
        DIV,
        DIVI,
        FX_DIV,
        FX_DIVI,
        AND,
        ANDI,
        TEST,
        NAND,
        NANDI,
        OR,
        ORI,
        XOR,
        XORI,
        LSL,
        LSLI,
        LSR,
        LSRI,
        RSR,
        RSRI,
        NOT,
        UNKNOWN,
    };
} // namespace Garand

#endif