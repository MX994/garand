#include <cstdint>

#ifndef GARAND_INSTRUCTIONS_HPP
#define GARAND_INSTRUCTIONS_HPP

namespace Garand {
    struct InstructionWriteBack {
        uint64_t value;
        uint64_t location;
    };

    class InstructionSet {
        public:
            static Garand::InstructionWriteBack MemoryRead();
            static Garand::InstructionWriteBack MemoryWrite();
            static Garand::InstructionWriteBack Bind();
            static Garand::InstructionWriteBack Unbind();
            static Garand::InstructionWriteBack BRUHCC_AL();
            static Garand::InstructionWriteBack BRUHCC_EQ();
            static Garand::InstructionWriteBack BRUHCC_NE();
            static Garand::InstructionWriteBack BRUHCC_LO();
            static Garand::InstructionWriteBack BRUHCC_HS();
            static Garand::InstructionWriteBack BRUHCC_LT();
            static Garand::InstructionWriteBack BRUHCC_GE();
            static Garand::InstructionWriteBack BRUHCC_HI();
            static Garand::InstructionWriteBack BRUHCC_LS();
            static Garand::InstructionWriteBack BRUHCC_GT();
            static Garand::InstructionWriteBack BRUHCC_LE();
            static Garand::InstructionWriteBack BRUHCC_VC();
            static Garand::InstructionWriteBack BRUHCC_VS();
            static Garand::InstructionWriteBack BRUHCC_PL();
            static Garand::InstructionWriteBack BRUHCC_NG();
            static Garand::InstructionWriteBack BCC_AL();
            static Garand::InstructionWriteBack BCC_EQ();
            static Garand::InstructionWriteBack BCC_NE();
            static Garand::InstructionWriteBack BCC_LO();
            static Garand::InstructionWriteBack BCC_HS();
            static Garand::InstructionWriteBack BCC_LT();
            static Garand::InstructionWriteBack BCC_GE();
            static Garand::InstructionWriteBack BCC_HI();
            static Garand::InstructionWriteBack BCC_LS();
            static Garand::InstructionWriteBack BCC_GT();
            static Garand::InstructionWriteBack BCC_LE();
            static Garand::InstructionWriteBack BCC_VC();
            static Garand::InstructionWriteBack BCC_VS();
            static Garand::InstructionWriteBack BCC_PL();
            static Garand::InstructionWriteBack BCC_NG();
            static Garand::InstructionWriteBack Add();
            static Garand::InstructionWriteBack AddImmediate();
            static Garand::InstructionWriteBack FX_Add();
            static Garand::InstructionWriteBack FX_AddImmediate();
            static Garand::InstructionWriteBack Subtract();
            static Garand::InstructionWriteBack SubtractImmediate();
            static Garand::InstructionWriteBack Compare();
            static Garand::InstructionWriteBack CompareImmediate();
            static Garand::InstructionWriteBack FX_Subtract();
            static Garand::InstructionWriteBack FX_SubtractImmediate();
            static Garand::InstructionWriteBack Multiply();
            static Garand::InstructionWriteBack MultiplyImmediate();
            static Garand::InstructionWriteBack MultiplyAdd();
            static Garand::InstructionWriteBack FX_Multiply();
            static Garand::InstructionWriteBack FX_MultiplyImmediate();
            static Garand::InstructionWriteBack FX_MultiplyAdd();
            static Garand::InstructionWriteBack Divide();
            static Garand::InstructionWriteBack DivideImmediate();
            static Garand::InstructionWriteBack FX_Divide();
            static Garand::InstructionWriteBack FX_DivideImmediate();
            static Garand::InstructionWriteBack AND();
            static Garand::InstructionWriteBack ANDImmediate();
            static Garand::InstructionWriteBack Test();
            static Garand::InstructionWriteBack NAND();
            static Garand::InstructionWriteBack NANDImmediate();
            static Garand::InstructionWriteBack OR();
            static Garand::InstructionWriteBack ORImmediate();
            static Garand::InstructionWriteBack XOR();
            static Garand::InstructionWriteBack XORImmediate();
            static Garand::InstructionWriteBack LogicalShiftLeft();
            static Garand::InstructionWriteBack LogicalShiftLeftImmediate();
            static Garand::InstructionWriteBack LogicalShiftRight();
            static Garand::InstructionWriteBack LogicalShiftRightImmediate();
            static Garand::InstructionWriteBack RotationalShiftRight();
            static Garand::InstructionWriteBack RotationalShiftRightImmediate();
            static Garand::InstructionWriteBack NOT();
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
}

#endif