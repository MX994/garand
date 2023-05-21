#include <cstdint>
#include <optional>


#ifndef GARAND_INSTRUCTIONS_HPP
#define GARAND_INSTRUCTIONS_HPP
#include "Registers.hpp"
#include "Memory.hpp"

namespace Garand {
    using InstructionSize = uint32_t;

    struct GarandInstruction {
        uint8_t ConditionFlags : 4;
        uint8_t Operation : 6;
        InstructionSize InstructionSpecific: 22;
    };

    struct InstructionWriteBack {
        // If true, writeback. Else do nothing
        bool write_back = true;
        // If true, writeback to register. Else writeback to memory
        bool is_reg = true;
        uint64_t value;
        LoadSize* reg;
        uint64_t execute_cost = 1;
    };

    class InstructionSet {
        public:
            static InstructionWriteBack MemoryRead(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack MemoryWrite(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Bind(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Unbind(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_AL(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_EQ(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_NE(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_LO(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_HS(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_LT(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_GE(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_HI(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_LS(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_GT(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_LE(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_VC(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_VS(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_PL(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BRUHCC_NG(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_AL(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_EQ(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_NE(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_LO(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_HS(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_LT(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_GE(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_HI(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_LS(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_GT(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_LE(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_VC(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_VS(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_PL(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack BCC_NG(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Add(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack AddImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_Add(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_AddImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Subtract(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack SubtractImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Compare(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack CompareImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_Subtract(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_SubtractImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Multiply(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack MultiplyImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack MultiplyAdd(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_Multiply(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_MultiplyImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_MultiplyAdd(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Divide(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack DivideImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_Divide(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack FX_DivideImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack AND(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack ANDImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack Test(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack NAND(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack NANDImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack OR(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack ORImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack XOR(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack XORImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack LogicalShiftLeft(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack LogicalShiftLeftImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack LogicalShiftRight(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack LogicalShiftRightImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack RotationalShiftRight(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack RotationalShiftRightImmediate(GarandInstruction instr, Memory &mem, Registers *regs);
            static InstructionWriteBack NOT(GarandInstruction instr, Memory &mem, Registers *regs);
    };

    enum DecodedMnemonic {
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

    // Split it into a new class for easy refactor
    struct DecodedParameter {
        std::vector<uint8_t> Registers;
        std::optional<LoadSize> Immediate;
    };

    struct DecodedInstruction {
        DecodedMnemonic mnemonic;
        DecodedParameter parameter;
    };

    char const* get_ins_mnemonic(GarandInstruction);
}

#endif