#include <iostream>
#include "Instruction.hpp"

Garand::Instruction Garand::Instruction::Encode() {
    return Garand::Instruction();
}

Garand::DecodedInstruction Garand::Instruction::Decode(Garand::GarandInstruction instruction) {
    using Garand::DecodedInstruction;

    switch (instruction.Operation)
    {
        // Memory OPs
        case 0x0:
            switch (instruction.ConditionFlags)
            {
                // MREAD
                case 0x0:
                    return MREAD;
                // MWRITE
                case 0x1:
                    return MWRITE;
                // BIND
                case 0x2:
                    return BIND;
                // UNBIND
                case 0x3:
                    return UNBIND;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // BRUH.CC
        case 0x2:
            switch (instruction.ConditionFlags)
            {
                // AL
                case 0x0:
                    return BRUHCC_AL;
                // EQ
                case 0x1:
                    return BRUHCC_EQ;
                // NE
                case 0x2:
                    return BRUHCC_NE;
                // LO
                case 0x3:
                    return BRUHCC_LO;
                // AL
                case 0x4:
                    return BRUHCC_HS;
                // EQ
                case 0x5:
                    return BRUHCC_LT;
                // NE
                case 0x6:
                    return BRUHCC_GE;
                // LO
                case 0x7:
                    return BRUHCC_HI;
                // AL
                case 0x8:
                    return BRUHCC_LS;
                // EQ
                case 0x9:
                    return BRUHCC_GT;
                // NE
                case 0xA:
                    return BRUHCC_LE;
                // LO
                case 0xB:
                    return BRUHCC_VC;
                // AL
                case 0xC:
                    return BRUHCC_VS;
                // EQ
                case 0xD:
                    return BRUHCC_PL;
                // NE
                case 0xE:
                    return BRUHCC_NG;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // B.CC
        case 0x3:
            switch (instruction.ConditionFlags)
            {
                // AL
                case 0x0:
                    return BCC_AL;
                // EQ
                case 0x1:
                    return BCC_EQ;
                // NE
                case 0x2:
                    return BCC_NE;
                // LO
                case 0x3:
                    return BCC_LO;
                // AL
                case 0x4:
                    return BCC_HS;
                // EQ
                case 0x5:
                    return BCC_LT;
                // NE
                case 0x6:
                    return BCC_GE;
                // LO
                case 0x7:
                    return BCC_HI;
                // AL
                case 0x8:
                    return BCC_LS;
                // EQ
                case 0x9:
                    return BCC_GT;
                // NE
                case 0xA:
                    return BCC_LE;
                // LO
                case 0xB:
                    return BCC_VC;
                // AL
                case 0xC:
                    return BCC_VS;
                // EQ
                case 0xD:
                    return BCC_PL;
                // NE
                case 0xE:
                    return BCC_NG;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // ADDs
        case 0x4:
            switch (instruction.ConditionFlags)
            {
                // ADD
                case 0x0:
                    return ADD;
                // ADDI
                case 0x1:
                    return ADDI;
                // FX_ADD
                case 0x8:
                    return FX_ADD;
                // FX_ADDI
                case 0x9:
                    return FX_ADDI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // SUBs
        case 0x5:
            switch (instruction.ConditionFlags)
            {
                // SUB
                case 0x0:
                    return SUB;
                // SUBI
                case 0x1:
                    return SUBI;
                // CMP
                case 0x2:
                    return CMP;
                // CMPI
                case 0x3:
                    return CMPI;
                // FX_SUB
                case 0x8:
                    return FX_SUB;
                // FX_SUBI
                case 0x9:
                    return FX_SUBI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // MULs
        case 0x6:
            switch (instruction.ConditionFlags)
            {
                // MUL
                case 0x0:
                    return MUL;
                // MULI
                case 0x1:
                    return MULI;
                // MADD
                case 0x4:
                    return MADD;
                // FX_MUL
                case 0x8:
                    return FX_MUL;
                // FX_MULI
                case 0x9:
                    return FX_MULI;
                // FX_MADD
                case 0xC:
                    return FX_MADD;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // DIVs
        case 0x7:
            switch (instruction.ConditionFlags)
            {
                // DIV
                case 0x0:
                    return DIV;
                // DIVI
                case 0x1:
                    return DIVI;
                // FX_DIV
                case 0x8:
                    return FX_DIV;
                // FX_DIVI
                case 0x9:
                    return FX_DIVI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // ANDs
        case 0x8:
            switch (instruction.ConditionFlags)
            {
                // AND
                case 0x0:
                    return AND;
                // ANDI
                case 0x1:
                    return ANDI;
                // TEST
                case 0x2:
                    return TEST;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // NANDs
        case 0x9:
            switch (instruction.ConditionFlags)
            {
                // NAND
                case 0x0:
                    return NAND;
                // NANDI
                case 0x1:
                    return NANDI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // ORs
        case 0xA:
            switch (instruction.ConditionFlags)
            {
                // OR
                case 0x0:
                    return OR;
                // ORI
                case 0x1:
                    return ORI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // XORs
        case 0xB:
            switch (instruction.ConditionFlags)
            {
                // XOR
                case 0x0:
                    return XOR;
                // XORI
                case 0x1:
                    return XORI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // Shifts
        case 0xC:
            switch (instruction.ConditionFlags)
            {
                // LSL
                case 0x0:
                    return LSL;
                // LSLI
                case 0x1:
                    return LSLI;
                // LSR
                case 0x2:
                    return LSR;
                // LSRI
                case 0x3:
                    return LSRI;
                // RSR
                case 0x4:
                    return RSR;
                // RSRI
                case 0x5:
                    return RSRI;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        // NOT
        case 0xF:
            switch (instruction.ConditionFlags)
            {
                // NOT
                case 0x0:
                    return NOT;
                default:
                    return UNKNOWN;
            }
            return UNKNOWN;
        default:
            return UNKNOWN;
    }
}

Garand::InstructionWriteBack Garand::Instruction::Execute(Garand::DecodedInstruction instr) {
    Garand::InstructionWriteBack wb;
    switch (instr)
    {

    }
    return wb;
}