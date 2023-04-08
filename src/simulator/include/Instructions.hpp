#include <cstdint>

#include "Registers.hpp"
#include "Memory.hpp"

#ifndef GARAND_INSTRUCTIONS_HPP
#define GARAND_INSTRUCTIONS_HPP

namespace Garand {
    struct GarandInstruction {
        uint8_t ConditionFlags : 4;
        uint8_t Operation : 6;
        uint64_t InstructionSpecific: 22;
    };

    struct InstructionWriteBack {
        uint64_t value;
        uint64_t* reg;
    };

    class InstructionSet {
        public:
            static Garand::InstructionWriteBack MemoryRead(Garand::GarandInstruction instr, Garand::Memory mem, uint64_t* regs);
            static Garand::InstructionWriteBack MemoryWrite(Garand::GarandInstruction instr, Garand::Memory mem, uint64_t* regs);
            static Garand::InstructionWriteBack Bind(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Unbind(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_AL(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_EQ(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_NE(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_LO(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_HS(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_LT(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_GE(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_HI(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_LS(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_GT(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_LE(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_VC(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_VS(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_PL(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BRUHCC_NG(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_AL(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_EQ(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_NE(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_LO(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_HS(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_LT(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_GE(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_HI(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_LS(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_GT(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_LE(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_VC(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_VS(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_PL(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack BCC_NG(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Add(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack AddImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_Add(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_AddImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Subtract(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Compare(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack CompareImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_Subtract(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Multiply(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_Multiply(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Divide(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack DivideImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_Divide(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack FX_DivideImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack AND(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack ANDImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack Test(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack NAND(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack NANDImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack OR(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack ORImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack XOR(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack XORImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack LogicalShiftLeft(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack LogicalShiftLeftImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack LogicalShiftRight(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack LogicalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack RotationalShiftRight(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack RotationalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory mem);
            static Garand::InstructionWriteBack NOT(Garand::GarandInstruction instr, Garand::Memory mem);
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