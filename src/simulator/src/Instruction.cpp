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

Garand::InstructionWriteBack Garand::Instruction::Execute(Garand::DecodedInstruction decoded_instr, Garand::GarandInstruction instr, Garand::Memory mem) {
    switch (decoded_instr)
    {
        case MREAD:
            return Garand::InstructionSet::MemoryRead(instr, mem);
        case MWRITE:
            return Garand::InstructionSet::MemoryWrite(instr, mem);
        case BIND:
            return Garand::InstructionSet::Bind(instr, mem);
        case UNBIND:
            return Garand::InstructionSet::Unbind(instr, mem);
        case BRUHCC_AL:
            return Garand::InstructionSet::BRUHCC_AL(instr, mem);
        case BRUHCC_EQ:
            return Garand::InstructionSet::BRUHCC_EQ(instr, mem);
        case BRUHCC_NE:
            return Garand::InstructionSet::BRUHCC_NE(instr, mem);
        case BRUHCC_LO:
            return Garand::InstructionSet::BRUHCC_LO(instr, mem);
        case BRUHCC_HS:
            return Garand::InstructionSet::BRUHCC_HS(instr, mem);
        case BRUHCC_LT:
            return Garand::InstructionSet::BRUHCC_LT(instr, mem);
        case BRUHCC_GE:
            return Garand::InstructionSet::BRUHCC_GE(instr, mem);
        case BRUHCC_HI:
            return Garand::InstructionSet::BRUHCC_HI(instr, mem);
        case BRUHCC_LS:
            return Garand::InstructionSet::BRUHCC_LS(instr, mem);
        case BRUHCC_GT:
            return Garand::InstructionSet::BRUHCC_GT(instr, mem);
        case BRUHCC_LE:
            return Garand::InstructionSet::BRUHCC_LE(instr, mem);
        case BRUHCC_VC:
            return Garand::InstructionSet::BRUHCC_VC(instr, mem);
        case BRUHCC_VS:
            return Garand::InstructionSet::BRUHCC_VS(instr, mem);
        case BRUHCC_PL:
            return Garand::InstructionSet::BRUHCC_PL(instr, mem);
        case BRUHCC_NG:
            return Garand::InstructionSet::BRUHCC_NG(instr, mem);
        case BCC_AL:
            return Garand::InstructionSet::BCC_AL(instr, mem);
        case BCC_EQ:
            return Garand::InstructionSet::BCC_EQ(instr, mem);
        case BCC_NE:
            return Garand::InstructionSet::BCC_NE(instr, mem);
        case BCC_LO:
            return Garand::InstructionSet::BCC_LO(instr, mem);
        case BCC_HS:
            return Garand::InstructionSet::BCC_HS(instr, mem);
        case BCC_LT:
            return Garand::InstructionSet::BCC_LT(instr, mem);
        case BCC_GE:
            return Garand::InstructionSet::BCC_GE(instr, mem);
        case BCC_HI:
            return Garand::InstructionSet::BCC_HI(instr, mem);
        case BCC_LS:
            return Garand::InstructionSet::BCC_LS(instr, mem);
        case BCC_GT:
            return Garand::InstructionSet::BCC_GT(instr, mem);
        case BCC_LE:
            return Garand::InstructionSet::BCC_LE(instr, mem);
        case BCC_VC:
            return Garand::InstructionSet::BCC_VC(instr, mem);
        case BCC_VS:
            return Garand::InstructionSet::BCC_VS(instr, mem);
        case BCC_PL:
            return Garand::InstructionSet::BCC_PL(instr, mem);
        case BCC_NG:
            return Garand::InstructionSet::BCC_NG(instr, mem);
        case ADD:
            return Garand::InstructionSet::Add(instr, mem);
        case ADDI:
            return Garand::InstructionSet::AddImmediate(instr, mem);
        case FX_ADD:
            return Garand::InstructionSet::FX_Add(instr, mem);
        case FX_ADDI:
            return Garand::InstructionSet::FX_AddImmediate(instr, mem);
        case SUB:
            return Garand::InstructionSet::Subtract(instr, mem);
        case SUBI:
            return Garand::InstructionSet::SubtractImmediate(instr, mem);
        case CMP:
            return Garand::InstructionSet::Compare(instr, mem);
        case CMPI:
            return Garand::InstructionSet::CompareImmediate(instr, mem);
        case FX_SUB:
            return Garand::InstructionSet::FX_Subtract(instr, mem);
        case FX_SUBI:
            return Garand::InstructionSet::FX_SubtractImmediate(instr, mem);
        case MUL:
            return Garand::InstructionSet::Multiply(instr, mem);
        case MULI:
            return Garand::InstructionSet::MultiplyImmediate(instr, mem);
        case MADD:
            return Garand::InstructionSet::MultiplyAdd(instr, mem);
        case FX_MUL:
            return Garand::InstructionSet::FX_Multiply(instr, mem);
        case FX_MULI:
            return Garand::InstructionSet::FX_MultiplyImmediate(instr, mem);
        case FX_MADD:
            return Garand::InstructionSet::FX_MultiplyAdd(instr, mem);
        case DIV:
            return Garand::InstructionSet::Divide(instr, mem);
        case DIVI:
            return Garand::InstructionSet::DivideImmediate(instr, mem);
        case FX_DIV:
            return Garand::InstructionSet::FX_Divide(instr, mem);
        case FX_DIVI:
            return Garand::InstructionSet::FX_DivideImmediate(instr, mem);
        case AND:
            return Garand::InstructionSet::AND(instr, mem);
        case ANDI:
            return Garand::InstructionSet::ANDImmediate(instr, mem);
        case TEST:
            return Garand::InstructionSet::Test(instr, mem);
        case NAND:
            return Garand::InstructionSet::NAND(instr, mem);
        case NANDI:
            return Garand::InstructionSet::NANDImmediate(instr, mem);
        case OR:
            return Garand::InstructionSet::OR(instr, mem);
        case ORI:
            return Garand::InstructionSet::ORImmediate(instr, mem);
        case XOR:
            return Garand::InstructionSet::XOR(instr, mem);
        case XORI:
            return Garand::InstructionSet::XORImmediate(instr, mem);
        case LSL:
            return Garand::InstructionSet::LogicalShiftLeft(instr, mem);
        case LSLI:
            return Garand::InstructionSet::LogicalShiftLeftImmediate(instr, mem);
        case LSR:
            return Garand::InstructionSet::LogicalShiftRight(instr, mem);
        case LSRI:
            return Garand::InstructionSet::LogicalShiftRightImmediate(instr, mem);
        case RSR:
            return Garand::InstructionSet::RotationalShiftRight(instr, mem);
        case RSRI:
            return Garand::InstructionSet::RotationalShiftRightImmediate(instr, mem);
        case NOT:
            return Garand::InstructionSet::NOT(instr, mem);
        default:
            Garand:InstructionWriteBack wb;
            return wb;
    }
}

void Garand::Instruction::WriteBack(Garand::InstructionWriteBack write_back) {
    // TODO: Implement WriteBack
}
