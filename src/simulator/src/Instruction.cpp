#include "Instruction.hpp"
#include "Disassemble.hpp"
#include <iostream>

namespace Garand {
GarandInstruction Instruction::Encode(uint32_t raw) {
    return GarandInstruction{.ConditionFlags = (uint8_t)((raw >> 28) & 0xf),
                             .Operation = (uint8_t)((raw >> 22) & 0x3f),
                             .InstructionSpecific = raw & 0x3fffff};
}

DecodedMnemonic Instruction::DecodeMnemonic(GarandInstruction instruction) {
    using Garand::DecodedMnemonic;

    switch (instruction.Operation) {
    // Memory OPs
    case 0x0:
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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
        switch (instruction.ConditionFlags) {
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

InstructionWriteBack Instruction::Execute(DecodedInstruction decoded_instr,
                                          GarandInstruction instr, Memory &mem,
                                          Registers *regs) {
    switch (decoded_instr.mnemonic) {
    case MREAD:
        return InstructionSet::MemoryRead(instr, mem, regs);
    case MWRITE:
        return InstructionSet::MemoryWrite(instr, mem, regs);
    case BIND:
        return InstructionSet::Bind(instr, mem, regs);
    case UNBIND:
        return InstructionSet::Unbind(instr, mem, regs);
    case BRUHCC_AL:
        return InstructionSet::BRUHCC_AL(instr, mem, regs);
    case BRUHCC_EQ:
        return InstructionSet::BRUHCC_EQ(instr, mem, regs);
    case BRUHCC_NE:
        return InstructionSet::BRUHCC_NE(instr, mem, regs);
    case BRUHCC_LO:
        return InstructionSet::BRUHCC_LO(instr, mem, regs);
    case BRUHCC_HS:
        return InstructionSet::BRUHCC_HS(instr, mem, regs);
    case BRUHCC_LT:
        return InstructionSet::BRUHCC_LT(instr, mem, regs);
    case BRUHCC_GE:
        return InstructionSet::BRUHCC_GE(instr, mem, regs);
    case BRUHCC_HI:
        return InstructionSet::BRUHCC_HI(instr, mem, regs);
    case BRUHCC_LS:
        return InstructionSet::BRUHCC_LS(instr, mem, regs);
    case BRUHCC_GT:
        return InstructionSet::BRUHCC_GT(instr, mem, regs);
    case BRUHCC_LE:
        return InstructionSet::BRUHCC_LE(instr, mem, regs);
    case BRUHCC_VC:
        return InstructionSet::BRUHCC_VC(instr, mem, regs);
    case BRUHCC_VS:
        return InstructionSet::BRUHCC_VS(instr, mem, regs);
    case BRUHCC_PL:
        return InstructionSet::BRUHCC_PL(instr, mem, regs);
    case BRUHCC_NG:
        return InstructionSet::BRUHCC_NG(instr, mem, regs);
    case BCC_AL:
        return InstructionSet::BCC_AL(instr, mem, regs);
    case BCC_EQ:
        return InstructionSet::BCC_EQ(instr, mem, regs);
    case BCC_NE:
        return InstructionSet::BCC_NE(instr, mem, regs);
    case BCC_LO:
        return InstructionSet::BCC_LO(instr, mem, regs);
    case BCC_HS:
        return InstructionSet::BCC_HS(instr, mem, regs);
    case BCC_LT:
        return InstructionSet::BCC_LT(instr, mem, regs);
    case BCC_GE:
        return InstructionSet::BCC_GE(instr, mem, regs);
    case BCC_HI:
        return InstructionSet::BCC_HI(instr, mem, regs);
    case BCC_LS:
        return InstructionSet::BCC_LS(instr, mem, regs);
    case BCC_GT:
        return InstructionSet::BCC_GT(instr, mem, regs);
    case BCC_LE:
        return InstructionSet::BCC_LE(instr, mem, regs);
    case BCC_VC:
        return InstructionSet::BCC_VC(instr, mem, regs);
    case BCC_VS:
        return InstructionSet::BCC_VS(instr, mem, regs);
    case BCC_PL:
        return InstructionSet::BCC_PL(instr, mem, regs);
    case BCC_NG:
        return InstructionSet::BCC_NG(instr, mem, regs);
    case ADD:
        return InstructionSet::Add(instr, mem, regs);
    case ADDI:
        return InstructionSet::AddImmediate(instr, mem, regs);
    case FX_ADD:
        return InstructionSet::FX_Add(instr, mem, regs);
    case FX_ADDI:
        return InstructionSet::FX_AddImmediate(instr, mem, regs);
    case SUB:
        return InstructionSet::Subtract(instr, mem, regs);
    case SUBI:
        return InstructionSet::SubtractImmediate(instr, mem, regs);
    case CMP:
        return InstructionSet::Compare(instr, mem, regs);
    case CMPI:
        return InstructionSet::CompareImmediate(instr, mem, regs);
    case FX_SUB:
        return InstructionSet::FX_Subtract(instr, mem, regs);
    case FX_SUBI:
        return InstructionSet::FX_SubtractImmediate(instr, mem, regs);
    case MUL:
        return InstructionSet::Multiply(instr, mem, regs);
    case MULI:
        return InstructionSet::MultiplyImmediate(instr, mem, regs);
    case MADD:
        return InstructionSet::MultiplyAdd(instr, mem, regs);
    case FX_MUL:
        return InstructionSet::FX_Multiply(instr, mem, regs);
    case FX_MULI:
        return InstructionSet::FX_MultiplyImmediate(instr, mem, regs);
    case FX_MADD:
        return InstructionSet::FX_MultiplyAdd(instr, mem, regs);
    case DIV:
        return InstructionSet::Divide(instr, mem, regs);
    case DIVI:
        return InstructionSet::DivideImmediate(instr, mem, regs);
    case FX_DIV:
        return InstructionSet::FX_Divide(instr, mem, regs);
    case FX_DIVI:
        return InstructionSet::FX_DivideImmediate(instr, mem, regs);
    case AND:
        return InstructionSet::AND(instr, mem, regs);
    case ANDI:
        return InstructionSet::ANDImmediate(instr, mem, regs);
    case TEST:
        return InstructionSet::Test(instr, mem, regs);
    case NAND:
        return InstructionSet::NAND(instr, mem, regs);
    case NANDI:
        return InstructionSet::NANDImmediate(instr, mem, regs);
    case OR:
        return InstructionSet::OR(instr, mem, regs);
    case ORI:
        return InstructionSet::ORImmediate(instr, mem, regs);
    case XOR:
        return InstructionSet::XOR(instr, mem, regs);
    case XORI:
        return InstructionSet::XORImmediate(instr, mem, regs);
    case LSL:
        return InstructionSet::LogicalShiftLeft(instr, mem, regs);
    case LSLI:
        return InstructionSet::LogicalShiftLeftImmediate(instr, mem, regs);
    case LSR:
        return InstructionSet::LogicalShiftRight(instr, mem, regs);
    case LSRI:
        return InstructionSet::LogicalShiftRightImmediate(instr, mem, regs);
    case RSR:
        return InstructionSet::RotationalShiftRight(instr, mem, regs);
    case RSRI:
        return InstructionSet::RotationalShiftRightImmediate(instr, mem, regs);
    case NOT:
        return InstructionSet::NOT(instr, mem, regs);
    default:
        InstructionWriteBack wb;
        return wb;
    }
}

uint64_t Instruction::WriteBack(InstructionWriteBack write_back, Memory &mem) {
    if (write_back.write_back) {
        if (write_back.is_reg) {
            *(write_back.reg) = write_back.value;
        } else {
            auto Addr = static_cast<AddressSize>(
                reinterpret_cast<uint64_t>(write_back.reg));
            auto cost = mem.GetCacheCycle(Addr);
            mem.store(Addr, write_back.value);
            return cost;
        }
    }
    return 1;
}

DecodedParameter Instruction::DecodeParameter(GarandInstruction instr,
                                              DecodedMnemonic mne) {
    auto raw = instr.InstructionSpecific;
    constexpr auto convert = [](auto &&...args) -> DecodedParameter {
        DecodedParameter param;
        static const auto push = [&param](auto const v) {
            if constexpr (std::is_same<decltype(v), Parameter::Reg>::value) {
                param.Registers.push_back(v.N);
            } else if constexpr (std::is_same<decltype(v), Parameter::Imm>::value) {
                param.Immediate = v.N;
            }
        };
        (push(args), ...);
        return param;
    };
    using namespace Parameter;
    switch (mne) {
    case MREAD:
    case MWRITE:
    case ADD:
    case SUB:
    case MUL:
    case MADD:
    case DIV:
    case AND:
    case NAND:
    case OR:
    case XOR:
    case LSL:
    case LSR:
    case RSR:
        return std::apply(convert, Reg3(raw));
    case ADDI:
    case SUBI:
    case MULI:
    case DIVI:
    case ANDI:
    case NANDI:
    case ORI:
    case XORI:
    case LSLI:
    case LSRI:
    case RSRI:
        return std::apply(convert, Reg2Imm1(raw));
    case BRUHCC_AL:
    case BRUHCC_EQ:
    case BRUHCC_NE:
    case BRUHCC_LO:
    case BRUHCC_HS:
    case BRUHCC_LT:
    case BRUHCC_GE:
    case BRUHCC_HI:
    case BRUHCC_LS:
    case BRUHCC_GT:
    case BRUHCC_LE:
    case BRUHCC_VC:
    case BRUHCC_VS:
    case BRUHCC_PL:
    case BRUHCC_NG:
    case UNBIND:
        return std::apply(convert, Reg1(raw));
    case BCC_AL:
    case BCC_EQ:
    case BCC_NE:
    case BCC_LO:
    case BCC_HS:
    case BCC_LT:
    case BCC_GE:
    case BCC_HI:
    case BCC_LS:
    case BCC_GT:
    case BCC_LE:
    case BCC_VC:
    case BCC_VS:
    case BCC_PL:
    case BCC_NG:
        return std::apply(convert, Add1(raw));
    case NOT:
    case CMP:
    case TEST:
        return std::apply(convert, Reg2(raw));
    case BIND:
    case CMPI:
        return std::apply(convert, Reg1Imm1(raw));
    case FX_ADD:
    case FX_ADDI:
    case FX_SUB:
    case FX_SUBI:
    case FX_MUL:
    case FX_MULI:
    case FX_MADD:
    case FX_DIV:
    case FX_DIVI:
    default:
        return DecodedParameter{};
    }
    return DecodedParameter{};
}

DecodedInstruction Instruction::Decode(GarandInstruction instr) {
    auto mne = DecodeMnemonic(instr);
    return DecodedInstruction{
        .mnemonic = mne,
        .parameter = DecodeParameter(instr, mne),
    };
}

} // namespace Garand