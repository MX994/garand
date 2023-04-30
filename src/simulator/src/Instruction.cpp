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
                // NG
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
                // NG
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

Garand::InstructionWriteBack Garand::Instruction::Execute(Garand::DecodedInstruction decoded_instr, Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    switch (decoded_instr)
    {
        case MREAD:
            return Garand::InstructionSet::MemoryRead(instr, mem, regs);
        case MWRITE:
            return Garand::InstructionSet::MemoryWrite(instr, mem, regs);
        case BIND:
            return Garand::InstructionSet::Bind(instr, mem, regs);
        case UNBIND:
            return Garand::InstructionSet::Unbind(instr, mem, regs);
        case BRUHCC_AL:
            return Garand::InstructionSet::BRUHCC_AL(instr, mem, regs);
        case BRUHCC_EQ:
            return Garand::InstructionSet::BRUHCC_EQ(instr, mem, regs);
        case BRUHCC_NE:
            return Garand::InstructionSet::BRUHCC_NE(instr, mem, regs);
        case BRUHCC_LO:
            return Garand::InstructionSet::BRUHCC_LO(instr, mem, regs);
        case BRUHCC_HS:
            return Garand::InstructionSet::BRUHCC_HS(instr, mem, regs);
        case BRUHCC_LT:
            return Garand::InstructionSet::BRUHCC_LT(instr, mem, regs);
        case BRUHCC_GE:
            return Garand::InstructionSet::BRUHCC_GE(instr, mem, regs);
        case BRUHCC_HI:
            return Garand::InstructionSet::BRUHCC_HI(instr, mem, regs);
        case BRUHCC_LS:
            return Garand::InstructionSet::BRUHCC_LS(instr, mem, regs);
        case BRUHCC_GT:
            return Garand::InstructionSet::BRUHCC_GT(instr, mem, regs);
        case BRUHCC_LE:
            return Garand::InstructionSet::BRUHCC_LE(instr, mem, regs);
        case BRUHCC_VC:
            return Garand::InstructionSet::BRUHCC_VC(instr, mem, regs);
        case BRUHCC_VS:
            return Garand::InstructionSet::BRUHCC_VS(instr, mem, regs);
        case BRUHCC_PL:
            return Garand::InstructionSet::BRUHCC_PL(instr, mem, regs);
        case BRUHCC_NG:
            return Garand::InstructionSet::BRUHCC_NG(instr, mem, regs);
        case BCC_AL:
            return Garand::InstructionSet::BCC_AL(instr, mem, regs);
        case BCC_EQ:
            return Garand::InstructionSet::BCC_EQ(instr, mem, regs);
        case BCC_NE:
            return Garand::InstructionSet::BCC_NE(instr, mem, regs);
        case BCC_LO:
            return Garand::InstructionSet::BCC_LO(instr, mem, regs);
        case BCC_HS:
            return Garand::InstructionSet::BCC_HS(instr, mem, regs);
        case BCC_LT:
            return Garand::InstructionSet::BCC_LT(instr, mem, regs);
        case BCC_GE:
            return Garand::InstructionSet::BCC_GE(instr, mem, regs);
        case BCC_HI:
            return Garand::InstructionSet::BCC_HI(instr, mem, regs);
        case BCC_LS:
            return Garand::InstructionSet::BCC_LS(instr, mem, regs);
        case BCC_GT:
            return Garand::InstructionSet::BCC_GT(instr, mem, regs);
        case BCC_LE:
            return Garand::InstructionSet::BCC_LE(instr, mem, regs);
        case BCC_VC:
            return Garand::InstructionSet::BCC_VC(instr, mem, regs);
        case BCC_VS:
            return Garand::InstructionSet::BCC_VS(instr, mem, regs);
        case BCC_PL:
            return Garand::InstructionSet::BCC_PL(instr, mem, regs);
        case BCC_NG:
            return Garand::InstructionSet::BCC_NG(instr, mem, regs);
        case ADD:
            return Garand::InstructionSet::Add(instr, mem, regs);
        case ADDI:
            return Garand::InstructionSet::AddImmediate(instr, mem, regs);
        case FX_ADD:
            return Garand::InstructionSet::FX_Add(instr, mem, regs);
        case FX_ADDI:
            return Garand::InstructionSet::FX_AddImmediate(instr, mem, regs);
        case SUB:
            return Garand::InstructionSet::Subtract(instr, mem, regs);
        case SUBI:
            return Garand::InstructionSet::SubtractImmediate(instr, mem, regs);
        case CMP:
            return Garand::InstructionSet::Compare(instr, mem, regs);
        case CMPI:
            return Garand::InstructionSet::CompareImmediate(instr, mem, regs);
        case FX_SUB:
            return Garand::InstructionSet::FX_Subtract(instr, mem, regs);
        case FX_SUBI:
            return Garand::InstructionSet::FX_SubtractImmediate(instr, mem, regs);
        case MUL:
            return Garand::InstructionSet::Multiply(instr, mem, regs);
        case MULI:
            return Garand::InstructionSet::MultiplyImmediate(instr, mem, regs);
        case MADD:
            return Garand::InstructionSet::MultiplyAdd(instr, mem, regs);
        case FX_MUL:
            return Garand::InstructionSet::FX_Multiply(instr, mem, regs);
        case FX_MULI:
            return Garand::InstructionSet::FX_MultiplyImmediate(instr, mem, regs);
        case FX_MADD:
            return Garand::InstructionSet::FX_MultiplyAdd(instr, mem, regs);
        case DIV:
            return Garand::InstructionSet::Divide(instr, mem, regs);
        case DIVI:
            return Garand::InstructionSet::DivideImmediate(instr, mem, regs);
        case FX_DIV:
            return Garand::InstructionSet::FX_Divide(instr, mem, regs);
        case FX_DIVI:
            return Garand::InstructionSet::FX_DivideImmediate(instr, mem, regs);
        case AND:
            return Garand::InstructionSet::AND(instr, mem, regs);
        case ANDI:
            return Garand::InstructionSet::ANDImmediate(instr, mem, regs);
        case TEST:
            return Garand::InstructionSet::Test(instr, mem, regs);
        case NAND:
            return Garand::InstructionSet::NAND(instr, mem, regs);
        case NANDI:
            return Garand::InstructionSet::NANDImmediate(instr, mem, regs);
        case OR:
            return Garand::InstructionSet::OR(instr, mem, regs);
        case ORI:
            return Garand::InstructionSet::ORImmediate(instr, mem, regs);
        case XOR:
            return Garand::InstructionSet::XOR(instr, mem, regs);
        case XORI:
            return Garand::InstructionSet::XORImmediate(instr, mem, regs);
        case LSL:
            return Garand::InstructionSet::LogicalShiftLeft(instr, mem, regs);
        case LSLI:
            return Garand::InstructionSet::LogicalShiftLeftImmediate(instr, mem, regs);
        case LSR:
            return Garand::InstructionSet::LogicalShiftRight(instr, mem, regs);
        case LSRI:
            return Garand::InstructionSet::LogicalShiftRightImmediate(instr, mem, regs);
        case RSR:
            return Garand::InstructionSet::RotationalShiftRight(instr, mem, regs);
        case RSRI:
            return Garand::InstructionSet::RotationalShiftRightImmediate(instr, mem, regs);
        case NOT:
            return Garand::InstructionSet::NOT(instr, mem, regs);
        default:
            Garand:InstructionWriteBack wb;
            return wb;
    }
}

void Garand::Instruction::WriteBack(Garand::InstructionWriteBack write_back, Garand::Memory &mem) {
    if (write_back.write_back) {
        if (write_back.is_reg)
            *(write_back.reg) = write_back.value;
        else {
            std::cout << "MWRITE"  << std::endl;
            mem.store((uint32_t) *write_back.reg, write_back.value);
        }
    }
}
